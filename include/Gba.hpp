#pragma once

// https://www.reddit.com/r/EmuDev/comments/cfy60z/resources_to_build_a_gba_emulator/
// *** ARM7TDMI Datasheet: https://www.gbadev.org/docs.php OR https://www.gbadev.org/docs.php?showinfo=17

#include <cstdint>
#include <string>
#include <iostream>

struct Gba {
    // Display resolution is 240 (w) x 160 (h)
    static constexpr uint8_t disp_w = 240;
    static constexpr uint8_t disp_h = 160;
    static int disp_scale;
    static uint16_t display[];  // 15-bit colors

    // Internal memory
    static constexpr uint32_t sys_rom_offset = 0;
    static constexpr uint32_t sys_rom_size = 16384;
    static constexpr uint32_t ob_wram_offset = 0x02000000;
    static constexpr uint32_t ob_wram_size = 262144;
    static constexpr uint32_t oc_wram_offset = 0x03000000;
    static constexpr uint32_t oc_wram_size = 32768;
    static constexpr uint32_t io_ram_offset = 0x04000000;
    static constexpr uint32_t io_ram_size = 1023;
    static constexpr uint32_t palette_ram_offset = 0x05000000;
    static constexpr uint32_t palette_ram_size = 1024;
    static constexpr uint32_t vram_offset = 0x06000000;
    static constexpr uint32_t vram_size = 98304;
    static constexpr uint32_t oam_offset = 0x07000000;
    static constexpr uint32_t oam_size = 1024;

    static uint8_t sys_rom[];       // 0x0000_0000 - 0x0000_3fff, 16kB
    static uint8_t ob_wram[];       // 0x0200_0000 - 0x0203_ffff, 256kB, "on-board work RAM"
    static uint8_t oc_wram[];       // 0x0300_0000 - 0x0300_7fff, 32kB, "on-chip work RAM"
    static uint8_t io_ram[];        // 0x0400_0000 - 0x0400_03fe, 1kB
    static uint8_t palette_ram[];   // 0x0500_0000 - 0x0500_03ff, 1kB
    static uint8_t vram[];          // 0x0600_0000 - 0x0601_7fff, 96kB, "video RAM"
    static uint8_t oam[];           // 0x0700_0000 - 0x0700_03ff, 1kB, "object attributes memory"

    // External memory
    static constexpr uint32_t pak_rom0_offset = 0x08000000;
    static constexpr uint32_t pak_rom1_offset = 0x0a000000;
    static constexpr uint32_t pak_rom2_offset = 0x0c000000;
    static uint32_t pak_size;
    static constexpr uint32_t pak_sram_offset = 0x0e000000;
    static constexpr uint32_t pak_sram_size = 65536;

    static uint8_t* pak_rom[];  // 0x0800_0000 - 0x09ff_ffff, 0x0a00_0000 - 0x0bff_ffff, 0x0c00_0000 - 0x0dff_ffff, 32MB each, game pak ROM; going to dynamically allocate because of large max size and pak size varies
    static uint8_t pak_sram[];  // 0x0e00_0000 - 0x0e00_ffff, 64kB, for saved data

    // ARM CPU Registers
    // CPSR = [N, Z, C, V, -, -, ... , -, I, F, T, M4, M3, M2, M1, M0] * See 3.8: Program Status Registers
    // I = IRQ Disable, F = FIQ Disable, T = (0 = ARM, 1 = THUMB)
    enum Reg : uint8_t {
        R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, SP, LR, PC, 
        R8_fiq, R9_fiq, R10_fiq, R11_fiq, R12_fiq, R13_fiq, R14_fiq,
        R13_svc, R14_svc,
        R13_abt, R14_abt,
        R13_irq, R14_irq,
        R13_und, R14_und,
        
        CPSR,
        SPSR_fiq, SPSR_svc, SPSR_abt, SPSR_irq, SPSR_und,

        REG_COUNT
    };

    // From CPSR's mode bits. * See 3.8.2: The control bits
    enum OpMode : uint8_t {
        USR = 0x10, // "User"; normal ARM execution mode
        FIQ = 0x11, // For data transfer or channel process
        IRQ = 0x12, // General-purpose interrupt handling
        SVC = 0x13, // "Supervisor"; protected mode for operating system
        ABT = 0x17, // "Abort"; entered after a data instruction or prefetch abort
        UND = 0x1b, // "Undefined"; entered when undefined instruction is executed
        SYS = 0x1f, // "System"; privileged user mode for the operating system

        OPMODE_COUNT
    };

    static uint32_t reg[];

    static void init(bool init);
    static bool insertRom(std::string path);
    static uint32_t armFetch();
    static uint16_t thumbFetch();
    static void armDecode(uint32_t w);
    static void thumbDecode(uint16_t half_w);
    static void cycle();

    // Helper Functions
    static uint32_t& regRef(uint8_t reg_num);
    static uint8_t& memRef(uint32_t addr);
    static uint32_t armDpOp2(uint32_t w);
    static void shiftOp(bool set_cond, uint8_t shift, uint32_t& op);
    static void armLogSetCond(uint32_t res, uint8_t rd);
    static void armAriSetCond(uint32_t res, bool c_flag, bool v_flag, uint8_t rd);
    static void armPsrTransfer(uint32_t w);

    // ARM Instructions
    static void armAdc(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd);
    static void armAdd(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd);
    static void armAnd(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd);
    static void armAsr(bool set_cond, uint8_t shift_am, uint32_t& op);
    static void armB(uint32_t offset);
    static void armBic(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd);
    static void armBl(uint32_t offset);
    static void armBx(uint8_t rn);
    static void armCmn(uint32_t op1, uint32_t op2);
    static void armCmp(uint32_t op1, uint32_t op2);
    static void armEor(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd);
    static void armLdr(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset);
    static void armLdrb(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset);
    static void armLsl(bool set_cond, uint8_t shift_am, uint32_t& op);
    static void armLsr(bool set_cond, uint8_t shift_am, uint32_t& op);
    static void armMla(bool set_cond, uint32_t op1, uint32_t op2, uint32_t acc, uint8_t rd);
    static void armMlal(bool set_cond, bool sign, uint32_t op1, uint32_t op2, uint8_t rd_h, uint8_t rd_l);
    static void armMov(bool set_cond, uint32_t op2, uint8_t rd);
    static void armMul(bool set_cond, uint32_t op1, uint32_t op2, uint32_t& acc, uint8_t rd);
    static void armMull(bool set_cond, bool sign, uint32_t op1, uint32_t op2, uint8_t rd_h, uint8_t rd_l);
    static void armMrs(uint8_t psr_num, uint8_t rd);
    static void armMsr(uint8_t psr_num, uint32_t w);
    static void armMvn(bool set_cond, uint32_t op2, uint8_t rd);
    static void armOrr(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd);
    static void armRor(bool set_cond, uint8_t shift_am, uint32_t& op);
    static void armRsb(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd);
    static void armRsc(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd);
    static void armSbc(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd);
    static void armStr();
    static void armStrb();
    static void armSub(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd);
    static void armTeq(uint32_t op1, uint32_t op2);
    static void armTst(uint32_t op1, uint32_t op2);

    // For debugging
    static bool debug;
    static void printDisp();
    static void printReg();
    static void printAll();

    ~Gba();
};