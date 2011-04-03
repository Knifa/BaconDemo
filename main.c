/*
 * File:   main.c
 * Author: Knifa
 *
 * Created on 01 April 2011, 07:13
 */

// Because fuck you.
typedef int bool;
#define true    1
#define false   0

// -----------------------------------------------------------------------------

// Standard includes
#include <stdio.h>

// GL includes
#include <GL/gl.h>

// SDL includes
#include "SDL.h"
#include "SDL_mixer.h"

// Bacon Demo includes
#include "defs.h"
#include "stars.h"
#include "sinescroller.h"
#include "sound.h"

// ----------------------------------------------------------------------------

// Function declartions.
void Init();
void Run();

// -----------------------------------------------------------------------------

// The SDL_Surface which points to our window.
SDL_Surface *surfScreen = NULL;

// -----------------------------------------------------------------------------

int main(int argc, char** argv) {
    Init();
    Run();

    return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------

void Init() {
    // Initalise SDL.
    SDL_Init(SDL_INIT_EVERYTHING);

    // Give us a clever, witty window title.
    SDL_WM_SetCaption("Bacon Demo", NULL);

    // Make our display surface.
    surfScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32,
            SDL_OPENGL | SDL_HWSURFACE);

    // Set up our projectional matrix, giving us an orthagonal view that is the
    // same dimensions as the screen.
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

    // Enable textures and blending.
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Init audio.
    Sound_Init();
    
    // Initalize the star field.
    Stars_Init();

    // Initalize the sine scroller.
    SineScroller_Init();
}

void Run() {
    bool isRunning = true;

    // Loop untl something tells us to gtfo.
    while (isRunning) {
        // Poll for events.
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                // Application been asked to close (window button pressed, etc)
                case SDL_QUIT:
                    isRunning = false;
                    break;
            }
        }

        Stars_Update();
        Stars_Draw();

        SineScroller_Update();
        SineScroller_Draw();
        
        // Swap back and front buffer and clear front buffer.
        SDL_GL_SwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Delay makes it 60fps ish.
        SDL_Delay((int)(1.0f/60.0f));
    }
}

// -----------------------------------------------------------------------------
