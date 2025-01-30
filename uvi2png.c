
#include <SDL3/SDL.h>
#include "utils.h"
#include "stb_image_write.h"
#include "yuv.h"

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

		/* do conversion */
		void *outputPixels = yuv9_to_rgb24(UVI_WIDTH, UVI_HEIGHT, inputIo, false);
		if (!outputPixels)
			goto cleanup;

		/* get output filename */
		char outputFilename[1024];
		make_output_filename(argv[arg], outputFilename, sizeof(outputFilename));

		/* save output file */
		if (stbi_write_png(outputFilename, UVI_WIDTH, UVI_HEIGHT, 3, outputPixels, UVI_WIDTH * 3) == 0)
			log_warning("Failed to save \"%s\"", outputFilename);
		else
			log_info("Successfully saved \"%s\"", outputFilename);

		/* clean up */
cleanup:
		if (inputIo) SDL_CloseIO(inputIo);
		if (outputPixels) SDL_free(outputPixels);
	}

	SDL_Quit();

	return 0;
}
