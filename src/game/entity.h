
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "vec2w.h"

typedef struct  {
	vec2w_t pos;
	vec2w_t speed;
	UBYTE flags;
} entity_t;

#endif
