/* The game headless, for checking that it plays bit for bit as it did: no window, no pacing, the
 * pads from scripts, and a line a frame of hashes (work RAM, the VDP, Z80 RAM at the game's wait;
 * the picture; the frame's sound), as ninety-four's --golden writes them, so the two can be
 * compared whole (test/check.sh).
 *   golden ROM --frames N --golden OUT [--script FILE] [--script2 FILE ...]
 * A script is "FRAME:BUTTON+BUTTON,..." (# lines are comments): pad 1's buttons held from that frame
 * on, --script2 to --script4 the same for pads 2-4. The HV counter comes from a fixed stand-in (the beam pulseman reported until
 * 2026-10-16), as ninety-four's golden runs have it. */
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "rom.h"
#include "mem.h"
#include "z80.h"
#include "console.h"

static jmp_buf out;
void console_quit(void) { longjmp(out, 1); }

static FILE *golden;
static uint64_t golden_ram, golden_vdp, golden_z80, golden_pic;
static uint32_t golden_beam;
static long stop_after = -1, run_offset;
static int run_known;

static uint64_t fnv(uint64_t h, const void *p, size_t n)
{
    const uint8_t *b = p;
    for (size_t i = 0; i < n; i++) h = (h ^ b[i]) * 0x100000001B3ull;
    return h;
}
#define FNV0 0xCBF29CE484222325ull

static uint16_t golden_hv(void *user)
{
    mdrt *m = user;
    uint32_t width = mdrt_vdp_width(mdrt_get_vdp(m)) == 320 ? 420 : 342, lines = mdrt_frame_mclks(m) / 3420;
    golden_beam += 97;
    uint32_t line = (224 + golden_beam / width) % lines, h = golden_beam % width;
    return (uint16_t)((line & 0xFF) << 8 | (h >> 1));
}

static void golden_present(void *user, const uint16_t *px, int w, int h, int pitch)
{
    (void)user;
    golden_pic = FNV0;
    for (int y = 0; y < h; y++) golden_pic = fnv(golden_pic, px + y * pitch, (size_t)w * 2);
}

static void golden_audio(void *user, const int16_t *s, int n)
{
    (void)user;
    fprintf(golden, "%016llx %016llx %016llx %016llx %016llx %d\n", (unsigned long long)golden_ram, (unsigned long long)golden_vdp,
            (unsigned long long)golden_z80, (unsigned long long)golden_pic, (unsigned long long)fnv(FNV0, s, (size_t)n * 4), n);
}

void console_frame_done(long frame, int wait)
{
    if (!run_known) { run_offset = frame - (long)mdrt_frame_count(console_m); run_known = 1; }
    long run = (long)mdrt_frame_count(console_m) + run_offset;
    if (wait) {
        mdrt_vdp_state s;
        mdrt_vdp_get_state(mdrt_get_vdp(console_m), &s);
        static uint8_t ram[RAM_SIZE];
        ram_pack(ram);                                    /* (RAM as the 68000 has it) */
        golden_ram = fnv(FNV0, ram, 0x10000);
        golden_vdp = fnv(fnv(fnv(fnv(FNV0, s.reg, 24), s.vram, 0x10000), s.cram, sizeof s.cram), s.vsram, sizeof s.vsram);
        golden_z80 = fnv(FNV0, z80_ram, 0x2000);
        golden_beam = 0;                                  /* (the beam starts again at the handshake) */
    }
    if (stop_after >= 0 && run >= stop_after) console_quit();
}

/* the pads' scripts */
typedef struct { long frame; uint16_t buttons; } entry;
static struct { entry *e; size_t len; } scripts[MDRT_PADS];

static int read_script(const char *path, int pad)
{
    static const struct { const char *name; uint16_t bit; } names[] = {
        { "UP", MDRT_PAD_UP }, { "DOWN", MDRT_PAD_DOWN }, { "LEFT", MDRT_PAD_LEFT }, { "RIGHT", MDRT_PAD_RIGHT },
        { "A", MDRT_PAD_A }, { "B", MDRT_PAD_B }, { "C", MDRT_PAD_C }, { "START", MDRT_PAD_START },
        { "X", MDRT_PAD_X }, { "Y", MDRT_PAD_Y }, { "Z", MDRT_PAD_Z }, { "MODE", MDRT_PAD_MODE },
    };
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "cannot open %s\n", path); return 0; }
    static char line[65536];
    size_t cap = 0;
    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#') continue;
        for (char *item = strtok(line, ",\r\n"); item; item = strtok(NULL, ",\r\n")) {
            char *colon = strchr(item, ':');
            if (!colon) continue;
            if (scripts[pad].len == cap) scripts[pad].e = realloc(scripts[pad].e, (cap = cap ? 2 * cap : 256) * sizeof *scripts[pad].e);
            entry *e = &scripts[pad].e[scripts[pad].len++];
            e->frame = atol(item);
            e->buttons = 0;
            for (char *name = colon + 1; *name; ) {
                size_t n = strcspn(name, "+"), i = 0;
                while (i < sizeof names / sizeof *names && (strlen(names[i].name) != n || strncmp(names[i].name, name, n))) i++;
                if (i == sizeof names / sizeof *names) { fprintf(stderr, "%s: no button %.*s\n", path, (int)n, name); fclose(f); return 0; }
                e->buttons |= names[i].bit;
                name += n + (name[n] == '+');
            }
        }
    }
    fclose(f);
    return 1;
}

static void script_source(void *user, uint16_t b[MDRT_PADS])
{
    long frame = (long)mdrt_frame_count(user);
    for (int pad = 0; pad < MDRT_PADS; pad++) {
        b[pad] = 0;
        for (size_t i = 0; i < scripts[pad].len; i++)
            if (scripts[pad].e[i].frame <= frame) b[pad] = scripts[pad].e[i].buttons;
    }
}

int main(int argc, char **argv)
{
    const char *rom = NULL, *golden_path = NULL;
    int inputs = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--frames") && i + 1 < argc) stop_after = atol(argv[++i]);
        else if (!strcmp(argv[i], "--golden") && i + 1 < argc) golden_path = argv[++i];
        else if (!strcmp(argv[i], "--script") && i + 1 < argc) { if (!read_script(argv[++i], 0)) return 1; inputs = 1; }
        else if (!strncmp(argv[i], "--script", 8) && argv[i][8] >= '2' && argv[i][8] <= '4' && !argv[i][9] && i + 1 < argc) {
            if (!read_script(argv[i + 1], argv[i][8] - '1')) return 1;
            i++;
            inputs = 1;
        } else if (argv[i][0] != '-' && !rom) rom = argv[i];
        else { fprintf(stderr, "usage: %s ROM --frames N --golden OUT [--script FILE] [--script2 FILE ...]\n", argv[0]); return 2; }
    }
    if (!rom || !golden_path) { fprintf(stderr, "usage: %s ROM --frames N --golden OUT [--script FILE] ...\n", argv[0]); return 2; }
    if (!rom_load(rom)) { fprintf(stderr, "cannot load the ROM %s\n", rom); return 1; }
    if (!(golden = fopen(golden_path, "wb"))) { fprintf(stderr, "cannot write %s\n", golden_path); return 1; }
    mdrt_config c = { 0 };
    c.audio_rate = 48000;
    c.sram_size = 0x2000;
    c.read68k = m8_read;
    c.platform = (mdrt_platform){ .present = golden_present, .audio = golden_audio };
    mdrt *m = mdrt_new(&c);
    if (!m) { fprintf(stderr, "no console (no FM core in this build?)\n"); return 1; }
    mdrt_set_hv_source(m, golden_hv, m);
    if (inputs) mdrt_set_pad_source(m, script_source, m);
    if (!setjmp(out)) {
        console_power_on(m);
        run68k(Start);
    }
    fclose(golden);
    mdrt_free(m);
    return 0;
}
