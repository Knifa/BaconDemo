/* 
 * File:   utils.h
 * Author: Knifa
 *
 * Created on 01 April 2011, 14:23
 */

#ifndef UTILS_H
#define	UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef PI
#define PI 3.14f
#endif

// -----------------------------------------------------------------------------

#define UTILS_SNAP      4
#define UTILS_LOOKUP_SIZE     512
    
// -----------------------------------------------------------------------------

float Utils_Random(float min, float max);
float Utils_Sin(float theta);

int Utils_Snap(float i, int snap_size);
int Utils_Snap(float i);


// -----------------------------------------------------------------------------

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

