#include <SDL.h>
#include <stdio.h>

#include "simple_logger.h"
#include "simple_json.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gf2d_font.h"

#include "gfc_list.h"
#include "gfc_audio.h"
#include "menus.h"
#include "mouse.h"

#include "camera.h"
#include "level.h"

#include "entity.h"
#include "bug_ent.h"
#include "player.h"
#include "enemy.h"
#include "pickup.h"

#include "tile_map.h"

int main_menu_update(Menu* menu, List* updateList)
{
    int i, count;
    Element* e;
    if (!menu)return 0;
    if (!updateList)return 0;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList, i);
        if (!e)continue;
        slog("updated element index %i", e->index);
        switch (e->index)
        {
        case 51:
            slog("ok");
            break;
        case 52:
            slog("cancel");
            break;
        }
    }
    return 0;
}

int pause_menu_update(Menu* menu, List* updateList)
{
    int i, count;
    Element* e;
    if (!menu)return 0;
    if (!updateList)return 0;
    count = gfc_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gfc_list_get_nth(updateList, i);
        if (!e)continue;
        slog("updated element index %i", e->index);
        switch (e->index)
        {
        case 51:
            slog("ok");
            break;
        case 52:
            slog("cancel");
            break;
        }
    }
    return 0;
}

Menu* main_menu_init()
{
    Menu* menu;
    SJson* json = NULL;
    json = sj_load("config/mainmenu.json");
    menu = menu_load_from_json(json);
    if (menu)
    {
        menu->update = main_menu_update;
    }
    sj_free(json);
    return menu;
}

Menu* pause_menu_init()
{
    Menu* menu;
    SJson* json = NULL;
    json = sj_load("config/pausemenu1.json");
    menu = menu_load_from_json(json);
    if (menu)
    {
        menu->update = pause_menu_update;
    }
    sj_free(json);
    return menu;
}


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Entity* player;
    Sprite* health, *armor, *ammo, *speed, *invis, *key;
    Sprite* bat, *pistol, *shotgun, *uzi, *mg;
    Vector2D player_aabb;
    Vector2D enemy_aabb;
    Sprite* enemy_bat, *enemy_pistol, *enemy_shotgun, *enemy_uzi, *enemy_mg;
    Sprite* crosshair;
    Vector4D mouseColor = {255,100,255,200};
    Level* level;
    //Menu* pri_menu, * pause_menu, * npc_menu, *level_menu;
    TileMap *tilemap;

    Sound* theme;
    
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
    menus_init(128);
    gf2d_font_init("config/font.cfg");
    tile_set_manager_init(16);
    entity_manager_init(1024);

    camera_set_dimensions(vector2d(1200, 900));
    camera_set_position(vector2d(100, 100));

    SDL_ShowCursor(SDL_DISABLE);

    gfc_audio_init(
        1024,
        2,
        2,
        1024,
        1,
        1);
    
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
    crosshair = gf2d_sprite_load_image("images/crosshair.png");

    level = level_load("levels/testlevel.json", tilemap);
    mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);

    theme = gfc_sound_load("sounds/menuTheme.wav", 1.0, 1);

    gfc_sound_play(theme, -1, 1.0, -1, -1);
   
    //sprite = gf2d_sprite_load_image("images/backgrounds/test_level.png");

    
    mouse_load("images/pointer.png");

    /*main game loop*/
    while(!done)
    {
        
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);

        entity_manager_think_all();
;
        check_collisions();
        mouse_update();
        entity_manager_update_all();
        level_update(level);
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //level drawn first
            level_draw(level);
            // draw other game elements
            entity_manager_draw_all();
            gf2d_sprite_draw_image(crosshair, get_crosshair_position(player));  
            //slog("state: %i", player_get_game_state);
            //menus_draw_all();
            if (player->state == 0) {
                //slog("--------here--------");
                sprite = gf2d_sprite_load_image("images/mainmenu.png");
                gf2d_sprite_draw_image(sprite, vector2d(0,0));
                gf2d_sprite_draw(
                    mouse,
                    vector2d(mx, my),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    &mouseColor,
                    (int)mf);
                if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON_LMASK != 0
                    & mx >= 300 & mx <= 850 & my >= 250 & my <= 575) {

                    if (my <= 400) {
                        player->state = 3; 
                       // slog("STATE: %i", player->state);
                    }
                    else if (my >= 450) {
                        done = 1;
                    }
                }
            }
            else if (player->state == 1) {
                if (player->level == 1) 
                sprite = gf2d_sprite_load_image("images/pausescreen1.png");
                else sprite = gf2d_sprite_load_image("images/pausescreen2.png");
                gf2d_sprite_draw_image(sprite, vector2d(0, 0));
                gf2d_sprite_draw(
                    mouse,
                    vector2d(mx, my),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    &mouseColor,
                    (int)mf);
                if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON_LMASK != 0
                    & mx >= 300 & mx <= 850 & my >= 250 & my <= 575) { 

                    if (my <= 395) {
                        player->state = 3;
                    }
                    else if (my >= 450) {
                        player->state = 0;
                        player->level = 1;
                    }
                }
            }
            else if (player->state == 2) {
                if (player->level == 1)
                    sprite = gf2d_sprite_load_image("images/npc1.png");
                else sprite = gf2d_sprite_load_image("images/npc2.png");
                gf2d_sprite_draw_image(sprite, vector2d(0, 0));
                gf2d_sprite_draw(
                    mouse,
                    vector2d(mx, my),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    &mouseColor,
                    (int)mf);
                if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON_LMASK != 0
                    & mx >= 300 & mx <= 850 & my >= 475 & my <= 575) {
                        player->state = 3;
                }
            }
            else if (player->state == 4) {
                sprite = gf2d_sprite_load_image("images/levelscreen.png");
                gf2d_sprite_draw_image(sprite, vector2d(0, 0));
                gf2d_sprite_draw(
                    mouse,
                    vector2d(mx, my),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    &mouseColor,
                    (int)mf);
                if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON_LMASK != 0
                    & mx >= 300 & mx <= 850 & my >= 250 & my <= 575) {

                    if (my <= 395) {
                        player->state = 3;
                        player->level++;
                        level_free;
                        level = level_load("levels/level2.json", tilemap);
                    }
                    else if (my >= 450) {
                        player->state = 0;
                        player->level = 1;
                    }
                }
                
            }
            //slog("STATE: %i", player->state);
            entity_set_game_state(player->state);
            level_set_game_state(player->state);
            //UI elements last
            /**/
  
            char *ui[30];
            char* fps[10];
            char* score[15];

            snprintf(ui, 100, "Health : %i/100 Armor: %i/25 Ammo: %i", get_player_health(), get_player_armor(), get_player_ammo());
            snprintf(fps, 100, "%f FPS", gf2d_graphics_get_frames_per_second());
            snprintf(score, 100, "Score: %i", get_player_score());

            gf2d_font_draw_line_tag(ui, FT_H4, gfc_color(0, 0, 0, 0), vector2d(10, 675));
            gf2d_font_draw_line_tag(fps, FT_H2, gfc_color(1, 1, 1, 1), vector2d(1000, 10));
            gf2d_font_draw_line_tag(score, FT_H4, gfc_color(1, 1, 1, 1), vector2d(100, 10));
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
 
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
  
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
