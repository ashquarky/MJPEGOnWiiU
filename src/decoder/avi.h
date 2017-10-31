#ifndef __AVI_H
#define __AVI_H

#define FOURCC(a, b, c, d) (unsigned int)((a << 24) | (b << 16) | (c << 8) | d)
#define BE_FROM_LE(x) __builtin_bswap32(x)

typedef struct _RIFFChunk {
    unsigned int fourcc;
    size_t le_size;
    unsigned char data;
} RIFFChunk;

typedef struct _RIFFListChunk {
    unsigned int fourcc;
    size_t le_size;
    unsigned int name;
    RIFFChunk firstChunk;
} RIFFListChunk;

#endif //__AVI_H
