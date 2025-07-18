// Heavily referenced: https://youtube.com/playlist?list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&si=lpjEMfmYyjf_TN1X

#include <iostream>

#include "../include/Emu.hpp"

#define DEBUG 0x01

int main(int argc, char** argv) {
    Gba gba;
    gba.init(static_cast<bool>(DEBUG));
    std::string path = "";
    bool insert_ret = true;

    do{
        std::cout << (!insert_ret ? "Could not find ROM by that name\n" : "") << "Enter path to ROM or \"Q\" to exit: " << std::endl;
        std::cin >> path;

        if (path == "Q" || path == "q") return(0);

        insert_ret = gba.insertRom(path);
    }while(path.empty() || !insert_ret);

    const int fps = 60;
    const int frame_delay = 1000 / fps; // 1000 ms / fps = ms per frame

    Uint32 frame_start;
    int frame_time;

    Emu emu;

    emu.init("GBA Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gba, 3, false);

    while(Emu::is_running) {
        frame_start = SDL_GetTicks();

        emu.handleEvents();
        emu.update();
        emu.render();

        frame_time = SDL_GetTicks() - frame_start;

        if (frame_delay > frame_time)
            SDL_Delay(frame_delay - frame_time);

        if (DEBUG == 0x01) {
            // gba.printReg();
            unsigned char c = getchar();

            if (c == 'q' || c == 'Q')
                Emu::is_running = false;
        }
    }

    emu.cleanUp();

    return(0);
};