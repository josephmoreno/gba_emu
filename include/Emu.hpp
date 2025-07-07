#pragma once

#include <SDL2/SDL.h>

// Only static data members and methods because not currently designed for multiple Game instances

class Emu {
private:
    static SDL_Window* window;

public:
    static int win_w, win_h;
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static bool is_running;
    // static AssetManager assets;

    static void init(const char* title, int x, int y, int w, int h, bool fullscreen);
    static void handleEvents();
    static void update();
    static void render();
    static void cleanUp();
};