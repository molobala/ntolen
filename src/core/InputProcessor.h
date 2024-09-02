#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#pragma once
namespace ntolen
{
    enum InputType {
        keybord,
        mouse,
        gamepad
    };

    class InputProcessor
    {
    public:
        virtual ~InputProcessor() {}
        virtual bool updateInput(InputType type, const char* key, bool state) { return false;};
        // virtual bool processMouseEvent(const char* key, bool state) {return false; };
        // virtual bool processGamepadEvent(const char* key, bool state) { return false;};
    };
}

#endif