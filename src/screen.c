#define FILE_ID "SCREEN"

#include <stdbool.h>
#include <malloc.h>
#include <coreinit/core.h>
#include <coreinit/screen.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <coreinit/cache.h>
#include <coreinit/systeminfo.h>
#include <gx2/state.h>
#include <gx2/event.h>
#include <dmae/dmae.h>
#include "common.h"
#include "log.h"
#include "screen.h"

/*  Uncomment for Decaf */
//#define DECAF

STATIC_VAR ScreenBuffer buffers[2][2] = {
    {
        { .screen = SCREEN_TV, .num = 0, .width = 1280, .height = 720 },
        { .screen = SCREEN_TV, .num = 1, .width = 1280, .height = 720 },
    },
    {
        { .screen = SCREEN_DRC, .num = 0, .width = 854, .height = 480 },
        { .screen = SCREEN_DRC, .num = 1, .width = 854, .height = 480 },
    },
};
STATIC_VAR int currentBuffer;

void ScreenPutPixel(OSScreenID id, ScreenCoord coords, ScreenColour colour) {
    ScreenBuffer workBuf = buffers[id][currentBuffer];
    size_t offset = coords.x + coords.y * workBuf.width;
    workBuf.image[offset] = colour;
}

void ScreenPushFramebuffer(OSScreenID id, unsigned int* fb, size_t fbSize)  {
    DCFlushRange(fb,fbSize);
    ScreenBuffer workBuf = buffers[id][currentBuffer];
    bool ret = DMAEWaitDone(DMAECopyMem(workBuf.image, fb, fbSize, 0));
    if (!ret) {
        LOG("DMAE Timeout!\n");
    }
    DCInvalidateRange(workBuf.image, workBuf.size);
}

void ScreenFlip() {
    for (int id = 0; id < 2; id++) {
        ScreenBuffer workBuf = buffers[id][currentBuffer];
        DCFlushRange(workBuf.image, workBuf.size);
        OSScreenFlipBuffersEx(id);
    }
    currentBuffer = !currentBuffer;
}

void ScreenInit() {
#ifndef DECAF
    GX2Init(NULL);
#else
    #warning Decaf build
#endif

    OSScreenInit();
    size_t tvBufSz = OSScreenGetBufferSizeEx(SCREEN_TV);
    size_t drcBufSz = OSScreenGetBufferSizeEx(SCREEN_DRC);

    void* tvBuf = memalign(0x100, tvBufSz);
    buffers[SCREEN_TV][0].image = tvBuf;
    buffers[SCREEN_TV][1].image = tvBuf + (tvBufSz / 2);
    buffers[SCREEN_TV][0].size = (tvBufSz / 2);
    buffers[SCREEN_TV][1].size = (tvBufSz / 2);
    LOGF("TV buffers: %08X@0x%08X; %08X@0x%08X\n", \
    tvBufSz / 2, tvBuf, tvBufSz / 2, tvBuf + (tvBufSz / 2));

    void* drcBuf = memalign(0x100, drcBufSz);
    buffers[SCREEN_DRC][0].image = drcBuf;
    buffers[SCREEN_DRC][1].image = drcBuf + (drcBufSz / 2);
    buffers[SCREEN_DRC][0].size = (drcBufSz / 2);
    buffers[SCREEN_DRC][1].size = (drcBufSz / 2);
    LOGF("DRC buffers: %08X@0x%08X; %08X@0x%08X\n", \
    drcBufSz / 2, drcBuf, drcBufSz / 2, drcBuf + (drcBufSz / 2));

    OSScreenSetBufferEx(SCREEN_TV, tvBuf);
    OSScreenSetBufferEx(SCREEN_DRC, drcBuf);

    OSScreenEnableEx(SCREEN_TV, true);
    OSScreenEnableEx(SCREEN_DRC, true);

    currentBuffer = 1;
}

void ScreenShutdown() {
    /*OSScreenEnableEx(SCREEN_TV, false);
    OSScreenEnableEx(SCREEN_DRC, false);*/

    free(buffers[SCREEN_TV][0].image);
    free(buffers[SCREEN_DRC][0].image);

#ifndef DECAF
    GX2Shutdown();
#endif
}

/*  Waits for vsync; which happens every 1/60s.
    NOT THREAD-SAFE. */
void WaitForVysnc() {
    if (OSIsMainCore()) {
        GX2WaitForVsync();
    } else {
        LOGF("ERROR: Vsync on core %d; only allowed on %d! Waiting 16ms.\n", \
        OSGetCoreId(), OSIsMainCore());
        OSSleepTicks(OSMilliseconds(16));
    }
}
