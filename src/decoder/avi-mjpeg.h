#ifndef __AVI_MJPEG_H
#define __AVI_MJPEG_H

void InitAVIMJPEGDecoder();
void CleanupAVIMJPEGDecoder();
unsigned int* AVIMJPEGDecodeNextFrame();
void AVIMJPEGLoadFile(void* fileIn, size_t fileSizeIn);

#endif //__AVI_MJPEG_H
