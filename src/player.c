#include "simple_logger.h"

#include "gfc_types.h"

#include "camera.h"
#include "player.h"
#include "bullet.h"

//TODO
//player on_death : bring to main menu
//enemy on_death : drop weponns
//enemy sprites
//spwan different enemies
//set boundaries for world/tile collisions
//user interface
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

void player_set_stats(Entity* self, int pickupType) {
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
    default:
        break;
    }
}

void player_set_weapon(Entity * self, int weaponType, int ammo, int rounds, int damage) {
    switch(weaponType) {
    case BAT:
        self->sprite = gf2d_sprite_load_all("images/player_bat.png", 32, 32, 9);
        self->draw_scale.x = 2;
        self->draw_scale.y = 2;
        self->draw_offset.x = -16;
        self->rotation.x = 16;
        self->max_walk_frame = 8;
        self->max_attack_frame = 9;
        self->frames_per_line = 9;
        self->max_ammo = -1;
        break;
    case PISTOL:
        self->sprite = gf2d_sprite_load_all("images/player_pistol(1).png", 44, 32, 8);
        self->draw_scale.x = 1.4545;
        self->draw_scale.y = 2;
        self->draw_offset.x = -22;
        self->rotation.x = 22;
        self->max_walk_frame = 8;
        self->max_attack_frame = 2;
        self->frames_per_line = 8;
        self->max_ammo = 8;
        break;
    case SHOTGUN:
        self->sprite = gf2d_sprite_load_all("images/player_shotgun.png", 44, 32, 12);
        self->draw_scale.x = 1.4545;
        self->draw_scale.y = 2;
        self->draw_offset.x = -22;
        self->rotation.x = 22;
        self->max_walk_frame = 8;
        self->max_attack_frame = 12;
        self->frames_per_line = 12;
        self->max_ammo = 4;
        break;
    case UZI:
        self->sprite = gf2d_sprite_load_all("images/player_uzi.png", 44, 32, 8);
        self->draw_scale.x = 1.4545;
        self->draw_scale.y = 2;
        self->draw_offset.x = -22;
        self->rotation.x = 22;
        self->max_walk_frame = 8;
        self->max_attack_frame = 2;
        self->frames_per_line = 8;
        self->max_ammo = 30;
        break;
    case MG:
        self->sprite = gf2d_sprite_load_all("images/player_mg.png", 44, 32, 8);
        self->draw_offset.x = 1.4545;
        self->draw_offset.y = 2;
        self->draw_offset.x = -22;
        self->rotation.x = 22;
        self->max_walk_frame = 8;
        self->max_attack_frame = 2;
        self->frames_per_line = 8;
        self->max_ammo = 20;
        break;
    default:
        break;
    }
    self->ammo = ammo;
    self->rounds = rounds;
    self->damage = damage;
    slog("player damage: %i", self->damage);
}

void player_set_bounding_box(Entity* self) {
    self->mins.x = self->position.x;
    self->mins.y = self->position.y;
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
        angle = vector2d_angle(direction);
        self->rotation.z = angle-90;
    }
    else if (yRotate > deadZone || yRotate < 0 - deadZone) {
        direction.y = (yRotate);
        direction.x = 0;
        angle = vector2d_angle(direction);
        self->rotation.z = angle-90;
        
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
        if (SDL_GameControllerGetButton(self->controller,
            SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) && self->ammo > 0 ||
            (self->frame >= self->frames_per_line && self->frame <= self->frames_per_line + self->max_attack_frame)) {
            self->frame = (self->frame + 0.05);
            player_attack(self);
        }
    }
    else {
        self->frame = (self->frame + 0.05);
        if ((SDL_GameControllerGetButton(self->controller,
            SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) && self->ammo>0) || 
            (self->frame >= self->frames_per_line && self->frame <= self->frames_per_line + self->max_attack_frame)) player_attack(self);
        else if (self->frame >= self->max_walk_frame)self->frame = 0;
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

void player_attack(Entity* self) {
    if (self->frame < self->frames_per_line || self->frame > self->frames_per_line + self->max_attack_frame && SDL_GameControllerGetButton(self->controller,
        SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))self->frame = self->frames_per_line + 1;
    if (self->frame == self->frames_per_line + 1)
    {
        bullet_new(vector2d(self->position.x - self->draw_offset.x, self->position.y - self->draw_offset.y), self->crosshair_position, self->rotation.z, 0, self->damage);
        self->ammo--;
        self->rounds--;
    }
   // slog("attack frame: %f", self->frame);
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
    ent->sprite = gf2d_sprite_load_all("images/player_walk.png", 32, 32, 8);
    ent->think = player_think;
    ent->update = player_update;
    ent->draw_scale.x = 2;
    ent->draw_scale.y = 2;
    ent->draw_offset.x = -16;
    ent->draw_offset.y = -16;
    ent->rotation.x = 16;
    ent->rotation.y = 16;
    ent->max_walk_frame = 8;
    ent->max_attack_frame = 0;
    ent->frames_per_line = 8;
    ent->health = 100;
    ent->ammo = 30;
    ent->entity = PLAYER;
    slog("player ent type: %i", ent->entity);
    //if (ent->entity != PLAYER) slog("this works");

    //ent->rotation.z = ;
    vector2d_copy(ent->position, position);
    return ent;
}

//eof
