#include "example.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc != 3) {
        fprintf(stderr, "Usage: hue-rotate <input> <angle>\n");
        return 1;
    }

    plutofilter_surface_t input = example__load_input(argv[1]);
    float angle = (float)atof(argv[2]);

    plutofilter_color_transform_hue_rotate(input, input, angle);

    example__write_output(input, argv[1], NULL, "hue-rotate-%g", angle);
    return 0;
}
