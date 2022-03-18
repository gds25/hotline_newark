
#include "simple_logger.h"

#include "entity.h"
#include "player.h"


typedef struct
{
    Uint32 max_entities;            /**<how many entities exist*/
    Entity  *entity_list;           /**<a big ole list of entities*/
	Uint32  current_ent_total;
}EntityManager;

static EntityManager entity_manager = {0};


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
			slog("total ents: %i", entity_manager.current_ent_total);
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
    int i;
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_think(&entity_manager.entity_list[i]);
    }
}

void entity_manager_update_all()
{
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
			//if (entity_manager.entity_list[i].entity == PLAYER)slog("player ent %i, %i", i, entity_manager.entity_list[i].entity);
			//if (entity_manager.entity_list[j].entity == PICKUP)slog("pickup ent %i, %i", j, entity_manager.entity_list[j].entity);
			if (entity_manager.entity_list[i].mins.x <= entity_manager.entity_list[j].maxs.x &&
				entity_manager.entity_list[i].mins.y <= entity_manager.entity_list[j].maxs.y &&
				entity_manager.entity_list[i].maxs.x >= entity_manager.entity_list[j].mins.x &&
				entity_manager.entity_list[i].maxs.y >= entity_manager.entity_list[j].mins.y) {
				//slog("collision %i, %i", i, j);
				if ((entity_manager.entity_list[i].entity == PLAYER && entity_manager.entity_list[j].entity == PICKUP)) {
					slog("collision %i, %i", i, j);
					player_set_stats(&entity_manager.entity_list[i], entity_manager.entity_list[j].pickup);
					entity_free(&entity_manager.entity_list[j]);
				}
				//vector2d_clear(entity_manager.entity_list[i].velocity);
				//vector2d_clear(entity_manager.entity_list[j].velocity);
			}

		}
	}
			/*if (entity_manager.entity_list[i].isPlayer && entity_manager.entity_list[j].isMonster) {
				//slog("here");
				if (entity_manager.entity_list[i].attackFrame > 4 && entity_manager.entity_list[i].attackFrame < 11 && !entity_manager.entity_list[j].attackedThisSwing) {
					if (entity_manager.entity_list[i].maxWeaponAABB.x >= entity_manager.entity_list[j].minAABB.x &&
						entity_manager.entity_list[i].maxWeaponAABB.y >= entity_manager.entity_list[j].minAABB.y &&
						entity_manager.entity_list[i].maxWeaponAABB.z >= entity_manager.entity_list[j].minAABB.z &&
						entity_manager.entity_list[i].minWeaponAABB.x <= entity_manager.entity_list[j].maxAABB.x &&
						entity_manager.entity_list[i].minWeaponAABB.y <= entity_manager.entity_list[j].maxAABB.y &&
						entity_manager.entity_list[i].minWeaponAABB.z <= entity_manager.entity_list[j].maxAABB.z) {
						entity_manager.entity_list[j].attackedThisSwing = 1;
						entity_manager.entity_list[j].health -= entity_manager.entity_list[i].damage;
						slog("damaged enemy; enemy health: %i", entity_manager.entity_list[j].health);
					}
				}
				else if (entity_manager.entity_list[i].attackFrame > 10 || (entity_manager.entity_list[i].character == 3 && entity_manager.entity_list[i].attackFrame > 8)) entity_manager.entity_list[j].attackedThisSwing = 0;
			}
			else if (entity_manager.entity_list[i].isMonster && entity_manager.entity_list[j].isPlayer) {
				if (entity_manager.entity_list[i].attackFrame > 4 && entity_manager.entity_list[i].attackFrame < 11 && !entity_manager.entity_list[i].hasAttacked) {
					if (entity_manager.entity_list[i].maxWeaponAABB.x >= entity_manager.entity_list[j].minAABB.x &&
						entity_manager.entity_list[i].maxWeaponAABB.y >= entity_manager.entity_list[j].minAABB.y &&
						entity_manager.entity_list[i].maxWeaponAABB.z >= entity_manager.entity_list[j].minAABB.z &&
						entity_manager.entity_list[i].minWeaponAABB.x <= entity_manager.entity_list[j].maxAABB.x &&
						entity_manager.entity_list[i].minWeaponAABB.y <= entity_manager.entity_list[j].maxAABB.y &&
						entity_manager.entity_list[i].minWeaponAABB.z <= entity_manager.entity_list[j].maxAABB.z) {
						entity_manager.entity_list[i].hasAttacked = 1;
						entity_manager.entity_list[j].health = MAX(1, entity_manager.entity_list[j].health - (entity_manager.entity_list[i].damage / entity_manager.entity_list[j].armor));

						slog("you took damage! health is now: %i", entity_manager.entity_list[j].health);
						/*slog("player pos x, y, z: %f, %f, %f", entity_manager.entity_list[i].position.x, entity_manager.entity_list[i].position.y, entity_manager.entity_list[i].position.z);
						slog("weapon max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", entity_manager.entity_list[i].maxWeaponAABB.x, entity_manager.entity_list[i].maxWeaponAABB.y, entity_manager.entity_list[i].maxWeaponAABB.z, entity_manager.entity_list[i].minWeaponAABB.x, entity_manager.entity_list[i].minWeaponAABB.y, entity_manager.entity_list[i].minWeaponAABB.z);
						slog("monster max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", entity_manager.entity_list[j].maxAABB.x, entity_manager.entity_list[j].maxAABB.y, entity_manager.entity_list[j].maxAABB.z, entity_manager.entity_list[j].minAABB.x, entity_manager.entity_list[j].minAABB.y, entity_manager.entity_list[j].minAABB.z);
						slog("monster pos x, y, z: %f, %f, %f", entity_manager.entity_list[j].position.x, entity_manager.entity_list[j].position.y, entity_manager.entity_list[j].position.z);*/
			/* }
				}
				else if (entity_manager.entity_list[i].attackFrame > 10) entity_manager.entity_list[i].hasAttacked = 0;
			}

			else if (entity_manager.entity_list[i].maxAABB.x >= entity_manager.entity_list[j].minAABB.x &&
				entity_manager.entity_list[i].maxAABB.y >= entity_manager.entity_list[j].minAABB.y &&
				entity_manager.entity_list[i].maxAABB.z >= entity_manager.entity_list[j].minAABB.z &&
				entity_manager.entity_list[i].minAABB.x <= entity_manager.entity_list[j].maxAABB.x &&
				entity_manager.entity_list[i].minAABB.y <= entity_manager.entity_list[j].maxAABB.y &&
				entity_manager.entity_list[i].minAABB.z <= entity_manager.entity_list[j].maxAABB.z) {
				//slog("hit entities %i, %i", i, j);
				//slog("i max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", entity_manager.entity_list[i].maxAABB.x, entity_manager.entity_list[i].maxAABB.y, entity_manager.entity_list[i].maxAABB.z, entity_manager.entity_list[i].minAABB.x, entity_manager.entity_list[i].minAABB.y, entity_manager.entity_list[i].minAABB.z);
				//slog("j max AABB x, y, z: %f, %f, %f, min AABB x, y, z: %f, %f, %f", entity_manager.entity_list[j].maxAABB.x, entity_manager.entity_list[j].maxAABB.y, entity_manager.entity_list[j].maxAABB.z, entity_manager.entity_list[j].minAABB.x, entity_manager.entity_list[j].minAABB.y, entity_manager.entity_list[j].minAABB.z);
				if (entity_manager.entity_list[i].isFireball && !entity_manager.entity_list[j].isPlayer) {
					if (entity_manager.entity_list[j].isMonster) {
						entity_manager.entity_list[j].health -= 20;
						//slog("damaged enemy; enemy health: %i", entity_manager.entity_list[j].health);
					}
					entity_free(&entity_manager.entity_list[i]);
				}
				else if (entity_manager.entity_list[j].isFireball && !entity_manager.entity_list[i].isPlayer) {
					if (entity_manager.entity_list[i].isMonster) {
						entity_manager.entity_list[i].health -= 20;
						//slog("damaged enemy; enemy health: %i", entity_manager.entity_list[i].health);
					}
					entity_free(&entity_manager.entity_list[j]);
				}
				else if (entity_manager.entity_list[j].isHealth && entity_manager.entity_list[i].isPlayer && entity_manager.entity_list[i].health < 100) {
					entity_free(&entity_manager.entity_list[j]);

					entity_manager.entity_list[i].health = MIN(100, entity_manager.entity_list[i].health + 20);
					slog("picked up health pack, health is now: %i", entity_manager.entity_list[i].health);
				}
				else if (!entity_manager.entity_list[i].isStatic) {
					entity_manager.entity_list[i].futurePosition.x = entity_manager.entity_list[i].position.x - (0.01 * cos(entity_manager.entity_list[i].rotation.z));
					entity_manager.entity_list[i].futurePosition.y = entity_manager.entity_list[i].position.y + (0.01 * sin(entity_manager.entity_list[i].rotation.z));
					entity_manager.entity_list[i].futurePosition.z = entity_manager.entity_list[i].position.z;
				}

				//entity_manager.entity_list[i].position.x -= entity_manager.entity_list[i].velocity.x * (0.10 * sin(entity_manager.entity_list[i].rotation.z));
				//entity_manager.entity_list[i].position.y += entity_manager.entity_list[i].velocity.y * (0.10 * cos(entity_manager.entity_list[i].rotation.z));
				//entity_manager.entity_list[i].position.z;
			}
			else if (entity_manager.entity_list[i].futurePosition.x < 295 && entity_manager.entity_list[i].futurePosition.y < 295 && entity_manager.entity_list[i].futurePosition.x > -295 && entity_manager.entity_list[i].futurePosition.y > -295) {
				if (entity_manager.entity_list[i].position.x > entity_manager.entity_list[i].futurePosition.x && entity_manager.entity_list[i].futurePosition.x < 10 && entity_manager.entity_list[i].futurePosition.x > -10 && !(entity_manager.entity_list[i].futurePosition.y < 273 && entity_manager.entity_list[i].futurePosition.y > 176) && !(entity_manager.entity_list[i].futurePosition.y > -273 && entity_manager.entity_list[i].futurePosition.y < -176)) {
					entity_manager.entity_list[i].futurePosition.x += 1; entity_manager.entity_list[i].position.x += 1;
				}
				else if (entity_manager.entity_list[i].position.x <  entity_manager.entity_list[i].futurePosition.x && entity_manager.entity_list[i].futurePosition.x > -10 && entity_manager.entity_list[i].futurePosition.x < 10 && !(entity_manager.entity_list[i].futurePosition.y < 273 && entity_manager.entity_list[i].futurePosition.y > 176) && !(entity_manager.entity_list[i].futurePosition.y > -273 && entity_manager.entity_list[i].futurePosition.y < -176)) {
					entity_manager.entity_list[i].futurePosition.x -= 1; entity_manager.entity_list[i].position.x -= 1;
				}
				else if (entity_manager.entity_list[i].position.y > entity_manager.entity_list[i].futurePosition.y && entity_manager.entity_list[i].futurePosition.y < 10 && entity_manager.entity_list[i].futurePosition.y > -10 && !(entity_manager.entity_list[i].futurePosition.x > -273 && entity_manager.entity_list[i].futurePosition.x < -176)) {
					entity_manager.entity_list[i].futurePosition.y += 1; entity_manager.entity_list[i].position.y += 1;
				}
				else if (entity_manager.entity_list[i].position.x < entity_manager.entity_list[i].futurePosition.y && entity_manager.entity_list[i].futurePosition.y > -10 && entity_manager.entity_list[i].futurePosition.y < 10 && !(entity_manager.entity_list[i].futurePosition.x > -273 && entity_manager.entity_list[i].futurePosition.x < -176)) {
					entity_manager.entity_list[i].futurePosition.y -= 1; entity_manager.entity_list[i].position.y -= 1;
				}
				else {
					entity_manager.entity_list[i].position.x = entity_manager.entity_list[i].futurePosition.x;
					entity_manager.entity_list[i].position.y = entity_manager.entity_list[i].futurePosition.y;
					entity_manager.entity_list[i].position.z = entity_manager.entity_list[i].futurePosition.z;
				}
			}
			else {
				if (entity_manager.entity_list[i].futurePosition.x > 295) { entity_manager.entity_list[i].futurePosition.x -= 10; entity_manager.entity_list[i].position.x -= 10; }
				if (entity_manager.entity_list[i].futurePosition.y > 295) { entity_manager.entity_list[i].futurePosition.y -= 10; entity_manager.entity_list[i].position.y -= 10; }
				if (entity_manager.entity_list[i].futurePosition.x < -295) { entity_manager.entity_list[i].futurePosition.x += 10; entity_manager.entity_list[i].position.x += 10; }
				if (entity_manager.entity_list[i].futurePosition.y < -295) { entity_manager.entity_list[i].futurePosition.y += 10; entity_manager.entity_list[i].position.y += 10; }

			}
		}
	}*/
}

// eof
