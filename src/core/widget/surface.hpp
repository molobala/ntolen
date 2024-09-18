#ifndef NTOLEN_SURFACE_HPP
#define NTOLEN_SURFACE_HPP
#include <SDL3/SDL.h>
#include "helper/color.hpp"

namespace ntolen
{

    void setPixelAt(SDL_Surface *surface, int64_t x, int64_t y, uint32_t c)
    {
        uint8_t newA = transparency(c);
        if (newA == 0)
        {
            return;
        }

        if (newA < 0xFF)
        {
            uint32_t currentPixel = ((uint32_t *)surface->pixels)[surface->w * y + x];
            double aNorm = newA / (double)UINT8_MAX;
            double diffA = 1 - aNorm;

            uint8_t r, g, b, newR, newG, newB, a = 0xFF;
            colorComponents(c, &r, &g, &b);
            colorComponents(currentPixel, &newR, &newG, &newB);
            r = r * diffA + newR * diffA;
            g = g * diffA + newG * diffA;
            b = b * diffA + newB * diffA;
            c = colorValueOf(r, g, b, a);
        }

        *(((uint32_t *)surface->pixels) + (surface->w * y + x)) = c;
    }
}

#endif