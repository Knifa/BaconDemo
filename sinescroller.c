// Standard inclludes
#include <math.h>

// SDL includes
#include "SDL.h"
#include "SDL_ttf.h"

// GL includes
#if defined(__APPLE__)&& defined(__MACH__)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
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
    // Initialise the TTF lib
    TTF_Init();
    TTF_Font* font = TTF_OpenFont(SCROLLER_FONT, SCROLLER_FONT_SIZE);

    // Render the text surface.
    SDL_Color color = {255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, SCROLLER_TEXT, color);
    
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

    // Set the texture scaling business.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Save the texture dimensions
    texture_width = surface->w;
    texture_height = surface->h;
}

void SineScroller_Update(double tdelta) {
    // Move the text along.
    x_origin -= SCROLLER_SPEED * (tdelta / 1000.f);

    // Wrap around the screen.
    if (x_origin < -(texture_width*SCROLLER_STRIP_SIZE))
        x_origin = SCREEN_WIDTH;

    // Increase angle.
    angle = fmod(angle + (1.5f * (tdelta / 1000.f)), 2 * PI);
}

void SineScroller_Draw() {
    // Enable texturing, and set our texture to the font.
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Loop through each horizontal strip of the text so we can
    // make them go in a sine wave.
    for (int i = 0; i < texture_width; i++) {
        // Calculate the sin value for this loop.
        float theta = i * SCROLLER_STRIP_SIZE / SCREEN_WIDTH * PI + angle;
        float sin_value = Utils_Sin(theta);

        // Get origin of shape. Apply low-res effect.
        int x_offset = Utils_Snap(x_origin + (i*SCROLLER_STRIP_SIZE));
        int y_offset = Utils_Snap((SCREEN_HEIGHT/2) +
            (sin_value * texture_height * 5.0f));

        // Get height (or rather, half of height) for the strip drawing.
        int height_offset = abs(
            (texture_height * SCROLLER_STRIP_SIZE / 2.0f) *
            ((Utils_Sin((sin_value*PI)/2)+1) / 2));

        // Generate a colour also based off of the sin thing.
        float color = roundf(((sin_value + 1) / 2) / 0.1) * 0.1f;

        // Only draw what we can see.
        if (x_offset >= 0 && x_offset <= SCREEN_WIDTH)
        {
            // Draw the strip.
            glBegin(GL_QUADS);
            {
                // Set our colour.
                glColor4f(1.0f - color, color, 1.0f, 0.8f);

                // Top left.
                glTexCoord2f(1.0f/texture_width*i, 0);
                glVertex2f(
                        x_offset,
                        (y_offset - height_offset)
                        );

                // Top right
                glTexCoord2f(1.0f/texture_width*(i+1), 0);
                glVertex2f(
                        (x_offset + SCROLLER_STRIP_SIZE),
                        (y_offset - height_offset)
                        );

                // Bottom right
                glTexCoord2f(1.0f/texture_width*(i+1), 1);
                glVertex2f(
                        (x_offset + SCROLLER_STRIP_SIZE),
                        (y_offset + height_offset)
                        );

                // Bottom left
                glTexCoord2f(1.0f/texture_width*i, 1);
                glVertex2f(
                        x_offset,
                        (y_offset + height_offset)
                        );
            }
            glEnd();
        }
    }

    // Disable texturing so we can draw the shadow thing.
    glDisable(GL_TEXTURE_2D);

    // Loop through the entire width of the screen. Draw a border.
    for (int i = 0; i < SCREEN_WIDTH; i += SCROLLER_STRIP_SIZE)
    {
        // Calculate sin value for this loop.
        float theta = 
            i / 4.0f * SCROLLER_STRIP_SIZE / SCREEN_WIDTH * PI + angle;
        float sin_value = Utils_Sin(theta);

        // Get the colour
        float color = roundf(((sin_value + 1) / 2) / 0.1) * 0.1f;

        // Draw the border lines.
        glBegin(GL_QUADS);
        {
            // Set our colour.
            glColor4f(1.0f - color, color, 1.0f, 0.8f);
                 
            // Top
            glVertex2f(i, SCROLLER_STRIP_SIZE);
            glVertex2f(i + SCROLLER_STRIP_SIZE, SCROLLER_STRIP_SIZE);
            glVertex2f(i + SCROLLER_STRIP_SIZE, SCROLLER_STRIP_SIZE*2);
            glVertex2f(i, SCROLLER_STRIP_SIZE*2);

            // Bottom
            glVertex2f(i, SCREEN_HEIGHT - SCROLLER_STRIP_SIZE);
            glVertex2f(i + SCROLLER_STRIP_SIZE, 
                    SCREEN_HEIGHT - SCROLLER_STRIP_SIZE);
            glVertex2f(i + SCROLLER_STRIP_SIZE, 
                    SCREEN_HEIGHT - (SCROLLER_STRIP_SIZE*2));
            glVertex2f(i, SCREEN_HEIGHT - (SCROLLER_STRIP_SIZE*2));
            
            // Change colour to a more transparent version, draw another
            // line directly above and below.
            glColor4f(1.0f - color, color, 1.0f, 0.33f);

            // Top (shadow)
            glVertex2f(i, SCROLLER_STRIP_SIZE*2);
            glVertex2f(i + SCROLLER_STRIP_SIZE, SCROLLER_STRIP_SIZE*2);
            glVertex2f(i + SCROLLER_STRIP_SIZE, SCROLLER_STRIP_SIZE*3);
            glVertex2f(i, SCROLLER_STRIP_SIZE*3);

            // Bottom (shadow)
            glVertex2f(i, SCREEN_HEIGHT - SCROLLER_STRIP_SIZE*2);
            glVertex2f(i + SCROLLER_STRIP_SIZE, 
                    SCREEN_HEIGHT - SCROLLER_STRIP_SIZE*2);
            glVertex2f(i + SCROLLER_STRIP_SIZE, 
                    SCREEN_HEIGHT - (SCROLLER_STRIP_SIZE*3));
            glVertex2f(i, SCREEN_HEIGHT - (SCROLLER_STRIP_SIZE*3));
        }
        glEnd();
    }
}
