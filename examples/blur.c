#include "example.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: blur <input> <radius-x> [radius-y]\n");
        return 1;
    }

    plutofilter_surface_t input = example__load_input(argv[1]);
    float radius_x = (float)atof(argv[2]);
    float radius_y = (argc == 4) ? (float)atof(argv[3]) : radius_x;

    plutofilter_gaussian_blur(input, input, radius_x, radius_y);

    example__write_output(input, argv[1], NULL, "blur-%g-%g", radius_x, radius_y);
    return 0;
}
