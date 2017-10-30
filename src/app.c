#define FILE_ID "APP"

#include <stdlib.h>
#include <turbojpeg.h>
#include "common.h"
#include "log.h"
#include "screen.h"
#include "../pir.h"

#define CHECK_ERR(x) if (x) { \
    LOGF("Error: %d on line %d!\n", x, __LINE__); \
    goto cleanup; \
} \

void* framebuffer;
size_t framebufferSz = 1280 * 720 * 4 + 0x1000;
tjhandle jpegDecompress;

void RunApp() {
    LOG("Hello!\n");

    if (!jpegDecompress) {
        jpegDecompress = tjInitDecompress();
    }
    if (!framebuffer) {
        framebuffer = malloc(framebufferSz);
    }

    int ret = tjDecompress2(jpegDecompress, pir_jpg, pir_jpg_len, framebuffer, \
        1280, 0, 720, TJPF_RGBX, TJFLAG_FASTDCT);
    CHECK_ERR(ret);

    for (int y = 0; y < 720; y++) {
        for (int x = 0; x < 1280; x++) {
            ScreenPutPixel(SCREEN_TV, (ScreenCoord){x, y}, \
                ((unsigned int*)framebuffer)[x + y * 1280]);
        }
    }
    ScreenFlip();

cleanup:

    WaitForVysnc();
}

void AppCleanup() {
    free(framebuffer);
    tjDestroy(jpegDecompress);
}
