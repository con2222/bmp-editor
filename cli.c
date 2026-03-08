#include "bmp.h"

void print_usage(const char* prog_name, AppOptions* opts) {
	opts->help_flag = 1;
    printf("BMP Editor Tool - A command-line utility for BMP image processing\n\n");
    printf("Usage: %s [OPTIONS] input_file.bmp\n", prog_name);
    printf("\n");
    
    printf("Options:\n");
    printf("  -h, --help                        Show this help and exit\n");
    printf("  -i, --input FILE                  Set input file name (default is last argument)\n");
    printf("  -o, --output FILE                 Set output file name (default is out.bmp)\n");
    printf("	  --info FILE                   Show BMP file header info");
    printf("\n");

    printf("  -i, --inverse                     Invert colors of selected area\n");
    printf("  -g, --gray                        Convert selected area to grayscale\n");
    printf("      --left_up x.y                 Set top-left corner for --inverse or --gray\n");
    printf("      --right_down x.y              Set bottom-right corner for --inverse or --gray\n");
    printf("\n");
    printf("  -s, --resize                      Resize image\n");
    printf("      --left N                      Add/remove pixels on the left\n");
    printf("      --right N                     Add/remove pixels on the right\n");
    printf("      --above N                     Add/remove pixels above\n");
    printf("      --below N                     Add/remove pixels below\n");
    printf("      --color R.G.B                 Set color for resizing (default is black)\n");
    printf("\n");

    printf("Examples:\n");
    printf("  %s --left 100 --color 255.0.0 input.bmp\n", prog_name);
    printf("  %s --inverse --left_up 100.100 --right_down 200.200 input.bmp\n", prog_name);
    printf("  %s -o result --resize --above 50 input.bmp\n", prog_name);
}

void parse_coordinates(const char* optarg, int* x, int* y, int* n) {
	if (sscanf(optarg, "%d.%d%n", x, y, n) != 2 || optarg[*n] != '\0') {
		printf("Invalid coordinates format");
		exit(VALID_COORDINATES);
	}
}

void parse_color(const char* optarg, int color[3], int* n) {
	if (sscanf(optarg, "%d.%d.%d%n", &color[0], &color[1], &color[2], n) != 3 || optarg[*n] != '\0') {
		printf("Invalid color format");
		exit(VALID_COLOR);
	}
}

void parse_arguments(int argc, char *argv[], AppOptions *opts) {
    const char* short_options = SHORT_OPTIONS;
    const struct option long_options[] = {
        {"inverse", no_argument, NULL, INVERSE},
        {"gray", no_argument, NULL, GRAY},
        {"resize", no_argument, NULL, RESIZE},
        {"left_up", required_argument, NULL, LEFT_UP},
        {"right_down", required_argument, NULL, RIGHT_DOWN},
        {"left", required_argument, NULL, OPT_LEFT},
        {"right", required_argument, NULL, OPT_RIGHT},
        {"above", required_argument, NULL, OPT_ABOVE},
        {"below", required_argument, NULL, OPT_BELOW},
        {"color", required_argument, NULL, COLOR},
        {"output", required_argument, NULL, OUTPUT},
        {"help", no_argument, NULL, HELP},
        {"input", required_argument, NULL, INPUT},
        {"info", no_argument, NULL, INFO},
        {"shift", no_argument, NULL, '9'},
        {"step", required_argument, NULL, '8'},
        {"axis", required_argument, NULL, '7'},
        {0, 0, 0, 0}
    };

    int rez, option_index, n = 0;

    while ((rez = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (rez) {
            case INVERSE: opts->inverse_flag = OPTION_FOUND; break;
            case GRAY: opts->gray_flag = OPTION_FOUND; break;
            case RESIZE: opts->resize_flag = OPTION_FOUND; break;
            case '9': opts->shift_flag = 1; break;
            case '8':
                sscanf(optarg, "%d", &opts->step);
                break;
            case '7':
                sscanf(optarg, "%s", opts->t);
                break;
            case LEFT_UP:
                if (sscanf(optarg, "%d.%d%n", &opts->x0, &opts->y0, &n) != 2 || optarg[n] != '\0') {
                    printf("Invalid coordinates format");
                    exit(VALID_COORDINATES);
                }
                break;

            case RIGHT_DOWN:
                if (sscanf(optarg, "%d.%d%n", &opts->x1, &opts->y1, &n) != 2 || optarg[n] != '\0') {
                    printf("Invalid coordinates format");
                    exit(VALID_COORDINATES);
                }
                break;

            case OPT_LEFT:
                sscanf(optarg, "%d%n", &opts->size[0], &n);
                break;

            case OPT_RIGHT:
                sscanf(optarg, "%d%n", &opts->size[1], &n);
                break;

            case OPT_ABOVE:
                sscanf(optarg, "%d%n", &opts->size[2], &n);
                break;

            case OPT_BELOW:
                sscanf(optarg, "%d%n", &opts->size[3], &n);
                break;

            case COLOR:
                if (sscanf(optarg, "%d.%d.%d%n", &opts->color[0], &opts->color[1], &opts->color[2], &n) != 3 || optarg[n] != '\0') {
                    printf("Invalid color format");
                    exit(VALID_COLOR);
                }
                opts->color_flag = 1;
                break;

            case OUTPUT:
                strcpy(opts->o_filename, optarg);
                break;
            case HELP:
                print_usage(argv[0], opts);
                break;
            case INFO:
                opts->info_flag = 1;
                break;
            case INPUT:
                strcpy(opts->i_filename, optarg);   
                break;
            case NO_ARGUMENT:
                exit(ERROR);
            default:
                break;
        }
    }
}