
#ifndef _YUV_H_
#define _YUV_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>

/**
 * \brief convert an Intel YUV9 image from a stream into an RGB24 buffer
 * \param inputWidth width of source image
 * \param inputHeight height of source image
 * \param inputIo input stream
 * \returns allocated buffer of inputWidth * inputHeight RGB24 pixels, free with SDL_free()
 */
void *yuv9_to_rgb24(int inputWidth, int inputHeight, SDL_IOStream *inputIo);

#ifdef __cplusplus
}
#endif
#endif /* _YUV_H_ */
