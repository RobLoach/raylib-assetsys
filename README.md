# raylib-assetsys

Use the file system abstraction library, [assetsys.h](https://github.com/mattiasgustavsson/libs/blob/main/assetsys.h), with [raylib](https://github.com/raysan5/raylib).

## API

``` c
bool InitAssetsys(char const* path, char const* mount_as);
bool MountAssetsys(const char* path, const char* mount_as);
bool IsAssetsysReady();
void CloseAssetsys();
unsigned char *LoadFileDataFromAssetsys(const char *fileName, int *dataSize);
char *LoadFileTextFromAssetsys(const char *fileName);
Image LoadImageFromAssetsys(const char* fileName);
```

## Example

``` c
#include <raylib.h>

#define RAYLIB_ASSETSYS_IMPLEMENTATION
#include "raylib-assetsys.h"

int main(int argc, char* argv[]) {
    // Initialize assetsys, and mount resources.zip as /data
    InitAssetsys("resources.zip", "/data");

    // Load cat.png from the zip
    Image cat = LoadImageFromAssetsys("/data/cat.png");
    UnloadImage(cat);

    // Close the assetsys
    CloseAssetsys();

    return 0;
}
```

## License

*raylib-assetsys* is licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.
