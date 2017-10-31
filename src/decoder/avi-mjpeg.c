#define FILE_ID "AVIMJPEG"

#include <stdlib.h>
#include <turbojpeg.h>
#include "common.h"
#include "log.h"
#include "avi.h"
#include "monitor.h"

#define CHECK_ERR(x) if (x) { \
    LOGF("Error: %d on line %d!\n", x, __LINE__); \
    goto cleanup; \
} \

void* file;
size_t fileSize;

RIFFListChunk* movi;
RIFFChunk* currentVideoChunk;

void* framebuffer;
size_t framebufferSz = 1280 * 720 * 4 + 0x1000;
tjhandle jpegDecompress;

void InitAVIMJPEGDecoder() {
    jpegDecompress = tjInitDecompress();
    framebuffer = malloc(framebufferSz);
}

void CleanupAVIMJPEGDecoder() {
    free(framebuffer);
    tjDestroy(jpegDecompress);
}

unsigned int* AVIMJPEGDecodeNextFrame() {
    if (!jpegDecompress || !framebuffer || !currentVideoChunk) {
        return 0;
    }
    if (currentVideoChunk >= ((void*)movi + BE_FROM_LE(movi->le_size) + 8)) {
        LOG("Video finished!");
        return 0;
    }

    int ret = tjDecompress2(jpegDecompress, &currentVideoChunk->data, \
        BE_FROM_LE(currentVideoChunk->le_size), framebuffer, 1280, 0, 720, \
        TJPF_RGBX, TJFLAG_FASTDCT | TJFLAG_FASTUPSAMPLE);

/*  If odd... */
    if (BE_FROM_LE(currentVideoChunk->le_size) & 1) {
        currentVideoChunk = (void*)currentVideoChunk + \
            BE_FROM_LE(currentVideoChunk->le_size) + 9;
    } else {
        currentVideoChunk = (void*)currentVideoChunk + \
            BE_FROM_LE(currentVideoChunk->le_size) + 8;
    }

    CHECK_ERR(ret);

    return framebuffer;
cleanup:
    LOGF("JPEG error: %s\n", tjGetErrorStr());
    return 0;
}

void AVIMJPEGLoadFile(void* fileIn, size_t fileSizeIn) {
    file = fileIn;
    fileSize = fileSizeIn;
    //frameCounter = 0;

    if (*(unsigned int*)file != FOURCC('R', 'I', 'F', 'F')) {
        LOG("File is not RIFF-formatted!\n");
        return;
    }

    RIFFListChunk* riffStart = (RIFFListChunk*)file;
    if (riffStart->name != FOURCC('A', 'V', 'I', ' ')) {
        LOG("File is not an AVI!\n");
        return;
    }

    LOG("Traversing AVI...\n");
    bool foundMovi = false;
    RIFFListChunk* list = (RIFFListChunk*)&riffStart->firstChunk;
    while (list < (file + fileSize)) {
        if (list->fourcc == FOURCC('L', 'I', 'S', 'T')) {
            if (list->name == FOURCC('m', 'o', 'v', 'i')) {
                foundMovi = true;
                break;
            }
        }
        list = (void*)list + BE_FROM_LE(list->le_size) + 8;
    }
    if (!foundMovi) {
        LOG("MOVI not found!\n");
        return;
    }
    movi = list;
    currentVideoChunk = &movi->firstChunk;

    bool foundVideoChunk = false;
    while (currentVideoChunk < ((void*)movi + BE_FROM_LE(movi->le_size) + 8)) {
        if (currentVideoChunk->fourcc == FOURCC('0', '0', 'd', 'c')) {
            foundVideoChunk = true;
            break;
        } else if (currentVideoChunk->fourcc == FOURCC('0', '0', 'd', 'b')) {
            foundVideoChunk = true;
            break;
        }
        currentVideoChunk = (void*)currentVideoChunk + \
            BE_FROM_LE(currentVideoChunk->le_size) + 8;
    }
    if (!foundVideoChunk) {
        LOG("AVI has no video!\n");
        return;
    }


    LOGF("First image at %08X\n", (unsigned int)&currentVideoChunk->data - (unsigned int)file);
}
