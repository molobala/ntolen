#ifndef EVENT_H
#define EVENT_H
#include <SDL3/SDL.h>
#include "Mosc.h"
#include "InputProcessor.h"

#pragma once

namespace ntolen
{
    class InputModuleProvider : public ModuleProvider
    {
    public:
        static InputModuleProvider provider;
        const char *moduleName() const
        {
            return "input";
        }
        const char** moduleSource() const;
        void provide(ModuleBuilder &builder);
    };
    class Input: public InputProcessor
    {
    public:
        Input();
        ~Input();
        virtual bool processKeybordEvent(const char* key, bool state);
        virtual bool processMouseEvent(const char* key, bool state);
        virtual bool processGamepadEvent(const char* key, bool state);
        MSCHandle* keybordClass;
        MSCHandle* mouseClass;
        MSCHandle* gamepadClass;
    private:
    };

} // namespace ntolen

#endif