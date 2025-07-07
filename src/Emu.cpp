#include <iostream>

#include "../include/Emu.hpp"

void Emu::init(const char* title, int x, int y, int w, int h, bool fullscreen) {
    int flags = 0;
    
    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL sub-systems initialized" << std::endl;

        window = SDL_CreateWindow(title, x, y, w, h, flags);
        if (window) {
            std::cout << "Window created" << std::endl;
            win_w = w;
            win_h = h;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 100, 147, 237, 255);
            std::cout << "Renderer created" << std::endl;
        }

        // Initialize SDL_mixer.
        // Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

        is_running = true;
    }else
        is_running = false;

    return;
};

void Emu::handleEvents() {
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_QUIT:
            is_running = false;
            break;

        default:
            break;
    }

    return;
};

void Emu::update() {
    return;
};

void Emu::render() {
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    return;
};

void Emu::cleanUp() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    // IMG_Quit();
    // Mix_Quit();
    SDL_Quit();

    std::cout << "Game cleaned up" << std::endl;

    return;
};