#include <stdlib.h>
#include <string.h>
#include "menu_elements.h"
//#include "menu_element_actor.h"
#include "button.h"
//#include "menu_element_list.h"
#include "label.h"
#include "simple_logger.h"

Element* menu_element_new()
{
    Element* e;
    e = (Element*)malloc(sizeof(Element));
    if (!e)
    {
        slog("failed to allocate a new menu element");
        return NULL;
    }
    memset(e, 0, sizeof(Element));
    return e;
}

Element* menu_element_new_full(
    int      index,
    TextLine name,
    SDL_Rect bounds,
    Color color,
    int state
)
{
    Element* e;
    e = menu_element_new();
    if (!e)return NULL;
    gfc_line_cpy(e->name, name);
    e->index = index;
    e->color = color;
    e->state = state;
    e->bounds = bounds;
    return e;
}

void menu_element_free(Element* e)
{
    if (!e)return;
    if (e->free_data)
    {
        e->free_data(e);
    }
    free(e);
}

void menu_element_draw(Element* e, Vector2D offset)
{
    SDL_Rect rect;
    if (!e)
    {
        slog("no element to draw");
        return;
    }
    
    if (e->draw) { e->draw(e, offset); }
      gfc_rect_set(rect,offset.x + e->bounds.x,offset.y + e->bounds.y,e->bounds.w,e->bounds.h);
     // SDL_RenderDrawRect(gf2d_graphics_get_renderer(), (const struct SDL_Rect*)&rect);
}

List* menu_element_update(Element* e, Vector2D offset)
{
    if (!e)
    {
        return NULL;
    }
    if (e->draw)return e->update(e, offset);
    return NULL;
}

Element* menu_element_load_from_config(SJson* json)
{
    Element* e = NULL;
    SJson* value;
    const char* type;
    Vector4D vector;
    if (!sj_is_object(json))return NULL;
    e = menu_element_new();
    if (!e)return NULL;
    value = sj_object_get_value(json, "name");
    gfc_line_cpy(e->name, sj_get_string_value(value));

    value = sj_object_get_value(json, "id");
    sj_get_integer_value(value, &e->index);

    value = sj_object_get_value(json, "color");
    vector4d_set(vector, 255, 255, 255, 255);
    sj_value_as_vector4d(value, &vector);
    e->color = gfc_color_from_vector4(vector);

    value = sj_object_get_value(json, "bounds");
    sj_value_as_vector4d(value, &vector);
    gfc_rect_set(e->bounds, vector.x, vector.y, vector.z, vector.w);

    value = sj_object_get_value(json, "type");
    type = sj_get_string_value(value);
    if (strcmp(type, "label") == 0)
    {
        menu_element_load_label_from_config(e, json);
    }
    else if (strcmp(type, "button") == 0)
    {
        menu_element_load_button_from_config(e, json);
    }
    else if (strcmp(type, "percent") == 0)
    {
    }
    return e;
}

SDL_Rect menu_element_get_absolute_bounds(Element* element, Vector2D offset)
{
    SDL_Rect r = { 0 };
    if (!element)return r;
    r.x = element->bounds.x + offset.x;
    r.y = element->bounds.y + offset.y;
    r.w = element->bounds.w;
    r.h = element->bounds.h;
    return r;
}

/*eol@eof*/