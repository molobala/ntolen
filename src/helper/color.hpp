#include <stdlib.h>

namespace ntolen
{
    inline void colorComponents(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b)
    {
        *r = color & 0xFF;
        *g = (color & (0xFF << 8)) >> 8;
        *b = (color & (0xFF << 16)) >> 16;
    }
    inline uint8_t transparency(uint32_t color)
    {
        return (color & (0xFF << 24)) >> 24;
    }

    inline uint32_t colorValueOf(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        return (a << 24) | (b << 16) | (g << 8) | r;
    }
}