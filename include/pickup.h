#ifndef __PICKUP_H__
#define __PICKUP_H__

#include "entity.h"

/**
 * @brief Create a new health_pack entity
 * @param position where to spawn the pickup at
 * @param pickupType which item/powerup will this be
 * @param filename name of image file to load
 * @return NULL on error, or a health_pack entity pointer on success
 */
Entity* pickup_new(Vector2D position, int pickupType, char* filename);

/**
* @brief enemy entity logic
* @param self the entity in question
*/
void pickup_think(Entity* self);

/**
* @brief update enemy entity
* @param self the entity in question
*/
void pickup_update(Entity* self);

/**
 * @brief set bounding box cooridnates of health_pack entity
 * @param the entity in question
 */
void pickup_set_bounding_box(Entity* self);


/**
 * @brief what happens when the player picks this up
 * @param the entity in question
 */
void on_player_pickup(Entity* self);

#endif