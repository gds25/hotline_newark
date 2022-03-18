#include <SDL.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"

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
    Sprite* health;
    Vector2D player_aabb;
    Vector2D enemy_aabb;
    Sprite* enemy_bat;
    Sprite* enemy_pistol;
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

    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    player = player_new(vector2d(500, 300), controller);
    health = pickup_new(vector2d(100, 100), 0, "images/health.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    crosshair = gf2d_sprite_load_image("images/crosshair.png");
    enemy_bat = enemy_new(vector2d(1000,200), 1);
    enemy_pistol = enemy_new(vector2d(800,500), 2);
    tilemap = tilemap_load("levels/testlevel.json");

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        entity_manager_think_all();
        check_collisions();
        entity_manager_update_all();
        
        player_aabb = player_get_bounding_box();
        enemy_aabb = enemy_get_bounding_box();
        SDL_Rect player_box = { player_aabb.x, player_aabb.y, 64, 64 };
        SDL_Rect enemy_box = { enemy_aabb.x, enemy_aabb.y, 64, 64 };
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            // draw other game elements
            tilemap_draw(tilemap);
            entity_manager_draw_all();
            gf2d_draw_rect(player_box, vector4d(255, 255, 255, 255));
            gf2d_draw_rect(enemy_box, vector4d(255, 255, 255, 255));
            gf2d_sprite_draw_image(crosshair, get_crosshair_position(player));
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
       // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
