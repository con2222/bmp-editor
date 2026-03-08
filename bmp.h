#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define DEFAULT_RESULT_NAME "out.bmp"
#define BM 0x4D42
#define OPTION_FOUND 1
#define OPTION_NOT_FOUND 0
#define SHORT_OPTIONS "hi:o:"
#define INVERSE 'p'
#define GRAY 'g'
#define RESIZE 's'
#define LEFT_UP 'l'
#define RIGHT_DOWN 'r'
#define COLOR 'c' 
#define OUTPUT 'o'
#define HELP 'h'
#define INPUT 'i'
#define INFO 'a'
#define NO_ARGUMENT '?'
#define ERROR_OPENING_FILE 40
#define VALID_BMP 41
#define VALID_COORDINATES 42
#define VALID_COLOR 43
#define ERROR 44
#define OPT_LEFT 1001
#define OPT_RIGHT 1002
#define OPT_ABOVE 1003
#define OPT_BELOW 1004



typedef enum {
	LEFT,
	RIGHT,
	ABOVE,
	BELOW,
	UNKNOWN_DIRECTION
} Direction;

#pragma pack (push, 1)
typedef struct {
	unsigned short signature;
	unsigned int filesize;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int pixelArrOffset;
} BitmapFileHeader;

typedef struct {
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    unsigned int xPixelsPerMeter;
    unsigned int yPixelsPerMeter;
    unsigned int colorsInColorTable;
    unsigned int importantColorCount;
} BitmapInfoHeader;

typedef struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} RGB;
#pragma pack(pop)

typedef struct {
	int inverse_flag;
	int gray_flag;
	int resize_flag;
	int color_flag;
	int help_flag;
	int info_flag;
	int shift_flag;
	int step;
	char t[50];
	int need_change_filename;
	int x0, y0, x1, y1;
	int size[4];
	int color[3];
	char i_filename[100];
	char o_filename[100];
} AppOptions;

RGB** read_bmp(char file_name[], BitmapFileHeader* bmfh, BitmapInfoHeader* bmif);
void write_bmp(char file_name[], RGB **arr, int H, int W, BitmapFileHeader bmfh, BitmapInfoHeader bmif);
void free_RGB_matrix(RGB** arr, int H);
void print_info(BitmapInfoHeader* bmif, BitmapFileHeader* bmfh);

RGB** reverse(RGB** arr, int H, int W);
RGB** resize(RGB** arr, BitmapInfoHeader* bmif, BitmapFileHeader* bmfh, int* size, int* color);
void gray(RGB** arr, int H, int W, int x0, int y0, int x1, int y1);
void inverse(RGB** arr, int H, int W, int x0, int y0, int x1, int y1);
RGB** shift(RGB** image, BitmapInfoHeader* bmif, BitmapFileHeader* bmfh, AppOptions *opts);
void apply_operations(RGB*** image, BitmapInfoHeader* bmif, BitmapFileHeader* bmfh, AppOptions *opts);

void print_usage(const char* prog_name, AppOptions* opts);
void parse_arguments(int argc, char *argv[], AppOptions *opts);


#endif