#ifndef __LOG_H
#define __LOG_H

#include <whb/log.h>

#define LOG(msg) WHBLogWrite(FILE_ID ": " msg)
#define LOGF(msg, ...) WHBLogWritef(FILE_ID ": " msg, __VA_ARGS__)

#endif
