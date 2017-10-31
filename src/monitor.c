#define FILE_ID "MONITOR"

#include <coreinit/time.h>
#include "common.h"
#include "log.h"

STATIC_VAR OSTime lastDecodeStart = 0;
STATIC_VAR OSTime lastDecodeFinish = 0;
STATIC_VAR OSTime rollingDecodeTime = 0;
STATIC_VAR OSTime rollingFrameTime = 0;
STATIC_VAR int frameCounter = 0;

void MonitorDecodeStart() {
    lastDecodeStart = OSGetTime();
}

void MonitorDecodeFinished() {
    lastDecodeFinish = OSGetTime();
}

void MonitorFrameRendered() {
    OSTime frameFinish = OSGetTime();
    if (++frameCounter > 60) {
        frameCounter = 0;
        LOGF("Decoding takes %lld; rendering takes %lld\n", \
            rollingDecodeTime, rollingFrameTime);
    }
    rollingFrameTime = \
        (rollingFrameTime + (frameFinish - lastDecodeFinish)) / 2;
    rollingDecodeTime = \
        (rollingDecodeTime + (lastDecodeFinish - lastDecodeStart)) / 2;
}
