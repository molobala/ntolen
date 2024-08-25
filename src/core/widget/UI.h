#ifndef UI_H
#define UI_H
#include <map>
#include "msc.h"
#include "core/Mosc.h"
#include <SDL3/SDL.h>

#pragma once

namespace ntolen
{
    enum UIFeature
    {
        video = 1 << 0,
        audio = 1 << 1,
        joystick = 1 << 2,
        gameController = 1 << 2,
    };

    class UI
    {
    public:
        UI();
        ~UI();
        static bool init(Uint32 feature);
        static void install();
        static void clean();
        static bool isInitialized()
        {
            return initialized;
        }

    private:
        static bool initialized;
    };

} // namespace ntolen

#endif