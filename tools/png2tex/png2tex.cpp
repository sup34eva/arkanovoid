#include "png2tex.h"

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++) {
		char out_name[100];
		unsigned error;
		unsigned char* image;
		unsigned width, height;
		uint8_t* pixel_data;

		printf("Processing %s ...\n", argv[i]);

		error = lodepng_decode32_file(&image, &width, &height, argv[i]);

		if (error) {
			fprintf(stderr, "Error %u: %s\n", error, lodepng_error_text(error));
		}

		struct {
			int  	 width;
			int  	 height;
			int  	 channels;
		} meta = { width, height, 4, };

		char *p1, *p2;
		p1 = strrchr(argv[i], '\\');
		p2 = strrchr(argv[i], '.');

		int start = p1 - argv[i] + 1,
			end = (p2 - argv[i] + 1) - start - 1;

		sprintf(out_name, "%.*s.tex", end, argv[i] + start);

		FILE* dest = fopen(out_name, "wb");
		fwrite(&meta, sizeof(meta), 1, dest);

		int len = width * height * 4 + 1;
		pixel_data = new uint8_t[len];

		for (unsigned int j = 0; j < len; j++)
			pixel_data[j] = image[j];

		fwrite(pixel_data, 1, len, dest);

		free(image);
		free(pixel_data);
		fclose(dest);

		printf("Image %s done\n", argv[i]);
	}
	return 0;
}

