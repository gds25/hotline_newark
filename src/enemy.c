#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "player.h"
#include "bullet.h"
#include "enemy.h"

static Entity enemy = { 0 };

Entity* enemy_new(Vector2D position, int enemyType) {
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/enemy.png", 128, 128, 6);
    ent->entity = ENEMY;
    ent->enemyType = enemyType;
    ent->think = enemy_think;
    ent->update = enemy_update;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->ammo = 1;

    slog("enemy ent type: %i", ent->entity);
    //ent->rotation.z = 
    vector2d_copy(ent->position, position);
    return ent;
}

void enemy_think(Entity* self) {
    if (!self)return;
    self->frame = (self->frame + 0.1);
    if ((int)self->frame%7 == 5)// && 6 * (self->enemyType + 1) - 5 != (int)self->frame)
        self->frame -= 6;
    //slog("modulo frame/6: %i", (int)self->frame % 6);

    Vector2D playerPos;
    playerPos = get_player_position();
    float d;
    d = ((self->position.x - playerPos.x) * (self->position.x - playerPos.x)) + ((self->position.y - playerPos.y) * (self->position.y - playerPos.y));
    //if (d > 500*self->enemyType)enemy_chase(self, playerPos);
    //else 
        enemy_attack(self, playerPos);
}

void enemy_update(Entity* self) {
    vector2d_add(self->position, self->position, self->velocity);
    enemy_set_bounding_box(self);
    //slog("%f, %f", self->position.x, self->position.y);
}


void enemy_chase(Entity* self, Vector2D playerPos) {
    if (self->frame >6)self->frame = 0;
    Vector2D direction;
    float angle;
    direction.x = playerPos.x - self->position.x;
    direction.y = playerPos.y - self->position.y;
    angle = vector2d_angle(direction);
    self->rotation.z = angle;
    //slog("%f, %f", self->rotation.z);
    vector2d_set_magnitude(&direction, 1);
    vector2d_copy(self->velocity, direction);
    //slog("%f, %f", playerPos.x, playerPos.y);
}

/**
* @brief enemy should walk around aimlessly **TODO** switch rand() for a different random number generator
* @param self the entity in question
*/
void enemy_pace(Entity* self);


void enemy_attack(Entity* self, Vector2D playerPos) {
    if (self->frame < 7)self->frame = 6*(self->enemyType)+1;
    Vector2D direction;
    float angle;
    direction.x = playerPos.x - self->position.x;
    direction.y = playerPos.y - self->position.y;
    angle = vector2d_angle(direction);
    self->rotation.z = angle;

    //if (self->frame >= 6 * (self->enemyType) + 3 && self->frame >= 6 * (self->enemyType) + 3.01) {
    //    slog("here");
    if (self->ammo > 0) {
        bullet_new(self->position, playerPos, self->rotation.z);
        self->ammo--;
    }
       

   // slog("frame: %f", self->frame);

  //  vector2d_scale(self->velocity, self->velocity, 0.5);
  //  if (vector2d_magnitude(self->velocity) < 0.05)
  //  {
  //      vector2d_clear(self->velocity);
  //  }
}

Vector2D enemy_get_bounding_box() {
    return enemy.mins;
}

void enemy_set_bounding_box(Entity* self) {
    self->mins.x = self->position.x - 32;
    self->mins.y = self->position.y - 32;
    self->maxs.x = self->position.x + 32;
    self->maxs.y = self->position.y + 32;

    enemy.mins.x = self->mins.x;
    enemy.mins.y = self->mins.y;
    enemy.maxs.x = self->maxs.x;
    enemy.maxs.y = self->maxs.y;
}

/**
 * @brief get bounding box cooridnates of swipe/punch attack
 * @param the entity in question
 */
void attack_get_aabb(Entity* self);

/**
 * @brief free entity on death and drop a health_pack entity
 * @param the entity in question
 */
void enemy_die(Entity* self);