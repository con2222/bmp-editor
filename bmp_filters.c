#include "bmp.h"

RGB** resize_left(RGB** arr, BitmapInfoHeader* bmif, BitmapFileHeader* bmfh, int size, int* color) {
	int H = bmif->height;
	int W = bmif->width + size;
	RGB** new_arr = (RGB**)malloc(sizeof(RGB*) * H);
	for(int i = 0; i < H; i++) {
		new_arr[i] = (RGB*)malloc(sizeof(RGB) * W);
	}

	for(int i = 0; i < H; i++) {
		if (size > 0) {
			for(int j = 0; j < W; j++) {
				if (j < size) {
					new_arr[i][j].r = color[0];
					new_arr[i][j].g = color[1];
					new_arr[i][j].b = color[2];
				} else {
					new_arr[i][j] = arr[i][j - size];
				}
			}
		} else {
			for (int j = 0; j < W; j++) {
				new_arr[i][j] = arr[i][j - size];
			}
		}
	}

	free_RGB_matrix(arr, H);
	arr = new_arr;
	bmif->width = W;
	bmif->imageSize = bmif->width * bmif->height;
	bmfh->filesize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + bmif->imageSize;
	return arr;
}

RGB** resize_right(RGB** arr, BitmapInfoHeader* bmif, BitmapFileHeader* bmfh, int size, int* color) {
	int H = bmif->height;
	int W = bmif->width + size;
	RGB** new_arr = (RGB**)malloc(sizeof(RGB*) * H);
	for(int i = 0; i < H; i++) {
		new_arr[i] = (RGB*)malloc(sizeof(RGB) * W);
	}
	
	for(int i = 0; i < H; i++) {
		if (size > 0) {
			for(int j = 0; j < W; j++) {
				if (j >= bmif->width) {
					new_arr[i][j].r = color[0];
					new_arr[i][j].g = color[1];
					new_arr[i][j].b = color[2];
				} else {
					new_arr[i][j] = arr[i][j];
				}
			}
		} else {
			for (int j = 0; j < W; j++) {
				new_arr[i][j] = arr[i][j];
			}
		}
	}

	free_RGB_matrix(arr, H);
	arr = new_arr;
	bmif->width = W;
	bmif->imageSize = bmif->width * bmif->height;
	bmfh->filesize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + bmif->imageSize;
	return arr;
}

RGB** resize_above(RGB** arr, BitmapInfoHeader* bmif, BitmapFileHeader* bmfh, int size, int* color) {
	int H = bmif->height + size;
	int W = bmif->width;
	RGB** new_arr = (RGB**)malloc(sizeof(RGB*) * H);
	for(int i = 0; i < H; i++) {
		new_arr[i] = (RGB*)malloc(sizeof(RGB) * W);
	}

	for(int i = 0; i < H; i++) {
		if (size > 0) {
			for (int j = 0; j < W; j++) {
				if (i < size) {
					new_arr[i][j].r = color[0];
					new_arr[i][j].g = color[1];
					new_arr[i][j].b = color[2];
				} else {
					new_arr[i][j] = arr[i - size][j];
				}
			}
		} else {
			for (int j = 0; j < W; j++) {
				new_arr[i][j] = arr[i - size][j];
			}
		}
	}

	free_RGB_matrix(arr, bmif->height);
	arr = new_arr;
	bmif->height = H;
	bmif->imageSize = bmif->width * bmif->height;
	bmfh->filesize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + bmif->imageSize;
	return arr;
}

RGB** resize_below(RGB** arr, BitmapInfoHeader* bmif, BitmapFileHeader* bmfh, int size, int* color) {
	int H = bmif->height + size;
	int W = bmif->width;
	RGB** new_arr = (RGB**)malloc(sizeof(RGB*) * H);
	for(int i = 0; i < H; i++) {
		new_arr[i] = (RGB*)malloc(sizeof(RGB) * W);
	}

	
	for(int i = 0; i < H; i++) {
		if (size > 0) {
			for (int j = 0; j < W; j++) {
				if (i >= bmif->height) {
					new_arr[i][j].r = color[0];
					new_arr[i][j].g = color[1];
					new_arr[i][j].b = color[2];
				} else {
					new_arr[i][j] = arr[i][j];
				}
			}
		} else {
			for (int j = 0; j < W; j++) {
				new_arr[i][j] = arr[i][j];
			}
		}
	}

	free_RGB_matrix(arr, bmif->height);
	arr = new_arr;
	bmif->height = H;
	bmif->imageSize = bmif->width * bmif->height;
	bmfh->filesize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + bmif->imageSize;
	return arr;
}

RGB** resize(RGB** arr, BitmapInfoHeader* bmif, BitmapFileHeader* bmfh, int* size, int* color) {
	if (size[0] != 0) {
		arr = resize_left(arr, bmif, bmfh, size[0], color);
	}
	if (size[1] != 0) {
		arr = resize_right(arr, bmif, bmfh, size[1], color);
	}
	if (size[2] != 0) {
		arr = resize_above(arr, bmif, bmfh, size[2], color);
	}
	if (size[3] != 0) {
		arr = resize_below(arr, bmif, bmfh, size[3], color);
	}
	return arr;
}

void average(RGB* pixel) {
    int gray_value = (int)(0.299 * pixel->r + 0.587 * pixel->g + 0.114 * pixel->b + 0.5);

    if (gray_value < 0) {gray_value = 0;}
    if (gray_value > 255) {gray_value = 255;}

    pixel->r = gray_value;
    pixel->g = gray_value;
    pixel->b = gray_value;
}

void gray(RGB** arr, int H, int W, int x0, int y0, int x1, int y1) {
    int min_x = (x0 < 0) ? 0 : (x0 >= W ? W - 1 : x0);
    int max_x = (x1 < 0) ? 0 : (x1 >= W ? W - 1 : x1);
    int min_y = (y0 < 0) ? 0 : (y0 >= H ? H - 1 : y0);
    int max_y = (y1 < 0) ? 0 : (y1 >= H ? H - 1 : y1);

    if (min_x > max_x) { int tmp = min_x; min_x = max_x; max_x = tmp; }
    if (min_y > max_y) { int tmp = min_y; min_y = max_y; max_y = tmp; }

    for (int i = min_y; i < max_y; i++) {
        for (int j = min_x; j < max_x; j++) {
            average(&arr[i][j]);
        }
    }
}

void inverse_pixel(RGB* pixel) {
	pixel->r = 255 - pixel->r;
	pixel->b = 255 - pixel->b;
	pixel->g = 255 - pixel->g;
}

void inverse(RGB** arr, int H, int W, int x0, int y0, int x1, int y1) {
    int min_x = (x0 < 0) ? 0 : (x0 >= W ? W - 1 : x0);
    int max_x = (x1 < 0) ? 0 : (x1 >= W ? W - 1 : x1);
    int min_y = (y0 < 0) ? 0 : (y0 >= H ? H - 1 : y0);
    int max_y = (y1 < 0) ? 0 : (y1 >= H ? H - 1 : y1);

    if (min_x > max_x) { int tmp = min_x; min_x = max_x; max_x = tmp; }
    if (min_y > max_y) { int tmp = min_y; min_y = max_y; max_y = tmp; }

    for (int i = min_y; i < max_y; i++) {
        for (int j = min_x; j < max_x; j++) {
            inverse_pixel(&arr[i][j]);
        }
    }
}

void apply_operations(RGB*** image, BitmapInfoHeader* bmif, BitmapFileHeader* bmfh, AppOptions *opts) {
	if (opts->inverse_flag && opts->x0 < opts->x1 && opts->y0 < opts->y1) {
		inverse(*image, bmif->height, bmif->width, opts->x0, opts->y0, opts->x1, opts->y1);
	}

	if (opts->gray_flag && opts->x0 < opts->x1 && opts->y0 < opts->y1) {
		gray(*image, bmif->height, bmif->width, opts->x0, opts->y0, opts->x1, opts->y1);
	}

	if (opts->resize_flag) {
		if (!opts->color_flag) {
			opts->color[0] = opts->color[1] = opts->color[2] = 0;
		}
		*image = resize(*image, bmif, bmfh, opts->size, opts->color);
	}
	if (opts->info_flag) {
		print_info(bmif, bmfh);
	}
	if (opts->shift_flag) {
		*image = shift(*image, bmif, bmfh, opts);
	}
}

RGB** shift(RGB** image, BitmapInfoHeader* bmif, BitmapFileHeader* bmfh, AppOptions *opts) {
	int H = bmif->height;
	int W = bmif->width;

	int flag = 0;
	int step = opts->step;

	RGB** new_arr = (RGB**)malloc(sizeof(RGB*) * H);
	for (int i = 0; i < H; i++) {
		new_arr[i] = (RGB*)malloc(sizeof(RGB) * W);
	}

	if (!strcmp(opts->t, "x")) {flag = 1;}
	if (!strcmp(opts->t, "y")) {flag = 2;}
	if (!strcmp(opts->t, "xy")) {flag = 3;}

	if (flag == 1) {
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (j < step) {
					new_arr[i][j] = image[i][j + W - step];
				} else {new_arr[i][j] = image[i][j - step];}
			}
		}
	}

	if (flag == 2) {
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (i < step) {
					new_arr[i][j] = image[i + H - step][j];
				} else {new_arr[i][j] = image[i - step][j];}
			}
		}
	}

	if (flag == 3) {
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (j < step) {
					new_arr[i][j] = image[i][j + W - step];
				} else {new_arr[i][j] = image[i][j - step];}
			}
		}
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (i < step) {
					image[i][j] = new_arr[i + H - step][j];
				} else {image[i][j] = new_arr[i - step][j];}
			}
		}
		free_RGB_matrix(new_arr, H);
		return image;
	}

	free_RGB_matrix(image, H);
	image = new_arr;
	return image;
}