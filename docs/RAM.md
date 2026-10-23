# The game's RAM

The Genesis has 64 KB of work RAM ($FFFF0000-$FFFFFFFF). Here it is `struct Ram g` in
`game/ram.h`, every field at the 68000's offset (packed), with its original address and the original's
comment beside it. `ram.h` is generated from the community's RAM map; it is the complete list.
This page is a map into it.

## The layout

| Offset (from $FFFF0000) | What |
|---|---|
| $0000-$AFFF | The instant replay: 458 frames of $62 bytes, a ring; between games the save RAM's copy, and the game's RAM kept during a highlight |
| $B000-$B049 | The VDP's setup (the scroll, the planes' maps, where each set of tiles is in VRAM), printing, `vbint` and `vcount` |
| $B04A-$B849 | The sort objects: 16 of $80 bytes (the players, the nets, the puck) |
| $B84A-$B8B3 | The sprites' sort lists, the crowd |
| $B8B4-$BD17 | The DMA list: tiles and maps waiting for the vblank to send them to VRAM |
| $BD18-$C017 | The camera and scrolling, the palette fade, the faceoff men, the pads and 4 Way Play, passes, shots, one-timers, faceoffs |
| $C018-$C2E9 | The sprite table (`Satt`) and its size |
| $C2EA-$C473 | The game's mode and flags, the crowd, which player each controller has, the two teams, penalties, the referee, the clock |
| $C474-$CD95 | The score and penalty summaries, the two team structs |
| $CD96-$D17F | Stats, the night's games and the playoff tree, the menus, the three stars, the options, the random seed |
| $D180-$D403 | The sound driver's state on the 68000's side: its channels and voices, the Z80's command buffer |
| $D404-$DEF1 | The screens' variables: player cards, records and the name log, the shootout, the matchup, the arena's animation, hot / cold, pictures |
| $DEF2-$FFFF | `PALflag`, then the 68000's stack, which grows down from the top (`Stack`) |

## The structures

| Struct | What | Where |
|---|---|---|
| `Ram` (`g`) | All of work RAM: the game's variables, flags, buffers | `ram.h` |
| `SortObj` | One object on the ice: a player, the puck, a net (position, velocity, facing, animation, his ratings and flags) | 16 of them from $FFFFB04A, `$80` bytes each; `sortobj(i)`: 0-5 home (goalie, defense, forwards), 6-11 away, 12-13 the nets, 14 the puck, 15 its shadow |
| `SpriteHead` | The head of a sort object: what the sprite code draws | inside each `SortObj` |
| `Team` | One team in a game: its players' stats (goals, assists, shots, checks, penalty minutes), energy, lines, hot / cold, the power play | `team_struct(0)` home, `team_struct(1)` away |
| `Matchup`, `ReplayFrame`, `Channel`, `Voice`, `MenuPos`, `FoMan`, `PuckCross`, `Plane` | The matchup screen, the replay buffer, the sound driver's state, menus, faceoffs, the puck crossing lines, the VDP's planes | `ram.h` |

## Variables worth knowing

| Variable | Address | What |
|---|---|---|
| `g.vbint` | $FFFFB03A | the vblank routine's address (`call68k` runs it) |
| `g.vcount`, `g.oldvcount` | $FFFFB03E, $FFFFB040 | vblanks so far, and at the last game frame (their difference: `elapsed`) |
| `g.puckx` ... | $FFFFB74A | the puck |
| `g.Hpos` | $FFFFBD1C | the camera |
| `g.gmode`, `g.sflags`, `g.gmode2` | $FFFFC2EA, $FFFFC2EC, $FFFFC2FA | the game's mode and state flags (clock stopped, pause, shootout ...) |
| `g.HomeTeam`, `g.VisTeam` | $FFFFC330, $FFFFC332 | the two teams |
| `g.gameclock` | $FFFFC468 | the period's clock, in seconds |
| `g.OptPlayMode` ... `g.OptLine` | (the options) | the setup screen's choices: mode, players, teams, period length, goalie, records, penalties, line changes |

Search `ram.h` for a name or an address ($FFFFxxxx) to find any other; each has
`_Static_assert`s that keep it at its original offset.
