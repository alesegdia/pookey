
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "vec2w.h"

typedef struct  {
	UBYTE pos_offset;
	vec2w_t pos;
	vec2w_t speed;
	UBYTE flags;

	struct {
		UBYTE left;
		UBYTE right;
		UBYTE top;
		UBYTE bot;
	} bounds;

} entity_t;

#endif
