// Standard includes
#include <math.h>

// SDL includes
#include "SDL.h"

// GL includes
#ifdef MSVC
#include <windows.h>
#endif

#if defined(__APPLE__)&& defined(__MACH__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// Bacon includes
#include "defs.h"
#include "stars.h"
#include "utils.h"

// -----------------------------------------------------------------------------

// Star array for the starfield.
Star stars[STARS_MAX];
float stars_xVel = 0.0f;
float stars_yVel = 0.0f;
float stars_angle = 0.0f;

// -----------------------------------------------------------------------------

void Stars_Init() {
    int i;
    for (i = 0; i < STARS_MAX; i++) {
        Star* s = &stars[i];

        // Generate a random position for our stars. Snap X, Y to nearest 4.
        s->x = Utils_Snap(Utils_Random(0, SCREEN_WIDTH));
        s->y = Utils_Snap(Utils_Random(0, SCREEN_HEIGHT));
        s->z = Utils_Random(1, STARS_MAX_DEPTH);
    }
}

void Stars_Draw() {
    int i;

    // star values
    int x;
    int y; 
    float size;

    // Disable textures to make sure we aren't randomly given a texture.
    glDisable(GL_TEXTURE_2D);

    // Loop through ze stars and stuff, yo.
    for (i = 0; i < STARS_MAX; i++) {
        Star* s = &stars[i];

        // Change the colour depending on the Z distance.
        glColor3f(1.0f/powf(s->z, STARS_DEPTH_POW),
            1.0f/powf(s->z, STARS_DEPTH_POW),
            1.0f/powf(s->z, STARS_DEPTH_POW));

        // Calculate the size of the star.
        size = STARS_SIZE / powf(s->z, STARS_DEPTH_POW);
        if (size < 1)
            size = 1;

        // Low-res effect
        x = Utils_Snap(s->x);
        y = Utils_Snap(s->y);

        // Draw the actual star.
        glBegin(GL_QUADS);
        {
            glVertex2f(x - size, y - size);
            glVertex2f(x + size, y - size);
            glVertex2f(x + size, y + size);
            glVertex2f(x - size, y + size);
        }
        glEnd();
    }
}

void Stars_Update(double tdelta) {
    int i;

    // Update the xvelocity and increase the angle.
    stars_xVel = (Utils_Sin(stars_angle) * STARS_SPEED);
    stars_angle = fmod(stars_angle + 0.5f * (tdelta / 1000.f), 2 * PI);
    
    // Update the X velocity.
    stars_xVel = (Utils_Sin(stars_angle) * STARS_SPEED);

    // Loop through all of the stars
    for (i = 0; i < STARS_MAX; i++) {
        Star* s = &stars[i];

        // Move the stars according to Z value.
        s->x -= (stars_xVel / s->z) * (tdelta / 1000.f);
        s->y -= (STARS_SPEED / s->z) * (tdelta / 1000.f);

        // Make sure we wrap around the screen.
        if (s->x > SCREEN_WIDTH)
            s->x = 0;
        else if (s->x < 0)
            s->x = SCREEN_WIDTH;

        if (s->y > SCREEN_HEIGHT)
            s->y = 0;
        else if (s->y < 0)
            s->y = SCREEN_HEIGHT;
    }
}
