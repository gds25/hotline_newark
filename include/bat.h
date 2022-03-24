#ifndef __BAT_H__
#define __BAT_H__

#include "entity.h"

Entity* bat_new(Vector2D position, int damage, enum bulletType bat);

void bat_think(Entity* self);

void bat_update(Entity* self);

#endif
