#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "menus.h"
#include "simple_logger.h"

typedef struct
{
    Sprite* generic_border;
    Sprite* generic_background;
    Menu* menu_list;    /**<*/
    int menu_max;         /**<how many menus can exist at once*/
    List* menu_deque;     /**<draw order is back to front, update order is front to back*/
    int drawbounds;         /**<if true draw rects around menu bounds*/
    enum entType{ START, QUIT } buttonType;
}menuManager;

static menuManager menu_manager = { 0 };

void draw_menu_border_generic(Vector4D rect, Vector4D color)
{
    draw_menu_border(menu_manager.generic_border, menu_manager.generic_background, rect, color);
}

void draw_menu_border(Sprite* border, Sprite* bg, Vector4D rect, Vector4D color)
{
    Vector2D scale = { 0 };
    if (bg)
    {
        scale.x = rect.z / bg->frame_w;
        scale.y = rect.w / bg->frame_h;
        gf2d_sprite_draw(
            bg,
            vector2d(rect.x, rect.y),
            &scale,
            NULL,
            NULL,
            NULL,
            &color,
            0);
        slog("drew background");
    }
    if (!border) { slog("no border");  return; }
    scale.x = (rect.w - border->frame_w) / (float)border->frame_w;
    scale.y = 1;
    gf2d_sprite_draw(
        border,
        vector2d(rect.x + border->frame_w / 2, rect.y - border->frame_w / 2),
        &scale,
        NULL,
        NULL,
        NULL,
        &color,
        BE_Top);
    gf2d_sprite_draw(
        border,
        vector2d(rect.x + border->frame_w / 2, rect.y + rect.w - border->frame_w / 2),
        &scale,
        NULL,
        NULL,
        NULL,
        &color,
        BE_Bottom);

    scale.y = (rect.w - border->frame_h) / (float)border->frame_h;
    scale.x = 1;

    gf2d_sprite_draw(
        border,
        vector2d(rect.x - border->frame_w / 2, rect.y + border->frame_w / 2),
        &scale,
        NULL,
        NULL,
        NULL,
        &color,
        BE_Left);
    gf2d_sprite_draw(
        border,
        vector2d(rect.x + rect.w - border->frame_w / 2, rect.y + border->frame_w / 2),
        &scale,
        NULL,
        NULL,
        NULL,
        &color,
        BE_Right);

    gf2d_sprite_draw(
        border,
        vector2d(rect.x + rect.w - border->frame_w / 2, rect.y - border->frame_w / 2),
        NULL,
        NULL,
        NULL,
        NULL,
        &color,
        BE_TR);
    gf2d_sprite_draw(
        border,
        vector2d(rect.x - border->frame_w / 2, rect.y + rect.w - border->frame_w / 2),
        NULL,
        NULL,
        NULL,
        NULL,
        &color,
        BE_BL);
    gf2d_sprite_draw(
        border,
        vector2d(rect.x - border->frame_w / 2, rect.y - border->frame_w / 2),
        NULL,
        NULL,
        NULL,
        NULL,
        &color,
        BE_TL);
    gf2d_sprite_draw(
        border,
        vector2d(rect.x + rect.z - border->frame_w / 2, rect.y + rect.w - border->frame_w / 2),
        NULL,
        NULL,
        NULL,
        NULL,
        &color,
        BE_BR);
    slog("drew borders");
    if (menu_manager.drawbounds)
    {
        slog("draw bounds");
        gf2d_draw_rect(gfc_SDL_Rect(rect.x,rect.y,rect.z,rect.w), vector4d(255, 100, 100, 255));
    }
}

void menus_close()
{
    int i;
    for (i = 0; i < menu_manager.menu_max; i++)
    {
        if (menu_manager.menu_list[i]._inuse)
        {
            menu_free(&menu_manager.menu_list[i]);
        }
    }
    gfc_list_delete(menu_manager.menu_deque);
    slog("menu system closed");
}

void menus_init(int max_menus)
{
    if (max_menus <= 0)
    {
        slog("cannot initilize menu system for 0 menus");
        return;
    }
    menu_manager.menu_list = (Menu*)malloc(sizeof(Menu) * max_menus);
    if (menu_manager.menu_list == NULL)
    {
        slog("failed to allocate memory for menu system");
        return;
    }
    memset(menu_manager.menu_list, 0, sizeof(Menu) * max_menus);
    menu_manager.menu_max = max_menus;
    menu_manager.menu_deque = gfc_list_new();
    menu_manager.generic_background = gf2d_sprite_load_image("images/menu_background.png");
    menu_manager.generic_border = gf2d_sprite_load_all("images/menu_border.png", 64, 64, 8);
    menu_manager.drawbounds = 1;
    slog("menu system initilized");
    atexit(menus_close);
}

void menu_free(Menu* win)
{
    int i, count;
    if (!win)return;
    if (win->free_data != NULL)
    {
        win->free_data(win);
    }
    gfc_list_delete_data(menu_manager.menu_deque, win);
    count = gfc_list_get_count(win->elements);
    for (i = 0; i < count; i++)
    {
        menu_element_free((Element*)gfc_list_get_nth(win->elements, i));
    }
    gfc_list_delete(win->elements);
    gf2d_sprite_free(win->background);
    gf2d_sprite_free(win->border);
    memset(win, 0, sizeof(Menu));
}

void menu_draw(Menu* win)
{
    int count, i;
    Vector2D offset;
    if (!win) { slog("no menu"); return; }
    if (!win->isActive) return;
    if (!win->no_draw_generic)
    {
        slog("generic draw");
        draw_menu_border_generic(win->dimensions, win->color);
    }
    offset.x = win->dimensions.x + win->canvas.x;
    offset.y = win->dimensions.y + win->canvas.y;
    count = gfc_list_get_count(win->elements);
    for (i = 0; i < count; i++)
    {
        slog("----drawing elements----");
        menu_element_draw((Element*)gfc_list_get_nth(win->elements, i), offset);
    }
}

void menu_update(Menu* win)
{
    int count, i;
    Vector2D offset;
    List* updateList = NULL;
    List* updated = NULL;
    Element* e;
    if (!win)return;
    updateList = gfc_list_new();
    offset.x = win->dimensions.x + win->canvas.x;
    offset.y = win->dimensions.y + win->canvas.y;
    count = gfc_list_get_count(win->elements);
    for (i = 0; i < count; i++)
    {
        e = (Element*)gfc_list_get_nth(win->elements, i);
        if (!e)continue;
        updated = menu_element_update(e, offset);
        if (updated)
        {
            if (!updateList)
            {
                updateList = gfc_list_new();
            }
            gfc_list_concat_free(updateList, updated);
        }
        if (e->type == "button" && e->state == ES_active) {
            if (i <= 2) menu_manager.buttonType = START;
            else menu_manager.buttonType = QUIT;
        }
    }
    if (win->update)
    {
        win->update(win, updateList);
    }
    gfc_list_delete(updateList);
}

Menu* menu_new()
{
    int i;
    for (i = 0; i < menu_manager.menu_max; i++)
    {

        if (!menu_manager.menu_list[i]._inuse)
        {
            menu_manager.menu_list[i]._inuse = 1;
            gfc_list_append(menu_manager.menu_deque, &menu_manager.menu_list[i]);
            menu_manager.menu_list[i].elements = gfc_list_new();
            return &menu_manager.menu_list[i];
        }
    }
    return NULL;
}

void menu_add_element(Menu* win, Element* e)
{
    if (!win)return;
    if (!e)return;
    gfc_list_append(win->elements, e);
}

void menus_draw_all()
{
    int i, count;
    count = gfc_list_get_count(menu_manager.menu_deque);
    for (i = 0; i < count; i++)
    {
        slog("drawing menu");
        menu_draw((Menu*)gfc_list_get_nth(menu_manager.menu_deque, i));
    }
}

void menus_update_all()
{
    int i, count;
    count = gfc_list_get_count(menu_manager.menu_deque);
    for (i = count - 1; i >= 0; i--)
    {
        menu_update((Menu*)gfc_list_get_nth(menu_manager.menu_deque, i));
    }
}

Menu* menu_load_from_json(SJson* json)
{
    Menu* win = NULL;
    int i, count;
    Vector4D vector = { 255,255,255,255 };
    SJson* elements, * value;
    if (!json)
    {
        slog("json not provided");
        return NULL;
    }
    json = sj_object_get_value(json, "menu");
    if (!json)
    {
        slog("json does not contain menu definition");
        return NULL;
    }
    win = menu_new();
    if (!win)
    {
        slog("failed to create new menu");
        return NULL;
    }
    sj_value_as_vector4d(sj_object_get_value(json, "color"), &vector);
    win->color = vector;

    vector4d_clear(vector);
    sj_value_as_vector4d(sj_object_get_value(json, "dimensions"), &vector);
    win->dimensions = vector4d(vector.x, vector.y, vector.z, vector.w);


    elements = sj_object_get_value(json, "elements");
    count = sj_array_get_count(elements);
    for (i = 0; i < count; i++)
    {
        value = sj_array_get_nth(elements, i);
        if (!value)continue;
        slog("adding element");
        menu_add_element(win, menu_element_load_from_config(value));
    }
    slog("menu elements added");
    return win;
}

/*eol@eof*/