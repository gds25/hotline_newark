#ifndef __MENUS_H__
#define __MENUS_H__

#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gfc_list.h"
#include "menu_elements.h"
#include "label.h"
#include "button.h"
//#include "gf2d_element_list.h"
#include <simple_json.h>

typedef enum
{
    BE_TL,
    BE_Top,
    BE_TR,
    BE_Left,
    BE_Right,
    BE_BL,
    BE_Bottom,
    BE_BR
}BorderElements;

typedef struct Menu_S
{
    int _inuse;             /**<do not touch*/
    int no_draw_generic;    /**<if true, do not use the generic menu draw style*/
    List* elements;         /**<all the components of the menu*/
    Sprite* background;     /**<background image*/
    Sprite* border;         /**<border sprites*/
    Vector4D dimensions;        /**<where on the screen*/
    Vector4D canvas;            /**<Where within the menu we draw things*/
    Vector4D color;         /**<color to draw the menu with*/
    int (*update)(struct menu_S* win, List* updateList);
    int (*draw)(struct menu_S* win);
    int (*free_data)(struct menu_S* win);
    void* data;             /**<custom data*/

    Uint8 isActive;
}Menu;

/**
 * @brief initialize the menu system
 * @param max_menus the limit of active menus
 */
void menus_init(int max_menus);

/**
 * @brief draw all active menus
 */
void menus_draw_all();

/**
 * @brief update all active menus
 */
void menus_update_all();

/**
 * @brief get a new initialized menu
 * @return NULL on error or a menu pointer
 */
Menu* menu_new();

/**
 * @brief load a menu config from file and return a handle to it
 * @param the filename of the config file
 * @return NULL on error or a menu pointer on success
 */
Menu* menu_load(char* filename);

/**
 * @brief free a menu no longer in use
 * @param win the menu to free
 */
void menu_free(Menu* win);

/**
 * @brief add a gui element to the menu
 * @param win the menu to add an element to
 * @param w the element to add
 */
void menu_add_element(Menu* win, Element* e);

/**
 * @brief update a menu and all of its elements
 * @param win the menu to update
 */
void menu_update(Menu* win);

/**
 * @brief draw a menu to the screen
 * @param win the menu to draw
 */
void menu_draw(Menu* win);

/**
 * @brief draw a menu given the border sprites and background image
 * @param border the sprite containing the border elements
 * @param bg the sprite to use for the background image (it will be stretch to match the rect
 * @param rect the dimensions of the menu to draw
 * @param color the color to draw the menu with
 */
void draw_menu_border(Sprite* border, Sprite* bg, Vector4D rect, Vector4D color);

/**
 * @brief draw a generic menu using the common border assets
 * @param rect the dimensions of the menu to draw
 * @param color the color to draw the menu with
 */
void draw_menu_border_generic(Vector4D rect, Vector4D color);

/**
 * @brief make a new menu based on the contents of a json config file
 * @param json the json file containing a menu config
 * @return NULL on error or the new menu otherwise
 */
Menu* menu_load_from_json(SJson* json);

#endif
