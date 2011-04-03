/* Oh dear god fuck I just want to oop oh god oh god */

#ifndef TIMESLICE_H
#define TIMESLICE_H 1

typedef struct timer_s {
    int ticks_start;
    int ticks_paused;
    
    int ispaused;
    int isstarted;
} timeslice;

void timeslice_init(timeslice *t);
void timeslice_start(timeslice *t);
void timeslice_stop(timeslice *t);
void timeslice_pause(timeslice *t);
void timeslice_unpause(timeslice *t);
double timeslice_get_ticks(timeslice *t);
#endif



