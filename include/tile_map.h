#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "gfc_text.h"

#include "tile_set.h"
#include "entity.h"

typedef struct
{
    TextLine filename;      //map file
    TileSet *tileset;        // which tiles to draw
    Uint32  *tilemap;
    Uint32   tilemap_count;  // how many tiles we have
    Uint32   tilemap_width;  // how many tiles across we have
}TileMap;

TileMap *tilemap_load(char *filename);
void tilemap_free(TileMap *map);
void tilemap_draw(TileMap *map);

void tilemap_collision(TileMap *map, Entity* ent);

#endif
