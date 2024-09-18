#include "UI.h"
#include "Window.h"
#include "core/Ntolen.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
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
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
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
    initialized = SDL_Init(sdlFlag) == 0;
    initialized = initialized && (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0);
    if(TTF_Init()) {
        initialized = initialized && true;
    }
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
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