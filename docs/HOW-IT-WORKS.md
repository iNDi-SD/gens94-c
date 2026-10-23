# How the game works

NHL '94 for the GENS, as C. Each file in `game/` is one module of the
original assembly (`hockey.c` is `hockey94.asm`), and each function one of its routines, named
after its label, with a comment that says which (`docs/ROUTINES.md` lists them all). The C does
what the 68000 did, step by step: same order, same arithmetic, same RAM. That is why it plays
exactly as the cartridge (`test/check.sh`).

## The pieces

| Where | What |
|---|---|
| `game/play/` | The game on the ice: skating and the players' AI (`assign.c`, `checks.c`), collisions, checks, the puck and goalies (`collide.c`, `goalie.c`), one-timers, penalties and fights (`penalty.c`), periods (`period.c`), the shootout, the controls (`input.c`), the game loop (`hockey.c`) |
| `game/screen/` | The picture: the VDP and the drawing routines (`video.c`), sprites, the camera and scrolling (`display.c`), replays and the objects' motion (`replay.c`), the crowd and hot / cold streaks (`crowd.c`), player cards, the title, attract mode, the scouting report |
| `game/menus/` | Setup and the options (`optsetup.c`, `setup.c`), the menu engine (`menu.c`), user records, stats |
| `game/sound/` | The sound driver's commands (`sound.c`) and the Z80 that runs it (`z80.c`: the Z80's program ported to C, writing to the YM2612) |
| `game/system/` | Power on (`start.c`), the 68000's jumps through RAM (`dispatch.c`), RAM, the ROM, save RAM, the 4 Way Play, data, the teams' tables (`rosters.c`) |
| `game/*.h` | `game.h` (every routine's declaration, helpers), `ram.h` (work RAM: docs/RAM.md), `hal.h` (the hardware the game touches), `rom.h`, `mem.h`, `symbols_gen.h` (the ROM's addresses), `anim_gen.h` (animation frames) |
| `platform/` | The console under the game: `hal.c` (VDP, sound chips, pads and save RAM on the runtime), `main.c` (the window), `menu.c` (the settings menu), `padsetup.c` (the controller setup screen), `settings.c` (`settings.ini`) |
| `runtime/` | pulseman's console core: the VDP, the PSG, the YM2612 (Nuked-OPN2), the pads, save RAM |
| `test/` | `golden.c` (the game headless, a line of hashes a frame), `check.sh`, the input scripts |

## From power on

1. `Start` (`system/start.c`): Sega's hardware init (`SegaInit`), the ROM checksum, then `Begin`.
2. `Begin` (`play/hockey.c`): the publisher's logo, the title, the setup screen (`GameSetUp`,
   `menus/optsetup.c`), the matchup, then a game.
3. A period starts with `setupice` (`menus/setup.c`: the rink, the teams' graphics and colours),
   the clock (`ResetClock`) and the faceoff.
4. `Gameloop` (`play/hockey.c`) runs one `DoGameFrame` after another until the period ends:
   - wait for the vblank (`hal_wait_vblank`), then take how many vblanks passed (`elapsed`);
   - `periodicevents`: penalties, the crowd, the arena's animation, the sound; once a second the
     goalies, the excitement, the bench's energy, injuries, the power play;
   - `updateplayers`: every player's assignment (the AI) and motion, the puck, collisions;
   - songs, the window, the replay buffer (`updatereplay`);
   - `setvideo`: the sprites and scrolling for the next picture, which the vblank interrupt
     (`VBlank`, `screen/display.c`, through `g.vbint`) puts on screen; it also runs the game
     clock and counts the vblanks (`vcount`).
5. The pause menu (`Pausemode`), intermissions, stats, the three stars, the playoffs: each is a
   screen with its own small loop, in the module the original put it in.

## The interrupts

The game runs on one interrupt, the vblank, which the VDP raises at the end of each picture (when
its register 1 enables it). The 68000 then runs the routine whose address is in `g.vbint`, which
each screen sets (`call68k` turns the address into its C function):

- **`VBlank`** in play (`screen/display.c`): when the game has a frame ready (`dfok`), the scroll
  values, the sprite table and the DMA list go to the VDP (`DumpSprites`); then the palette fade,
  the game clock (a second every $AAA jiffies, the shootout clock in a penalty shot), `vcount`, and
  the music (`p_music_vblank`, `sound/sound.c`: the pads, the song's tracks, the commands for the
  Z80).
- the others are the same with less: `vb2` for fades, `VBlank_SetOptions` on the setup screen,
  `VBlank_TitleScreen` (a line scroll), `VBlank_Playoff`, and `VBcount` (`vcount` alone) while save
  RAM is set up.

A wait in the game (`hal_wait_vblank`) is the original's loop polling `vcount`: the frame ends
there (the window draws the picture and plays the frame's sound), and the next begins with the
vblank routine (`platform/hal.c`). The Z80 runs the sound driver beside the 68000 the whole time;
here it runs on whenever the 68000 touches its bus, and through the rest of each frame.

## How the C stands in for the 68000

- **RAM** is one struct, `g` (`ram.h`), laid out at the 68000's offsets: every variable has its
  original address in its comment. `mem.h` reads and writes it by address, as the 68000 did.
- **The ROM** is the player's own cartridge: graphics, text, tables and team data are read from
  it at run time by their address (`rom_ptr(SYM_x)`), so none of the game's art or data is in this
  repository beyond the small strings and tables in the `*_gen.h` files.
- **Hardware**: every access the 68000 made to the VDP, the sound chips, the pads or save RAM is a
  call in `hal.h` (one call, one access), implemented in `platform/hal.c`.
- **Jumps through RAM**: the original jumps to addresses kept in RAM (the vblank routine: `VBlank`
  in play, others for the title, the setup screen, fades; menu
  handlers); `call68k` (`system/dispatch.c`) turns an address back into its C function. Where
  the original reset the stack and jumped (a new period, a restart), `restart_at` does the same
  to the C stack.

## Time

The C has no instruction timing. The game waits for the vblank where the cartridge did, so it
plays at the console's 59.92 frames a second, a frame of play in one vblank. On the cartridge a
frame of play now and then takes two: the 68000 had too much to do (its slowdown), the game sees
two vblanks pass, and it moves everything by the vblanks that passed (`d7` in `DoGameFrame`), so
the play keeps its speed but the picture skips. The C is never slow, so that never happens here:
`d7` is always 1 in play, and every frame of play is drawn.

## Reading a routine

Open its file in `game/`, find it by name (or look it up in `docs/ROUTINES.md`). The comment
before it names the original (`display94.asm:VBlank`: the module and the label) and says what it
does; its declaration in `game.h` gives the registers it takes and leaves (`a0 -> d0`); comments
inside in brackets quote the original where the C needs a reason. That module, in the community's
byte-matching rebuild of the assembly, is the original it was written from.
