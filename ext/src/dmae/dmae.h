/*  Argh! WUT doesn't have this...
    https://wiiubrew.org/wiki/Dmae.rpl
    Tried to strip out obvious SDK stuff where possible. */

#ifndef __DMAE_H
#define __DMAE_H
#include <stdint.h>
#include <stdbool.h>

extern void InitDMAEFunctionPointers();

extern uint64_t (*DMAECopyMem)(void* dst, void* src, size_t size, int flags);
extern bool (*DMAEWaitDone)(uint64_t t);

#endif
