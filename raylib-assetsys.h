/**********************************************************************************************
*
*   raylib-assetsys: Use assetsys, a file system abstraction library, to read raylib assets from zip files.
*
*   Copyright 2023 Rob Loach (@RobLoach)
*
*   DEPENDENCIES:
*       raylib https://www.raylib.com/
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

#ifndef RAYLIB_ASSETSYS_H_
#define RAYLIB_ASSETSYS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RAYLIB_ASSETSYS_API
    #define RAYLIB_ASSETSYS_API
#endif

#ifndef RAYLIB_ASSETSYS_RAYLIB_H
#define RAYLIB_ASSETSYS_RAYLIB_H "raylib.h"
#endif
#include RAYLIB_ASSETSYS_RAYLIB_H

#ifndef RAYLIB_ASSETSYS_ASSETSYS_H
/**
 * The path to where assetsys.h exists.
 */
#define RAYLIB_ASSETSYS_ASSETSYS_H "libs/assetsys.h"
#endif
#include RAYLIB_ASSETSYS_ASSETSYS_H

bool InitAssetsys(char const* path, char const* mount_as);
bool MountAssetsys(const char* path, const char* mount_as);
bool IsAssetsysReady();
void CloseAssetsys();
unsigned char *LoadFileDataFromAssetsys(const char *fileName, int *dataSize);
char *LoadFileTextFromAssetsys(const char *fileName);
Image LoadImageFromAssetsys(const char* fileName);

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_ASSETSYS_H_

#ifdef RAYLIB_ASSETSYS_IMPLEMENTATION
#ifndef RAYLIB_ASSETSYS_IMPLEMENTATION_ONCE
#define RAYLIB_ASSETSYS_IMPLEMENTATION_ONCE

// Dependency configuration
#define ASSETSYS_MALLOC(ctx, size) (MemAlloc((unsigned int)size))
#define ASSETSYS_FREE(ctx, ptr) (MemFree((void*)ptr))
#define MINIZ_NO_TIME
#define MINIZ_NO_ARCHIVE_WRITING_APIS

// strpool
#define STRPOOL_MALLOC(ctx, size) (ASSETSYS_MALLOC(ctx, size))
#define STRPOOL_FREE(ctx, ptr) (ASSETSYS_FREE(ctx, ptr))
#define STRPOOL_IMPLEMENTATION
#ifndef RAYLIB_ASSETSYS_STRPOOL_H
/**
 * The location where strpool.h exists.
 */
#define RAYLIB_ASSETSYS_STRPOOL_H "libs/strpool.h"
#endif  // RAYLIB_ASSETSYS_STRPOOL_H
#include RAYLIB_ASSETSYS_STRPOOL_H

// assetsys
#define ASSETSYS_IMPLEMENTATION
#include RAYLIB_ASSETSYS_ASSETSYS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Pointer to raylib's active assetsys.
 */
static assetsys_t* raylib_assetsys = NULL;

bool InitAssetsys(char const* path, char const* mount_as) {
    if (raylib_assetsys != NULL) {
        CloseAssetsys();
    }

    raylib_assetsys = assetsys_create(NULL);
    if (raylib_assetsys == NULL) {
        TraceLog(LOG_ERROR, "ASSETSYS: Failed to create assetsys");
        return false;
    }

    if (TextLength(path) > 0 && TextLength(mount_as) > 0) {
        if (!MountAssetsys(path, mount_as)) {
            CloseAssetsys();
            return false;
        }
    }

    return true;
}

bool IsAssetsysReady() {
    return raylib_assetsys != NULL;
}

bool MountAssetsys(const char* path, const char* mount_as) {
    if (TextLength(path) <= 0 && TextLength(mount_as) <= 0) {
        TraceLog(LOG_ERROR, "ASSETSYS: MountAssetsys() requires value path and mount_as parameters");
        return false;
    }

    // If the file is found by raylib, use raylib's file loading functions.
    if (FileExists(path) && !DirectoryExists(path)) {
        int dataSize;
        unsigned char* data = LoadFileData(path, &dataSize);
        if (assetsys_mount_from_memory(raylib_assetsys, (const void*)data, (size_t)dataSize, mount_as) == ASSETSYS_SUCCESS) {
            TraceLog(LOG_DEBUG, "ASSETSYS: Mounted file %s as %s", path, mount_as);
            return true;
        }
    }
    else {
        if (assetsys_mount(raylib_assetsys, path, mount_as) == ASSETSYS_SUCCESS) {
            TraceLog(LOG_DEBUG, "ASSETSYS: Mounted %s as %s", path, mount_as);
            return true;
        }
    }

    TraceLog(LOG_ERROR, "ASSETSYS: Failed to mount %s as %s", path, mount_as);
    return false;
}

void CloseAssetsys() {
    if (raylib_assetsys == NULL) {
        return;
    }

    assetsys_destroy(raylib_assetsys);
    raylib_assetsys = NULL;
}

unsigned char *LoadFileDataFromAssetsys(const char *fileName, int *dataSize) {
    if (raylib_assetsys == NULL || TextLength(fileName) <= 0) {
        return NULL;
    }

    // Load the file information from assetsys.
    assetsys_file_t file;
    if (assetsys_file(raylib_assetsys, fileName, &file) != 0) {
        return NULL;
    }

    // Find out the size of the file.
    int size = assetsys_file_size(raylib_assetsys, file);
    if (size <= 0) {
        return NULL;
    }

    // Create the memory buffer.
    unsigned char* out = MemAlloc((unsigned int)(size + 1));
    if (out == NULL) {
        return NULL;
    }

    // Load the file into the buffer.
    int outSize = 0;
    if (assetsys_file_load(raylib_assetsys, file, &outSize, (void*)out, size) != 0) {
        MemFree(out);
        return NULL;
    }

    // Output the file size.
    if (dataSize != NULL) {
        *dataSize = (int)size;
    }

    return out;
}

char *LoadFileTextFromAssetsys(const char *fileName) {
    int dataSize;
    unsigned char* data = LoadFileDataFromAssetsys(fileName, &dataSize);
    if (data == NULL) {
        return NULL;
    }

    data[dataSize] = '\0';
    return data;
}

Image LoadImageFromAssetsys(const char* fileName) {
    int dataSize;
    unsigned char* data = LoadFileDataFromAssetsys(fileName, &dataSize);
    if (data == NULL) {
        Image out = { 0 };
        return out;
    }

    const char* extension = GetFileExtension(fileName);
    Image out = LoadImageFromMemory(extension, data, dataSize);
    UnloadFileData(data);
    return out;
}

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_ASSETSYS_IMPLEMENTATION_ONCE
#endif  // RAYLIB_ASSETSYS_IMPLEMENTATION
