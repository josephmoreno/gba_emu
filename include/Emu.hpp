#pragma once

#include <SDL2/SDL.h>

#include "Gba.hpp"

// Only static data members and methods because not currently designed for multiple Emu instances

class Emu {
private:
    static Gba* gba;
    static SDL_Window* window;

public:
    static int win_w, win_h, win_scale;
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static bool is_running;
    // static AssetManager assets;

    static void init(const char* title, int x, int y, Gba& gba, int scale, bool fullscreen);
    static void handleEvents();
    static void update();
    static void render();
    static void cleanUp();
};