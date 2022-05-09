#include "simple_logger.h"

#include "simple_json.h"
#include "tile_map.h"

TileMap *tilemap_new()
{
    TileMap *map;
    map = (TileMap *)malloc(sizeof(TileMap));
    if (!map)
    {
        slog("failed to allocate memory for a new tilemap");
        return NULL;
    }
    memset(map,0,sizeof(TileMap));
    return map;
}

TileMap *tilemap_load(char *filename)
{
    int i,r,c,e;
    const char *spriteFile = NULL;
    SJson *levelJS, *json,*rows,*columns,*element;
    TileMap *map;
    int tile_width;
    int tile_height;
    int tile_count;
    
    int tilemap_width;
    int tilemap_height;
    int tilemap_count;
    if (!filename)return NULL;
    json = sj_load(filename);
    if (!json)return NULL;
    map = tilemap_new();

    levelJS = sj_object_get_value(json, "level");
    if (!levelJS)
    {
        slog("level json missing level object");
        sj_free(json);
        return NULL;
    }
    
    spriteFile = sj_get_string_value(sj_object_get_value(levelJS,"tile_set"));

    sj_get_integer_value(sj_object_get_value(levelJS,"tile_width"),&tile_width);
    sj_get_integer_value(sj_object_get_value(levelJS,"tile_height"),&tile_height);
    sj_get_integer_value(sj_object_get_value(levelJS,"tile_count"),&tile_count);
    
    map->tileset = tile_set_load(
        (char *)spriteFile,
        tile_width,
        tile_height,
        tile_count);
    
    rows = sj_object_get_value(levelJS,"tile_map");
    tilemap_height = sj_array_get_count(rows);
    if (!tilemap_height)
    {
        slog("tilemap contains no data!");
        sj_free(json);
        return map;
    }
    columns = sj_array_get_nth(rows,0);
    tilemap_width = sj_array_get_count(columns);
    
    tilemap_count = tilemap_width * tilemap_height;
    
    map->tilemap = (Uint32*)gfc_allocate_array(sizeof(Uint32),tilemap_count);
    if (!map->tilemap)
    {
        sj_free(json);
        return map;
    }
    
    map->tilemap_count = tilemap_count;
    map->tilemap_width = tilemap_width;
    
    for (i = 0, r = 0; r < tilemap_height;r++)
    {
        columns = sj_array_get_nth(rows,r);
        if (!columns)continue;
        for (c = 0; c < tilemap_width;c++)
        {
            element = sj_array_get_nth(columns,c);
            if (!element)continue;
            sj_get_integer_value(element,&e);
            map->tilemap[i++] = e;
        }
    }
    
    sj_free(json);
    return map;
}

void tilemap_free(TileMap *map)
{
    if (!map)return;
    tile_set_free(map->tileset);
    free(map->tilemap);
    free(map);
}

void tilemap_draw(TileMap *map)
{
    int i;
    if (!map)return;
    if (!map->tileset)return;
    if (!map->tilemap)return;
    for (i = 0; i < map->tilemap_count; i++)
    {
        if (!map->tilemap[i])continue;
        tile_set_draw(
            map->tileset,
            map->tilemap[i] - 1,
            vector2d(
                (i % map->tilemap_width) * map->tileset->tile_width,
                (i / map->tilemap_width) * map->tileset->tile_height));
    }
}

// eol
