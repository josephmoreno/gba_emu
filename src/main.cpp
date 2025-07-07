// Heavily referenced: https://youtube.com/playlist?list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&si=lpjEMfmYyjf_TN1X

#include "../include/Emu.hpp"
#include "../include/Gba.hpp"

int main(int argc, char** argv) {
    const int fps = 60;
    const int frame_delay = 1000 / fps; // 1000 ms / fps = ms per frame

    Uint32 frame_start;
    int frame_time;

    Emu emu;
    Gba gba;
    gba.disp_scale = 2;

    emu.init("GBA Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gba.disp_w * gba.disp_scale, gba.disp_h * gba.disp_scale, false);

    while(Emu::is_running) {
        frame_start = SDL_GetTicks();

        emu.handleEvents();
        emu.update();
        emu.render();

        frame_time = SDL_GetTicks() - frame_start;

        if (frame_delay > frame_time)
            SDL_Delay(frame_delay - frame_time);
    }

    emu.cleanUp();

    return(0);
};