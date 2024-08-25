#include "Window.h"
#include <iostream>
#include "core/Ntolen.h"
#include "UI.h"
using namespace ntolen;


Renderer::Renderer(SDL_Renderer *renderer) : _renderer(renderer)
{
}
Renderer::~Renderer()
{
}

Window::Window(const char *t, int w, int h) : UIComponent(), _title(t), _width(w), _height(h), updateFn(nullptr)
{
    _window = SDL_CreateWindow(_title, w, h, SDL_WINDOW_MAXIMIZED);
    _renderer = SDL_CreateRenderer(_window, NULL);
}
void Window::show(bool y)
{
    if (y)
    {
        SDL_ShowWindow(_window);
    }
    else
    {
        SDL_HideWindow(_window);
    }
}
Window::~Window()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

void Window::render(float fps)
{
    if (!updateFn)
    {
        return;
    }
    SDL_RenderClear(_renderer);
    Djuru *djuru = MSCGetCurrentDjuru(Ntolen::instance()->runtime()->vm());
    MSCEnsureSlots(djuru, 3);
    MSCSetSlotHandle(djuru, 0, updateFn);
    MSCSetSlotDouble(djuru, 2, fps);
    MSCSetSlotHandle(djuru, 1, Ntolen::instance()->runtime()->makeHandle(djuru, 1, "ui", "Renderer"));
    Renderer *rend = (Renderer *)MSCSetSlotNewExtern(djuru, 1, 1, sizeof(Renderer));
    new (rend) Renderer(renderer());
   
    MSCCall(djuru, updateFn);

}
void  WindowClassProvider::provide(ClassBuilder& builder) {
    builder
    .constructor([](Djuru *djuru)
                     {
                
                    std::cout<<"Initiating"<<std::endl;
                    auto title = MSCGetSlotString(djuru, 1);
                    auto w = (int)MSCGetSlotDouble(djuru, 2);
                    auto h = (int)MSCGetSlotDouble(djuru, 3);
                    std::cout<<"Initiating:title"<<title<<" "<<w<<" "<<h<<std::endl;
                    MSCSetSlotHandle(djuru, 0, Ntolen::instance()->runtime()->makeHandle(djuru, 0, "ui", "Window"));
                    Window *win = (Window *)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(Window));
                    new(win) Window(title, w, h); })
        .finalizer(([](void *data)
                    {
            std::cout<<"Destroying"<<std::endl;
            auto win = (Window*)data;
            if(win->updateFn) {
                MSCReleaseHandle(Ntolen::instance()->runtime()->vm(), win->updateFn);
                win->updateFn = nullptr;
            }
            (win)->~Window(); }))
        .method(false, "show()", [](Djuru *djuru)
                {
            auto win = (Window*)MSCGetSlotExtern(djuru, 0);
            win->show(true); })
        .method(false, "hide()", [](Djuru *djuru)
                {
            auto win = (Window*)MSCGetSlotExtern(djuru, 0);
            win->show(false); })
        .method(false, "renderer", [](Djuru *djuru)
                {
            // create a new renderer
            std::cout<<"Creating renderer"<<std::endl;
            auto win = (Window*)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotHandle(djuru, 0, Ntolen::instance()->runtime()->makeHandle(djuru, 0, "ui", "Renderer"));
            Renderer *rend = (Renderer *)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(Renderer));
                    new(rend) Renderer(win->renderer()); })
        .method(false, "onUpdate=(_)", [](Djuru *djuru)
                {
                    auto win = (Window *)MSCGetSlotExtern(djuru, 0);
                    win->updateFn = (MSCHandle *)MSCGetSlotHandle(djuru, 1); })

        .end()
        .clazz("Renderer")
        // .constructor([](Djuru *djuru) {

        // })
        .finalizer([](void *data)
                   {
            std::cout<<"Destroying renderer"<<std::endl;
            auto rend = (Renderer*) data;
            if(rend != nullptr) {
                rend->~Renderer();
            } })
        .method(false, "clear()", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->clear();
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "rect(_,_,_,_)", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->drawRect(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3), MSCGetSlotDouble(djuru, 4));
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "point(_,_)", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->drawPoint(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2));
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "fill(_,_,_,_)", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->fillRect(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3), MSCGetSlotDouble(djuru, 4));
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "line(_,_,_,_)", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->drawLine(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3), MSCGetSlotDouble(djuru, 4));
            MSCSetSlotBool(djuru, 0, true); })
        // .method(false, "lines(_,_,_,_)", [](Djuru* djuru) {
        //     auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
        //     renderer->drawLines(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3), MSCGetSlotDouble(djuru, 4));
        //     MSCSetSlotBool(djuru, 0, true);
        // })
        .method(false, "color(_,_,_,_)", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->setDrawColor(MSCGetSlotDouble(djuru, 1), (int)MSCGetSlotDouble(djuru, 2), (int)MSCGetSlotDouble(djuru, 3), (int)MSCGetSlotDouble(djuru, 4));
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "complete()", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->complete();
            MSCSetSlotBool(djuru, 0, true); })
        .end();
}
WindowClassProvider WindowClassProvider::provider = WindowClassProvider();