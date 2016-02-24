#include <stdio.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>
#include <gb/gb.h>

#include "game/gameplay.h"

UBYTE joypad_state;

// to be able to use WORD, we need to declare it
// outside the main function


void main()
{
	disable_interrupts();
	DISPLAY_OFF;
	SPRITES_8x8;

	game_start();

	SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;

	enable_interrupts();

	while(1)
	{
		wait_vbl_done();
		joypad_state = joypad();
		gameplay(joypad_state);
	}
}
