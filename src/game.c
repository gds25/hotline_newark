#include <SDL.h>
#include <stdio.h>

#include <simple_logger.h>
#include "gfc_input.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_windows.h"
#include "gf2d_entity.h"
#include "gf2d_font.h"
#include "gf2d_mouse.h"
#include "gf2d_draw.h"
#include "camera.h"
#include "windows_common.h"

#include "galaxy.h"
#include "galaxy_view.h"
#include "system_view.h"
#include "systems.h"
#include "planet.h"

static int _done = 0;
static Window *_quit = NULL;

void onCancel(void *data)
{
    _quit = NULL;
}
void onExit(void *data)
{
    _done = 1;
    _quit = NULL;
}


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int i;
    int fullscreen = 0;
    int debug = 0;
    Galaxy *galaxy;
    System *system;
    int mx,my;
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"--fullscreen") == 0)
        {
            fullscreen = 1;
        }
        if (strcmp(argv[i],"--debug") == 0)
        {
            debug = 1;
        }
    }
    
    /*program initializtion*/
    init_logger("conquest.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Conquest",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        fullscreen,
        debug);
    gf2d_graphics_set_frame_delay(16);
    gfc_audio_init(256,16,4,1,1,1);
    gf2d_sprite_init(1024);
    gf2d_action_list_init(128);
    gf2d_font_init("config/font.cfg");
    gfc_input_init("config/input.cfg");
    gf2d_windows_init(128);
    gf2d_entity_system_init(1024);
    
    camera_set_dimensions(0,0,1200,700);
    
    SDL_ShowCursor(SDL_DISABLE);
    // game specific setup
        // init mouse, editor window
    gf2d_mouse_load("actors/mouse.actor");
    /*main game loop*/
    galaxy_init();
    system_init();
    planet_init();
    galaxy = galaxy_generate(200,1);    
    srand(SDL_GetTicks());
    
    system = galaxy_get_nearest_system(galaxy,NULL,vector2d(0.5,0.5),0.5);

    if (!system)slog("found no system");
    system_view_window(system);
//    galaxy_view_window(galaxy);
    while(!_done)
    {
        gfc_input_update();
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        gf2d_windows_update_all();
                
        gf2d_entity_think_all();
        gf2d_mouse_update();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            
            gf2d_windows_draw_all();
            //backgrounds drawn first
        
            //UI elements last
            
            gf2d_font_draw_line_tag("Press F4 to quit!",FT_H1,gfc_color(255,255,255,255), vector2d(0,0));
            
            gf2d_mouse_draw();
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if ((gfc_input_command_down("exit"))&&(_quit == NULL))
        {
            _quit = window_yes_no("Exit?",onExit,onCancel,NULL,NULL);
        }
        if (debug)slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    galaxy_free(galaxy);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
