import sys,os

def CheckSDLLib(context):
    """
    On Mac OS X, SDL seems to use the same ugly trick
    it uses on Windows (redefining main() with SDLmain).
    This wrecks CheckLibWithHeader() who creates a very simple
    main(), which unfortunately won't match the prototype in SDL_main.h,
    thus causing the check to fail. Boo!
    No idea why it actually worked on Windows, though.
    """
    context.Message('Checking for SDL...')
    text = """
#include "SDL.h"
int main(int argc, char* argv[]) { return 0; }
"""
    if context.BuildProg(text, ".c"):
        context.Result("failed")
        return False
    else:
        context.Result("ok")
        return True

# create build environment
env = Environment(ENV = os.environ)
conf = Configure(env, custom_tests = {
    'CheckSDLLib': CheckSDLLib,
})

if sys.platform == "win32":
    print "Building for Windows."
    env.Tool('msvc')
    env.Append(CFLAGS = ["/MD", "/Ox"])
    env.Append(LINKFLAGS = ["/subsystem:console"])
    env.Append(CPPPATH = env['ENV']['INCLUDE'])
    env.Append(LIBPATH = env['ENV']['LIB'])

    # Additional Libraries
    env.Append(LIBS = ['SDL', 'SDLmain', 'SDL_image', 'SDL_ttf', 'SDL_mixer'])

else:
    # compiler and linker flags for SDL
    print "Building for Unix."
    try:
        env.ParseConfig('sdl-config --cflags')
        env.ParseConfig('sdl-config --libs')
    except Exception:
        print "Error: Unable to execute sdl-config"

    # add additional compiler flags
    env.Append(CFLAGS = ['-g', '-O2', '-Wall', '-std=c99'])

    # add additional libraries to link against
    env.Append(LIBS = ['SDL_image', 'SDL_ttf', 'SDL_mixer', 'GL'])


# We now use our own check for this.
if not conf.CheckSDLLib():
    print "Could not find SDL headers."
    if sys.platform == "win32":
        print "win32 tip: Check the INCLUDE and LIBRARY env. variables."

    Exit(1)

env = conf.Finish()

# gather a list of source files
SOURCES = ['main.c','sinescroller.c','sound.c','stars.c','timeslice.c','utils.c',]

# build target
env.Program(target = 'bacondemo', source = SOURCES)
