#include "raylib.h"

#define RAYLIB_ASSETSYS_IMPLEMENTATION
#include "../raylib-assetsys.h"

#include "raylib-assert.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // InitAssetsys()
    Assert(InitAssetsys("resources", "/res"));

    // IsAssetsysReady()
    Assert(IsAssetsysReady());

    // LoadFileTextFromAssetsys()
    int dataSize;
    unsigned char* data = LoadFileTextFromAssetsys("/res/test.txt");
    AssertNotEqual(data, NULL);
    AssertEqual(TextLength(data), 14);
    Assert(TextIsEqual(TextSubtext(data, 0, 5), "Hello"));
    UnloadFileText(data);

    // LoadImageFromAssetsys()
    Image carl = LoadImageFromAssetsys("/res/carlsagan.png");
    Assert(IsImageReady(carl));
    UnloadImage(carl);

    // CloseAssetsys()
    CloseAssetsys();
    AssertNot(IsAssetsysReady());

    TraceLog(LOG_INFO, "[ASSETSYS] Tests passed");

    return 0;
}
