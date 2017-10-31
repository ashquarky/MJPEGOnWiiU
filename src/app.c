#define FILE_ID "APP"

#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "log.h"
#include "screen.h"
#include "monitor.h"
#include "decoder/avi-mjpeg.h"

typedef enum _AppState {
    INITIALISING,
    PLAYING,
} AppState;
AppState state = INITIALISING;

void* video;
size_t videoSz;

void RunApp() {
    //LOG("Hello!\n");

/*  This is in the main loop so we can make it async later */
    if (state == INITIALISING) {
        FILE* fd = fopen("/vol/external01/pir.avi", "rb");
        if (!fd) {
            LOG("Couldn't open /vol/external01/pir.avi!\n");
            goto cleanup;
        }

        fseek(fd, 0L, SEEK_END);
        videoSz = ftell(fd);
        fseek(fd, 0L, SEEK_SET);
        LOGF("Video is %08X bytes\n", videoSz);

        video = malloc(videoSz);
        if (!video) {
            LOG("Couldn't allocate memory for video (too big?)");
            goto cleanup;
        }

        fread(video, videoSz, 1, fd);
        LOGF("Read in video; %08X@0x%08X. Magic: %c%c%c%c (expect RIFF)\n", \
            videoSz, video, \
            ((unsigned char*)video)[0], ((unsigned char*)video)[1], \
            ((unsigned char*)video)[2], ((unsigned char*)video)[3]);

        AVIMJPEGLoadFile(video, videoSz);

        fclose(fd);
        state = PLAYING;
    } else if (state == PLAYING) {
        MonitorDecodeStart();

        unsigned int* frame = AVIMJPEGDecodeNextFrame();
        if (!frame) {
            //LOG("Decoder failed!\n");
            goto cleanup;
        }

        MonitorDecodeFinished();

        /*for (int y = 0; y < 720; y++) {
            for (int x = 0; x < 1280; x++) {
                ScreenPutPixel(SCREEN_TV, (ScreenCoord){x, y}, frame[x + y * 1280]);
            }
        }*/
        ScreenPushFramebuffer(SCREEN_TV, frame, 1280*720);
        ScreenFlip();

        MonitorFrameRendered();
    }

cleanup:
    WaitForVysnc();
}

void AppInit() {
    InitAVIMJPEGDecoder();
}

void AppCleanup() {
    CleanupAVIMJPEGDecoder();
    if (video) {
        free(video);
    }
}
