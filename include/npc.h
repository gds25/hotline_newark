#ifndef __NPC_H__
#define __NPC_H__

#include "entity.h"

/**
 * @brief Create a new bullet entity
 * @param position where to spawn the bullet at (most likely at current player position)
 * @param rotation direction in which the bullet will travel (radian angle around origin)
 * @return NULL on error, or a bullet entity pointer on success
 */
Entity* npc_new(Vector2D position);



/**
 * @brief get bounding box cooridnates of bullet entity
 * @param the entity in question
 */

void npc_set_aabb(Entity* self);

#endif