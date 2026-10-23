/* The teams' tables (rosters.h), read off the cartridge's own. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rosters.h"
#include "rom.h"
#include "symbols_gen.h"

struct RosterTeam roster_team[ROSTERS_MAX];
uint8_t roster_seats[32][16];

static uint16_t be16(uint32_t a) { const uint8_t *p = rom_ptr(a); return (uint16_t)(p[0] << 8 | p[1]); }
static uint32_t be32(uint32_t a) { return (uint32_t)be16(a) << 16 | be16(a + 2); }

void rosters_bad_team(uint16_t team)
{
    fprintf(stderr, "rosters_team: team %u is past the tables' %d\n", team, ROSTERS_MAX);
    abort();
}

/* the String at a (as it is: its length word, bytes and pad), on past it */
static void read_text(uint32_t *a, struct RosterText *t)
{
    uint16_t n = be16(*a);
    if (n > RS_TEXT) { fprintf(stderr, "rosters: a String of %u bytes at $%06X\n", n, *a); abort(); }
    t->at = *a;
    memcpy(t->s, rom_ptr(*a), n);
    *a += n;
}

/* each team's data (TeamList: its players, lines, texts, colours and the rest), its tables'
 * entries, and the playoff draws */
void rosters_read(void)
{
    memset(roster_team, 0, sizeof roster_team);
    for (uint16_t t = 0; t < SYM_NumTeams; t++) {
        struct RosterTeam *T = &roster_team[t];
        uint32_t b = be32(SYM_TeamList + 4u * t);
        T->data = b;
        for (int k = 0; k < 16; k++) {
            T->colours[0][k] = be16(b + be16(b + 2) + 2u * k);
            T->colours[1][k] = be16(b + be16(b + 2) + 32 + 2u * k);
            T->matchup[0][k] = be16(SYM_TeamPalettes + 64u * t + 2u * k);
            T->matchup[1][k] = be16(SYM_TeamPalettes + 64u * t + 32 + 2u * k);
        }
        const uint8_t *sr = rom_ptr(b + be16(b + 8));
        T->offense = sr[0] >> 4; T->defense = sr[0] & 15;
        T->pk = sr[1] >> 4; T->pp = sr[1] & 15;
        T->home = sr[2] >> 4; T->away = sr[2] & 15;
        T->forwards = sr[3] >> 4; T->defensemen = sr[3] & 15;
        T->starts = be16(b + be16(b + 10));
        uint16_t w = T->starts;                         /* (as GetGoalieCount counts them: to its last nibble not 0) */
        do T->goalies++; while ((w = (uint16_t)(w << 4)) != 0);
        memcpy(T->lines, rom_ptr(b + be16(b + 6)), sizeof T->lines);
        uint32_t p = b + be16(b);
        for (; be16(p) != 2; T->players++) {
            if (T->players == RS_ROSTER) { fprintf(stderr, "rosters: team %u has more than %d players\n", t, RS_ROSTER); abort(); }
            struct RosterPlayer *P = &T->player[T->players];
            read_text(&p, &P->name);
            const uint8_t *a = rom_ptr(p);
            P->number = a[0];
            for (int k = 0; k < LR_RATINGS; k++) P->rating[k] = (uint8_t)(a[1 + k / 2] >> (k & 1 ? 0 : 4) & 15);
            p += 8;
        }
        p = b + be16(b + 4);
        read_text(&p, &T->city);
        read_text(&p, &T->abbr);
        read_text(&p, &T->nickname);
        read_text(&p, &T->arena);
        T->rating = *rom_ptr(SYM_TeamRatings + t);
        memcpy(T->songs, rom_ptr(SYM_TeamSongs + 6u * t), 6);
        T->records = be16(SYM_PlayerRecordOffsets + 2u * t);
        T->pictures_at = be32(SYM_FeaturedPictures + 4u * t);
        for (uint32_t a = T->pictures_at; be32(a) != 0; a += 6) {
            if (T->pictures == RS_PICTURES) { fprintf(stderr, "rosters: team %u has more than %d pictures\n", t, RS_PICTURES); abort(); }
            T->picture[T->pictures].pic = be32(a);
            T->picture[T->pictures++].slot = be16(a + 4);
        }
        T->logo = be32(SYM_TeamLogoBitmaps + 4u * t);
        T->ice = be32(SYM_TeamGfxList + 4u * t);
        for (int k = 0; k < 16; k++) T->logo_colours[k] = be16(SYM_TeamLogoPalettes + 32u * t + 2u * k);
    }
    memset(roster_seats, 0, sizeof roster_seats);
    memcpy(roster_seats, rom_ptr(SYM_playoffseats), 16u * SYM_PlayoffRows);
}
