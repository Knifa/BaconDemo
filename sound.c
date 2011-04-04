// SDL Includes
#include "SDL_mixer.h"

// Bacon Includes
#include "sound.h"

// -----------------------------------------------------------------------------

void Sound_Init() {
    // Initalize the library and open the mixer.
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

    // Load the audio file.
    Mix_Music* music = Mix_LoadMUS(AUDIO_FILE);

    // Play it forever!
    Mix_PlayMusic(music, -1);
}
