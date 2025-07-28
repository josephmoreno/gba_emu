#include <catch2/catch_test_macros.hpp>
#include "../include/Gba.hpp"

// TEST_CASE("TEST", "[test]") {
//     Gba gba;
//     REQUIRE(1 == 1);
// };

TEST_CASE("GBA Initial State", "[reset, init]") {
    Gba gba;
    gba.init(false);

    for(uint8_t i = 0; i < Gba::REG_COUNT; ++i) {
        if (i != Gba::PC && i != Gba::CPSR)
            REQUIRE(Gba::reg[i] == 0);
    }
    
    for(uint32_t i = 0; i < Gba::ob_wram_size; ++i) {
        if (i < Gba::sys_rom_size) REQUIRE(Gba::sys_rom[i] == 0);
        if (i < Gba::ob_wram_size) REQUIRE(Gba::ob_wram[i] == 0);
        if (i < Gba::oc_wram_size) REQUIRE(Gba::oc_wram[i] == 0);
        if (i < Gba::io_ram_size) REQUIRE(Gba::io_ram[i] == 0);
        if (i < Gba::palette_ram_size) REQUIRE(Gba::palette_ram[i] == 0);
        if (i < Gba::vram_size) REQUIRE(Gba::vram[i] == 0);
        if (i < Gba::oam_size) REQUIRE(Gba::oam[i] == 0);
        if (i < Gba::pak_sram_size) REQUIRE(Gba::pak_sram[i] == 0);
    }

    REQUIRE(Gba::reg[Gba::PC] == Gba::pak_rom0_offset);
    REQUIRE(Gba::reg[Gba::CPSR] == 0x00000013);
};

TEST_CASE("GBA Helper Functions", "[helper functions, memory management]") {
    Gba gba;
    gba.init(false);

    // regRef; SVC mode
    REQUIRE(&Gba::regRef(0) == &Gba::reg[Gba::R0]);
    REQUIRE(&Gba::regRef(1) == &Gba::reg[Gba::R1]);
    REQUIRE(&Gba::regRef(2) == &Gba::reg[Gba::R2]);
    REQUIRE(&Gba::regRef(3) == &Gba::reg[Gba::R3]);
    REQUIRE(&Gba::regRef(4) == &Gba::reg[Gba::R4]);
    REQUIRE(&Gba::regRef(5) == &Gba::reg[Gba::R5]);
    REQUIRE(&Gba::regRef(6) == &Gba::reg[Gba::R6]);
    REQUIRE(&Gba::regRef(7) == &Gba::reg[Gba::R7]);
    REQUIRE(&Gba::regRef(8) == &Gba::reg[Gba::R8]);
    REQUIRE(&Gba::regRef(9) == &Gba::reg[Gba::R9]);
    REQUIRE(&Gba::regRef(10) == &Gba::reg[Gba::R10]);
    REQUIRE(&Gba::regRef(11) == &Gba::reg[Gba::R11]);
    REQUIRE(&Gba::regRef(12) == &Gba::reg[Gba::R12]);
    REQUIRE(&Gba::regRef(13) == &Gba::reg[Gba::R13_svc]);
    REQUIRE(&Gba::regRef(14) == &Gba::reg[Gba::R14_svc]);
    REQUIRE(&Gba::regRef(15) == &Gba::reg[Gba::PC]);

    // regRef; USR mode
    Gba::reg[Gba::CPSR] = (Gba::reg[Gba::CPSR] & 0xffffffe0) | static_cast<uint32_t>(Gba::USR);

    REQUIRE(&Gba::regRef(0) == &Gba::reg[Gba::R0]);
    REQUIRE(&Gba::regRef(1) == &Gba::reg[Gba::R1]);
    REQUIRE(&Gba::regRef(2) == &Gba::reg[Gba::R2]);
    REQUIRE(&Gba::regRef(3) == &Gba::reg[Gba::R3]);
    REQUIRE(&Gba::regRef(4) == &Gba::reg[Gba::R4]);
    REQUIRE(&Gba::regRef(5) == &Gba::reg[Gba::R5]);
    REQUIRE(&Gba::regRef(6) == &Gba::reg[Gba::R6]);
    REQUIRE(&Gba::regRef(7) == &Gba::reg[Gba::R7]);
    REQUIRE(&Gba::regRef(8) == &Gba::reg[Gba::R8]);
    REQUIRE(&Gba::regRef(9) == &Gba::reg[Gba::R9]);
    REQUIRE(&Gba::regRef(10) == &Gba::reg[Gba::R10]);
    REQUIRE(&Gba::regRef(11) == &Gba::reg[Gba::R11]);
    REQUIRE(&Gba::regRef(12) == &Gba::reg[Gba::R12]);
    REQUIRE(&Gba::regRef(13) == &Gba::reg[Gba::SP]);
    REQUIRE(&Gba::regRef(14) == &Gba::reg[Gba::LR]);
    REQUIRE(&Gba::regRef(15) == &Gba::reg[Gba::PC]);

    // regRef; FIQ mode
    Gba::reg[Gba::CPSR] = (Gba::reg[Gba::CPSR] & 0xffffffe0) | static_cast<uint32_t>(Gba::FIQ);

    REQUIRE(&Gba::regRef(0) == &Gba::reg[Gba::R0]);
    REQUIRE(&Gba::regRef(1) == &Gba::reg[Gba::R1]);
    REQUIRE(&Gba::regRef(2) == &Gba::reg[Gba::R2]);
    REQUIRE(&Gba::regRef(3) == &Gba::reg[Gba::R3]);
    REQUIRE(&Gba::regRef(4) == &Gba::reg[Gba::R4]);
    REQUIRE(&Gba::regRef(5) == &Gba::reg[Gba::R5]);
    REQUIRE(&Gba::regRef(6) == &Gba::reg[Gba::R6]);
    REQUIRE(&Gba::regRef(7) == &Gba::reg[Gba::R7]);
    REQUIRE(&Gba::regRef(8) == &Gba::reg[Gba::R8_fiq]);
    REQUIRE(&Gba::regRef(9) == &Gba::reg[Gba::R9_fiq]);
    REQUIRE(&Gba::regRef(10) == &Gba::reg[Gba::R10_fiq]);
    REQUIRE(&Gba::regRef(11) == &Gba::reg[Gba::R11_fiq]);
    REQUIRE(&Gba::regRef(12) == &Gba::reg[Gba::R12_fiq]);
    REQUIRE(&Gba::regRef(13) == &Gba::reg[Gba::R13_fiq]);
    REQUIRE(&Gba::regRef(14) == &Gba::reg[Gba::R14_fiq]);
    REQUIRE(&Gba::regRef(15) == &Gba::reg[Gba::PC]);

    // regRef; IRQ mode
    Gba::reg[Gba::CPSR] = (Gba::reg[Gba::CPSR] & 0xffffffe0) | static_cast<uint32_t>(Gba::IRQ);

    REQUIRE(&Gba::regRef(0) == &Gba::reg[Gba::R0]);
    REQUIRE(&Gba::regRef(1) == &Gba::reg[Gba::R1]);
    REQUIRE(&Gba::regRef(2) == &Gba::reg[Gba::R2]);
    REQUIRE(&Gba::regRef(3) == &Gba::reg[Gba::R3]);
    REQUIRE(&Gba::regRef(4) == &Gba::reg[Gba::R4]);
    REQUIRE(&Gba::regRef(5) == &Gba::reg[Gba::R5]);
    REQUIRE(&Gba::regRef(6) == &Gba::reg[Gba::R6]);
    REQUIRE(&Gba::regRef(7) == &Gba::reg[Gba::R7]);
    REQUIRE(&Gba::regRef(8) == &Gba::reg[Gba::R8]);
    REQUIRE(&Gba::regRef(9) == &Gba::reg[Gba::R9]);
    REQUIRE(&Gba::regRef(10) == &Gba::reg[Gba::R10]);
    REQUIRE(&Gba::regRef(11) == &Gba::reg[Gba::R11]);
    REQUIRE(&Gba::regRef(12) == &Gba::reg[Gba::R12]);
    REQUIRE(&Gba::regRef(13) == &Gba::reg[Gba::R13_irq]);
    REQUIRE(&Gba::regRef(14) == &Gba::reg[Gba::R14_irq]);
    REQUIRE(&Gba::regRef(15) == &Gba::reg[Gba::PC]);

    // regRef; ABT mode
    Gba::reg[Gba::CPSR] = (Gba::reg[Gba::CPSR] & 0xffffffe0) | static_cast<uint32_t>(Gba::ABT);

    REQUIRE(&Gba::regRef(0) == &Gba::reg[Gba::R0]);
    REQUIRE(&Gba::regRef(1) == &Gba::reg[Gba::R1]);
    REQUIRE(&Gba::regRef(2) == &Gba::reg[Gba::R2]);
    REQUIRE(&Gba::regRef(3) == &Gba::reg[Gba::R3]);
    REQUIRE(&Gba::regRef(4) == &Gba::reg[Gba::R4]);
    REQUIRE(&Gba::regRef(5) == &Gba::reg[Gba::R5]);
    REQUIRE(&Gba::regRef(6) == &Gba::reg[Gba::R6]);
    REQUIRE(&Gba::regRef(7) == &Gba::reg[Gba::R7]);
    REQUIRE(&Gba::regRef(8) == &Gba::reg[Gba::R8]);
    REQUIRE(&Gba::regRef(9) == &Gba::reg[Gba::R9]);
    REQUIRE(&Gba::regRef(10) == &Gba::reg[Gba::R10]);
    REQUIRE(&Gba::regRef(11) == &Gba::reg[Gba::R11]);
    REQUIRE(&Gba::regRef(12) == &Gba::reg[Gba::R12]);
    REQUIRE(&Gba::regRef(13) == &Gba::reg[Gba::R13_abt]);
    REQUIRE(&Gba::regRef(14) == &Gba::reg[Gba::R14_abt]);
    REQUIRE(&Gba::regRef(15) == &Gba::reg[Gba::PC]);

    // regRef; UND mode
    Gba::reg[Gba::CPSR] = (Gba::reg[Gba::CPSR] & 0xffffffe0) | static_cast<uint32_t>(Gba::UND);

    REQUIRE(&Gba::regRef(0) == &Gba::reg[Gba::R0]);
    REQUIRE(&Gba::regRef(1) == &Gba::reg[Gba::R1]);
    REQUIRE(&Gba::regRef(2) == &Gba::reg[Gba::R2]);
    REQUIRE(&Gba::regRef(3) == &Gba::reg[Gba::R3]);
    REQUIRE(&Gba::regRef(4) == &Gba::reg[Gba::R4]);
    REQUIRE(&Gba::regRef(5) == &Gba::reg[Gba::R5]);
    REQUIRE(&Gba::regRef(6) == &Gba::reg[Gba::R6]);
    REQUIRE(&Gba::regRef(7) == &Gba::reg[Gba::R7]);
    REQUIRE(&Gba::regRef(8) == &Gba::reg[Gba::R8]);
    REQUIRE(&Gba::regRef(9) == &Gba::reg[Gba::R9]);
    REQUIRE(&Gba::regRef(10) == &Gba::reg[Gba::R10]);
    REQUIRE(&Gba::regRef(11) == &Gba::reg[Gba::R11]);
    REQUIRE(&Gba::regRef(12) == &Gba::reg[Gba::R12]);
    REQUIRE(&Gba::regRef(13) == &Gba::reg[Gba::R13_und]);
    REQUIRE(&Gba::regRef(14) == &Gba::reg[Gba::R14_und]);
    REQUIRE(&Gba::regRef(15) == &Gba::reg[Gba::PC]);

    // regRef; SYS mode
    Gba::reg[Gba::CPSR] = (Gba::reg[Gba::CPSR] & 0xffffffe0) | static_cast<uint32_t>(Gba::SYS);

    REQUIRE(&Gba::regRef(0) == &Gba::reg[Gba::R0]);
    REQUIRE(&Gba::regRef(1) == &Gba::reg[Gba::R1]);
    REQUIRE(&Gba::regRef(2) == &Gba::reg[Gba::R2]);
    REQUIRE(&Gba::regRef(3) == &Gba::reg[Gba::R3]);
    REQUIRE(&Gba::regRef(4) == &Gba::reg[Gba::R4]);
    REQUIRE(&Gba::regRef(5) == &Gba::reg[Gba::R5]);
    REQUIRE(&Gba::regRef(6) == &Gba::reg[Gba::R6]);
    REQUIRE(&Gba::regRef(7) == &Gba::reg[Gba::R7]);
    REQUIRE(&Gba::regRef(8) == &Gba::reg[Gba::R8]);
    REQUIRE(&Gba::regRef(9) == &Gba::reg[Gba::R9]);
    REQUIRE(&Gba::regRef(10) == &Gba::reg[Gba::R10]);
    REQUIRE(&Gba::regRef(11) == &Gba::reg[Gba::R11]);
    REQUIRE(&Gba::regRef(12) == &Gba::reg[Gba::R12]);
    REQUIRE(&Gba::regRef(13) == &Gba::reg[Gba::SP]);
    REQUIRE(&Gba::regRef(14) == &Gba::reg[Gba::LR]);
    REQUIRE(&Gba::regRef(15) == &Gba::reg[Gba::PC]);

    // regRef; PSR registers
    REQUIRE(&Gba::regRef(Gba::CPSR) == &Gba::reg[Gba::CPSR]);
    REQUIRE(&Gba::regRef(Gba::SPSR_fiq) == &Gba::reg[Gba::SPSR_fiq]);
    REQUIRE(&Gba::regRef(Gba::SPSR_svc) == &Gba::reg[Gba::SPSR_svc]);
    REQUIRE(&Gba::regRef(Gba::SPSR_abt) == &Gba::reg[Gba::SPSR_abt]);
    REQUIRE(&Gba::regRef(Gba::SPSR_und) == &Gba::reg[Gba::SPSR_und]);

    // memRef
    REQUIRE(&Gba::memRef(Gba::sys_rom_offset) == &Gba::sys_rom[0]);
    REQUIRE(&Gba::memRef(Gba::ob_wram_offset) == &Gba::ob_wram[0]);
    REQUIRE(&Gba::memRef(Gba::oc_wram_offset) == &Gba::oc_wram[0]);
    REQUIRE(&Gba::memRef(Gba::io_ram_offset) == &Gba::io_ram[0]);
    REQUIRE(&Gba::memRef(Gba::palette_ram_offset) == &Gba::palette_ram[0]);
    REQUIRE(&Gba::memRef(Gba::vram_offset) == &Gba::vram[0]);
    REQUIRE(&Gba::memRef(Gba::oam_offset) == &Gba::oam[0]);
    REQUIRE(&Gba::memRef(Gba::pak_sram_offset) == &Gba::pak_sram[0]);
};