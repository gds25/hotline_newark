#include "simple_logger.h"

#include "gfc_types.h"

#include "camera.h"
#include "player.h"


static Entity player = { 0 };

Entity get_player() {
    //slog("%f, %f", player.position.x, player.position.y);
    return player;
}

Vector2D get_player_position() {
    //slog("%f, %f", player.position.x, player.position.y);
    return player.position;
}

void set_player_position(Entity* self) {
    player.position = self->position;
}

Vector2D player_get_bounding_box() {
    //slog("%f, %f", player.position.x, player.position.y);
    //slog("%f, %f", player.mins.x, player.mins.y);
    return player.mins;
}

void player_set_stats(Entity *self, int pickupType) {
    switch (pickupType) {
    case HEALTH:
        self->health += 10;
        slog("player health: %i", self->health);
        break;
    case ARMOR:
        self->armor += 10;
        //slog("player health: %i", self.health);
        break;
    case AMMO:
        self->ammo += 10;
        break;
    case SPEED:
        self->ammo += 10;
        break;
    case INVIS:
        self->ammo += 10;
        break;
    case BAT:
        self->ammo += 10;
        break;
    case PISTOL:
        self->ammo += 10;
        break;
    case SHOTGUN:
        self->ammo += 10;
        break;
    case UZI:
        self->ammo += 10;
        break;
    case MG:
        self->ammo += 10;
        break;
    default:
        break;
    }
}

void player_set_bounding_box(Entity* self) {
    self->mins.x = self->position.x - 32;
    self->mins.y = self->position.y - 32;
    self->maxs.x = self->position.x + 32;
    self->maxs.y = self->position.y + 32;

    player.mins.x = self->mins.x;
    player.mins.y = self->mins.y;
    player.maxs.x = self->maxs.x;
    player.maxs.y = self->maxs.y;

}
void player_think(Entity* self)
{
    Vector2D movement;
    Vector2D direction;
    float angle;

    if (!self)return;
    self->frame = (self->frame + 0.1);
    if (self->frame >= 16)self->frame = 0;

    const Uint8* keys;
    const int deadZone = 100;
    float xRotate, yRotate, xMove, yMove;

    //int mx, my;

    xRotate = SDL_GameControllerGetAxis(self->controller, SDL_CONTROLLER_AXIS_RIGHTX);
    yRotate = SDL_GameControllerGetAxis(self->controller, SDL_CONTROLLER_AXIS_RIGHTY);
    xMove = SDL_GameControllerGetAxis(self->controller, SDL_CONTROLLER_AXIS_LEFTX);
    yMove = SDL_GameControllerGetAxis(self->controller, SDL_CONTROLLER_AXIS_LEFTY);

    //SDL_GetMouseState(&mx, &my);
    //direction.x = mx - self->position.x;
    //direction.y = mx - self->position.y;
    //slog("%i, %i", mx, my);
    //angle = vector2d_angle(direction) - 90;
    //self->rotation.z = angle;

   // slog("%f, %f", direction.x, direction.y);

    if (xRotate > deadZone || xRotate < 0 - deadZone) {
        direction.x = (xRotate);
        if (yRotate > deadZone || yRotate < 0 - deadZone) {
            direction.y = yRotate;
        }
        else {
            direction.y = 0;
        }
        angle = vector2d_angle(direction) - 90;
        self->rotation.z = angle;
    }
    else if (yRotate > deadZone || yRotate < 0 - deadZone) {
        direction.y = (yRotate);
        direction.x = 0;
        angle = vector2d_angle(direction) - 90;
        self->rotation.z = angle;
        
    }


    /*direction.x = xRotate; //-self->position.x;
    direction.y = yRotate;// -self->position.y;
    angle = vector2d_angle(direction) - 90;
    self->rotation.z = angle;*/
    movement.x = 0;
    movement.y = 0;
    if (xMove > deadZone) {
        movement.x = 5;
        //slog("xmove: %i", xMove);
        //slog("xrotate: %f", direction.x);
    }
    else if (xMove < 0 - deadZone) {
        movement.x = -5;
    }


    if (yMove < 0 - deadZone) {
        // move forward
        //direction.x = self->position.x;
        //direction.y = self->position.y;
        movement.y = -5;
      
    }
    else if (yMove > deadZone) {
        // move forward
        //direction.x = self->position.x;
        //direction.y = self->position.y;
        movement.y = 5;
        
    }



    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    /*if (keys[SDL_SCANCODE_W])
    {
        // move forward
        vector2d_set_magnitude(&direction, 1);
        vector2d_copy(self->velocity, direction);
    }
    else
    {
        vector2d_scale(self->velocity, self->velocity, 0.5);
        if (vector2d_magnitude(self->velocity) < 0.05)
        {
            vector2d_clear(self->velocity);
        }
    }*/
    if ((xMove > 0 - deadZone && xMove < deadZone) && (yMove > 0 - deadZone && yMove < deadZone)) {
        vector2d_scale(self->velocity, self->velocity, 0.5);
        if (vector2d_magnitude(self->velocity) < 0.05)
        {
            vector2d_clear(self->velocity);
        }
    }
    else {
        vector2d_set_magnitude(&movement, 2);
        vector2d_copy(self->velocity, movement);
    }
    //slog("%f, %f", self->position.x, self->position.y);
    
    //slog("crosshair: %f, %f", self->crosshair_position.x, self->crosshair_position.y);
}

void player_update(Entity* self) {
   // slog("angle: %f", self->rotation.z);
    vector2d_add(self->position, self->position, self->velocity);
    vector2d_copy(self->crosshair_position, vector2d(self->position.x-16- 150* sin((M_PI/180)*self->rotation.z), self->position.y-16+ 150 * cos((M_PI / 180)*self->rotation.z)));
    
    Vector2D camera;
    Vector2D cameraSize;

    if (!self)return;
    cameraSize = camera_get_dimensions();
    camera.x = (self->position.x + 64) - (cameraSize.x * 0.5);
    camera.y = (self->position.y + 64) - (cameraSize.y * 0.5);
    camera_set_position(camera);
    // apply dampening on velocity
    set_player_position(self);
    player_set_bounding_box(self);
    //slog("player health: %i", player.health);
}


Vector2D get_crosshair_position(Entity* self) {
    return self->crosshair_position;
}

Entity* player_new(Vector2D position, SDL_GameController* gameController)

{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("no space for bugs");
        return NULL;
    }
    ent->controller = gameController;
    ent->sprite = gf2d_sprite_load_all("images/space_bug_top.png", 128, 128, 16);
    ent->think = player_think;
    ent->update = player_update;
    ent->draw_offset.x = -64;
    ent->draw_offset.y = -64;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->health = 100;
    ent->entity = PLAYER;
    slog("player ent type: %i", ent->entity);
    //if (ent->entity != PLAYER) slog("this works");

    //ent->rotation.z = ;
    vector2d_copy(ent->position, position);
    return ent;
}

//eof
