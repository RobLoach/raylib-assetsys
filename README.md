# SDL_assetsys

Use the file system abstraction library, [assetsys.h](https://github.com/mattiasgustavsson/libs/blob/main/assetsys.h), with SDL.

## API

``` c
SDL_RWops* SDL_RWFromAssetsys(assetsys_t* sys, const char* path);
SDL_Surface* SDL_LoadBMP_Assetsys(asset_t* sys, const char* path);
SDL_Surface* IMG_Load_Assetsys(asset_t* sys, const char* path);
```

## Example

``` c
#include <stdio.h>
#include <SDL2/SDL.h>

#define SDL_ASSETSYS_IMPLEMENTATION
#include "SDL_assetsys.h"

int main(int argc, char* argv[]) {
	SDL_Init(0);

    assetsys_t* sys = assetsys_create(0);
    assetsys_mount(sys, "resources", "/res");

    SDL_RWops* file = SDL_RWFromAssetsys(sys, "/res/test.txt");

    int size = SDL_RWsize(file);

    char output[256];
    SDL_RWread(file, output, 1, size);

    printf("Output: %s\n", output);
    // => Hello, World!

    SDL_RWclose(file);

	SDL_Quit();

    return 0;
}
```

## License

*SDL_assetsys* is licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.
