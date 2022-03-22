#ifndef __BULLET_H__
#define __BULLET_H__

#include "entity.h"

/**
 * @brief Create a new bullet entity
 * @param position where to spawn the bullet at (most likely at current player position)
 * @param rotation direction in which the bullet will travel (radian angle around origin)
 * @return NULL on error, or a bullet entity pointer on success
 */
Entity* bullet_new(Vector2D position, Vector2D goalPos, float rotation, enum bulletType bullet, int damage);

/**
* @brief bullet logic
* @param self the entity in question
*/
void bullet_think(Entity* self);

/**
* @brief update bullet in world space
* @param self the entity in question
*/
void bullet_update(Entity* self);

/**
 * @brief get bounding box cooridnates of bullet entity
 * @param the entity in question
 */

void bullet_get_aabb(Entity* self);

#endif