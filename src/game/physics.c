
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
	e->pos.x.w = e->pos.x.w + e->speed.x.w;
	e->pos.y.w = e->pos.y.w + e->speed.y.w;
}

void handleMapCollision( entity_t* e, map_t* map )
{
	UBYTE colx, coly;
	colx = 0;
	coly = 0;

	//if( e->speed.x.w != 0 )
	{
		fixed f;
		UBYTE off, x, y, ymin, ymax, extra;
		ymin = (((int)e->pos.y.b.h) >> 3) + 2;
		ymax = ymin + 1;
		f.w = e->pos.x.w + e->speed.x.w;
		x = f.b.h / 8 + 2;
		if( e->speed.x.w > 0x7FFF )
		{
			for( y = ymin; y <= ymax; y++ )
			{
				unsigned char mt = get_tilemap_tile( map, x-1, y );
				if( mt != 0 )
				{
					e->speed.x.w = 0;
					e->pos.x.b.h = (x - 1) * 8 - 3;
					colx = 1;
				}
			}
		}
		else if( e->speed.x.w > 0 )
		{
			for( y = ymin; y <= ymax; y++ )
			{
				unsigned char mt = get_tilemap_tile( map, x+1, y );
				if( mt != 0 )
				{
					e->speed.x.w = 0;
					e->pos.x.b.h = (x - 2) * 8 - 0;
					colx = 1;
				}
			}
		}
	}

	//if( e->speed.y.w != 0 )
	{
		fixed f;
		UBYTE offset_x_pixels, x, y, xmin, xmax, offset_y_tiles, up;
		if( e->speed.y.w > 0x7FFF )
		{
			offset_y_tiles = 3;
			up = 1;
		}
		else if( e->speed.y.w > 0 )
		{
			offset_y_tiles = 4;
			up = 0;
		}
		offset_x_pixels = 8 * 9;
		xmin = (((int)e->pos.x.b.h) + offset_x_pixels) >> 3;
		xmax = xmin + 1;
		f.w = e->pos.y.w + e->speed.y.w;
		y = f.b.h / 8 + offset_y_tiles;
		for( x = xmin; x <= xmax; x++ )
		{
			unsigned char mt;
			if( up == 1 )
			{
				mt = get_tilemap_tile( map, x, y-1 );
			}
			else
			{
				mt = get_tilemap_tile( map, x, y-1 );
			}
			if( mt != 0 )
			{
				e->speed.y.w = 0;
				coly = 1;
				if( up == 0 )
				{
					e->pos.y.b.h = (y - offset_y_tiles) * 8;
					setGrounded(e);
				}
			}
		}
	}

	if( !colx && !coly )
	{
		e->flags = 0x0;
	}
}
