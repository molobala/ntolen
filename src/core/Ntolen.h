#ifndef NTOLEN_H
#define NTOLEN_H
#include "Mosc.h"
#include <SDL3/SDL.h>
#include "UIComponent.h"
#include "Input.h"

#pragma once
namespace ntolen
{

    
    class Ntolen
    {
    public:
        MoscRuntime *runtime() const
        {
            return _runtime;
        }
        MVM* vm() {
            return _runtime->vm();
        }
        void run(int frame);
        static Ntolen *instance();
        static MVM *runningVm();
        static void clean();
        bool running() { return _running; }
        void stop() { _running = false; }
        void render(float fps);
        Input* input() { return _input;}
        void registerComponent(UIComponent *component);
        void unregisterComponent(UIComponent *component);
    private:
        Ntolen();
        ~Ntolen();
        Input *_input;
        bool _running;
        Uint64 _frame;
        MSCConfig *_config;
        MoscRuntime *_runtime;
        void _registersPackages();
        static Ntolen *_instance;
        std::vector<UIComponent *> _components;
        std::vector<InputProcessor *> _processors;
    };
}

#endif