#include "Window.h"
#include <iostream>
#include "core/Ntolen.h"
#include "UI.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "surface.hpp"
#include "helper/logger.h"

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
    MSCSetSlotDouble(djuru, 1, fps);
   
    MSCCall(djuru, updateFn);

}
void  WindowClassProvider::provide(ClassBuilder& builder) {
    builder
    .constructor([](Djuru *djuru)
                     {
                
                    auto title = MSCGetSlotString(djuru, 1);
                    auto w = (int)MSCGetSlotDouble(djuru, 2);
                    auto h = (int)MSCGetSlotDouble(djuru, 3);
                    MSCSetSlotHandle(djuru, 0, Ntolen::instance()->runtime()->makeHandle(djuru, 0, "ui", "Window"));
                    Window *win = (Window *)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(Window));
                    new(win) Window(title, w, h); })
        .finalizer(([](void *data)
                    {
            Logger::debug("Destroying Window");
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
            Logger::debug("Creating renderer");
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
            Logger::debug("Destroying renderer");
            auto rend = (Renderer*) data;
            if(rend != nullptr) {
                rend->~Renderer();
            Logger::debug("Renderer destroyed");
            } })
        .method(false, "clear_()", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->clear();
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "rect_(_,_,_,_)", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->drawRect(std::move(Rect(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3), MSCGetSlotDouble(djuru, 4))));
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "point_(_,_)", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->drawPoint(std::move(Point(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2))));
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "fill_(_,_,_,_)", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->fillRect(std::move(Rect(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3), MSCGetSlotDouble(djuru, 4))));
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "line_(_,_,_,_)", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->drawLine(std::move(Point(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2))), std::move(Point(MSCGetSlotDouble(djuru, 3), MSCGetSlotDouble(djuru, 4))));
            MSCSetSlotBool(djuru, 0, true); })
        // .method(false, "lines(_,_,_,_)", [](Djuru* djuru) {
        //     auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
        //     renderer->drawLines(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3), MSCGetSlotDouble(djuru, 4));
        //     MSCSetSlotBool(djuru, 0, true);
        // })
        .method(false, "color_(_,_,_,_)", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->setDrawColor(std::move(Color(MSCGetSlotDouble(djuru, 1), (int)MSCGetSlotDouble(djuru, 2), (int)MSCGetSlotDouble(djuru, 3), (int)MSCGetSlotDouble(djuru, 4))));
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "commit_()", [](Djuru *djuru)
                {
            auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
            renderer->commit();
            MSCSetSlotBool(djuru, 0, true); })
        .method(false, "drawTexture_(_)", [](Djuru* djuru) {
             auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
             auto texture = (Texture *)MSCGetSlotExtern(djuru, 1);
             renderer->drawTexture(texture->handle);
            MSCSetSlotBool(djuru, 0, true);
        })
        .method(false, "drawTexture_(_,_,_)", [](Djuru* djuru) {
             auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 0);
             auto texture = (Texture *)MSCGetSlotExtern(djuru, 1);
             auto src = (MSCGetSlotType(djuru, 2) == MSC_TYPE_NULL) ? nullptr :(Rect*) MSCGetSlotExtern(djuru, 2);
             auto dest = (MSCGetSlotType(djuru, 3) == MSC_TYPE_NULL) ? nullptr :(Rect*) MSCGetSlotExtern(djuru, 3);
             renderer->drawTexture(texture->handle, src, dest);
            MSCSetSlotBool(djuru, 0, true);
        })
        .end()
 
        .clazz("Surface")
        .constructor([](Djuru* djuru) {
            SDL_Surface **surface = (SDL_Surface **)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(SDL_Surface*));
            (*surface) = SDL_CreateSurface(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), SDL_PixelFormat::SDL_PIXELFORMAT_ABGR8888);
            Logger::debug("Creating surface: %p", *surface);
        })
        .finalizer([](void* data) {
                Logger::debug("Destroying Surface");
                SDL_Surface **surface = (SDL_Surface**)data;
                Logger::debug("Surface: %p", (*surface));
                SDL_DestroySurface(*surface);
                (*surface) = NULL;
                Logger::debug("Surface Destroyed");
            })
        .method(false, "width", [](Djuru* djuru) {
            MSCSetSlotDouble(djuru, 0, (*(SDL_Surface**)MSCGetSlotExtern(djuru, 0))->w);
        })
        .method(false, "height", [](Djuru* djuru) {
            MSCSetSlotDouble(djuru, 0, (*(SDL_Surface**)MSCGetSlotExtern(djuru, 0))->h);
        })
        .method(false, "pixels", [](Djuru* djuru) {
            auto surface = (*(SDL_Surface**)MSCGetSlotExtern(djuru, 0));
            MSCSetSlotBytes(djuru, 0, (const char*)surface->pixels, surface->pitch * surface->h);
        })
        .method(false, "size", [](Djuru* djuru) {
            auto surface = (*(SDL_Surface**)MSCGetSlotExtern(djuru, 0));
            MSCSetSlotDouble(djuru, 0, surface->pitch * surface->h);
        })
        .method(false, "setAt(_,_,_)", [](Djuru* djuru) {
            setPixelAt((*(SDL_Surface**)MSCGetSlotExtern(djuru, 0)), MSCGetSlotDouble(djuru, 1),MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3));
        })
        .method(false, "clear(_,_,_,_)", [](Djuru* djuru) {
             auto surface = (*(SDL_Surface**)MSCGetSlotExtern(djuru, 0));
             SDL_ClearSurface(surface, MSCGetSlotDouble(djuru, 1),MSCGetSlotDouble(djuru, 2),MSCGetSlotDouble(djuru, 3),MSCGetSlotDouble(djuru, 4));
        })
        .end()
        .clazz("FileSystemImage")
            .method(false, "load_(_)", [](Djuru* djuru) {
                MSCSetSlotHandle(djuru, 0, Ntolen::instance()->runtime()->makeHandle(djuru, 0, "ui", "Surface"));
                SDL_Surface **surface = (SDL_Surface **)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(SDL_Surface*));
                (*surface) = IMG_Load(MSCGetSlotString(djuru, 1));
            })
            
        .end()
        .clazz("Font")
            .constructor([](Djuru* djuru) {
                TTF_Init();
                TTF_Font **font = (TTF_Font **)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(TTF_Font*));
                *font = TTF_OpenFont(MSCGetSlotString(djuru, 1), MSCGetSlotDouble(djuru, 2));
            })
             .finalizer([](void* data) {
                Logger::debug("Destroying Font");
                TTF_Font **font = (TTF_Font**)data;
                TTF_CloseFont(*font);
                TTF_Quit();
            })
            .method(false, "createTexture_(_,_)", [](Djuru* djuru) {
                TTF_Font *font = *((TTF_Font **)MSCGetSlotExtern(djuru, 0));
                auto color = (Color *)MSCGetSlotExtern(djuru, 2);
                MSCSetSlotHandle(djuru, 0, Ntolen::instance()->runtime()->makeHandle(djuru, 0, "ui", "Surface"));
                SDL_Surface **surface = (SDL_Surface **)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(SDL_Surface*));
                *surface = TTF_RenderText_Solid(font, MSCGetSlotString(djuru, 1), {color->r, color->g, color->b, color->a});
            })

        .end()
        .clazz("Texture")
            .constructor([](Djuru* djuru) {
                Texture *texture = (Texture *)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(Texture));
                 auto renderer = (Renderer *)MSCGetSlotExtern(djuru, 1);
                 auto surface = (SDL_Surface **)MSCGetSlotExtern(djuru, 2);
                auto handle = renderer->texture(*surface);
                new(texture) Texture(handle, (*surface)->w, (*surface)->h);
            })
            .finalizer([](void* data) {
                Logger::debug("Destroying Texture");
                Texture *texture = (Texture*)data;
                SDL_DestroyTexture(texture ->handle);
            })
            .method(false, "width", [](Djuru* djuru) {
                 MSCSetSlotDouble(djuru, 0, ((Texture*)MSCGetSlotExtern(djuru, 0))->width);
            })
            .method(false, "height", [](Djuru* djuru) {
                 MSCSetSlotDouble(djuru, 0, ((Texture*)MSCGetSlotExtern(djuru, 0))->height);
            })
        .end()
        .clazz("Point")
        .constructor([](Djuru* djuru) {
             Point *pt = (Point *)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(Point));
             new(pt) Point(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2));
        })
        .method(false, "x", [](Djuru* djuru) {
             auto point = (Point *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, point->x);
        })
        .method(false, "y", [](Djuru* djuru) {
             auto point = (Point *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, point->y);
        })
        .end()
        .clazz("Vector")
        .constructor([](Djuru* djuru) {
             Vector *vect = (Vector *)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(Vector));
             new(vect) Vector(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3));
        })
         .method(false, "x", [](Djuru* djuru) {
             auto vect = (Vector *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, vect->x);
        })
        .method(false, "y", [](Djuru* djuru) {
            auto vect = (Vector *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, vect->y);
        })
        .method(false, "z", [](Djuru* djuru) {
            auto vect = (Vector *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, vect->z);
        })
        .end()
        
        .clazz("Rect")
        .constructor([](Djuru* djuru) {
             Rect *rect = (Rect *)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(Rect));
             new(rect) Rect(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3), MSCGetSlotDouble(djuru, 4));
        })
         .method(false, "x", [](Djuru* djuru) {
             auto rect = (Rect *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, rect->x);
        })
        .method(false, "y", [](Djuru* djuru) {
            auto rect = (Rect *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, rect->y);
        })
        .method(false, "w", [](Djuru* djuru) {
            auto rect = (Rect *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, rect->w);
        })
        .method(false, "h", [](Djuru* djuru) {
            auto rect = (Rect *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, rect->h);
        })
        .end()
        .clazz("Color")
        .constructor([](Djuru* djuru) {
             Color *color = (Color *)MSCSetSlotNewExtern(djuru, 0, 0, sizeof(Vector));
             new(color) Color(MSCGetSlotDouble(djuru, 1), MSCGetSlotDouble(djuru, 2), MSCGetSlotDouble(djuru, 3), MSCGetSlotDouble(djuru, 4));
        })
         .method(false, "r", [](Djuru* djuru) {
             auto color = (Color *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, color->r);
        })
        .method(false, "g", [](Djuru* djuru) {
            auto color = (Color *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, color->g);
        })
        .method(false, "b", [](Djuru* djuru) {
            auto color = (Color *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, color->b);
        })
        .method(false, "a", [](Djuru* djuru) {
            auto color = (Color *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, color->a);
        })
        .method(false, "value", [](Djuru* djuru) {
            auto color = (Color *)MSCGetSlotExtern(djuru, 0);
            MSCSetSlotDouble(djuru, 0, colorValueOf(color->r, color->g, color->b, color->a));
        })
        .end()
        ;
}
WindowClassProvider WindowClassProvider::provider = WindowClassProvider();

Rect Rect::none = Rect(-1,-1,-1,-1);
Point Point::none = Point(0, 0);