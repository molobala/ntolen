#include "UI.h"
#include "Window.h"
#include "core/Ntolen.h"
static const char *source =
#include "UI.msc.inc"
    ;

using namespace ntolen;

UI::UI()
{
}

UI::~UI()
{
}
void UI::clean()
{
   
}
bool UI::initialized = false;

bool UI::init(Uint32 feature)
{

    int sdlFlag = SDL_INIT_VIDEO;
    if (feature & UIFeature::audio)
    {
        sdlFlag |= SDL_INIT_AUDIO;
    }
    if (feature & UIFeature::joystick)
    {
        sdlFlag |= SDL_INIT_JOYSTICK;
    }
    if (feature & UIFeature::gameController)
    {
        sdlFlag |= SDL_INIT_GAMEPAD;
    }
    initialized = SDL_Init(sdlFlag);

    return initialized;
}
void UI::install()
{
    // Window::package()

    // Window::install(moduler);
    MoscRuntime::install(PackageBuilder("ui")
                             .module("ui", &source)
                             .provide(WindowClassProvider::provider)
                             .end()
                             .build());
}