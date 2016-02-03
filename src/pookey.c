#include <stdio.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>
#include <gb/gb.h>

// vec2 containing one WORD for each component
typedef struct  {
	fixed x, y;
} vec2w;

typedef struct  {
	vec2w pos;
	vec2w speed;
} entity;

void makePlayer(entity* e)
{
	e->pos.x.w = 0x1000;
	e->pos.y.w = 0x1000;
	e->speed.x.w = 0;
	e->speed.y.w = 0;
}

#define GRAVITY 0x0001;
void applyGravity( entity* e )
{
	e->speed.y.w += GRAVITY;
}

void updatePos( entity* e )
{

}

unsigned char sprite[] = {
	0x0, 0x0, 0x8, 0x8, 0x4, 0x4, 0xea, 0xa, 0x30, 0xc0, 0x18, 0x60, 0x8, 0xf0, 0x0, 0xf8, 
	0x80, 0xf0, 0xe0, 0xe0, 0xe0, 0x0, 0x40, 0xa0, 0x0, 0xe0, 0xa0, 0xe0, 0xa0, 0xb0, 0xb0, 0xb8, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x7, 0x0, 0xf, 0xc, 0xf, 
	0x7, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x3
};

unsigned char bgchars[] = {
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
};

unsigned char bgmap[] = {
	0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,
	1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3
};

fixed x, y, yforce;
UBYTE counter, ypos;

void main()
{
	// must be first
    font_t ibm_font, italic_font, min_font;

	entity player;
	makePlayer(&player);

	x.w = 0x1000;
	y.w = 0x1000;
	yforce.w = 0;
	ypos = 0x10;

	disable_interrupts();
	DISPLAY_OFF;

	// load background
	set_bkg_data(0,4,bgchars);
	for(counter=0;counter<=16;counter+=2)
		set_bkg_tiles( 0, counter, 20, 2, bgmap);

	// load sprite
	SPRITES_8x8;
	set_sprite_data(0, 4, sprite);
	set_sprite_tile(0,2);
	set_sprite_tile(1,0);
	set_sprite_tile(2,3);
	set_sprite_tile(3,1);

	SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;
	enable_interrupts();

    font_init();
    ibm_font = font_load(font_ibm);  /* 96 tiles */
    italic_font = font_load(font_italic);   /* 93 tiles */
    color(WHITE, DKGREY, SOLID);
    min_font = font_load(font_min);

	font_set(ibm_font);
	printf("Pookey\n");

	while(1) {
		/* Skip four VBLs (slow down animation) */
		for(counter = 0; counter < 4; counter++)
		{
			wait_vbl_done();
		}
		counter = joypad();

		if(counter & J_UP)
		{
			y.b.l -= 1;
		}

		if(counter & J_LEFT)
		{
			x.w -= 0x0010;
		}

		if(counter & J_RIGHT)
		{
			x.w += 0x0010;
		}

		if( counter & J_A )
		{
			/*
			if( y == 100 )
			{
				yforce -= 3;
			}
			*/
		}

		//y.b.l += 0x0F;
		y.w += 0x0010;
		//ypos = ( y.w & 0xFF00 ) >> 8;
		ypos = y.b.h;
		/*
		if( y.b.h > 100 )
		{
			y.b.h = 100;
		}
		*/

		move_sprite(0,x.b.h,ypos);
		move_sprite(1,x.b.h+8,ypos);
		move_sprite(2,x.b.h,ypos+8);
		move_sprite(3,x.b.h+8,ypos+8);


	}
}
