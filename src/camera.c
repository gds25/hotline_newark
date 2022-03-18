#include "camera.h"
static SDL_Rect Camera = { 0 };
void camera_set_dimensions(Vector2D size)
{
    Camera.w = size.x;
    Camera.h = size.y;
}

Vector2D camera_get_dimensions()
{
    Vector2D size;
    size.x = Camera.w;
    size.y = Camera.h;
    return size;
}


void camera_set_position(Vector2D position)
{
    Camera.x = (int)position.x;
    Camera.y = (int)position.y;
}

void camera_move(Vector2D move)
{
    Camera.x += move.x;
    Camera.y += move.y;
}
Bool camera_point_on_screen(Vector2D point)
{
    if ((point.x < Camera.x) || (point.x > Camera.x + Camera.w) ||
        (point.y < Camera.y) || (point.y > Camera.y + Camera.h))
    {
        return false;
    }
    return true;
}
Bool camera_rect_on_screen(SDL_Rect rect)
{
    if (((rect.x + rect.w) < Camera.x) || (rect.x > (Camera.x + Camera.w)) ||
        ((rect.y + rect.h) < Camera.y) || (rect.y > (Camera.y + Camera.h)))
    {
        return false;
    }
return true;
}

SDL_Rect camera_get_rectangle()
{
    return Camera;
}

Vector2D camera_get_position()
{
    Vector2D position;
    position.x = Camera.x;
    position.y = Camera.y;
    return position;
}
Vector2D world_get_offset()
{
    Vector2D position;
    position.x = -Camera.x;
    position.y = -Camera.y;
    return position;
}
/*eol@eof*/