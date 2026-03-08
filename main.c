#include "bmp.h"

int main(int argc, char *argv[]) {
	BitmapFileHeader bmfh;
	BitmapInfoHeader bmif;
	RGB** image;

	AppOptions options = {0};
	strcpy(options.o_filename, DEFAULT_RESULT_NAME);

	if (argc <= 1) {
		print_usage(argv[0], &options);
		return 0;
	}

	strcpy(options.i_filename, argv[argc - 1]);
	parse_arguments(argc, argv, &options);

	if (options.help_flag == 0) {
		image = read_bmp(options.i_filename, &bmfh, &bmif);
		image = reverse(image, bmif.height, bmif.width);
	} else {
		return 0;
	}

	apply_operations(&image, &bmif, &bmfh, &options);

	image = reverse(image, bmif.height, bmif.width);
	write_bmp(options.o_filename, image, bmif.height, bmif.width, bmfh, bmif);

	free_RGB_matrix(image, bmif.height);
	return 0;
}