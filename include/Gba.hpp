#pragma once

// https://www.reddit.com/r/EmuDev/comments/cfy60z/resources_to_build_a_gba_emulator/

#include <cstdint>

struct Gba {
    // Display resolution is 240 (w) x 160 (h)
    static constexpr uint32_t disp_w = 240;
    static constexpr uint32_t disp_h = 160;
    int disp_scale = 1;
    uint16_t display[disp_w * disp_h];  // 15-bit colors

    // Internal memory
    uint8_t sys_rom[16383];         // 0x0000_0000 - 0x0000_3fff, 16kB
    uint8_t ob_wram[262143];        // 0x0200_0000 - 0x0203_ffff, 256kB, "on-board work RAM"
    uint8_t oc_wram[32767];         // 0x0300_0000 - 0x0300_7fff, 32kB, "on-chip work RAM"
    uint8_t io_ram[1022];           // 0x0400_0000 - 0x0400_03fe, 1kB
    uint8_t palette_ram[1023];      // 0x0500_0000 - 0x0500_03ff, 1kB
    uint8_t vram[98303];            // 0x0600_0000 - 0x0601_7fff, 96kB, "video RAM"
    uint8_t oam[1023];              // 0x0700_0000 - 0x0700_03ff, 1kB, "object attributes memory"

    // External memory
    uint8_t pak_rom[3][33554431];   // 0x0800_0000 - 0x09ff_ffff, 0x0a00_0000 - 0x0bff_ffff, 0x0c00_0000 - 0x0dff_ffff, 32MB each, game pak ROM
    uint8_t pak_sram[65535];        // 0x0e00_0000 - 0x0e00_ffff, 64kB, for saved data

    // ARM CPU Registers
    enum Reg : uint8_t {
        R0, R1, R2, R3, R4, R5, R6, R7,
        R8, R8_fiq,
        R9, R9_fiq,
        R10, R10_fiq,
        R11, R11_fiq,
        R12, R12_fiq,
        SP, R13_fiq, R13_svc, R13_abt, R13_irq, R13_und,
        LR, R14_fiq, R14_svc, R14_abt, R14_irq, R14_und,
        PC,
        CPSR,
        SPSR_fiq, SPSR_svc, SPSR_abt, SPSR_irq, SPSR_und,

        REG_COUNT
    };

    uint32_t reg[Reg::REG_COUNT];
};