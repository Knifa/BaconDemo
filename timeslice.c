#include "SDL.h"

#include "timeslice.h"

/**
 * Initialize timeslice structure
 * Sets initial values and resets status
 */
void timeslice_init(timeslice *t)
{
    t->ticks_start = 0;
    t->ticks_paused = 0;
    t->ispaused = 0;
    t->isstarted = 0;
}

/**
 * Start timeslice structure and records initial
 * tick count from SDL
 */
void timeslice_start(timeslice *t) 
{
    t->isstarted = 1;
    t->ispaused = 0;

    t->ticks_start = SDL_GetTicks();
}

/**
 * Stops timeslice structure, sets status to stopped.
 */
void timeslice_stop(timeslice *t) {
    if (t->isstarted == 1) {
        t->isstarted = 0;
        t->ispaused = 0;
    }
}

/**
 * Pauses timeslice structure, effectively puts all motion
 * affected by it on hold.
 */
void timeslice_pause(timeslice *t)
{
    if ((t->isstarted == 1) && (t->ispaused == 0)) {
        t->ispaused = 1;
        
        t->ticks_paused = SDL_GetTicks() - t->ticks_start;
    }
}

/**
 * Unpause timeslice structure, sets start tick value
 * to include elapsed ticks during paused time.
 */
void timeslice_unpause(timeslice *t)
{
    if(t->ispaused == 1) {
        t->ispaused = 0;
        t->ticks_start = SDL_GetTicks() - t->ticks_paused;
        t->ticks_paused = 0; // reset paused tick count
    }
}


/**
 * Get the current elapsed ticks since last run from
 * timeslice structure. Used to step rendering and motions
 * in a stable fashion.
 */
double timeslice_get_ticks(timeslice *t)
{
    if(t->isstarted == 1) {
        if(t->ispaused == 1) {
            return t->ticks_paused;
        } else {
            return SDL_GetTicks() - t->ticks_start;
        }
    } else {
        return 0;
    }
}
