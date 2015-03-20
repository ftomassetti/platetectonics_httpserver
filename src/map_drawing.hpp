#ifndef MAP_DRAWING
#define MAP_DRAWING

#include <stdio.h>
#include <png.h>

#ifdef __cplusplus
extern "C" {
#endif

// This function actually writes out the PNG image file. The string 'title' is
// also written into the image file
int writeImageGray(const char* filename, int width, int height, float *heightmap, const char* title);

int writeImageColors(const char* filename, int width, int height, float *heightmap, const char* title);

#ifdef __cplusplus
}
#endif

#endif
