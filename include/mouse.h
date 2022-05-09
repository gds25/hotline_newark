#ifndef __GF2D_MOUSE_H__
#define __GF2D_MOUSE_H__

#include <SDL.h>
#include <stdio.h>
#include "gfc_vector.h"
#include "gfc_text.h"
#include "gf2d_sprite.h"

typedef enum
{
    ART_ERROR,
    ART_NORMAL,
    ART_END,
    ART_START
}ActionReturnType;

typedef enum
{
    AT_NONE,
    AT_LOOP,
    AT_PASS
}ActionType;

typedef struct
{
    Uint32      ref_count;
    TextLine    filename;
    //TextLine    spritename;
    int         frameWidth;
    int         frameHeight;
    int         framesPerLine;
    Vector2D    scale;        /**<scale to draw at*/
    //Vector2D    position;        /**<position of mouse*/
    Vector4D    color;
    Vector4D    colorSpecial;

    //Sprite* sprite;       /**<which sprite to draw this entity with*/
    float       frame;        /**<current frame for the sprite*/
    TextLine    action;       /**<the current action*/
    ActionReturnType at;      /**<set automatically each frame*/

    int startFrame;
    int endFrame;
    float frameRate;
    ActionType type;
}Mouse_Data;

/**
 * @brief load a mouse graphic from an actor
 * @param actorFile the file to use to load the mouse information
 * @note it automatically frees previously loaded actor information
 */
void mouse_load(char* actorFile);

/**
 * @brief call once per frame to update the internal mouse information
 */
void mouse_update();

/**
 * @brief Draw the mouse to the screen.  Should probably be the last thing you do each frame
 */
void mouse_draw();

/**
 * @brief check if the mouse has moved since last frame
 * gf2d_mouse_update() must have been called or this just returns 0
 * @return 1 if the mouse has changed state at all
 */
int mouse_moved();

/**
 * @brief check if the mouse button is down at the moment
 * @param button the button number to check (left mouse button is 0, right mouse button is 1, etc)
 * @return 1 if it is down, 0 otherwise
 */
int mouse_button_state(int button);

/**
 * @brief check if the mouse button was pressed this frame
 * @param button the button number to check (left mouse button is 0, right mouse button is 1, etc)
 * @return 1 if it has, 0 otherwise
 */
int mouse_button_pressed(int button);

/**
 * @brief check if the mouse button was held for this frame
 * @param button the button number to check (left mouse button is 0, right mouse button is 1, etc)
 * @return 1 if it has, 0 otherwise
 */
int mouse_button_held(int button);

/**
 * @brief check if the mouse button was pressed this frame
 * @param button the button number to check (left mouse button is 0, right mouse button is 1, etc)
 * @return 1 if it has, 0 otherwise
 */
int mouse_button_released(int button);

/**
 * @brief check the state of the mouse button
 * @param button the button number to check (left mouse button is 0, right mouse button is 1, etc)
 * @return 1 if it is down, 0 otherwise
 */
int mouse_button_state(int button);

/**
 * @brief get the current mouse position
 * @return a vector containing the mouse position information.  Position will be -1,-1 if never updated
 */
Vector2D mouse_get_position();

/**
 * @brief get the change in position from the last frame
 * @return a vector containing the change in mouse position.
 */
Vector2D mouse_get_movement();

/**
 * @brief check if the mouse is in the given rect
 * @param r the rectangle to check against
 * @return 1 if it is within the rect, 0 otherwise
 */
int mouse_in_rect(Vector4D r);
#endif