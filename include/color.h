#ifndef _COLOR_H_
#define _COLOR_H_

#include <stdint.h>

typedef struct {
    float r, g, b, a;
} Color;

Color color_from_hex(uint32_t rgba);
Color color_from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint32_t color_to_hex(Color c);

#endif // _COLOR_H_