#include "simple_logger.h"

#include "gfc_types.h"

#include "player.h"
#include "bullet.h"

Entity* bullet_new(Vector2D position, Vector2D goalPos, float rotation, int bulletType, int damage, TileMap *map) {
    Entity* ent;
    Vector2D movement;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/bullet.png", 16, 16, 1);
    ent->think = bullet_think;
    ent->update = bullet_update;
    ent->entity = BULLET;
    ent->bullet = bulletType;
    ent->draw_offset.x = -8;
    ent->draw_offset.y = -8;
    ent->rotation.x = 8;
    ent->rotation.y = 8;
    ent->rotation.z = rotation-90;
    ent->damage = damage;
    ent->tileMap = map;

    movement.x = goalPos.x - position.x;
    movement.y = goalPos.y - position.y;
    vector2d_copy(ent->position, position);

    vector2d_set_magnitude(&movement, 1);
    vector2d_copy(ent->velocity, movement);
    //slog("created bullet");
    //slog("velocity: %f, %f", ent->velocity.x, ent->velocity.y);
    return ent;
}

/**
* @brief bullet logic
* @param self the entity in question
*/
void bullet_think(Entity* self) {
    Vector2D playerPos = player_get_bounding_box();
    /*if (self->position.x >= playerPos.x &&
        self->position.x <= playerPos.x + 64 &&
        self->position.y >= playerPos.y &&
        self->position.y <= playerPos.y + 64
        )*/
        //entity_free(self);
}

/**
* @brief update bullet in world space
* @param self the entity in question
*/
void bullet_update(Entity* self) {
    vector2d_add(self->position, self->position, self->velocity);
    vector2d_copy(self->mins, self->position);
    vector2d_copy(self->maxs, self->position);
    bullet_set_aabb(self);
    bullet_tilemap_collision(self->tileMap, self);
    //slog("velocity: %f, %f", self->velocity.x, self->velocity.y);
    //slog("position: %f, %f", self->position.x, self->position.y);
}


void bullet_set_aabb(Entity* self) {
    self->mins.x = self->position.x;
    self->mins.y = self->position.y;
    self->maxs.x = self->position.x;
    self->maxs.y = self->position.y;
}

void bullet_tilemap_collision(TileMap* map, Entity* ent)
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
                /*slog("max tile x %i", (i % map->tilemap_width) * map->tileset->tile_width + (map->tileset->tile_width / 2));
                slog("min player x %f", ent->mins.x);
                slog("min tile x %i", (i % map->tilemap_width) * map->tileset->tile_width - (map->tileset->tile_width / 2));
                slog("max player x %f", ent->maxs.x);
                */
                slog("bullet");
                entity_free(ent);
            }
        }
    }
}