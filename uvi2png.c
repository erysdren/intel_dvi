
#include <SDL3/SDL.h>
#include "utils.h"
#include "stb_image_write.h"

#define UVI_WIDTH 256
#define UVI_HEIGHT 240

static void make_output_filename(const char *input, char *output, size_t output_size)
{
	size_t inputLen = SDL_strlen(input);

	if (string_endswith(input, ".uvi"))
	{
		SDL_snprintf(output, output_size, "%s", input);
		SDL_snprintf(output + inputLen - 4, output_size - inputLen - 4, ".png", input);
	}
	else
	{
		SDL_snprintf(output, output_size, "%s_converted.png", input);
	}
}

int main(int argc, char **argv)
{
	for (int arg = 1; arg < argc; arg++)
	{
		log_info("Processing \"%s\"", argv[arg]);

		/* open input file */
		SDL_IOStream *inputIo = SDL_IOFromFile(argv[arg], "rb");
		if (!inputIo)
		{
			log_warning("Failed to open \"%s\" for reading", argv[arg]);
			goto cleanup;
		}

		/* open temporary buffer for writing */
		SDL_IOStream *outputIo = SDL_IOFromDynamicMem();
		if (!outputIo)
		{
			log_warning("Failed to create output buffer");
			goto cleanup;
		}

		/* allocate input planes */
		size_t inputPlaneYLen = UVI_WIDTH * UVI_HEIGHT;
		size_t inputPlaneULen = (UVI_WIDTH >> 2) * (UVI_HEIGHT >> 2);
		size_t inputPlaneVLen = (UVI_WIDTH >> 2) * (UVI_HEIGHT >> 2);

		Uint8 *inputPlaneY = SDL_malloc(inputPlaneYLen);
		Uint8 *inputPlaneV = SDL_malloc(inputPlaneVLen);
		Uint8 *inputPlaneU = SDL_malloc(inputPlaneULen);

		/* read planes */
		if (SDL_ReadIO(inputIo, inputPlaneY, inputPlaneYLen) != inputPlaneYLen)
		{
			log_warning("Failed to read Y plane");
			goto cleanup;
		}
		if (SDL_ReadIO(inputIo, inputPlaneU, inputPlaneULen) != inputPlaneULen)
		{
			log_warning("Failed to read U plane");
			goto cleanup;
		}
		if (SDL_ReadIO(inputIo, inputPlaneV, inputPlaneVLen) != inputPlaneVLen)
		{
			log_warning("Failed to read V plane");
			goto cleanup;
		}

		/* convert output pixels */
		for (int y = 0; y < UVI_HEIGHT; y++)
		{
			for (int x = 0; x < UVI_WIDTH; x++)
			{
				/* get float yuv */
				float fy = inputPlaneY[y * UVI_WIDTH + x];
				float fu = inputPlaneU[(y >> 2) * (UVI_WIDTH >> 2) + (x >> 2)];
				float fv = inputPlaneV[(y >> 2) * (UVI_WIDTH >> 2) + (x >> 2)];

				/* get float rgb */
				float fr = ((298.082 * fy) / 256.0f) + ((408.583 * fu) / 256.0f) - 222.921;
				float fg = ((298.082 * fy) / 256.0f) - ((100.291 * fv) / 256.0f) - ((208.120 * fu) / 256.0f) + 135.576;
				float fb = ((298.082 * fy) / 256.0f) + ((516.412 * fv) / 256.0f) - 276.836;

#ifdef DEBUG
				log_info("%dx%d YUV: %0.4f %0.4f %0.4f", x, y, fy, fu, fv);
				log_info("%dx%d RGB: %0.4f %0.4f %0.4f", x, y, fr, fg, fb);
#endif

				/* write rgb24 */
				SDL_WriteU8(outputIo, (Uint8)SDL_clamp(fr, 0, 255));
				SDL_WriteU8(outputIo, (Uint8)SDL_clamp(fg, 0, 255));
				SDL_WriteU8(outputIo, (Uint8)SDL_clamp(fb, 0, 255));
			}
		}

		/* get output filename */
		char outputFilename[1024];
		make_output_filename(argv[arg], outputFilename, sizeof(outputFilename));

		/* get pointer to the buffer we wrote */
		Sint64 outputSize = SDL_GetIOSize(outputIo);
		SDL_PropertiesID outputProps = SDL_GetIOProperties(outputIo);
		void *outputData = SDL_GetPointerProperty(outputProps, SDL_PROP_IOSTREAM_DYNAMIC_MEMORY_POINTER, NULL);

		/* test output buffer size */
		if (outputSize != UVI_WIDTH * UVI_HEIGHT * 3)
		{
			log_warning("Output buffer has incorrect size");
			goto cleanup;
		}

		/* save output file */
		if (stbi_write_png(outputFilename, UVI_WIDTH, UVI_HEIGHT, 3, outputData, UVI_WIDTH * 3) == 0)
			log_warning("Failed to save \"%s\"", outputFilename);
		else
			log_info("Successfully saved \"%s\"", outputFilename);

		/* clean up */
cleanup:
		if (inputPlaneY) SDL_free(inputPlaneY);
		if (inputPlaneU) SDL_free(inputPlaneU);
		if (inputPlaneV) SDL_free(inputPlaneV);
		if (inputIo) SDL_CloseIO(inputIo);
		if (outputIo) SDL_CloseIO(outputIo);
	}

	SDL_Quit();

	return 0;
}
