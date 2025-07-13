#include "example.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static plutofilter_blend_mode_t parse_blend_mode(const char* name)
{
    static const struct {
        const char* name;
        plutofilter_blend_mode_t mode;
    } table[] = {
        {"normal",      PLUTOFILTER_BLEND_MODE_NORMAL},
        {"multiply",    PLUTOFILTER_BLEND_MODE_MULTIPLY},
        {"screen",      PLUTOFILTER_BLEND_MODE_SCREEN},
        {"overlay",     PLUTOFILTER_BLEND_MODE_OVERLAY},
        {"darken",      PLUTOFILTER_BLEND_MODE_DARKEN},
        {"lighten",     PLUTOFILTER_BLEND_MODE_LIGHTEN},
        {"color-dodge", PLUTOFILTER_BLEND_MODE_COLOR_DODGE},
        {"color-burn",  PLUTOFILTER_BLEND_MODE_COLOR_BURN},
        {"hard-light",  PLUTOFILTER_BLEND_MODE_HARD_LIGHT},
        {"soft-light",  PLUTOFILTER_BLEND_MODE_SOFT_LIGHT},
        {"difference",  PLUTOFILTER_BLEND_MODE_DIFFERENCE},
        {"exclusion",   PLUTOFILTER_BLEND_MODE_EXCLUSION},
        {NULL}
    };

    for(int i = 0; table[i].name; ++i) {
        if(strcmp(name, table[i].name) == 0) {
            return table[i].mode;
        }
    }

    fprintf(stderr, "invalid blend mode: '%s': valid options are: (", name);
    for(int i = 0; table[i].name; ++i) {
        fprintf(stderr, "'%s'%s", table[i].name, table[i + 1].name ? ", " : ")\n");
    }

    exit(1);
}

int main(int argc, char* argv[])
{
    if(argc != 4) {
        fprintf(stderr, "Usage: blend <input1> <input2> <mode>\n");
        return 1;
    }

    plutofilter_surface_t input1 = example__load_input(argv[1]);
    plutofilter_surface_t input2 = example__load_input(argv[2]);
    plutofilter_blend_mode_t mode = parse_blend_mode(argv[3]);

    plutofilter_blend(input1, input2, input1, mode);

    example__write_output(input1, argv[1], argv[2], "blend-%s", argv[3]);
    return 0;
}
