#include <stdio.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>
#include <gb/gb.h>

//#define POOKEY_DEBUG


void debugFixed( fixed* f )
{
#ifdef POOKEY_DEBUG
	printf("%x %x %x\n", f->w, f->b.h, f->b.l);
#endif
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

// vec2 containing one WORD for each component
typedef struct  {
	fixed x, y;
} vec2w;

typedef struct  {
	vec2w pos;
	vec2w speed;
	UBYTE flags;
} entity;

int isGrounded( entity* e )
{
	return e->flags & 0x01;
}

void setGrounded( entity* e )
{
	e->flags |= 0x01;
}

void makePlayer(entity* e)
{
	e->pos.x.w   = 0x1000;
	e->pos.y.w   = 0x1000;
	e->speed.x.w = 0x0000;
	e->speed.y.w = 0x0000;
	e->flags = 0x00;
}

#define POOKEY_GRAVITY 		0x0080
#define POOKEY_JUMP_FORCE	0x0400
#define POOKEY_SJUMP_FORCE 	0x0500

#define POOKEY_WALK_SPEED 	0x0200
#define POOKEY_RUN_SPEED 	0x0400

void applyGravity( entity* e )
{
	e->speed.y.w += POOKEY_GRAVITY;
}

void applySpeed( entity* e )
{
	e->pos.x.w = e->pos.x.w + e->speed.x.w;
	e->pos.y.w = e->pos.y.w + e->speed.y.w;
}
UBYTE counter, xpos, ypos, impulse_timer, running;

// to be able to use fixed, we need to declare it
// outside the main function
entity player;


void main()
{
	// must be first
    font_t ibm_font, italic_font, min_font;

	makePlayer(&player);

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

		if(counter & J_LEFT)
		{
			if( counter & J_B )
			{
				player.pos.x.w -= POOKEY_RUN_SPEED;
			}
			else
			{
				player.pos.x.w -= POOKEY_WALK_SPEED;
			}
		}

		if(counter & J_RIGHT)
		{
			if( counter & J_B )
			{
				player.pos.x.w += POOKEY_RUN_SPEED;
			}
			else
			{
				player.pos.x.w += POOKEY_WALK_SPEED;
			}
		}

		running = (counter & J_RIGHT || counter & J_LEFT) && counter & J_B;

		if( counter & J_A )
		{
			if( isGrounded(&player) )
			{
				if( running != 0 )
				{
					player.speed.y.w -= POOKEY_SJUMP_FORCE;
				}
				else
				{
					player.speed.y.w -= POOKEY_JUMP_FORCE;
				}
				player.flags = 0x00;
			}
			else
			{
				if( impulse_timer != 0xFF && impulse_timer < 0x5 )
				{
					if( running )
					{
						player.speed.y.w = -POOKEY_SJUMP_FORCE;
					}
					else
					{
						player.speed.y.w = -POOKEY_JUMP_FORCE;
					}
					impulse_timer++;
				}
			}
		}
		else if( impulse_timer != 0xFF )
		{
			impulse_timer = 0xFF;
		}

		if( !isGrounded(&player) )
		{
			applyGravity( &player );
		}
		else
		{
			impulse_timer = 0;
		}

		debugFixed( &(player.pos.y) );
		debugFixed( &(player.speed.y) );

		applySpeed(&player);

		if( player.pos.y.w > 0x5500 )
		{
			player.pos.y.w = 0x5500;
			setGrounded(&player);
			impulse_timer = 0;
		}

		if( isGrounded(&player) )
		{
			player.speed.y.w = 0x0000;
			player.pos.y.w = 0x5500;
		}

		xpos = player.pos.x.b.h;
		ypos = player.pos.y.b.h;

		// x: scroll, y: fixed
		move_bkg(xpos, 0);

#define OFFX (80)
#define OFFY ypos + 24

		move_sprite(0, 0 + OFFX, 0 + OFFY);
		move_sprite(1, 8 + OFFX, 0 + OFFY);
		move_sprite(2, 0 + OFFX, 8 + OFFY);
		move_sprite(3, 8 + OFFX, 8 + OFFY);

	}
}
