// Standard inclludes
#include <stdio.h>
#include <math.h>

#ifdef MSVC
#include <windows.h>
#endif

// SDL includes
#include "SDL.h"
#include "SDL_ttf.h"

// GL includes
// TODO: Cleanup this mess with either platform specific headers, or
//       with a proper config.h header that defines what we need. Urgh.

// Mac OS X and win32-msvc vs The World
#if defined(__APPLE__)&& defined(__MACH__)
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#ifdef MSVC
    #include "compat/glext.h"
#else
    #if defined(__APPLE__)&& defined(__MACH__)
        #include <OpenGL/glext.h>
    #else
        #include <GL/glext.h>
    #endif
#endif

// Bacon includes
#include "defs.h"
#include "sinescroller.h"
#include "utils.h"

// -----------------------------------------------------------------------------

// The OpenGL texture number.
GLuint texture;


// Dimensions of the scroller
int texture_width = 0;
int texture_height = 0;

// Current x offset
float x_origin = SCREEN_WIDTH;

// Angle
float angle = 0;

// -----------------------------------------------------------------------------

void SineScroller_Init() {
    // SDL / SDL_ttf structures
    SDL_Surface* surface;
    SDL_Color color = {255, 255, 255}; 
    TTF_Font* font;

    // Init the TTF lib
    TTF_Init();
    font = TTF_OpenFont(SCROLLER_FONT, SCROLLER_FONT_SIZE);

    // Render the text surface.
    surface = TTF_RenderText_Blended(font, SCROLLER_TEXT, color);
    
    // Generate an OpenGL texture for us.
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGBA,
            surface->w,
            surface->h,
            0,
            GL_BGRA,
            GL_UNSIGNED_BYTE,
            surface->pixels);

    // Set the textur scaling business.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Save the texture deimsnions
    texture_width = surface->w;
    texture_height = surface->h;
}

void SineScroller_Update(double tdelta) {
    // Move the text along.
    x_origin -= SCROLLER_SPEED * (tdelta / 1000.f);

    // Wrap around the screen.
    if (x_origin < -(texture_width*SCROLLER_STRIP_SIZE))
        x_origin = SCREEN_WIDTH;

    // Increase angle
    angle = fmod(angle + 0.02f, 2 * PI);
}

void SineScroller_Draw() {
    int i;

    // Shape values
    float theta;
    float sin_value;
    int x_offset;
    int y_offset;
    int height_offset;
    float color; // FIXME: Ambiguous because SDL_color structure

    // Enable texturing, and set our texture to the font.
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Loop through each horizontal strip of the text so we can
    // make them go in a sine wave.
    for (i = 0; i < texture_width; i++) {
        // Calculate the sin value for this loop.
        theta = i * SCROLLER_STRIP_SIZE / SCREEN_WIDTH * PI + angle;
        sin_value = Utils_Sin(theta);

        // Get origin of shape.
        x_offset = x_origin + (i*SCROLLER_STRIP_SIZE);
        y_offset = (SCREEN_HEIGHT/2) +
            (sin_value * texture_height * 5.0f);

        // Get height (or rather, half of height) for the strip drawing.
        height_offset = abs(
            (texture_height * SCROLLER_STRIP_SIZE / 2.0f) *
            ((Utils_Sin((sin_value*PI)/2)+1) / 2));

        // Low res effect
        x_offset = (int)(x_offset / 4) * 4;
        y_offset = (int)(y_offset / 4) * 4;

        // Generate a colour also based off of the sin thingy.
        color = (int)(((sin_value + 1) / 2) / 0.1) * 0.1f;
        glColor4f(1.0f - color, color, 1.0f, 0.8f);

        // Draw the strip.
        glBegin(GL_QUADS);
        {
            glTexCoord2f(1.0f/texture_width*i, 0);
            glVertex2f(
                    x_offset,
                    (y_offset - height_offset)
                    );

            glTexCoord2f(1.0f/texture_width*(i+1), 0);
            glVertex2f(
                    (x_offset + SCROLLER_STRIP_SIZE),
                    (y_offset - height_offset)
                    );


            glTexCoord2f(1.0f/texture_width*(i+1), 1);
            glVertex2f(
                    (x_offset + SCROLLER_STRIP_SIZE),
                    (y_offset + height_offset)
                    );

            glTexCoord2f(1.0f/texture_width*i, 1);
            glVertex2f(
                    x_offset,
                    (y_offset + height_offset)
                    );
        }
        glEnd();
    }

    // Disable texturing.
    glDisable(GL_TEXTURE_2D);

    // Loop through the entire width of the screen. Draw a border.
    for (i = 0; i < SCREEN_WIDTH; i += 8)
    {
        // Calculate sin value for this loop.
        theta = 
            i / 4.0f * SCROLLER_STRIP_SIZE / SCREEN_WIDTH * PI + angle;
        sin_value = Utils_Sin(theta);

        // Get the colour and set it.
        color = (int)(((sin_value + 1) / 2) / 0.1) * 0.1f;
        glColor4f(1.0f - color, color, 1.0f, 0.8f);

        // Draw the border lines.
        glBegin(GL_QUADS);
        {
            glVertex2f(i, 8);
            glVertex2f(i+8, 8);
            glVertex2f(i+8, 16);
            glVertex2f(i, 16);

            glVertex2f(i, SCREEN_HEIGHT-8);
            glVertex2f(i+8, SCREEN_HEIGHT-8);
            glVertex2f(i+8, SCREEN_HEIGHT-16);
            glVertex2f(i, SCREEN_HEIGHT-16);

            // Change colour to a more tarnsparent version, draw another
            // line directly above and below.
            glColor4f(1.0f - color, color, 1.0f, 0.33f);

            glVertex2f(i, 16);
            glVertex2f(i+8, 16);
            glVertex2f(i+8, 24);
            glVertex2f(i, 24);

            glVertex2f(i, SCREEN_HEIGHT-16);
            glVertex2f(i+8, SCREEN_HEIGHT-16);
            glVertex2f(i+8, SCREEN_HEIGHT-24);
            glVertex2f(i, SCREEN_HEIGHT-24);
        }
        glEnd();
    }
}
