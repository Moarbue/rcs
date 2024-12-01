#include "color.h"

Color color_from_hex(uint32_t rgba)
{
    return color_from_rgba(
        (rgba >> 24) & 0xFF,
        (rgba >> 16) & 0xFF,
        (rgba >>  8) & 0xFF,
        (rgba >>  0) & 0xFF
    );
}

Color color_from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return (Color) {
        .r = (float)r / 255.0f,
        .g = (float)g / 255.0f,
        .b = (float)b / 255.0f,
        .a = (float)a / 255.0f,
    };
}

uint32_t color_to_hex(Color c)
{
    uint32_t r, g, b, a;

    r = c.r * 255.0f;
    g = c.g * 255.0f;
    b = c.b * 255.0f;
    a = c.a * 255.0f;

    return (
        ((r & 0xFF) << 24) |
        ((g & 0xFF) << 16) |
        ((b & 0xFF) <<  8) |
        ((a & 0xFF) <<  0)
    );
}
