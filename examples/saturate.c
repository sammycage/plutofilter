#include "example.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc != 3) {
        fprintf(stderr, "Usage: saturate <input> <amount>\n");
        return 1;
    }

    plutofilter_surface_t input = example__load_input(argv[1]);
    float amount = (float)atof(argv[2]);

    plutofilter_color_transform_saturate(input, input, amount);

    example__write_output(input, argv[1], NULL, "saturate-%g", amount);
    return 0;
}
