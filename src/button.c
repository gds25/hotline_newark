#include "button.h"
#include "menu_elements.h"
#include "mouse.h"
//#include "gf2d_element_actor.h"
#include "simple_logger.h"

void menu_element_button_draw(Element* element, Vector2D offset)
{
    slog("DRAWING BUTTON");
    ButtonElement* button;
    Vector2D position;
    //Actor* actor;
    if (!element) { slog("not an element");  return; }
    button = (ButtonElement*)element->data;
    if (!button) { slog("not a button"); return; }
    //slog("bdjkbfjksdb");
    vector2d_add(position, offset, element->bounds);
    //actor = gf2d_element_actor_get_actor(button->actor);
    /*switch (element->state)
    {
    case ES_hidden:
    case ES_disable:
        return;
    case ES_idle:
        gf2d_actor_set_action(actor, "idle");
        break;
    case ES_highlight:
        gf2d_actor_set_action(actor, "high");
        break;
    case ES_active:
        gf2d_actor_set_action(actor, "press");
        break;
    }*/
    //menu_element_draw(button->actor, position); 
    gf2d_draw_rect(gfc_SDL_Rect(button->bounds.x, button->bounds.y, button->bounds.z, button->bounds.w), vector4d(255, 100, 100, 255));
    //menu_element_draw(element, position);
    gf2d_sprite_draw(
        button->sprite,
        position,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        0);
    gf2d_font_draw_line_tag(element->name, FT_H5, gfc_color8(0, 0, 0, 0), position);
}


List* menu_element_button_update(Element* element, Vector2D offset)
{
   // Actor* actor;
    SDL_Rect bounds;
    List* list;
    ButtonElement* button;
    if (!element)return NULL;
    button = (ButtonElement*)element->data;
    if (!button)return NULL;
   // actor = menu_element_actor_get_actor(button->actor);
   // menu_actor_next_frame(actor);
    bounds = menu_element_get_absolute_bounds(element, offset);
    if (mouse_in_rect(vector4d(bounds.x, bounds.y, bounds.w, bounds.h)))
    {
        element->state = ES_highlight;
        if (mouse_button_state(0))
        {
            element->state = ES_active;
        }
        else if (mouse_button_released(0))
        {
            list = gfc_list_new();
            gfc_list_append(list, element);
            return list;
        }
    }
    else
    {
        element->state = ES_idle;
    }

    return NULL;
}

void menu_element_button_free(Element* element)
{
    ButtonElement* button;
    if (!element)return;
    button = (ButtonElement*)element->data;
    if (button != NULL)
    {
        menu_element_free(button->label);
        //menu_element_free(button->actor);
        free(button);
    }
}

ButtonElement* menu_element_button_new()
{
    ButtonElement* button;
    button = (ButtonElement*)malloc(sizeof(ButtonElement));
    if (!button)
    {
        slog("failed to allocate memory for button");
        return NULL;
    }
    memset(button, 0, sizeof(ButtonElement));
    return button;
}


void menu_element_make_button(Element* e, ButtonElement* button)
{
    if (!e)return;
    e->data = button;
    e->type = ET_Button;
    e->draw = menu_element_button_draw;
    e->update = menu_element_button_update;
    e->free_data = menu_element_button_free;
}

ButtonElement* menu_element_button_new_full(Element* label, Color highColor, Color pressColor, Vector4D bounds)
{
    slog("CREATING BUTTON");
    ButtonElement* button;
    button = menu_element_button_new();
    if (!button) { slog("not a button"); return NULL; }
    button->label = label;
    //button->actor = actor;
    button->highColor = highColor;
    button->pressColor = pressColor;
    button->sprite = gf2d_sprite_load_image("images/button_bg.png");
    button->bounds = bounds;
    
    return button;
}

void menu_element_load_button_from_config(Element* e, SJson* json)
{
    Vector4D highColor = { 255,255,255,255 }, pressColor = { 255,255,255,255 };
    Vector4D bounds = { 255,255,255,255 };
    Element* label = NULL;
    Element* actor = NULL;
    SJson* value;

    if ((!e) || (!json))
    {
        slog("call missing parameters");
        return;
    }

    value = sj_object_get_value(json, "highColor");
    if (!sj_value_as_vector4d(value, &highColor))
    {
        slog("highColor not provided");
    }

    value = sj_object_get_value(json, "pressColor");
    if (!sj_value_as_vector4d(value, &pressColor))
    {
        slog("pressColor not provided");
    }
    
    value = sj_object_get_value(json, "bounds");
    if (!sj_value_as_vector4d(value, &bounds))
    {
        slog("pressColor not provided");
    }

    value = sj_object_get_value(json, "label");
    if (value)
    {
        label = menu_element_load_from_config(value);
    }
    value = sj_object_get_value(json, "actor");
    if (value)
    {
        actor = menu_element_load_from_config(value);
    }
    menu_element_make_button(e, menu_element_button_new_full(label, gfc_color_from_vector4(highColor), gfc_color_from_vector4(pressColor), bounds));
}


/*eol@eof*/