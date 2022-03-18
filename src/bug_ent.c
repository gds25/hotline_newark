#include "simple_logger.h"
#include "bug_ent.h"

void bug_think(Entity *self)
{
    Vector2D direction;
    int mx,my;
    float angle;
    const Uint8 *keys;

    if (!self)return;
    self->frame = (self->frame + 0.1);
    if (self->frame >= 16)self->frame = 0;

    const int deadZone = 8000;
    
    SDL_GetMouseState(&mx,&my);
    direction.x = mx - self->position.x;
    direction.y = my - self->position.y;
    angle = vector2d_angle(direction) - 90;
    self->rotation.z = angle;
    if (SDL_GameControllerGetAxis(self->controller, SDL_CONTROLLER_AXIS_RIGHTX) > deadZone) {
        self->rotation.x;
    }

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    
    if (SDL_GameControllerGetAxis(self->controller, SDL_CONTROLLER_AXIS_LEFTX)>deadZone) {
        // move forward
        direction.x = self->position.x;
        direction.y = self->position.y;
        vector2d_set_magnitude(&direction, 1);
        vector2d_copy(self->velocity, direction);
    }
    else if (keys[SDL_SCANCODE_W])
    {
        // move forward
        vector2d_set_magnitude(&direction,3);
        vector2d_copy(self->velocity,direction);
        slog("bug position: %f, %f", self->position.x, self->position.y);
    }
    else
    {
        vector2d_scale(self->velocity,self->velocity,0.5);
        if (vector2d_magnitude(self->velocity)<0.05)
        {
            vector2d_clear(self->velocity);
        }
    }

}

Entity *bug_ent_new(Vector2D position, SDL_GameController* gameController)

{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }
    ent->controller = gameController;
    ent->sprite = gf2d_sprite_load_all("images/space_bug_top.png",128,128,16);
    ent->think = bug_think;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    //ent->rotation.z = 
    vector2d_copy(ent->position, position);
    return ent;
}


//eof
