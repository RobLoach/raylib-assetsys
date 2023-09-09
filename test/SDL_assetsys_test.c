#include <SDL2/SDL.h>

#define SDL_ASSETSYS_IMPLEMENTATION
#include "../SDL_assetsys.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

	if (SDL_Init(0) != 0) {
        return 1;
    }

    // assetsys_create()
    assetsys_t* sys = assetsys_create(0);
    SDL_assert(sys != NULL);

    // assetsys_mount()
    SDL_assert(assetsys_mount(sys, "resources", "/res") == ASSETSYS_SUCCESS);

    // SDL_assetsys_RWFromFile()
    SDL_RWops* file = SDL_RWFromAssetsys(sys, "/res/test.txt");
    SDL_assert(file != NULL);

    // SDL_RWsize()
    Sint64 size = SDL_RWsize(file);
    SDL_assert(size > 5);

    // SDL_RWread()
    char output[256];
    SDL_assert(SDL_RWread(file, output, (size_t)1, (size_t)size) > 5);

    SDL_assert(output[0] == 'H');
    SDL_assert(output[1] == 'e');
    SDL_assert(output[2] == 'l');
    SDL_assert(output[3] == 'l');
    SDL_assert(output[4] == 'o');

    // SDL_RWclose()
    SDL_assert(SDL_RWclose(file) == 0);

    SDL_Log("SDL_assert_test: Tests passed\n");
	SDL_Quit();
    
    return 0;
}
