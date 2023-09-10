/**********************************************************************************************
*
*   SDL_assetsys: Use assetsys, a file system abstraction library, to read SDL assets from zip files.
*
*   Copyright 2023 Rob Loach (@RobLoach)
*
*   DEPENDENCIES:
*       SDL https://github.com/libsdl-org/SDL
*       assetsys https://github.com/mattiasgustavsson/libs/blob/main/assetsys.h
*       strpool https://github.com/mattiasgustavsson/libs/blob/main/strpool.h
*
*   LICENSE: MIT
*
*       Copyright (c) 2023 Rob Loach (@RobLoach)
*
*       Permission is hereby granted, free of charge, to any person obtaining a copy of this
*       software and associated documentation files (the "Software"), to deal in the Software
*       without restriction, including without limitation the rights to use, copy, modify,
*       merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
*       permit persons to whom the Software is furnished to do so, subject to the following
*       conditions:

*       The above copyright notice and this permission notice shall be included in all copies
*       or substantial portions of the Software.

*       THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
*       INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
*       PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
*       HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
*       CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
*       OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
**********************************************************************************************/

#ifndef SDL_ASSETSYS_H_
#define SDL_ASSETSYS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SDL_ASSETSYS_API
    #define SDL_ASSETSYS_API
#endif

#ifndef SDL_ASSETSYS_ASSETSYS_H
/**
 * The path to where assetsys.h exists.
 */
#define SDL_ASSETSYS_ASSETSYS_H "libs/assetsys.h"
#endif
#include SDL_ASSETSYS_ASSETSYS_H

/**
 * Load a SDL_RWops from the given path in the provided assetsys_t.
 *
 * @param sys The assetsys file system.
 * @param path The file path to load.
 *
 * @return The SDL_RWops for the given file path.
 */
SDL_ASSETSYS_API SDL_RWops* SDL_RWFromAssetsys(assetsys_t* sys, const char* path);

/**
 * Load an SDL_Surface from a .bmp file within the given assetsys_t.
 *
 * @param sys The assets_t* for the assetsys.
 * @param path The path of the .bmp file to load.
 *
 * @return An SDL_Surface* representing the bitmap file.
 * @see SDL_LoadBMP_RW()
 */
#define SDL_LoadBMP_Assetsys(sys, path) (SDL_LoadBMP_RW(SDL_RWFromAssetsys((sys), (path)), 1))

/**
 * Load an SDL_Surface from an image using SDL_image.
 *
 * @param sys The assets_t* for the assetsys.
 * @param path The path of the image file to load.
 *
 * @return An SDL_Surface* representing the image.
 * @see IMG_Load_RW()
 */
#define IMG_Load_Assetsys(sys, path) (IMG_Load_RW(SDL_RWFromAssetsys((sys), (path)), 1))

/**
 * @see SDL_LoadWAV_RW()
 */
#define SDL_LoadWAV_Assetsys(sys, path, spec, audio_buf, audio_len) (SDL_LoadWAV_RW(SDL_RWFromAssetsys((sys), (path)), 1, (spec), (audio_buf), (audio_len)))
#define Mix_LoadMUS_Assetsys(sys, path) (Mix_LoadMUS_RW(SDL_RWFromAssetsys((sys), (path)), 1))
#define Mix_LoadWAV_Assetsys(sys, path) (Mix_LoadWAV_RW(SDL_RWFromAssetsys((sys), (path)), 1))

/**
 * Load all the file data from the given path in the assetsys.
 *
 * @param sys The assetsys_t* to load from.
 * @param path The path of the file to load.
 * @param datasize Where to save the size of the file, needs to be a size_t*.
 *
 * @return A void* of the file data. Make sure to SDL_free() it afterwards.
 * @see SDL_LoadFile_RW()
 */
#define SDL_LoadFile_Assetsys(sys, path, datasize) (SDL_LoadFile_RW(SDL_RWFromAssetsys((sys), (path)), (datasize), 1))

/**
 * SDL_stbimage support for loading images with stb_image.h with assetsys.
 *
 * https://github.com/DanielGibson/Snippets/blob/master/SDL_stbimage.h
 */
#define STBIMG_Load_Assetsys(sys, path) (STBIMG_Load_RW(SDL_RWFromAssetsys((sys), (path)), 1))

/**
 * Function used to allocate memory within assetsys which routes to SDL's malloc function.
 *
 * @param ctx The memory context for assetsys.
 * @param size The size of the memory to allocate.
 *
 * @return A pointer to the allocated memory.
 *
 * @internal
 */
SDL_ASSETSYS_API void* SDL_assetsys_malloc(void* ctx, ASSETSYS_U64 size);

#ifdef __cplusplus
}
#endif

#endif  // SDL_ASSETSYS_H_

#ifdef SDL_ASSETSYS_IMPLEMENTATION
#ifndef SDL_ASSETSYS_IMPLEMENTATION_ONCE
#define SDL_ASSETSYS_IMPLEMENTATION_ONCE

// Dependency configuration
#define ASSETSYS_MALLOC(ctx, size) (SDL_assetsys_malloc(ctx, size))
#define ASSETSYS_FREE(ctx, ptr) (SDL_free(ptr))
#define MINIZ_NO_TIME
#define MINIZ_NO_ARCHIVE_WRITING_APIS

// strpool
#define STRPOOL_MALLOC(ctx, size) (ASSETSYS_MALLOC(ctx, size))
#define STRPOOL_FREE(ctx, ptr) (ASSETSYS_FREE(ctx, ptr))
#define STRPOOL_IMPLEMENTATION
#ifndef SDL_ASSETSYS_STRPOOL_H
/**
 * The location where strpool.h exists.
 */
#define SDL_ASSETSYS_STRPOOL_H "libs/strpool.h"
#endif  // SDL_ASSETSYS_STRPOOL_H
#include SDL_ASSETSYS_STRPOOL_H

// assetsys
#define ASSETSYS_IMPLEMENTATION
#include SDL_ASSETSYS_ASSETSYS_H

#ifdef __cplusplus
extern "C" {
#endif

void* SDL_assetsys_malloc(void* ctx, ASSETSYS_U64 size) {
    (void)ctx;
    SDL_malloc_func malloc_func;
    SDL_GetMemoryFunctions(&malloc_func, NULL, NULL, NULL);
    return malloc_func((size_t)size);
}

SDL_ASSETSYS_API SDL_RWops* SDL_RWFromAssetsys(assetsys_t* sys, const char* path) {
    if (sys == NULL || path == NULL) {
        return NULL;
    }

    // Load the file information from assetsys.
    assetsys_file_t file;
    if (assetsys_file(sys, path, &file) != 0) {
        return NULL;
    }

    // Find out the size of the file.
    int size = assetsys_file_size(sys, file);
    if (size <= 0) {
        return NULL;
    }

    // Create the memory buffer.
    unsigned char* out = SDL_assetsys_malloc(NULL, (ASSETSYS_U64)size);
    if (out == NULL) {
        return NULL;
    }

    // Load the file into the buffer.
    int outSize = 0;
    if (assetsys_file_load(sys, file, &outSize, (void*)out, size) != 0) {
        SDL_free(out);
        return NULL;
    }

    return SDL_RWFromMem(out, outSize);
}

#ifdef __cplusplus
}
#endif

#endif  // SDL_ASSETSYS_IMPLEMENTATION_ONCE
#endif  // SDL_ASSETSYS_IMPLEMENTATION
