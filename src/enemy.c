#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "player.h"
#include "pickup.h"
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

    SJson* json, * pjson, * mjson;
    char* enemy[8];
    snprintf(enemy, 8, "%i", enemyType);

    slog("enemytype %s", enemy);

    json = sj_load("config/enemy.json");
    if (!json)
    {
        slog("failed to load json file config/enemy.json for enemy stats");
        free(ent);
        return NULL;
    }
    pjson = sj_object_get_value(json, enemy);
    if (!pjson)
    {
        slog("failed to find enemy object in config/enemy.json");
        free(ent);
        sj_free(json);
        return NULL;
    }
    ent->health = sj_get_integer_value(sj_object_get_value(pjson, "health"), NULL);
    ent->ammo = sj_get_integer_value(sj_object_get_value(pjson, "ammo"), NULL);
    ent->rounds = sj_get_integer_value(sj_object_get_value(pjson, "rounds"), NULL);
    ent->max_ammo = sj_get_integer_value(sj_object_get_value(pjson, "max_ammo"), NULL);
    ent->damage = sj_get_integer_value(sj_object_get_value(pjson, "damage"), NULL);
    ent->speed = sj_get_integer_value(sj_object_get_value(pjson, "speed"), NULL);
    ent->sprite = gf2d_sprite_load_all(sj_get_string_value(sj_object_get_value(pjson, "sprite")), 32, 32, 8);
    ent->enemyType = enemyType;
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->rotation.x = 8;
    ent->rotation.y = 8;
    ent->entity = ENEMY;

    ent->think = enemy_think;
    ent->update = enemy_update;
    //TODO: set enemy stats based off enemy type in json

    slog("enemy health: %i", ent->health);
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
    enemy_chase(self, playerPos);
}

void enemy_update(Entity* self) {
    vector2d_add(self->position, self->position, self->velocity);
    enemy_set_bounding_box(self);
    //slog("%i", self->health);
    if (self->health < 0) {
        //slog("health %i", self->health);
        enemy_on_death(self);
    }
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
    vector2d_scale(self->velocity, self->velocity, self->speed);
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
       //slog("here");
    if (self->ammo > 0) {
        bullet_new(self->position, playerPos, self->rotation.z, 1, self->damage);
        self->ammo--;
    }
}

void enemy_on_death(Entity *self) {
    slog("here");
    weapon_new(self->position, self->enemyType, self->ammo, self->rounds, self->damage, "images/pistol.png");
    entity_free(self);
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