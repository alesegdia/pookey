
#include <stdio.h>

#include "gameplay.h"
#include "game/vec2w.h"
#include "game/entity.h"
#include "game/physics.h"
#include "game/defs.h"

// graphics
#include "assets/pookey_gfx.h"
#include "assets/tiles_gfx.h"
#include "assets/lvl0_map.h"


entity_t player;

void makePlayer()
{
	//player.pos.y.w   = 0x5600;
	player.pos.x.w   = POOKEY_START_X;
	player.pos.y.w   = POOKEY_START_Y;
	player.speed.x.w = 0x0000;
	player.speed.y.w = 0x0000;
	player.flags = 0x00;
	player.bounds.left = 4;
}

void game_start()
{
	makePlayer();

	// load background
	set_bkg_data(0, 2, maptiles);
	set_bkg_tiles( 0, 0, lvl0_map.width, lvl0_map.height, lvl0_map.data);

	// load sprite
	set_sprite_data(0, 4, sprite);
	set_sprite_tile(0,2);
	set_sprite_tile(1,0);
	set_sprite_tile(2,3);
	set_sprite_tile(3,1);
}

UBYTE compute_tile( entity_t* ent )
{
	return (ent->pos_offset * GB_TILEMAP_WIDTH) + ((ent->pos.x.b.h+ POOKEY_X_OFFSET * 8) >> 3);
	//return (ent->pos_offset * 32) + ((ent->pos.x.b.h+92) >> 3);
}

UBYTE last_tile = 0;
UBYTE first_tile = 0;

void scroll()
{
	UBYTE new_last_tile = compute_tile(&player);
	UBYTE new_first_tile = new_last_tile - 9;
	UBYTE y_tile;
	//printf("%d\n", new_last_tile);

	if( new_last_tile != last_tile )
	{
		last_tile = new_last_tile;

		for( y_tile = 0; y_tile < 18; y_tile++ )
		{
			set_bkg_tiles( new_last_tile+16, y_tile, 1, 1, get_tilemap_data_ptr(&lvl0_map, new_last_tile+16, y_tile) );
		}
	}

	if( (BYTE)new_first_tile >= 0 && new_first_tile != first_tile )
	{
		first_tile = new_first_tile;
		for( y_tile = 0; y_tile < 18; y_tile++ )
		{
			set_bkg_tiles( new_first_tile, y_tile, 1, 1, get_tilemap_data_ptr(&lvl0_map, new_first_tile, y_tile) );
		}
	}
}

void draw_player()
{
	UBYTE ypos = player.pos.y.b.h;
	UBYTE xpos = player.pos.x.b.h + 80;

	if( xpos < 80 && player.pos_offset == 0 )
	{
		xpos = 80 + player.pos.x.b.h;
	}
	else
	{
		xpos = 80;
		move_bkg(player.pos.x.b.h, 0);
	}

	move_sprite(0, 0 + xpos, 0 + ypos);
	move_sprite(1, 8 + xpos, 0 + ypos);
	move_sprite(2, 0 + xpos, 8 + ypos);
	move_sprite(3, 8 + xpos, 8 + ypos);
}

UBYTE impulse_timer, 	// current jump impulse
	  is_player_running; 			// is the player is_player_running?

void gameplay(UBYTE joypad_state)
{

	if( player.speed.x.w > 0x7FFF )
	{
		if( player.speed.x.w <= 0xFFFF )
		{
			player.speed.x.w += 10;
		}
	}
	else
	{
		if( player.speed.x.w > 1 )
		{
			player.speed.x.w -= 10;
		}
	}

	// previous hack to get correct player right movement,
	// probably some undefined behaviour?
	// player.speed.x.w = player.speed.x.w + 0;

	if(joypad_state & J_LEFT)
	{
		if( joypad_state & J_B )
		{
			player.speed.x.w -= ACCEL;
		}
		else
		{
			player.speed.x.w -= ACCEL;
		}
	}

	if(joypad_state & J_RIGHT)
	{
		if( joypad_state & J_B )
		{
			player.speed.x.w += ACCEL;
		}
		else
		{
			player.speed.x.w += ACCEL;
		}
	}

	is_player_running = (joypad_state & J_RIGHT || joypad_state & J_LEFT) && joypad_state & J_B;

	if( player.speed.x.w < 0xF000 )
	{
		if( is_player_running )
		{
			if( player.speed.x.w > POOKEY_RUN_SPEED )
			{
				player.speed.x.w = POOKEY_RUN_SPEED;
			}
		}
		else
		{
			if( player.speed.x.w > POOKEY_WALK_SPEED )
			{
				player.speed.x.w = POOKEY_WALK_SPEED;
			}
		}
	}
	else
	{
		if( is_player_running )
		{
			if( player.speed.x.w < -POOKEY_RUN_SPEED )
			{
				player.speed.x.w = -POOKEY_RUN_SPEED;
			}
		}
		else
		{
			if( player.speed.x.w < -POOKEY_WALK_SPEED )
			{
				player.speed.x.w = -POOKEY_WALK_SPEED;
			}
		}
	}

	if( player.speed.x.b.h == 0  && player.speed.x.b.l < 10 )
	{
		player.speed.x.b.l = 0;
	}

	if( joypad_state & J_A )
	{
		if( isGrounded(&player) )
		{
			if( is_player_running != 0 )
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
			if( impulse_timer != 0xFF && impulse_timer < 0xA )
			{
				if( is_player_running )
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

	handleMapCollision(&player, &lvl0_map);
	applySpeed(&player);

	if( isGrounded(&player) )
	{
		impulse_timer = 0;
		player.speed.y.w = 0x0000;
	}

	scroll();
	draw_player();


}

