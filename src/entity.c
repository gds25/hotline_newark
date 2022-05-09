
#include "simple_logger.h"

#include "entity.h"
#include "player.h"

#include "tile_map.h"


typedef struct
{
    Uint32 max_entities;            /**<how many entities exist*/
    Entity  *entity_list;           /**<a big ole list of entities*/
	Uint32  current_ent_total;
    int state;
}EntityManager;

static EntityManager entity_manager = {0};

void entity_set_game_state(int state) {
    entity_manager.state = state;
}

void entity_manager_close()
{
    entity_manager_clear(); // clear all entities first
    if(entity_manager.entity_list != NULL)
    {
        free(entity_manager.entity_list);
    }
    slog("entity manager closed");    
}

void entity_manager_init(Uint32 max_entities)
{
    if (max_entities == 0)
    {
        slog("cannot allocate memory for zero entities!");
        return;
    }
    if (entity_manager.entity_list != NULL)
    {
        slog("entity manager already initialized");
        return;
    }
    entity_manager.max_entities = max_entities;
    entity_manager.entity_list = gfc_allocate_array(sizeof(Entity),max_entities);
    atexit(entity_manager_close);
    slog("entity manager initialized");
}

void entity_manager_clear()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_free(&entity_manager.entity_list[i]);
    }
}

Entity *entity_new()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)
        {
            //GOT ONE!
            entity_manager.entity_list[i]._inuse = 1;
            entity_manager.entity_list[i].draw_scale.x = 1;
            entity_manager.entity_list[i].draw_scale.y = 1;
			entity_manager.current_ent_total++;
			//slog("total ents: %i", entity_manager.current_ent_total);
            return &entity_manager.entity_list[i];
        }
    }
    slog("out of entities");
    return NULL;
}

void entity_think(Entity *ent)
{
    
    if (!ent)return;
    //generic upkeep
   
    if (ent->think)
    {
        ent->think(ent);
    }
    
}



void entity_update(Entity *ent)
{
    if (!ent)return;
    //generic upkeep
    
    if (ent->update)
    {
        ent->update(ent);
    }

}

void entity_manager_think_all()
{
    if (entity_manager.state != 3) return;
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_think(&entity_manager.entity_list[i]);
    }
}

void entity_manager_update_all()
{
    if (entity_manager.state != 3) { return; }
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_update(&entity_manager.entity_list[i]);
    }
}

void entity_manager_draw_all()
{
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_draw(&entity_manager.entity_list[i]);
    }
}

void entity_draw(Entity *entity)
{
    Vector2D drawPosition;
    if (entity == NULL)
    {
        slog("null pointer provided, nothing to do!");
        return;
    }
    if (entity->sprite == NULL)return;// nothing to draw
    vector2d_add(drawPosition,entity->position,entity->draw_offset);
    gf2d_sprite_draw(
        entity->sprite,        
        drawPosition,
        &entity->draw_scale,
        NULL,
        &entity->rotation,
        NULL,
        NULL,
        (Uint32)entity->frame);
}

void entity_free(Entity *entity)
{
    if (entity == NULL)
    {
        slog("null pointer provided, nothing to do!");
        return;
    }
    if (entity->sprite != NULL)
    {
        gf2d_sprite_free(entity->sprite);
    }
	//entity_manager.entity_list[i]._inuse = 0;
    memset(entity,0,sizeof(Entity));
}

void check_collisions() {
    for (int i = 0; i < entity_manager.current_ent_total; i++) {
        for (int j = 0; j < entity_manager.current_ent_total; j++) {
            if (i == j) continue;
            if (entity_manager.entity_list[i].mins.x <= entity_manager.entity_list[j].maxs.x &&
                entity_manager.entity_list[i].mins.y <= entity_manager.entity_list[j].maxs.y &&
                entity_manager.entity_list[i].maxs.x >= entity_manager.entity_list[j].mins.x &&
                entity_manager.entity_list[i].maxs.y >= entity_manager.entity_list[j].mins.y) {

                if (entity_manager.entity_list[i].entity == PLAYER && entity_manager.entity_list[j].entity == PICKUP) {
                    slog("collision %i, %i", i, j);
                    player_set_stats(&entity_manager.entity_list[i], entity_manager.entity_list[j].pickup);
                    entity_free(&entity_manager.entity_list[j]);

                }
                else if (entity_manager.entity_list[i].entity == PLAYER && entity_manager.entity_list[j].entity == WEAPON) {
                    if (SDL_GameControllerGetButton(entity_manager.entity_list[i].controller,
                        SDL_CONTROLLER_BUTTON_A)) {
                        slog("collision %i, %i", i, j);
                        player_set_weapon(&entity_manager.entity_list[i], entity_manager.entity_list[j].weapon, entity_manager.entity_list[j].ammo, entity_manager.entity_list[j].rounds, entity_manager.entity_list[j].damage);
                        entity_free(&entity_manager.entity_list[j]);
                    }
                }
                else if (entity_manager.entity_list[i].entity == ENEMY && entity_manager.entity_list[j].bullet == FOR && entity_manager.entity_list[j].entity == BULLET) {
                    entity_manager.entity_list[i].health -= entity_manager.entity_list[j].damage;
                    if (entity_manager.entity_list[i].health <= 0) {
                        for (int h = 0; h < entity_manager.current_ent_total; h++) {
                            if (entity_manager.entity_list[h].entity == PLAYER)
                                entity_manager.entity_list[h].score += 1000 * (1 + entity_manager.entity_list[i].weapon * 0.5);
                        }
                    }



                    entity_free(&entity_manager.entity_list[j]);
                }
                else if (entity_manager.entity_list[i].entity == PLAYER && entity_manager.entity_list[j].bullet == AGAINST && entity_manager.entity_list[j].entity == BULLET) {

                    if (entity_manager.entity_list[i].armor > 0) {

                        entity_manager.entity_list[i].armor -= entity_manager.entity_list[j].damage;
                        if (entity_manager.entity_list[i].armor < 0) {
                            entity_manager.entity_list[i].health += entity_manager.entity_list[j].armor;
                            entity_manager.entity_list[i].armor = 0;
                        }
                    }
                    else {
                        entity_manager.entity_list[i].health -= entity_manager.entity_list[j].damage;
                    }
                    if (entity_manager.entity_list[i].health <= 0) {
                        slog("here");
                        entity_manager.entity_list[i].state = 0;
                        entity_manager.entity_list[i].score /= 2;
                        entity_manager.entity_list[i].health = 100;
                    }

                }
                else if (entity_manager.entity_list[i].entity == PLAYER && entity_manager.entity_list[j].entity == HOSTAGE) {
                    if (SDL_GameControllerGetButton(entity_manager.entity_list[i].controller,
                        SDL_CONTROLLER_BUTTON_A)) {
                        entity_manager.entity_list[i].state = 4;
                    }
                }
                else if (entity_manager.entity_list[i].entity == PLAYER && entity_manager.entity_list[j].entity == NPC) {
                    if (SDL_GameControllerGetButton(entity_manager.entity_list[i].controller,
                        SDL_CONTROLLER_BUTTON_A)) {
                        entity_manager.entity_list[i].state = 2;
                    }
                }
                else if (entity_manager.entity_list[i].entity == BOSS && entity_manager.entity_list[j].bullet == FOR && entity_manager.entity_list[j].entity == BULLET) {
                    entity_manager.entity_list[i].health -= entity_manager.entity_list[j].damage;
                    entity_free(&entity_manager.entity_list[j]);
                    if (entity_manager.entity_list[i].health <= 0) {
                        for (int i = 0; i < entity_manager.current_ent_total; i++) {
                            if (entity_manager.entity_list[i].entity == PLAYER)
                                entity_manager.entity_list[i].state = 0;
                        }
                    }
                }
            }

        }


    }
}

// eof
