#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <SDL.h>
#include "gf2d_sprite.h"
#include "tile_map.h"

typedef struct ENTITY_S
{
    Uint8       _inuse;     /**<this flag keeps track if this entity is active or free to reassign*/
    Sprite     *sprite;    /**<sprite used to draw the sprite*/
    float       frame;      /**<current frame to draw*/
    float       powerUp_frame_speed;      /**<current frame to draw*/
    float       powerUp_frame_invis;      /**<current frame to draw*/
    float       max_walk_frame;
    float       max_attack_frame;
    float       frames_per_line;
    Vector2D    draw_offset;/**<draw position relative to the entity position*/
    Vector2D    position;   /**<where our entity lives*/
    Vector2D    velocity;   /**<how our entity moves*/
    Vector3D    rotation;   /**<how to rotate the sprite*/
    Vector2D    draw_scale;  /**<the scale factor for drawing the sprite*/
    Vector2D    mins,maxs;  /**<describe the bounding box around this entity*/
    void (*think)(struct ENTITY_S *self);   /**<a pointer to a think function for this entity*/
    void (*update)(struct ENTITY_S *self);   /**<a pointer to an update function for this entity*/

    SDL_GameController* controller; /**<ONLY USED IN PLAYER.H - checks to see if controller is connected*/
    Vector2D crosshair_position; /**<ONLY USED IN PLAYER.H - position of crosshair*/
    int enemyType; /**<ONLY USED IN ENEMY.H - specifies enemy type*/

    enum entType { PLAYER, ENEMY, PICKUP, WEAPON, BULLET } entity;
    enum pickupType { HEALTH, ARMOR, AMMO, SPEED, INVIS } pickup; /**<ONLY USED IN PICKUP.H - specifies pickup type*/
    enum weaponType{ BAT, PISTOL, SHOTGUN, UZI, MG } weapon; /**<ONLY USED IN PICKUP.H - specifies weapon type*/
    enum bulletType{ FOR, AGAINST } bullet; /**<ONLY USED IN PICKUP.H - specifies weapon type*/
    
    int health;
    Uint32 armor;
    Uint32 damage;
    Uint32 ammo;
    Uint32 max_ammo;
    Uint32 rounds;
    Uint32 speed; /**<multiplier for velocity*/
    Uint32 range;

    Uint8 isStatic;
    Uint8 isPlayer; //is entity the player ent
    Uint8 isMonster; //is entity a monster ent
    Uint8 isPaused;
    Uint8 isDead;
    Uint8 isPoweredUp;

    Uint32 powerUpTime;

    TileMap* tileMap;
}Entity;


/**
 * @brief initialize the internal entity entity_manager_init
 * @note must be called before other entity functions
 * @param max_entities how many concurrent entities will be supported
 */
void entity_manager_init(Uint32 max_entities);

/**
 * @brief draws all active entities to the screen
 */
void entity_manager_draw_all();

/**
 * @brief runs any think function for all active entities
 */
void entity_manager_think_all();

/**
 * @brief runs any update function for all active entities
 */
void entity_manager_update_all();

/**
 * @brief free all active entities
 * @note for use in level transitions.
 */
void entity_manager_clear();

/**
 * @brief get a new empty entity
 * @returns NULL on error, or a pointer to a blank entity
 */
Entity *entity_new();

/**
 * @brief draws the given entity
 * @param entity the entity to draw
 */
void entity_draw(Entity *entity);

/**
 * @brief entity's logic
 * @param entity the entity to think
 */
void entity_think(Entity *entity);

/**
 * @brief updates position and state of the given entity
 * @param entity the entity to update
 */
void entity_update(Entity *entity);

/**
 * @brief free the memory of an entity
 * @param entity the entity to free
 */
void entity_free(Entity *entity);

/**
* @brief entity collision checker
*/
void check_collisions();

#endif
