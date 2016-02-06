
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
