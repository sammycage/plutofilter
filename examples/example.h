#ifndef PLUTOFILTER_EXAMPLE_H
#define PLUTOFILTER_EXAMPLE_H

#include "plutofilter.h"

plutofilter_surface_t example__load_input(const char* filename);
void example__write_output(plutofilter_surface_t out, const char* input1, const char* input2, const char* format, ...);

#endif // PLUTOFILTER_EXAMPLE_H
