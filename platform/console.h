/* The console the game runs on: pulseman's runtime (runtime/) behind the game's hardware calls
 * (game/hal.h, platform/hal.c). A program that runs the game (platform/main.c, test/golden.c)
 * makes the console, powers it on and calls the game's Start; it supplies the two functions below. */
#pragma once
#include "mdrt/mdrt.h"

extern mdrt *console_m;                 /* the console the game is running on */

void console_power_on(mdrt *m);         /* the game's own state as at power on, on console m */
uint8_t m8_read(void *user, uint32_t addr);   /* (mdrt_config.read68k: the VDP's DMA reads) */

/* the program's: after each frame (the frames so far; wait: at one of the game's waits for the
 * vblank), and the way out of the game when the program is done (it does not return) */
void console_frame_done(long frame, int wait);
void console_quit(void);
