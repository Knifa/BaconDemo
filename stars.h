/* 
 * File:   stars.h
 * Author: Knifa
 *
 * Created on 01 April 2011, 14:18
 */

#ifndef STARS_H
#define	STARS_H

#ifdef	__cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------

// Star configuration
#define STARS_MAX           1024
#define STARS_MAX_DEPTH     16
#define STARS_DEPTH_POW     1.4f
#define STARS_SIZE          4.0f
#define STARS_SPEED         400.0f

// -----------------------------------------------------------------------------

// Star struct for the starfield.
typedef struct {
    float x, y, z;
} Star;

// Starfield functions.
void Stars_Init();
void Stars_Draw();
void Stars_Update(double tdelta);

// -----------------------------------------------------------------------------

#ifdef	__cplusplus
}
#endif

#endif	/* STARS_H */

