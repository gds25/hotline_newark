#include "simple_logger.h"

#include "gfc_types.h"

#include "player.h"
#include "pickup.h"

Entity* pickup_new(Vector2D position, int pickupType, char *filename) {
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }

    ent->sprite = gf2d_sprite_load_all(filename, 24, 24, 1);
    //ent->think = pickup_think;
    //ent->update = pickup_update;
    ent->draw_offset.x = -24;
    ent->draw_offset.y = -24;
    ent->rotation.x = 12;
    ent->rotation.y = 12;
    ent->rotation.z = 0;

    ent->pickup = pickupType;
    ent->entity = PICKUP;

    slog("pickup ent type: %d", ent->entity);

    //ent->rotation.z = 
    vector2d_copy(ent->position, position);
    pickup_set_bounding_box(ent);
    return ent;
}

Entity* weapon_new(Vector2D position, int weaponType, int ammo, int rounds, int damage, char* filename) {
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }

    ent->sprite = gf2d_sprite_load_all(filename, 24, 24, 1);
    //ent->think = pickup_think;
    //ent->update = pickup_update;
    ent->draw_offset.x = -24;
    ent->draw_offset.y = -24;
    ent->rotation.x = 12;
    ent->rotation.y = 12;
    ent->rotation.z = 0;

    ent->weapon = weaponType;
    ent->entity = WEAPON;

    ent->damage = damage;

    ent->ammo = ammo;
    ent->rounds = rounds;

    slog("pickup ent type: %d", ent->entity);

    //ent->rotation.z = 
    vector2d_copy(ent->position, position);
    pickup_set_bounding_box(ent);
    return ent;
}

void pickup_set_bounding_box(Entity* self) {
    self->mins.x = self->position.x - 12;
    self->mins.y = self->position.y - 12;
    self->maxs.x = self->position.x + 12;
    self->maxs.y = self->position.y + 12;
}



/*void pickup_update(Entity* self) {
    Vector2D playerPos = player_get_bounding_box();
    //slog("pickup position: %f, %f, %f, %f", self->mins.x, self->mins.y, self->maxs.x, self->maxs.y);
    //slog("player position: %f, %f", playerPos.x, playerPos.y);
    if (self->maxs.x >= playerPos.x &&
        self->mins.x <= playerPos.x + 64 &&
        self->maxs.y >= playerPos.y &&
        self->mins.y <= playerPos.y + 64
        ) {
        // on_player_pickup(self);
        slog("here");
    }
}

void on_player_pickup(Entity* self) {
    entity_free(self);
}
*/