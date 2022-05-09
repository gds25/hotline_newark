#include "simple_logger.h"

#include "gfc_types.h"

#include "camera.h"
#include "player.h"
#include "bullet.h"
#include "bat.h"

static Entity player = { 0 };

int player_get_game_state() {
    slog("PLAYER STATE = %i", player.state);
    return player.state;
}

void player_set_game_state(Entity* self) {
    player.state = self->state;
}

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

int get_player_health() {
    //slog("%f, %f", player.position.x, player.position.y);
    return player.health;
}

void set_player_health(Entity* self) {
    player.health = self->health;
}

int get_player_armor() {
    //slog("%f, %f", player.position.x, player.position.y);
    return player.armor;
}

void set_player_armor(Entity* self) {
    player.armor = self->armor;
}
int get_player_score() {
    //slog("%f, %f", player.position.x, player.position.y);
    return player.score;
}

void set_player_score(Entity* self) {
    player.score = self->score;
}

void set_player_ammo(Entity* self) {
    player.ammo = self->ammo;
    player.rounds = self->rounds;
}

Uint32 get_player_ammo() {
    //slog("%f, %f", player.position.x, player.position.y);
    return player.ammo;
}

Uint32 get_player_rounds() {
    //slog("%f, %f", player.position.x, player.position.y);
    return player.rounds;
}


Vector2D player_get_bounding_box() {
    //slog("%f, %f", player.position.x, player.position.y);
    //slog("%f, %f", player.mins.x, player.mins.y);
    return player.mins;
}

//void player_on_death(Entity *self) {

//}

void set_invis_frame(Entity* self) {
    player.powerUp_frame_invis = self->powerUp_frame_invis;
}

float get_invis_frame() {
    return player.powerUp_frame_invis;
}

void player_set_stats(Entity* self, int pickupType) {
    switch (pickupType) {
    case HEALTH:
        self->health = MIN(100, self->health + 25);
       // slog("player health: %i", self->health);
        break;
    case ARMOR:
        self->armor = MIN(125, self->armor+25);
        //slog("player health: %i", self.health);
        break;
    case AMMO:
        self->ammo += 10;
        break;
    case SPEED:
        self->powerUp_frame_speed += 0.01;
        break;
    case INVIS:
        self->powerUp_frame_invis += 0.01;
        break;
    case KEY:
        self->hasKey = 1;
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
        self->draw_offset.y = -16;
        self->rotation.x = 16;
        self->rotation.y = 16;
        self->max_walk_frame = 8;
        self->max_attack_frame = 9;
        self->frames_per_line = 9;
        self->max_ammo = 0;
        self->ammo = 0;
        self->weapon = BAT;
        break;
    case PISTOL:
        self->sprite = gf2d_sprite_load_all("images/player_pistol(1).png", 44, 32, 8);
        self->draw_scale.x = 2;
        self->draw_scale.y = 2;
        self->draw_offset.x = -22;
        self->rotation.x = 22;
        self->max_walk_frame = 8;
        self->max_attack_frame = 6;
        self->frames_per_line = 8;
        self->max_ammo = 8;
        self->weapon = PISTOL;
        self->ammo = 32;
        break;
    case SHOTGUN:
        self->sprite = gf2d_sprite_load_all("images/player_shotgun.png", 44, 32, 12);
        self->draw_scale.x = 2;
        self->draw_scale.y = 2;
        self->draw_offset.x = -22;
        self->draw_offset.y = -16;
        self->rotation.x = 22;
        self->rotation.y = 16;
        self->max_walk_frame = 8;
        self->max_attack_frame = 12;
        self->frames_per_line = 12;
        self->max_ammo = 4;
        self->weapon = SHOTGUN;
        self->ammo = 12;
        break;
    case UZI:
        self->sprite = gf2d_sprite_load_all("images/player_uzi.png", 44, 32, 8);
        self->draw_scale.x = 2;
        self->draw_scale.y = 2;
        self->draw_offset.x = -22;
        self->draw_offset.y = -16;
        self->rotation.x = 22;
        self->rotation.y = 16;
        self->max_walk_frame = 8;
        self->max_attack_frame = 2;
        self->frames_per_line = 8;
        self->max_ammo = 30;
        self->weapon = UZI;
        self->ammo = 120;
        break;
    case MG:
        self->sprite = gf2d_sprite_load_all("images/player_mg.png", 44, 32, 8);
        self->draw_scale.x = 2;
        self->draw_scale.y = 2;
        self->draw_offset.x = -22;
        self->draw_offset.y = -16;
        self->rotation.x = 22;
        self->rotation.y = 16;
        self->max_walk_frame = 8;
        self->max_attack_frame = 4;
        self->frames_per_line = 8;
        self->max_ammo = 20;
        self->weapon = MG;
        self->ammo = 60;
        break;
    default:
        break;
    }
    self->rounds = rounds;
    self->damage = damage;
    //slog("player damage: %i", self->damage);
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
        if ((SDL_GameControllerGetButton(self->controller,
            SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) && (self->ammo > 0 || self->weapon == BAT)) ||
            (self->frame >= self->frames_per_line && self->frame <= self->frames_per_line + self->max_attack_frame)) {
            self->frame = (self->frame + 0.1);
            player_attack(self);
        }
    }
    else {
        self->frame = (self->frame + 0.1);
        if ((SDL_GameControllerGetButton(self->controller,
            SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) && (self->ammo>0 || self->weapon == BAT)) || 
            (self->frame >= self->frames_per_line && self->frame <= self->frames_per_line + self->max_attack_frame)) player_attack(self);
        else if (self->frame >= self->max_walk_frame)self->frame = 0;
        if (self->powerUp_frame_speed > 0) {
            vector2d_set_magnitude(&movement, 4);
            vector2d_copy(self->velocity, movement);
            self->powerUp_frame_speed += 0.1;
            if (self->powerUp_frame_speed > 50) {
                self->powerUp_frame_speed = 0;
            }
        }
        else {
            vector2d_set_magnitude(&movement, 2);
            vector2d_copy(self->velocity, movement);
        }
    }
    if (self->powerUp_frame_invis > 0) {
        self->powerUp_frame_invis += 0.1;
        if (self->powerUp_frame_invis > 100) {
            self->powerUp_frame_invis = 0;
        }
    }
    //slog("%f, %f", self->position.x, self->position.y);
    
    //slog("crosshair: %f, %f", self->crosshair_position.x, self->crosshair_position.y);
    if ((SDL_GameControllerGetButton(self->controller,
        SDL_CONTROLLER_BUTTON_START)) && self->state == 3) {
        self->state = 1;
    }
    //slog("STATE: %i", self->state);
    //player_set_game_state(self);
} 

void player_update(Entity* self) {
   // slog("angle: %f", self->rotation.z);
    player_tilemap_collision(self->tileMap, self);
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
    set_player_health(self);
    set_player_armor(self);
    set_player_ammo(self);
    set_invis_frame(self);
    set_player_score(self);
    //slog("player health: %i", self->health);
}

void player_attack(Entity* self) {
    if (self->frame < self->frames_per_line || self->frame > self->frames_per_line + self->max_attack_frame && SDL_GameControllerGetButton(self->controller,
        SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))self->frame = self->frames_per_line + 1;
    if (self->frame == self->frames_per_line + 1)
    {
        if (self->weapon != BAT && self->ammo>0) {
           // slog("here");
            bullet_new(vector2d((self->position.x - self->draw_offset.x-2*sin((M_PI / 180) * self->rotation.z)), self->position.y - self->draw_offset.y + 2*cos((M_PI / 180) * self->rotation.z)), self->crosshair_position, self->rotation.z, 0, self->damage, self->tileMap);
           
           if (self->weapon == SHOTGUN) {
                slog("shotgun");
                bullet_new(vector2d(
                    (self->position.x - self->draw_offset.x - 2 * sin((M_PI / 180) * self->rotation.z)), self->position.y - self->draw_offset.y + 2 * cos((M_PI / 180) * self->rotation.z)),
                    vector2d(self->position.x - 16 - 150 * sin((M_PI / 180) * (self->rotation.z + (float)10)), self->position.y - 16 + 150 * cos((M_PI / 180) * (self->rotation.z + (float)10))),
                    self->rotation.z, 0, self->damage, self->tileMap);
                bullet_new(vector2d(
                    (self->position.x - self->draw_offset.x - 2 * sin((M_PI / 180) * self->rotation.z)), self->position.y - self->draw_offset.y + 2 * cos((M_PI / 180) * self->rotation.z)),
                    vector2d(self->position.x - 16 - 150 * sin((M_PI / 180) * (self->rotation.z + (float)5)), self->position.y - 16 + 150 * cos((M_PI / 180) * (self->rotation.z + (float)5))), 
                    self->rotation.z, 0, self->damage, self->tileMap);
                bullet_new(vector2d(
                    (self->position.x - self->draw_offset.x - 2 * sin((M_PI / 180) * self->rotation.z)), self->position.y - self->draw_offset.y + 2 * cos((M_PI / 180) * self->rotation.z)),
                    vector2d(self->position.x - 16 - 150 * sin((M_PI / 180) * (self->rotation.z - (float)10)), self->position.y - 16 + 150 * cos((M_PI / 180) * (self->rotation.z - (float)10))),
                    self->rotation.z, 0, self->damage, self->tileMap);
                bullet_new(vector2d(
                    (self->position.x - self->draw_offset.x - 2 * sin((M_PI / 180) * self->rotation.z)), self->position.y - self->draw_offset.y + 2 * cos((M_PI / 180) * self->rotation.z)),
                    vector2d(self->position.x - 16 - 150 * sin((M_PI / 180) * (self->rotation.z - (float)5)), self->position.y - 16 + 150 * cos((M_PI / 180) * (self->rotation.z - (float)5))),
                    self->rotation.z, 0, self->damage, self->tileMap);
            }
            self->ammo--;
            self->rounds--;
        }
        else bat_new(self->position, self->damage, 0);
    }
   // slog("attack frame: %f", self->frame);
    //slog("player health: %i", self->health);
}


Vector2D get_crosshair_position(Entity* self) {
    return self->crosshair_position;
}

void player_tilemap_collision(TileMap* map, Entity* ent)
{
    if (!map)return;
    if (!map->tileset)return;
    if (!map->tilemap)return;
    for (int i = 0; i < map->tilemap_count; i++)
    {
        if (map->tilemap[i]) {
            if ((i % map->tilemap_width) * map->tileset->tile_width + (map->tileset->tile_width) >= ent->mins.x &&
                (i % map->tilemap_width) * map->tileset->tile_width <= ent->maxs.x &&
                (i / map->tilemap_width) * map->tileset->tile_height + (map->tileset->tile_height) >= ent->mins.y &&
                (i / map->tilemap_width) * map->tileset->tile_height <= ent->maxs.y) {
                /*slog("max tile x %i", (i % map->tilemap_width) * map->tileset->tile_width + (map->tileset->tile_width / 2));
                slog("min player x %f", ent->mins.x);
                slog("min tile x %i", (i % map->tilemap_width) * map->tileset->tile_width - (map->tileset->tile_width / 2));
                slog("max player x %f", ent->maxs.x);
                */
                //slog("here");

                if (map->tilemap[i] == 3) {
                    if (ent->hasKey) {
                        slog("Door unlocked.");
                        ent->hasKey = 0;
                        map->tilemap[i] = 0;
                        return;
                    }
                }

                Vector2D direction;
                direction.x = (i % map->tilemap_width) * map->tileset->tile_width - ent->position.x;
                direction.y = (i / map->tilemap_width) * map->tileset->tile_height - ent->position.y;


                vector2d_set_magnitude(&direction, -1);
                vector2d_copy(ent->velocity, direction);
                //slog("velocity x, y = %f     %f", ent->velocity.x, ent->velocity.y);
                //ent->velocity.x = -ent->velocity.x;
                ///ent->velocity.y = -ent->velocity.y;
            }
        }
    }
}

Entity* player_new(Vector2D position, SDL_GameController* gameController, TileMap *map)
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
    ent->ammo = 0;
    ent->entity = PLAYER;
    ent->tileMap = map;
    ent->damage = 100;
    ent->state = 0;
    ent->level = 1;

    ent->weaponName = "this is the player entity";
    //slog("%s", ent->weaponName);
    //slog("entity %i", ent->entity);
    //slog("player ent type: %i", ent->entity);
    //if (ent->entity != PLAYER) slog("this works");

    //ent->rotation.z = ;
    //slog("player health: %i", ent->health);
    //slog("damage %i", ent->health);
    player_set_game_state(ent);
    vector2d_copy(ent->position, position);
    return ent;
}

//eof
