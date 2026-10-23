/* The teams: the cartridge's 26 clubs and its two All-Star teams. The game reads what it needs of
 * each team (its players, lines, colours, texts, songs, logo ...) from plain tables (roster_team,
 * below), read off the cartridge's own tables once at power on (rosters_read: the program calls it
 * before the game's Start), and its art by the cartridge's addresses (symbols_gen.h's SYM_). */
#pragma once
#include <stdint.h>

#define ROSTERS_MAX 32                                   /* the tables' room (the cartridge's 28: a team is a byte in the playoff tree and 5 bits elsewhere) */
/* ---- the teams' tables ---------------------------------------------------------------------
 * A String the printers read stays a String (struct RosterText: as the cartridge keeps one); at:
 * where the cartridge keeps it (the 68000's registers held those addresses). */
#define RS_TEXT 42                                      /* a String's room: its length word, the longest name, a 0 */
#define RS_ROSTER 26                                    /* players a team (the game's room, and its save RAM's) */
#define RS_PICTURES 32
struct RosterText { uint32_t at; uint8_t s[RS_TEXT]; };     /* s: a length word (itself counted, made even), the bytes, a 0 to make it even */
/* a player's 14 ratings (0-15), in the order the cartridge keeps them, two to a byte after his
 * number: the roster screen's names for a skater, and what each is to a goalie */
enum { LR_WEIGHT, LR_AGILITY, LR_SPEED, LR_OFF_AWARENESS, LR_DEF_AWARENESS, LR_SHOT_POWER /* goalie: puck control */,
       LR_CHECKING /* unused */, LR_HANDED_FIGHTING /* goalie: glove hand */, LR_STICK_HANDLING /* unused */,
       LR_SHOT_ACCURACY /* unused */, LR_ENDURANCE /* goalie: stick right */, LR_SHOOT_OR_PASS /* stick left */,
       LR_PASS_ACCURACY /* glove right */, LR_AGGRESSIVENESS /* glove left */, LR_RATINGS };
struct RosterPlayer {
    struct RosterText name;
    uint8_t number;                                     /* BCD, as the game prints it */
    uint8_t rating[LR_RATINGS];
};
struct RosterTeam {
    uint32_t data;                                      /* what the team struct keeps (tmdata): the address of the team's data */
    struct RosterText city, abbr, nickname, arena;
    uint16_t colours[2][16];                            /* its home and away palettes (setplayercolors) */
    uint16_t matchup[2][16];                            /* TeamPalettes: the setup screens' colours */
    uint8_t offense, defense;                           /* 0-7: its other playoff games, simulated (getscore) */
    uint8_t pp, pk, home, away;                         /* 0-15: its players' bonuses on the power play, killing a penalty, at home, away */
    uint8_t forwards, defensemen, goalies;              /* (the roster: its goalies, then forwards, then defensemen) */
    uint16_t starts;                                    /* .sodds: a nibble a goalie, how often he starts */
    uint8_t lines[8][8];                                /* the line sets (SC1 SC2 CHK PP1 PP2 PK1 PK2 L8): 7 roster slots + 1 each, then a 0 */
    uint8_t players;
    struct RosterPlayer player[RS_ROSTER];
    uint8_t rating;                                     /* TeamRatings */
    uint8_t songs[6];                                   /* the organ's (TeamSongs) */
    uint16_t records;                                   /* its players' records in save RAM (PlayerRecordOffsets: 4 bytes a word) */
    uint8_t pictures;                                   /* FeaturedPictures: a picture (its address) and the roster slot it is */
    struct { uint32_t pic; uint16_t slot; } picture[RS_PICTURES];
    uint32_t pictures_at;                               /* (the cartridge's address of the list) */
    uint32_t logo, ice;                                 /* its logo (TeamLogoBitmaps) and centre ice (TeamGfxList) */
    uint16_t logo_colours[16];
};
extern struct RosterTeam roster_team[ROSTERS_MAX];
extern uint8_t roster_seats[32][16];                        /* playoffseats: rows of 16 teams, the draws a playoff starts from */
void rosters_read(void);                                 /* the tables, from the cartridge (rom_cart) */
__attribute__((noreturn, cold)) void rosters_bad_team(uint16_t team);
/* team t (past the tables: a bug in the port) */
static inline struct RosterTeam *rosters_team(uint16_t t) { if (t >= ROSTERS_MAX) rosters_bad_team(t); return &roster_team[t]; }
/* byte k of a player's 8 the cartridge keeps after his name (0: his number; 1-7: his ratings, two
 * to a byte, the first high) */
static inline uint8_t roster_attr(const struct RosterPlayer *p, int k) { return k == 0 ? p->number : (uint8_t)(p->rating[2 * k - 2] << 4 | p->rating[2 * k - 1]); }
