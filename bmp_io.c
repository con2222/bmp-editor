#include "bmp.h"

RGB** read_bmp(char file_name[], BitmapFileHeader* bmfh, BitmapInfoHeader* bmif) {
	FILE* f = fopen(file_name, "rb");
	if (f == NULL) {
		printf("Error opening file\n");
		exit(ERROR_OPENING_FILE);
	}
	fread(bmfh, 1, sizeof(BitmapFileHeader), f);
	if (bmfh->signature != BM) {
		printf("File is not a valid BMP file\n");
		fclose(f);
		exit(VALID_BMP);
	}
	fread(bmif, 1, sizeof(BitmapInfoHeader), f);
	if (bmif->headerSize != sizeof(BitmapInfoHeader)) {
	    printf("Only standard BMP files (BITMAPINFOHEADER v3, 40 bytes) are supported\n");
	    fclose(f);
	    exit(VALID_BMP);
	}
	if (bmif->compression != 0) {
		printf("Only uncompressed BMP files are supported\n");
		exit(VALID_BMP);
	}
	if (bmif->bitsPerPixel != 24) {
		printf("Only 24 bits per pixel BMP files are supported\n");
		exit(VALID_BMP);
	}
	unsigned int H = bmif->height;
	unsigned int W = bmif->width;
    fseek(f, bmfh->pixelArrOffset, SEEK_SET);
	RGB** arr = (RGB**)malloc(H * sizeof(RGB*));
	for (int i = 0; i < H; i++) {
		int padding = (4 - ((W * 3) % 4)) % 4;
		arr[i] = (RGB*)malloc(W * sizeof(RGB));
		fread(arr[i], 1, W * 3, f);
		fseek(f, padding, SEEK_CUR);
	}
	fclose(f);
	return arr;
}

void write_bmp(char file_name[], RGB **arr, int H, int W, BitmapFileHeader bmfh, BitmapInfoHeader bmif) {
	FILE* ff = fopen(file_name, "wb");
	fwrite(&bmfh, 1, sizeof(BitmapFileHeader), ff);
	fwrite(&bmif, 1, sizeof(BitmapInfoHeader), ff);
	for(int i = 0; i < H; i++) {
		fwrite(arr[i], 1, W * 3, ff);
		int padding = (4 - ((W * 3) % 4)) % 4;
		if (padding > 0) {
			char* padding_bytes = calloc(1, padding);
			fwrite(padding_bytes, 1, padding, ff);
			free(padding_bytes);
		}
	}
	fclose(ff);
}

void free_RGB_matrix(RGB** arr, int H) {
	for(int i = 0; i < H; i++) {
		free(arr[i]);
	}
	free(arr);
}

RGB** reverse(RGB** arr, int H, int W) {
	RGB** reversed = (RGB**)malloc(H * sizeof(RGB*));
	for (int i = 0; i < H; i++) {
		reversed[i] = (RGB*)malloc(W * sizeof(RGB));
	}

	for (int i = H - 1; i >= 0; i--) {
		for (int j = 0; j < W; j++) {
			reversed[H - i - 1][j] = arr[i][j];
		}
	}

	free_RGB_matrix(arr, H);
	arr = reversed;

	return arr;
}

void print_file_header(BitmapFileHeader header) {
	printf("signature:\t%x (%hu)\n", header.signature, header.signature);
	printf("filesize:\t%x (%u)\n", header.filesize, header.filesize);
	printf("reserved1:\t%x (%hu)\n", header.reserved1, header.reserved1);
	printf("reserved2:\t%x (%hu)\n", header.reserved2, header.reserved2);
	printf("pixelArrOffset:\t%x (%u)\n", header.pixelArrOffset, header.pixelArrOffset);
}

void print_info_header(BitmapInfoHeader header) {
	printf("headerSize:\t%x (%u)\n", header.headerSize, header.headerSize);
	printf("width:	\t%x (%u)\n", header.width, header.width);
	printf("height:	\t%x (%u)\n", header.height, header.height);
	printf("planes:	\t%x (%hu)\n", header.planes, header.planes);
	printf("bitsPerPixel:\t%x (%hu)\n", header.bitsPerPixel, header.bitsPerPixel);
	printf("compression:\t%x (%u)\n", header.compression, header.compression);
	printf("imageSize:\t%x (%u)\n", header.imageSize, header.imageSize);
	printf("xPixelsPerMeter:\t%x (%u)\n", header.xPixelsPerMeter, header.xPixelsPerMeter);
	printf("yPixelsPerMeter:\t%x (%u)\n", header.yPixelsPerMeter, header.yPixelsPerMeter);
	printf("colorsInColorTable:\t%x (%u)\n", header.colorsInColorTable, header.colorsInColorTable);
	printf("importantColorCount:\t%x (%u)\n", header.importantColorCount, header.importantColorCount);
}

void print_info(BitmapInfoHeader* bmif, BitmapFileHeader* bmfh) {
	print_file_header(*bmfh);
	print_info_header(*bmif);
}