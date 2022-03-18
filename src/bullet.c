#include "simple_logger.h"

#include "gfc_types.h"

#include "player.h"
#include "bullet.h"

Entity* bullet_new(Vector2D position, Vector2D playerPos, float rotation) {
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
    ent->draw_offset.x = -8;
    ent->draw_offset.y = -8;
    ent->rotation.x = 8;
    ent->rotation.y = 8;
    ent->rotation.z = rotation;

    movement.x = playerPos.x - position.x;
    movement.y = playerPos.x - position.x;
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
    if (self->position.x >= playerPos.x &&
        self->position.x <= playerPos.x + 64 &&
        self->position.y >= playerPos.y &&
        self->position.y <= playerPos.y + 64
        )
        entity_free(self);
}

/**
* @brief update bullet in world space
* @param self the entity in question
*/
void bullet_update(Entity* self) {
    vector2d_add(self->position, self->position, self->velocity);
    //slog("velocity: %f, %f", self->velocity.x, self->velocity.y);
    //slog("position: %f, %f", self->position.x, self->position.y);
}

/**
 * @brief get bounding box cooridnates of bullet entity
 * @param the entity in question
 */

void bullet_get_aabb(Entity* self);