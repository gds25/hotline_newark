#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

Entity get_player();

/**
 * @brief Create a new player entity
 * @param position where to spawn the player at
 * @param filename JSON file that stores player info
 * @return NULL on error, or an player entity pointer on success
 */
Entity* player_new(Vector2D position, SDL_GameController* gameController, TileMap* map);

/**
* @brief player logic
* @param self the player entity
*/
void player_think(Entity* self);

/**
* @brief update entity in the world
* @param self the player entity
*/
void player_update(Entity* self);

/**
* @brief translate player position on z-axis
* @param self the player entity
*/
void player_jump(Entity* self);

/**
* @brief animate player rolling to the left or right
* @param self the player entity
*/
void player_roll(Entity* self);

/**
* @brief animate player on death
* @param self the player entity
*/
void player_on_death(Entity* self);

/**
* @brief change attack animation frame and calculate weapon collision points
* @param self the player entity
*/
void player_attack(Entity* self);

/**
* @brief get player position
* @return player entity position
*/
Vector2D get_player_position();

/**
* @brief set player position to use in monster.c
* @param self the player entity
*/
void set_player_position(Entity* self);

/**
* @brief get player health
* @return player entity health
*/
int get_player_health();

/**
* @brief set player health (for UI)
* @param self the player entity
*/
void set_player_health(Entity* self);

int get_player_armor();

void set_player_armor(Entity* self);

void set_player_ammo(Entity* self);

int get_player_ammo(); 

int get_player_rounds();

/**
 * @brief set bounding box cooridInates of PLayer entity
 * @param self the player entity
 */
void player_set_bounding_box(Entity* self);

/**
 * @brief get min bounding box cooridInates of PLayer entity
 * @param self the player entity
 */
Vector2D player_get_bounding_box();


void set_invis_frame(Entity* self);

float get_invis_frame();

/**
 * @brief check to see if game is paused
 * @param self the player entity
 */
Uint8 player_get_pause_state(Entity* self);

/**
* @brief set pause state to see whether to draw main menu
* @param self the player entity
*/
void player_set_pause_state(Entity* self);

Vector2D get_crosshair_position(Entity* self);

void player_set_stats(Entity* self, int pickupType);

void player_set_weapon(Entity* self, int weaponType, int ammo, int rounds, int damage);

void player_tilemap_collision(TileMap* tileMap, Entity* self);

#endif
