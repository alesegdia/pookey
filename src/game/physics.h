
#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "entity.h"
#include "map.h"

// checks if grounded bit is active
int isGrounded( entity_t* e );

// sets the grounded bit to true
void setGrounded( entity_t* e );

// applies gravity to entity speed
void applyGravity( entity_t* e );

// applies entity speed to entity position
void applySpeed( entity_t* e );

void handleMapCollision( entity_t* e, map_t* map );

#endif

