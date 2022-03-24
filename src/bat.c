#include "simple_logger.h"

#include "player.h"
#include "bat.h"

Entity* bat_new(Vector2D position, int damage, enum bulletType bat)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }
    ent->sprite = NULL;
    ent->think = bat_think;
    ent->update = bat_update;

    ent->damage = damage;
    ent->entity = BULLET;
    ent->bullet = bat;
    //slog("player ent type: %i", ent->entity);
    //if (ent->entity != PLAYER) slog("this works");

    //ent->rotation.z = ;
    vector2d_copy(ent->position, position);

    ent->weaponName = "this is the bat entity";

    //slog("%s", ent->weaponName);

    ent->mins.x = ent->position.x - 8;
    ent->mins.y = ent->position.y - 8;
    ent->maxs.x = ent->position.x + 40;
    ent->maxs.y = ent->position.y + 40;
    //slog("no space for bugs");
    return ent;
}

void bat_think(Entity* self) {
    self->frame += 0.1;
    if (self->frame >= 9) entity_free(self);
}

void bat_update(Entity* self) {
    Vector2D playerPos;
    playerPos = player_get_bounding_box();
    if (self->bullet == AGAINST &&
        self->maxs.x >= playerPos.x &&
        self->mins.x <= playerPos.x + 64 &&
        self->maxs.y >= playerPos.y &&
        self->mins.y <= playerPos.y + 64
        ) {
        entity_free(self);
        //slog("entity should be freed");
    }
}


//eof
