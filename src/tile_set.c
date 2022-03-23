#include "simple_logger.h"

#include "entity.h"
#include "tile_set.h"


typedef struct
{
    TileSet *tile_set_list;
    Uint32 tileset_count;
}TileSetManager;

static TileSetManager tile_set_manager = {0};

void tile_set_manager_close()
{
    int i;
    for (i = 0; i< tile_set_manager.tileset_count; i++)
    {
        tile_set_free(&tile_set_manager.tile_set_list[i]);
    }
    free(tile_set_manager.tile_set_list);
}

void tile_set_manager_init(Uint32 tileset_count)
{
    if (tileset_count <= 0)
    {
        slog("cannot init zero tilesets");
        return;
    }
    tile_set_manager.tile_set_list = (TileSet*)gfc_allocate_array(sizeof(TileSet),tileset_count);
    if (!tile_set_manager.tile_set_list)
    {
        return;
    }
    tile_set_manager.tileset_count = tileset_count;
}

TileSet *tile_set_new()
{
    int i;
    for (i = 0; i< tile_set_manager.tileset_count; i++)
    {
        if (!tile_set_manager.tile_set_list[i]._refcount)
        {
            tile_set_manager.tile_set_list[i]._refcount = 1;
            return &tile_set_manager.tile_set_list[i];
        }
    }
    slog("we are out of memory for tile sets");
    return NULL;
}

TileSet *tile_get_by_filename(char *filename)
{
    int i;
    if (!filename)return NULL;
    for (i = 0; i< tile_set_manager.tileset_count; i++)
    {
        if (!tile_set_manager.tile_set_list[i]._refcount)continue;
        if (gfc_line_cmp(filename,tile_set_manager.tile_set_list[i].filename) == 0)
        {
            return &tile_set_manager.tile_set_list[i];
        }
    }
    return NULL;
}

TileSet *tile_set_load(
    char       *filename,
    Uint32      tile_width,
    Uint32      tile_height,
    Uint32      tile_count)
{
    TileSet *tileset;
    if (!filename)return NULL;
    
    tileset = tile_get_by_filename(filename);
    if (tileset != NULL)
    {
        tileset->_refcount++;
        return tileset;
    }

    
    tileset = tile_set_new();
    if (!tileset)return NULL;
    tileset->tile_image = gf2d_sprite_load_all(
            filename,
            tile_width,
            tile_height,
            tile_count);
    gfc_line_cpy(tileset->filename,filename);
    tileset->tile_width = tile_width;
    tileset->tile_height= tile_height;
    tileset->tile_count = tile_count;
    
    return tileset;
}

void tile_set_delete(TileSet *tileset)
{
    if (!tileset)return;
    if (tileset->_refcount > 0)
    {
        slog("deleting tileset %s.  However refcount is greater than 0",tileset->filename);
    }
    gf2d_sprite_free(tileset->tile_image);
    memset(tileset,0,sizeof(TileSet));
}

void tile_set_free(TileSet *tileset)
{
    if (!tileset)return;
    if (tileset->_refcount == 0)
    {
        slog("trying to free %s tileset more times than loaded",tileset->filename);
    }
    tileset->_refcount--;
    if (tileset->_refcount == 0)
    {
        tile_set_delete(tileset);
    }
}

void tile_set_draw(TileSet *tileset,Uint32 tile,Vector2D position)
{
    if (!tileset)return;
    if (!tileset->tile_image)return;// nothing to draw
    gf2d_sprite_draw(
        tileset->tile_image,        
        position,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        tile);
}

//eol
