#ifndef WINDOW_H
#define WINDOW_H
#include "../Mosc.h"
#include "../UIComponent.h"

#include <SDL3/SDL.h>
#pragma once

namespace ntolen
{
    class WindowClassProvider : public ClassProvider
    {
    public:
        static WindowClassProvider provider;
        const char *className() const
        {
            return "Window";
        }
        void provide(ClassBuilder &builder);
    };
    class Window : public UIComponent
    {
    public:
        Window(const char *t, int w, int h);
        ~Window();
        void show(bool y);
        // static ModuleBuilder &install(ModuleBuilder &builder);
        MVM *vm;
        int width() const
        {
            return _width;
        }
        int height() const
        {
            return _height;
        }
        SDL_Window *window() const
        {
            return _window;
        }
        SDL_Renderer *renderer() const
        {
            return _renderer;
        }
        const char *title() const
        {
            return _title;
        }
        virtual void render(float fps);
        MSCHandle *updateFn;

    private:
        SDL_Window *_window;
        SDL_Renderer *_renderer;
        int _width;
        int _height;
        const char *_title;
    };
    class Point
    {
    public:
        Point(double px, double py) : x(px), y(py) {}
        double x, y;
        bool operator=(const Point &other) const
        {
            return other.x == x && other.y == y;
        }
        static Point none;
    };
    class Vector
    {
    public:
        Vector(double px, double py, double pz) : x(px), y(py), z(pz) {}
        double x, y, z;
        bool operator=(const Vector &other) const
        {
            return other.x == x && other.y == y && other.z == z;
        }
    };
    class Rect
    {
    public:
        Rect(double px, double py, double pw, double ph) : x(px), y(py), w(pw), h(ph) {}
        double x, y, w, h;
        bool operator=(const Rect &other) const
        {
            return other.x == x && other.y == y && other.w == w && other.h == h;
        }
        static Rect none;
    };

    class Color
    {
    public:
        Color() {}
        Color(Uint8 pr, Uint8 pg, Uint8 pb, Uint8 pa) : r(pr), b(pb), g(pg), a(pa) {}
        Uint8 r, g, b, a;
    };
    class Texture {
        public:
        Texture(SDL_Texture* phandle, int w, int h): handle(phandle), width(w), height(h) {}
        SDL_Texture* handle;
        int width;
        int height;
    };
    class Renderer
    {
    public:
        Renderer(SDL_Renderer *renderer);
        ~Renderer();
        void setDrawColor(Color color)
        {
            SDL_SetRenderDrawColor(_renderer, color.r, color.b, color.g, color.a);
        }
        Color getDrawColor()
        {
            Color color;
            SDL_GetRenderDrawColor(_renderer, &(color.r), &(color.g), &(color.b), &(color.a));
            return std::move(color);
        }
        void clear()
        {
            SDL_RenderClear(_renderer);
        }
        void fillRect(Rect dest)
        {
            SDL_FRect rect = {(float)dest.x, (float)dest.y, (float)dest.w, (float)dest.h};
            SDL_RenderFillRect(_renderer, &rect);
        }
        void drawRect(Rect dest)
        {
            SDL_FRect rect = {(float)dest.x, (float)dest.y, (float)dest.w, (float)dest.h};
            SDL_RenderRect(_renderer, &rect);
        }
        void drawLine(Point f, Point t)
        {
            SDL_RenderLine(_renderer, f.x, f.y, t.x, t.y);
        }
        void drawPoint(Point p)
        {
            SDL_RenderPoint(_renderer, p.x, p.y);
        }
        void drawLines(std::vector<Point> &points)
        {
            int count = points.size();
            if (count == 0)
            {
                return;
            }
            SDL_FPoint sdlPoints[count];
            for (auto i = 0; i < count; i++)
            {
                sdlPoints[i].x = points[i].x;
                sdlPoints[i].x = points[i].y;
            }
            SDL_RenderLines(_renderer, &sdlPoints[0], count);
        }
        void commit()
        {
            SDL_RenderPresent(_renderer);
        }

        void drawTexture(SDL_Texture *texture)
        {
            drawTexture_(texture, nullptr, nullptr);
        }
        void drawTexture(SDL_Texture *texture, Rect src)
        {
            SDL_FRect rect = {(float)src.x, (float)src.y, (float)src.w, (float)src.h};
            drawTexture_(texture, &rect, nullptr);
        }
        void drawTexture(SDL_Texture *texture, Rect *src, Rect *dest)
        {
            SDL_FRect rect = {};
            if(src != nullptr) {
                rect = {(float)src->x, (float)src->y, (float)src->w, (float)src->h};
            }
            SDL_FRect disp = {};
            if(dest != nullptr) {
             disp = {(float)dest->x, (float)dest->y, (float)dest->w, (float)dest->h};
            }
            drawTexture_(texture, src == nullptr ? NULL : &rect, dest == nullptr ? NULL : &disp);
        }

        SDL_Texture* texture(SDL_Surface* surface) {
            return SDL_CreateTextureFromSurface(_renderer, surface);
        }

    private:
        void drawTexture_(SDL_Texture *texture, SDL_FRect *src, SDL_FRect *dest)
        {
            SDL_RenderTexture(_renderer, texture, src, dest);
        }
        SDL_Renderer *_renderer;
    };
}

#endif