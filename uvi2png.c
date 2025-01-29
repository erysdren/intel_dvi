
#include <SDL3/SDL.h>
#include "utils.h"

#define UVI_WIDTH 256
#define UVI_HEIGHT 256

static void make_output_filename(const char *input, char *output, size_t output_size)
{
	size_t inputLen = SDL_strlen(input);

	if (string_endswith(input, ".uvi"))
	{
		SDL_snprintf(output, output_size, "%s", input);
		SDL_snprintf(output + inputLen - 4, output_size - inputLen - 4, ".bmp", input);
	}
	else
	{
		SDL_snprintf(output, output_size, "%s_converted.bmp", input);
	}
}

int main(int argc, char **argv)
{
	for (int arg = 1; arg < argc; arg++)
	{
		log_info("Processing \"%s\"", argv[arg]);

		/* open input file */
		size_t inputPixelsLen = 0;
		void *inputPixels = SDL_LoadFile(argv[arg], &inputPixelsLen);
		if (!inputPixels || inputPixelsLen == 0)
		{
			log_warning("Failed to open \"%s\" for reading", argv[arg]);
			goto cleanup;
		}

		/* create output surface */
		SDL_Surface *outputSurface = SDL_CreateSurfaceFrom(UVI_WIDTH, UVI_HEIGHT, SDL_PIXELFORMAT_YV12, inputPixels, UVI_WIDTH);
		if (!outputSurface)
		{
			log_warning("Failed to convert \"%s\" to SDL_Surface", argv[arg]);
			goto cleanup;
		}

		/* get output filename */
		char outputFilename[1024];
		make_output_filename(argv[arg], outputFilename, sizeof(outputFilename));

		/* save output file */
		if (!SDL_SaveBMP(outputSurface, outputFilename))
			log_warning("Failed to save \"%s\"", outputFilename);
		else
			log_info("Successfully saved \"%s\"", outputFilename);

		/* clean up */
cleanup:
		if (outputSurface) SDL_DestroySurface(outputSurface);
		if (inputPixels) SDL_free(inputPixels);
	}

	SDL_Quit();

	return 0;
}
