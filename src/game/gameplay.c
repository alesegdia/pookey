
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
	player.pos.x.w   = 0x0800;
	player.pos.y.w   = 0x2000;
	player.speed.x.w = 0x0000;
	player.speed.y.w = 0x0000;
	player.flags = 0x00;
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

	xpos = player.pos.x.b.h;
	ypos = player.pos.y.b.h;

	// x: scroll, y: fixed
	move_bkg(xpos, 0);
	xpos = 80;

	move_sprite(0, 0 + 80, 0 + ypos);
	move_sprite(1, 8 + 80, 0 + ypos);
	move_sprite(2, 0 + 80, 8 + ypos);
	move_sprite(3, 8 + 80, 8 + ypos);

}

