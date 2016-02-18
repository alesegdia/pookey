
#include "physics.h"
#include "defs.h"
#include <gb/gb.h>
#include <stdio.h>

int isGrounded( entity_t* e )
{
	return e->flags & 0x01;
}

void setGrounded( entity_t* e )
{
	e->flags |= 0x01;
}

void applyGravity( entity_t* e )
{
	e->speed.y.w += POOKEY_GRAVITY;
}

void applySpeed( entity_t* e )
{
	UBYTE f = e->pos.x.b.h;
	e->pos.x.w = e->pos.x.w + e->speed.x.w;
	e->pos.y.w = e->pos.y.w + e->speed.y.w;
	if( e->speed.x.b.h < 0x7FFF )
	{
		if( e->pos.x.w < f )
		{
			e->pos_offset++;
		}
	}
}

#define XOFFSET 80
#define BOX_RIGHT 2
#define BOX_LEFT 4
#define BOX_UP 4
#define BOX_DOWN 0

void handleMapCollision( entity_t* e, map_t* map )
{
	UBYTE x, y,
		  x_min_tile, x_max_tile,
		  y_min_tile, y_max_tile,
		  x_collision_exists, y_collision_exists,
		  moving_up;

	fixed f;
	x_collision_exists = 0;
	y_collision_exists = 0;

	//if( e->speed.x.w != 0 )
	{
		y_min_tile = (((UBYTE)e->pos.y.b.h-1 + BOX_UP) >> 3) - 1;
		y_max_tile = (((UBYTE)e->pos.y.b.h-1 + 8) >> 3) - 1;
		f.w = e->pos.x.w + e->speed.x.w;
		if( e->speed.x.w > 0x7FFF )
		{
			x = ((f.b.h + XOFFSET + BOX_LEFT) >> 3) - 1;
		}
		else if( e->speed.x.w > 0 )
		{
			x = ((f.b.h + XOFFSET - BOX_RIGHT) >> 3) + 1;
		}

		for( y = y_min_tile; y <= y_max_tile; y++ )
		{
			unsigned char mt = get_tilemap_tile( map, x, y );
			if( mt != 0 )
			{
				e->speed.x.w = 0;
				x_collision_exists = 1;
			}
		}
	}

	//if( e->speed.y.w != 0 )
	{
		x_min_tile = ((UBYTE)e->pos.x.b.h + XOFFSET - BOX_LEFT) / 8;
		x_max_tile = ((UBYTE)e->pos.x.b.h + XOFFSET + 8 - BOX_RIGHT) / 8;
		f.w = e->pos.y.w + e->speed.y.w;
		if( e->speed.y.w > 0x7FFF )
		{
			moving_up = 1;
			y = (f.b.h - 16 + BOX_UP) >> 3;
		}
		else if( e->speed.y.w > 0 )
		{
			moving_up = 0;
			y = (f.b.h) >> 3;
		}
		for( x = x_min_tile; x <= x_max_tile; x++ )
		{
			unsigned char mt = get_tilemap_tile( map, x, y );
			if( mt != 0 )
			{
				e->speed.y.w = 0;
				y_collision_exists = 1;
				if( moving_up == 0 )
				{
					setGrounded(e);
					e->pos.y.b.h = y * 8;
					e->pos.y.b.l = 0;
				}
			}
		}
	}

	if( !x_collision_exists && !y_collision_exists )
	{
		e->flags = 0x0;
	}
}
