#include "example.h"

#define PLUTOFILTER_IMPLEMENTATION
#include "plutofilter.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdarg.h>
#include <string.h>

plutofilter_surface_t example__load_input(const char* filename)
{
    int width, height;
    void* image = stbi_load(filename, &width, &height, NULL, STBI_rgb_alpha);
    if(image == NULL) {
        fprintf(stderr, "Unable to load image: '%s'\n", filename);
        exit(1);
    }

    plutofilter_surface_t out = { image, width, height, width };
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(out, x, y, b, g, r, a);
            PLUTOFILTER_SRGB_TO_LINEAR_RGB(r, g, b);
            PLUTOFILTER_PREMULTIPLY_PIXEL(r, g, b, a);
            PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, a);
        }
    }

    return out;
}

static int example__get_basename(const char* input, char* output, int index)
{
    const char* slash1 = strrchr(input, '/');
    const char* slash2 = strrchr(input, '\\');
    const char* last_slash = slash1 > slash2 ? slash1 : slash2;

    const char* it = last_slash ? last_slash + 1 : input;
    const char* end = strrchr(it, '.');
    while(*it && it != end) {
        output[index++] = *it++;
    }

    return index;
}

void example__write_output(plutofilter_surface_t out, const char* input1, const char* input2, const char* format, ...)
{
    char filename[512];
    int index = example__get_basename(input1, filename, 0);
    if(input2) {
        filename[index++] = '-';
        index = example__get_basename(input2, filename, index);
    }

    filename[index++] = '-';

    va_list args;
    va_start(args, format);
    index += vsnprintf(filename + index, sizeof(filename) - index, format, args);
    va_end(args);

    int has_alpha = 0;
    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(out, x, y, r, g, b, a);
            PLUTOFILTER_UNPREMULTIPLY_PIXEL(r, g, b, a);
            PLUTOFILTER_LINEAR_RGB_TO_SRGB(r, g, b);
            PLUTOFILTER_STORE_PIXEL(out, x, y, b, g, r, a);
            has_alpha |= (a < 255);
        }
    }

    filename[index++] = '.';
    filename[index++] = has_alpha ? 'p' : 'j';
    filename[index++] = has_alpha ? 'n' : 'p';
    filename[index++] = 'g';
    filename[index] = '\0';

    int success = 0;
    if(out.width && out.height) {
        if(has_alpha) {
            success = stbi_write_png(filename, out.width, out.height, 4, out.pixels, out.stride * 4);
        } else {
            success = stbi_write_jpg(filename, out.width, out.height, 4, out.pixels, out.stride * 4);
        }
    }

    stbi_image_free(out.pixels);
    if(!success) {
        fprintf(stderr, "Failed: '%s'\n", filename);
        exit(1);
    }

    fprintf(stdout, "Success: '%s'\n", filename);
}
