
#include <SDL3/SDL.h>
#include "yuv.h"
#include "utils.h"

void *yuv9_to_rgb24(int inputWidth, int inputHeight, SDL_IOStream *inputIo)
{
	Uint8 *outputPixels = NULL;
	Uint8 *inputPlaneY = NULL;
	Uint8 *inputPlaneU = NULL;
	Uint8 *inputPlaneV = NULL;

	/* probably safe to assume these? */
	if (inputWidth < 4 || inputHeight < 4 || inputWidth % 4 != 0 || inputHeight % 4 != 0)
	{
		log_warning("yuv9_to_rgb24(): Invalid input size %dx%d", inputWidth, inputHeight);
		goto failed;
	}

	/* allocate input planes */
	size_t inputPlaneYLen = inputWidth * inputHeight;
	size_t inputPlaneULen = (inputWidth >> 2) * (inputHeight >> 2);
	size_t inputPlaneVLen = (inputWidth >> 2) * (inputHeight >> 2);

	inputPlaneY = SDL_malloc(inputPlaneYLen);
	inputPlaneU = SDL_malloc(inputPlaneULen);
	inputPlaneV = SDL_malloc(inputPlaneVLen);

	/* read input planes */
	if (SDL_ReadIO(inputIo, inputPlaneY, inputPlaneYLen) != inputPlaneYLen)
	{
		log_warning("yuv9_to_rgb24(): Failed to read Y plane");
		goto failed;
	}
	if (SDL_ReadIO(inputIo, inputPlaneU, inputPlaneULen) != inputPlaneULen)
	{
		log_warning("yuv9_to_rgb24(): Failed to read U plane");
		goto failed;
	}
	if (SDL_ReadIO(inputIo, inputPlaneV, inputPlaneVLen) != inputPlaneVLen)
	{
		log_warning("yuv9_to_rgb24(): Failed to read V plane");
		goto failed;
	}

	/* allocate output pixels */
	outputPixels = SDL_malloc(inputWidth * inputHeight * 3);

	/* convert output pixels */
	for (int y = 0; y < inputHeight; y++)
	{
		for (int x = 0; x < inputWidth; x++)
		{
			/* get float yuv */
			float fy = inputPlaneY[y * inputWidth + x];
			float fu = inputPlaneU[(y >> 2) * (inputWidth >> 2) + (x >> 2)];
			float fv = inputPlaneV[(y >> 2) * (inputWidth >> 2) + (x >> 2)];

			/* get float rgb */
			float fr = ((298.082 * fy) / 256.0f) + ((408.583 * fu) / 256.0f) - 222.921;
			float fg = ((298.082 * fy) / 256.0f) - ((100.291 * fv) / 256.0f) - ((208.120 * fu) / 256.0f) + 135.576;
			float fb = ((298.082 * fy) / 256.0f) + ((516.412 * fv) / 256.0f) - 276.836;

			/* write rgb24 */
			outputPixels[y * (inputWidth * 3) + (x * 3) + 0] = (Uint8)SDL_clamp(fr, 0, 255);
			outputPixels[y * (inputWidth * 3) + (x * 3) + 1] = (Uint8)SDL_clamp(fg, 0, 255);
			outputPixels[y * (inputWidth * 3) + (x * 3) + 2] = (Uint8)SDL_clamp(fb, 0, 255);
		}
	}

	goto success;

failed:
	if (outputPixels) SDL_free(outputPixels);
	outputPixels = NULL;
success:
	if (inputPlaneY) SDL_free(inputPlaneY);
	if (inputPlaneU) SDL_free(inputPlaneU);
	if (inputPlaneV) SDL_free(inputPlaneV);
	return outputPixels;
}
