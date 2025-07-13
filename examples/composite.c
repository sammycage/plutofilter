#include "example.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static plutofilter_composite_operator_t parse_composite_operator(const char* name)
{
    static const struct {
        const char* name;
        plutofilter_composite_operator_t op;
    } table[] = {
        {"over", PLUTOFILTER_COMPOSITE_OPERATOR_OVER},
        {"in",   PLUTOFILTER_COMPOSITE_OPERATOR_IN},
        {"out",  PLUTOFILTER_COMPOSITE_OPERATOR_OUT},
        {"atop", PLUTOFILTER_COMPOSITE_OPERATOR_ATOP},
        {"xor",  PLUTOFILTER_COMPOSITE_OPERATOR_XOR},
        {NULL}
    };

    for(int i = 0; table[i].name; ++i) {
        if(strcmp(name, table[i].name) == 0) {
            return table[i].op;
        }
    }

    fprintf(stderr, "invalid composite operator: '%s': valid options are: (", name);
    for(int i = 0; table[i].name; ++i) {
        fprintf(stderr, "'%s'%s", table[i].name, table[i + 1].name ? ", " : ")\n");
    }

    exit(1);
}

int main(int argc, char* argv[])
{
    if(argc != 4) {
        fprintf(stderr, "Usage: composite <input1> <input2> <operator>\n");
        return 1;
    }

    plutofilter_surface_t input1 = example__load_input(argv[1]);
    plutofilter_surface_t input2 = example__load_input(argv[2]);
    plutofilter_composite_operator_t op = parse_composite_operator(argv[3]);

    plutofilter_composite(input1, input2, input1, op);

    example__write_output(input1, argv[1], argv[2], "composite-%s", argv[3]);
    return 0;
}
