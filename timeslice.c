#include "SDL.h"

#include "timeslice.h"

void timeslice_init(timeslice *t)
{
    t->ticks_start = 0;
    t->ticks_paused = 0;
    t->ispaused = 0;
    t->isstarted = 0;
}

void timeslice_start(timeslice *t) 
{
    t->isstarted = 1;
    t->ispaused = 0;

    t->ticks_start = SDL_GetTicks();
}

void timeslice_stop(timeslice *t) {
    if (t->isstarted == 1) {
        t->isstarted = 0;
        t->ispaused = 0;
    }
}

void timeslice_pause(timeslice *t)
{
    if ((t->isstarted == 1) && (t->ispaused == 0)) {
        t->ispaused = 1;
        
        t->ticks_paused = SDL_GetTicks() - t->ticks_start;
    }
}

void timeslice_unpause(timeslice *t)
{
    if(t->ispaused == 1) {
        t->ispaused = 0;
        t->ticks_start = SDL_GetTicks() - t->ticks_paused;
        t->ticks_paused = 0; // reset paused tick count
    }
}

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
