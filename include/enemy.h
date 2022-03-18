#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

/**
 * @brief Create a new enemy entity
 * @param position where to spawn the enemy at
 * @param filename JSON file that stores world info
 * @return NULL on error, or an enemy entity pointer on success
 */
Entity* enemy_new(Vector2D position, int enemyType);


/**
* @brief enemy entity logic
* @param self the entity in question
*/
void enemy_think(Entity* self);

/**
* @brief update enemy in the world
* @param self the entity in question
*/
void enemy_update(Entity* self);

/**
* @brief enemy should walk toward player entity "chasing" after them
* @param self the entity in question
* @param playerPos the position of the player
*/
void enemy_chase(Entity* self, Vector2D playerPos);

/**
* @brief enemy should walk around aimlessly **TODO** switch rand() for a different random number generator
* @param self the entity in question
*/
void enemy_pace(Entity* self);

/**
* @brief change attack animation frame and calculate weapon collision points
* @param self the entity in question
*/
void enemy_attack(Entity* self, Vector2D playerPos);

/**
 * @brief get bounding box cooridnates of enemy entity
 * @param the entity in question
 */
Vector2D enemy_get_bounding_box();

/**
 * @brief set bounding box cooridnates of enemy entity
 * @param the entity in question
 */
void enemy_set_bounding_box(Entity* self);

/**
 * @brief get bounding box cooridnates of swipe/punch attack
 * @param the entity in question
 */
Vector2D attack_get_bounding_box(Entity* self);

/**
 * @brief free entity on death and drop a health_pack entity
 * @param the entity in question
 */
void enemy_die(Entity* self);

#endif

