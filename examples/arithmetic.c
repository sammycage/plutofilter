#include "example.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc != 7) {
        fprintf(stderr, "Usage: arithmetic <input1> <input2> <k1> <k2> <k3> <k4>\n");
        return 1;
    }

    plutofilter_surface_t input1 = example__load_input(argv[1]);
    plutofilter_surface_t input2 = example__load_input(argv[2]);

    float k1 = (float)atof(argv[3]);
    float k2 = (float)atof(argv[4]);
    float k3 = (float)atof(argv[5]);
    float k4 = (float)atof(argv[6]);

    plutofilter_composite_arithmetic(input1, input2, input1, k1, k2, k3, k4);

    example__write_output(input1, argv[1], argv[2], "arithmetic-%g-%g-%g-%g", k1, k2, k3, k4);
    return 0;
}
