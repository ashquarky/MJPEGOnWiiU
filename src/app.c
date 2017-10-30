#define FILE_ID "APP"

#include "common.h"
#include "log.h"
#include "screen.h"

void RunApp() {
    LOG("Hello!\n");

    for (int y = 0; y < 200; y++) {
        for (int x = 0; x < 200; x++) {
            ScreenPutPixel(SCREEN_TV, (ScreenCoord){x, y}, C_RGB8(0xFF0000));
        }
    }
    ScreenFlip();

    WaitForVysnc();
}
