#ifndef __SCREEN_H
#define __SCREEN_H
#include <coreinit/screen.h>

typedef unsigned int ScreenColour;

typedef struct _ScreenCoord {
    unsigned int x;
    unsigned int y;
} ScreenCoord;

typedef struct _ScreenBuffer {
    OSScreenID screen;
    int num;
    ScreenColour* image;
    size_t size;
    unsigned int width;
    unsigned int height;
} ScreenBuffer;

void ScreenInit();
void ScreenShutdown();
void ScreenPutPixel(OSScreenID id, ScreenCoord coords, ScreenColour colour);

void WaitForVysnc();

#define C_RGB8(x) ((x << 8) | 0xFF)

#endif
