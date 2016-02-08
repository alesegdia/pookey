
#include "physics.h"
#include "defs.h"

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
	if( e->speed.x.w != 0 )
	{
		if( e->speed.x.w > 0x7FFF )
		{
			// going left
		}
		else if( e->speed.x.w > 0 )
		{
			// going right
		}
	}

	if( e->speed.y.w != 0 )
	{
		if( e->speed.y.w > 0x7FFF )
		{
			// going up
		}
		else if( e->speed.y.w > 0 )
		{
			// going down
		}
	}
}
