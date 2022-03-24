#include <SDL.h>
#include <stdio.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gf2d_font.h"

#include "camera.h"

#include "entity.h"
#include "bug_ent.h"
#include "player.h"
#include "enemy.h"
#include "pickup.h"

#include "tile_map.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Sprite* player;
    Sprite* health, *armor, *ammo, *speed, *invis;
    Sprite* bat, *pistol, *shotgun, *uzi, *mg;
    Vector2D player_aabb;
    Vector2D enemy_aabb;
    Sprite* enemy_bat, *enemy_pistol, *enemy_shotgun, *enemy_uzi, *enemy_mg;
    Sprite* crosshair;
    Vector4D mouseColor = {255,100,255,200};
    TileMap *tilemap;
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    gf2d_font_init("config/font.cfg");
    tile_set_manager_init(16);
    entity_manager_init(1024);

    camera_set_dimensions(vector2d(1200, 720));
    camera_set_position(vector2d(500, 300));

    SDL_ShowCursor(SDL_DISABLE);
    
    /* Code snippet from: https://wiki.libsdl.org/SDL_GameControllerOpen */
    /* Open the first available controller. */
    if (SDL_NumJoysticks() < 1) {
        slog("No controllers connected");
    }
    SDL_GameController* controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                break;
            }
            else {
                slog("Could not open gamecontroller %i: %s\n", i, SDL_GetError());
            }
        }
    }

    /*setup*/
    tilemap = tilemap_load("levels/testlevel.json");
    player = player_new(vector2d(100, 100), controller, tilemap);
    sprite = gf2d_sprite_load_image("images/backgrounds/test_level.png");
    

    health = pickup_new(vector2d(300, 600), 0, "images/health.png");
    armor = pickup_new(vector2d(400, 150), 1, "images/armor.png");
    ammo = pickup_new(vector2d(650, 400), 2, "images/ammo.png");
    speed = pickup_new(vector2d(1000, 150), 3, "images/speed.png");
    invis = pickup_new(vector2d(1050, 600), 4, "images/invisibility.png");

    //bat = weapon_new(vector2d(200, 200), 0, 0, 0, 20, "images/bat.png");
    //pistol = weapon_new(vector2d(200, 300), 1, 8, 24, 20, "images/pistol.png");
    //shotgun = weapon_new(vector2d(200, 450), 2, 4, 12, 80, "images/shotgun.png");
    //uzi = weapon_new(vector2d(200, 500), 3, 30, 120, 10, "images/uzi.png");
    mg = weapon_new(vector2d(200, 200), 4, 20, 60, 30, "images/machinegun.png");

    //mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    crosshair = gf2d_sprite_load_image("images/crosshair.png");
    enemy_bat = enemy_new(vector2d(1000,200), 0, tilemap);
    enemy_pistol = enemy_new(vector2d(800,500), 1, tilemap);
    enemy_shotgun = enemy_new(vector2d(400,500), 2, tilemap);
    enemy_uzi = enemy_new(vector2d(800,200), 3, tilemap);
    enemy_mg = enemy_new(vector2d(600,300), 4, tilemap);
    

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
       // SDL_GetMouseState(&mx,&my);
       // mf+=0.1;
       // if (mf >= 16.0)mf = 0;
        entity_manager_think_all();
        //tilemap_collision(tilemap, player);
        check_collisions();
        entity_manager_update_all();
        
       // player_aabb = player_get_bounding_box();
       // enemy_aabb = enemy_get_bounding_box();
        //SDL_Rect player_box = { player_aabb.x, player_aabb.y, 32, 32 };
        //SDL_Rect enemy_box = { enemy_aabb.x, enemy_aabb.y, 32, 32 };
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            // draw other game elements
            tilemap_draw(tilemap);
            entity_manager_draw_all();
          //  gf2d_draw_rect(player_box, vector4d(255, 255, 255, 255));
          //  gf2d_draw_rect(enemy_box, vector4d(255, 255, 255, 255));
            gf2d_sprite_draw_image(crosshair, get_crosshair_position(player));
            //UI elements last
            /*gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);*/
            //char ui_health[15];
            char *ui[30];
            //int player_health = get_player_health();
            //slog("health = %i", player_health);
            //const char* UI = "Health";
            snprintf(ui, 100, "Health : %i/100 Armor: %i/25 Ammo: %i", get_player_health(), get_player_armor(), get_player_ammo());
            //free(&ui);
            //snprintf(ui, 100, "Health : %i", get_player_health());
            //memccpy(memccpy(src, "Health :", '\0', 100) - 1, num, '\0', 100);
            gf2d_font_draw_line_tag(ui, FT_H4, gfc_color(0, 0, 0, 0), vector2d(10, 675));
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        //if (get_player_health() <= 0)done = 1;
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
       // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
