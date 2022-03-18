#ifndef __BUG_ENT_H__
#define __BUG_ENT_H__


#include "entity.h"

/**
 * @brief creates an instance of a bug entity
 * @param position the location in the world to spawn the bug at
 * @returns NULL on error, or a pointer to the bug
 */
Entity *bug_ent_new(Vector2D position, SDL_GameController* controller);

#endif
