
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

void handleMapCollision( entity_t* e, map_t* map )
{
	UBYTE x, y,
		  xmin, xmax,
		  ymin, ymax,
		  colx, coly,
		  up;

	fixed f;
	colx = 0;
	coly = 0;

	//if( e->speed.x.w != 0 )
	{
		ymin = (((UBYTE)e->pos.y.b.h-2) >> 3) - 1;
		ymax = ymin+1;
		f.w = e->pos.x.w + e->speed.x.w;
		if( e->speed.x.w > 0x7FFF )
		{
			x = (f.b.h + 4) >> 3;
			for( y = ymin; y <= ymax; y++ )
			{
				unsigned char mt = get_tilemap_tile( map, x-1, y );
				if( mt != 0 )
				{
					e->speed.x.w = 0;
					colx = 1;
				}
			}
		}
		else if( e->speed.x.w > 0 )
		{
			x = (f.b.h - 0) >> 3;
			for( y = ymin; y <= ymax; y++ )
			{
				unsigned char mt = get_tilemap_tile( map, x+1, y );
				if( mt != 0 )
				{
					e->speed.x.w = 0;
					colx = 1;
				}
			}
		}
	}

	//if( e->speed.y.w != 0 )
	{
		if( e->speed.y.w > 0x7FFF )
		{
			up = 1;
		}
		else if( e->speed.y.w > 0 )
		{
			up = 0;
		}
		xmin = ((UBYTE)e->pos.x.b.h) / 8;
		xmax = xmin + 1;
		f.w = e->pos.y.w + e->speed.y.w;
		y = (f.b.h - 1) / 8;
		for( x = xmin; x <= xmax; x++ )
		{
			unsigned char mt;
			if( up == 1 )
			{
				mt = get_tilemap_tile( map, x, y-2 );
			}
			else
			{
				mt = get_tilemap_tile( map, x, y );
			}
			if( mt != 0 )
			{
				e->speed.y.w = 0;
				coly = 1;
				if( up == 0 )
				{
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
