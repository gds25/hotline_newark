//#include "gf2d_actor.h"
#include <SDL.h>
#include "simple_logger.h"
#include "entity.h"
#include "mouse.h"

typedef struct
{
    Uint32 buttons;     /**<buttons mask*/
    Vector2D position;  /**<position of mouse*/
    //Sprite* sprite;
}MouseState;

typedef struct
{
    MouseState mouse[2];        /**<mouse state for the current and last frames*/
    Sprite* sprite;       /**<which sprite to draw this entity with*/
}Mouse;

static Mouse _mouse = { 0 };

void mouse_load(char* filename)
{
    /*Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }*/
    _mouse.sprite = gf2d_sprite_load_all(filename, 32, 32, 16);
    //ent->entity = MOUSE;
}

void mouse_update(Mouse_Data* self)
{
    int x, y;
    self->startFrame = 0;
    self->endFrame = 16;
    self->color = vector4d(255, 255, 255, 255);
    mouse_next_frame(self);
    memcpy(&_mouse.mouse[1], &_mouse.mouse[0], sizeof(MouseState));
    _mouse.mouse[0].buttons = SDL_GetMouseState(&x, &y);
    vector2d_set(_mouse.mouse[0].position, x, y);
    //slog("mouse position: %f, %f", _mouse.mouse[0].position.x, _mouse.mouse[0].position.y);
}

void mouse_draw(Mouse_Data *self)
{
    Vector2D drawScale;
    vector2d_copy(drawScale, self->scale);

    gf2d_sprite_draw(
        _mouse.sprite,
        _mouse.mouse[0].position,
        NULL,
        NULL,
        NULL,
        NULL,
        &self->color,
        (int)self->frame);
}

int mouse_moved()
{
    if ((_mouse.mouse[0].position.x != _mouse.mouse[1].position.x) ||
        (_mouse.mouse[0].position.y != _mouse.mouse[1].position.y) ||
        (_mouse.mouse[0].buttons != _mouse.mouse[1].buttons))
    {
        return 1;
    }
    return 0;
}

int mouse_button_pressed(int button)
{
    int mask;
    mask = 1 << button;
    if ((_mouse.mouse[0].buttons & mask) &&
        !(_mouse.mouse[1].buttons & mask))
    {
        return 1;
    }
    return 0;
}

int mouse_button_held(int button)
{
    int mask;
    mask = 1 << button;
    if ((_mouse.mouse[0].buttons & mask) &&
        (_mouse.mouse[1].buttons & mask))
    {
        return 1;
    }
    return 0;
}

int mouse_button_released(int button)
{
    int mask;
    mask = 1 << button;
    if (!(_mouse.mouse[0].buttons & mask) &&
        (_mouse.mouse[1].buttons & mask))
    {
        return 1;
    }
    return 0;
}

int mouse_button_state(int button)
{
    int mask;
    mask = 1 << button;
    return (_mouse.mouse[0].buttons & mask);
}

Vector2D mouse_get_position()
{
    return _mouse.mouse[0].position;
}

Vector2D mouse_get_movement()
{
    Vector2D dif;
    vector2d_sub(dif, _mouse.mouse[0].position, _mouse.mouse[1].position);
    return dif;
}

int mouse_in_rect(Vector4D r)
{
    SDL_Rect temp_rect;
    SDL_Point temp_point;
    temp_rect = gfc_SDL_Rect(r.x, r.y, r.z, r.w);
    temp_point = gfc_SDL_Point(_mouse.mouse[0].position.x, _mouse.mouse[0].position.y);
    const SDL_Rect *rect = &temp_rect;
    const SDL_Rect* point = &temp_point;
    return SDL_PointInRect(point, rect);
}

ActionReturnType mouse_next_frame(Mouse_Data* self)
{
    if (!self->frame)
    {
        slog("missing frame data");
        return ART_ERROR;
    }
    self->frame += self->frameRate;
    if (self->frame >= self->endFrame)
    {
        switch (self->type)
        {
        case AT_NONE:
            break;
        case AT_LOOP:
            self->frame = self->startFrame;
            break;
        case AT_PASS:
            self->frame = self->endFrame;
            return ART_END;
        }
    }
    return ART_NORMAL;
}
/*eol@eof*/