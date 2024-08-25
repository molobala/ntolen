#ifndef WINDOW_H
#define WINDOW_H
#include "../Mosc.h"
#include "../UIComponent.h"

#include <SDL3/SDL.h>
#pragma once

namespace ntolen
{
    class WindowClassProvider: public ClassProvider {
        public:
        static WindowClassProvider provider;
        const char* className() const {
            return "Window";
        }
        void provide(ClassBuilder& builder);
    };
    class Window: public UIComponent
    {
    public:
        Window(const char *t, int w, int h);
        ~Window();
        void show(bool y);
        //static ModuleBuilder &install(ModuleBuilder &builder);
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
    };
    class Color {
        public:
        Color() {}
        Color(Uint8 pr, Uint8 pg, Uint8 pb, Uint8 pa): r(pr), b(pb), g(pg), a(pa) {}
        Uint8 r, g, b, a;
    };
    class Renderer
    {
    public:
        Renderer(SDL_Renderer *renderer);
        ~Renderer();
        void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
        {
            SDL_SetRenderDrawColor(_renderer, r, b, g, a);
        }
        Color getDrawColor() {
            Color color;
            SDL_GetRenderDrawColor(_renderer, &(color.r), &(color.g), &(color.b), &(color.a));
            return std::move(color);
        }
        void clear()
        {
            SDL_RenderClear(_renderer);
        }
        void fillRect(double x, double y, double w, double h)
        {
            SDL_FRect rect = {(float)x, (float)y, (float)w, (float)h};
            SDL_RenderFillRect(_renderer, &rect);
        }
        void drawRect(double x, double y, double w, double h)
        {
            SDL_FRect rect = {(float)x, (float)y, (float)w, (float)h};
            SDL_RenderRect(_renderer, &rect);
        }
        void drawLine(double x1, double y1, double x2, double y2)
        {
            SDL_RenderLine(_renderer, x1, y1, x2, y2);
        }
        void drawPoint(double x, double y)
        {
            SDL_RenderPoint(_renderer, x, y);
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
        void complete()
        {
            SDL_RenderPresent(_renderer);
        }

       
    private:
        SDL_Renderer *_renderer;
    };
}

#endif