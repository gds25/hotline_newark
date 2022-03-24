#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "player.h"
#include "pickup.h"
#include "bullet.h"
#include "bat.h"
#include "enemy.h"

static Entity enemy = { 0 };

Entity* enemy_new(Vector2D position, int enemyType, TileMap *map) {
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

    //slog("enemytype %s", enemy);

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
    ent->range = sj_get_integer_value(sj_object_get_value(pjson, "range"), NULL);
    ent->max_walk_frame = sj_get_integer_value(sj_object_get_value(pjson, "max_walk_frame"), NULL);
    ent->max_attack_frame = sj_get_integer_value(sj_object_get_value(pjson, "max_attack_frame"), NULL);
    ent->frames_per_line = sj_get_integer_value(sj_object_get_value(pjson, "frames_per_line"), NULL);

    //todo finish filling json with correct data
    Sint32 frame_width;
    frame_width = sj_get_integer_value(sj_object_get_value(pjson, "frame_width"), NULL);
    ent->draw_offset.x = sj_get_integer_value(sj_object_get_value(pjson, "draw_offset_x"), NULL);;
    ent->rotation.x = sj_get_integer_value(sj_object_get_value(pjson, "rotation_x"), NULL);;
    ent->draw_offset.y = -16;
    ent->rotation.y = 16;
    ent->draw_scale.x = 2;
    ent->draw_scale.y = 2;


    ent->sprite = gf2d_sprite_load_all(sj_get_string_value(sj_object_get_value(pjson, "sprite")), frame_width, 32, ent->frames_per_line);
    ent->weaponName = sj_get_string_value(sj_object_get_value(pjson, "weapon_sprite"));

    ent->weapon = enemyType;
    ent->entity = ENEMY;
    ent->tileMap = map;

    ent->think = enemy_think;
    ent->update = enemy_update;

    //slog("enemy type: %i", ent->weapon);

    slog("%s", ent->weaponName);

    //ent->rotation.z = 
    vector2d_copy(ent->position, position);
    return ent;
}

void enemy_tilemap_collision(TileMap* map, Entity* ent)
{
    if (!map)return;
    if (!map->tileset)return;
    if (!map->tilemap)return;
    for (int i = 0; i < map->tilemap_count; i++)
    {
        if (map->tilemap[i]) {
            if ((i % map->tilemap_width) * map->tileset->tile_width + (map->tileset->tile_width) >= ent->mins.x &&
                (i % map->tilemap_width) * map->tileset->tile_width <= ent->maxs.x &&
                (i / map->tilemap_width) * map->tileset->tile_height + (map->tileset->tile_height) >= ent->mins.y &&
                (i / map->tilemap_width) * map->tileset->tile_height <= ent->maxs.y) {

                Vector2D direction;
                direction.x = (i % map->tilemap_width) * map->tileset->tile_width - ent->position.x;
                direction.y = (i / map->tilemap_width) * map->tileset->tile_height - ent->position.y;

                vector2d_set_magnitude(&direction, -1);
                vector2d_copy(ent->velocity, direction);
                //slog("velocity x, y = %f     %f", ent->velocity.x, ent->velocity.y);
            }
        }
    }
}

void enemy_think(Entity* self) {
    if (!self)return;
    //self->frame = (self->frame + 0.1);
    //if ((int)self->frame%7 == 5)// && 6 * (self->enemyType + 1) - 5 != (int)self->frame)
     //   self->frame -= 6;
    //slog("modulo frame/6: %i", (int)self->frame % 6);

    Vector2D playerPos;
    float invis;
    playerPos = get_player_position();
    invis = get_invis_frame(self);
    //slog("invis: %f", invis);
    float d;
    d = ((self->position.x - playerPos.x) * (self->position.x - playerPos.x)) + ((self->position.y - playerPos.y) * (self->position.y - playerPos.y));
    
    self->frame += 0.1;

    if ((d < self->range || (self->frame >= self->frames_per_line && self->frame <= self->frames_per_line + self->max_attack_frame)) &&
        invis == 0) {
        vector2d_clear(self->velocity);
        enemy_attack(self, playerPos);
    }
    else if (d < self->range*5 && invis == 0)enemy_chase(self, playerPos);
    else enemy_pace(self);
   //todo check if player invis powerup on
}

void enemy_update(Entity* self) {
    enemy_tilemap_collision(self->tileMap, self);
    vector2d_add(self->position, self->position, self->velocity);
    enemy_set_bounding_box(self);
    //slog("%i", self->health);
    if (self->health < 0) {
        //slog("health %i", self->health);
        enemy_on_death(self);
    }
}


void enemy_chase(Entity* self, Vector2D playerPos) {
    if(self->frame >= self->max_walk_frame)self->frame = 0;
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
void enemy_pace(Entity* self) {
    if (self->frame >= self->max_walk_frame)self->frame = 0;
    self->rotation.z += 0.1;
    self->velocity.x = self->velocity.x + 0.001 * sin((M_PI / 180) * self->rotation.z);
    self->velocity.y = self->velocity.y - 0.001 * cos((M_PI / 180) * self->rotation.z);
}

void enemy_attack(Entity* self, Vector2D playerPos) {
    //todo make sure each attach pattern works
    if (self->frame < self->frames_per_line || self->frame > self->frames_per_line + self->max_attack_frame)self->frame = self->frames_per_line + 1;
    Vector2D direction;
    float angle;
    direction.x = playerPos.x - self->position.x;
    direction.y = playerPos.y - self->position.y;
    angle = vector2d_angle(direction);
    self->rotation.z = angle;
    //if (self->weapon == BAT) {
        //create bounding box for bat and check for player collision
      //  }
    if (self->frame == self->frames_per_line + 1)
    {
        //slog("attacking");
        if (self->weapon != BAT) {
            //slog("shooting");
            bullet_new(vector2d((self->position.x - self->draw_offset.x - 2 * sin((M_PI / 180) * self->rotation.z)), self->position.y - self->draw_offset.y + 2 * cos((M_PI / 180) * self->rotation.z)), playerPos, self->rotation.z - 90, 1, self->damage, self->tileMap);
            if (self->weapon == SHOTGUN) {
               // slog("shotgun");
                bullet_new(vector2d(
                    (self->position.x - self->draw_offset.x - 2 * sin((M_PI / 180) * self->rotation.z)), self->position.y - self->draw_offset.y + 2 * cos((M_PI / 180) * self->rotation.z)),
                    vector2d(playerPos.x - 15 * sin((M_PI / 180) * (self->rotation.z + (float)15)), playerPos.y + 15 * cos((M_PI / 180) * (self->rotation.z + (float)15))),
                    self->rotation.z - 90, 1, self->damage, self->tileMap);
                bullet_new(vector2d(
                    (self->position.x - self->draw_offset.x - 2 * sin((M_PI / 180) * self->rotation.z)), self->position.y - self->draw_offset.y + 2 * cos((M_PI / 180) * self->rotation.z)),
                    vector2d(playerPos.x - 15 * sin((M_PI / 180) * (self->rotation.z + (float)30)), playerPos.y + 15 * cos((M_PI / 180) * (self->rotation.z + (float)30))),
                    self->rotation.z - 90, 1, self->damage, self->tileMap);
                bullet_new(vector2d(
                    (self->position.x - self->draw_offset.x - 2 * sin((M_PI / 180) * self->rotation.z)), self->position.y - self->draw_offset.y + 2 * cos((M_PI / 180) * self->rotation.z)),
                    vector2d(playerPos.x - 15 * sin((M_PI / 180) * (self->rotation.z - (float)30)), playerPos.y + 15 * cos((M_PI / 180) * (self->rotation.z - (float)30))),
                    self->rotation.z - 90, 1, self->damage, self->tileMap);
                bullet_new(vector2d(
                    (self->position.x - self->draw_offset.x - 2 * sin((M_PI / 180) * self->rotation.z)), self->position.y - self->draw_offset.y + 2 * cos((M_PI / 180) * self->rotation.z)),
                    vector2d(playerPos.x - 15 * sin((M_PI / 180) * (self->rotation.z - (float)15)), playerPos.y + 15 * cos((M_PI / 180) * (self->rotation.z - (float)15))),
                    self->rotation.z - 90, 1, self->damage, self->tileMap);
            }
        }
        else bat_new(self->position, self->damage, 1);
    }
}

void enemy_on_death(Entity *self) {
    //slog("here");
    weapon_new(self->position, self->weapon, self->ammo, self->rounds, self->damage, self->weaponName);
    entity_free(self);
}

Vector2D enemy_get_bounding_box() {
    return enemy.mins;
}

void enemy_set_bounding_box(Entity* self) {
    self->mins.x = self->position.x;
    self->mins.y = self->position.y;
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