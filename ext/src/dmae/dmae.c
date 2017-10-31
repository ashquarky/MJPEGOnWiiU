#define FILE_ID "DMAE"

#include <stdint.h>
#include <coreinit/dynload.h>
#include "../../../src/log.h"

uint64_t (*DMAECopyMem)(void* dst, void* src, size_t size, int flags);
bool (*DMAEWaitDone)(uint64_t t);

void InitDMAEFunctionPointers() {
    OSDynLoadModule dmae;
    OSDynLoad_Acquire("dmae.rpl", &dmae);
    LOGF("handle: %08X\n", dmae);
    OSDynLoad_FindExport(dmae, 0, "DMAECopyMem", &DMAECopyMem);
    OSDynLoad_FindExport(dmae, 0, "DMAEWaitDone", &DMAEWaitDone);
    LOGF("Functions: %08X %08X\n", DMAECopyMem, DMAEWaitDone);
}
