#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "player.h"

Entity* npc_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }

    ent->sprite = gf2d_sprite_load_all("images/player_walk.png", 32, 32, 8);
    ent->draw_scale.x = 2;
    ent->draw_scale.y = 2;
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->rotation.x = 16;
    ent->rotation.y = 16;
    ent->health = 100;
    ent->entity = NPC;

    vector2d_copy(ent->position, position);
    npc_set_aabb(ent);
    slog("created npc");
    return ent;
}

npc_set_aabb(Entity* self) {
    self->mins.x = self->position.x;
    self->mins.y = self->position.y;
    self->maxs.x = self->position.x + 32;
    self->maxs.y = self->position.y + 32;
}

//eof