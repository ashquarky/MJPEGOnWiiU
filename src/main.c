#define FILE_ID "MAIN"

#include <stdbool.h>
#include <coreinit/core.h>
#include <coreinit/foreground.h>
#include <proc_ui/procui.h>
#include <whb/log_cafe.h>
#include <whb/log_udp.h>
#include <dmae/dmae.h>
#include "common.h"
#include "log.h"
#include "app.h"
#include "screen.h"

STATIC void Init() {
    AppInit();
    ScreenInit();
}

STATIC void Shutdown() {
    AppCleanup();
    ScreenShutdown();
}

STATIC bool MainLoop() {
    ProcUIStatus status = ProcUIProcessMessages(true);

    if (status == PROCUI_STATUS_IN_FOREGROUND) {
        RunApp();
    } else if (status == PROCUI_STATUS_RELEASE_FOREGROUND) {
        LOG("ProcUI Release Foreground!\n");
        ProcUIDrawDoneRelease();
    } else if (status == PROCUI_STATUS_EXITING) {
    /*  This never seems to be called... aww. Maybe _Exit does it? */
        LOG("ProcUI Exit!\n");
        return false;
    }
    return true;
}

STATIC void SaveCallback() {
    OSSavesDone_ReadyToRelease();
}

int main(int argc, char** argv) {
    ProcUIInit(&SaveCallback);
    WHBLogCafeInit();
    WHBLogUdpInit();
    LOG("Hello World!\n");

    InitDMAEFunctionPointers();
    Init();

    while(MainLoop());

    Shutdown();

    WHBLogCafeDeinit();
    WHBLogUdpDeinit();
    ProcUIShutdown();
    _Exit(0);
    return 0;
}
