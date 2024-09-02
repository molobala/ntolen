#include "Ntolen.h"
#include <iostream>
#include "widget/Window.h"
#include <SDL3/SDL.h>
#include "widget/UI.h"
#include "Input.h"

static const char *appSource =
#include "Ntolen.msc.inc"
    ;

using namespace ntolen;

Ntolen::Ntolen() : _running(false), _components(), _processors()
{
    _config = new MSCConfig;
    _input = nullptr;
    MSCInitConfig(_config);
    _config->initialHeapSize = 1024 * 2;
    _config->writeFn = [](MVM *vm, const char *text)
    { std::cout << text; };
    _config->errorHandler = [](MVM *vm, MSCError type, const char *module, int line, const char *message) -> bool
    {
        switch (type)
        {
        case ERROR_COMPILE:
            std::cerr << "[" << module << " line " << line << "] " << message;
            break;

        case ERROR_RUNTIME:
            std::cerr << message;
            break;

        case ERROR_STACK_TRACE:
            std::cerr << "[" << module << " line " << line << "] " << message;
            break;
        }
        return true;
    };
    _runtime = new MoscRuntime(_config);

    _registersPackages();
    // SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
}
Ntolen *Ntolen::_instance = nullptr;

void Ntolen::clean()
{
    UI::clean();
    if (_instance == nullptr)
    {
        return;
    }
    if(InputModuleProvider::provider != nullptr) {
        delete InputModuleProvider::provider;
    }
    delete _instance;
    _instance = nullptr;
}
Ntolen *Ntolen::instance()
{
    if (_instance == nullptr)
    {
        _instance = new Ntolen();
    }
    return _instance;
}
MVM *Ntolen::runningVm()
{
    return _instance->vm();
}

Ntolen::~Ntolen()
{

    delete _config;
    // delete _input;
    delete _runtime;
}

void Ntolen::_registersPackages()
{
    // auto ui = Window::package();
    // MoscRuntime::install(ui);
    UI::init(UIFeature::video | UIFeature::audio);
    UI::install();
    MoscRuntime::install(PackageBuilder("app")
                             .module("app", &appSource)
                             .clazz("App")
                             .method(true, "running", [](Djuru *djuru)
                                     { MSCSetSlotBool(djuru, 0, Ntolen::_instance->running()); })
                             .method(true, "kill()", [](Djuru *djuru)
                                     {
                                Ntolen::_instance->stop();
                                MSCSetSlotBool(djuru, 0, true); })
                             //  .method(true, "event", [](Djuru *djuru)
                             //          {
                             //             MSCSetSlotHandle(djuru, 0, UI::makeHandle(djuru, 0, "app", "Event"));
                             //             Event **event = (Event **)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(Event*));
                             //             *event = Ntolen::_instance->event();
                             //             if((*event)->keyUp(40)) {
                             //                 SDL_Log("Key 40 up");
                             //             }
                             //         })
                             .method(true, "run(_)", [](Djuru *djuru)
                                     {
        Ntolen::_instance->run((int)MSCGetSlotDouble(djuru, 1));
        MSCSetSlotBool(djuru, 0, true); })
                             .end()
                             .end()
                             .provide(*InputModuleProvider::provider)
                             .build());
    // std::cout<<ui.module("ui")->source()<<std::endl;
    // std::cout<<ui.module("ui")->source()<<std::endl;
}

void Ntolen::run(int frame)
{
    _frame = 1000 / frame;
    _running = true;
    Uint64 startTime = 0, endTime = 0, elapsed = 0;
    SDL_Event event;
    while (_running)
    {
        startTime = SDL_GetTicks();
        float delta = (startTime - endTime) / 1000; // difference in second from the last render to now
        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_EVENT_QUIT)
            {
                stop();
                return;
            }
            if(!_input) {
                break;
            }
            switch (event.type)
            {
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
            {
                std::string keyName = std::string(SDL_GetKeyName(event.key.key));
                std::transform(keyName.begin(), keyName.end(), keyName.begin(),
                               [](unsigned char c)
                               { return c != ' ' ? std::tolower(c) : '_'; });
                _input->updateInput(keybord, keyName.c_str(), event.key.state == SDL_PRESSED);
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                const char *buttonName = "";
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    buttonName = "left";
                    break;
                case SDL_BUTTON_MIDDLE:
                    buttonName = "middle";
                    break;
                case SDL_BUTTON_RIGHT:
                    buttonName = "right";
                    break;
                case SDL_BUTTON_X1:
                    buttonName = "x1";
                    break;
                default:
                case SDL_BUTTON_X2:
                    buttonName = "x2";
                    break;
                }
                bool state = event.button.state == SDL_PRESSED;
                _input->updateInput(mouse, buttonName, event.key.state == SDL_PRESSED);
                break;
            }
            default:
                break;
            }
        }
        render(delta);
        endTime = SDL_GetTicks();
        if ((elapsed = (endTime - startTime) < _frame))
        {
            SDL_Delay(frame - elapsed);
        }
    }
}

void Ntolen::registerComponent(UIComponent *component)
{
    _components.push_back(component);
}
void Ntolen::unregisterComponent(UIComponent *component)
{
    auto it = std::find_if(_components.begin(), _components.end(), [component](UIComponent *c)
                           { return component == c; });
    if (it != _components.end())
    {
        _components.erase(it);
    }
}

void Ntolen::render(float fps)
{

    for (auto &comp : _components)
    {
        comp->render(fps);
    }
}
