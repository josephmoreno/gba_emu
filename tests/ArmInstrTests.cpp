#include <catch2/catch_test_macros.hpp>
#include "../include/Gba.hpp"

TEST_CASE("ARM Instructions", "[ARM, processor, instruction set]") {
    Gba gba;
    gba.init(false);
    bool set_cond = false;

    SECTION("ADC, set_cond = false") {
        // set_cond = false, set N
        gba.armAdc(set_cond, 0x80000000, 0, 0);
        REQUIRE(Gba::regRef(Gba::R0) == 0x80000000);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        gba.armAdc(set_cond, 0, 0, 1);
        REQUIRE(Gba::regRef(Gba::R1) == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set C
        gba.armAdc(set_cond, 0xffffffff, 0x00000002, 2);
        REQUIRE(Gba::regRef(Gba::R2) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set V, set C
        gba.armAdc(set_cond, 0x80000000, 0x80000000, 3);
        REQUIRE(Gba::regRef(Gba::R3) == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set V, set N
        gba.armAdc(set_cond, 0x7fffffff, 0, 3);
        REQUIRE(Gba::regRef(Gba::R3) == 0x7fffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("ADC, set_cond = true") {
        set_cond = true;

        // set N
        gba.armAdc(set_cond, 0x80000000, 0, 0);
        REQUIRE(Gba::regRef(Gba::R0) == 0x80000000);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armAdc(set_cond, 0, 0, 1);
        REQUIRE(Gba::regRef(Gba::R1) == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set C
        gba.armAdc(set_cond, 0xffffffff, 0x00000002, 2);
        REQUIRE(Gba::regRef(Gba::R2) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set V, set C
        gba.armAdc(set_cond, 0x80000000, 0x80000000, 3);
        REQUIRE(Gba::regRef(Gba::R3) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(Gba::vFlag());

        // set V, set N
        gba.armAdc(set_cond, 0x7fffffff, 0, 3);
        REQUIRE(Gba::regRef(Gba::R3) == 0x80000000);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(Gba::vFlag());
    }

    SECTION("ADD, set_cond = false") {
        // set_cond = false, set N
        gba.armAdd(set_cond, 0x80000000, 0, 0);
        REQUIRE(Gba::regRef(Gba::R0) == 0x80000000);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        gba.armAdd(set_cond, 0, 0, 1);
        REQUIRE(Gba::regRef(Gba::R1) == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set C
        gba.armAdd(set_cond, 0xffffffff, 0x00000002, 2);
        REQUIRE(Gba::regRef(Gba::R2) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set V, set C
        gba.armAdd(set_cond, 0x80000000, 0x80000001, 3);
        REQUIRE(Gba::regRef(Gba::R3) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set V, set N
        gba.armAdd(set_cond, 0x7fffffff, 0x00000001, 3);
        REQUIRE(Gba::regRef(Gba::R3) == 0x80000000);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("ADD, set_cond = true") {
        set_cond = true;

        // set N
        gba.armAdd(set_cond, 0x80000000, 0, 0);
        REQUIRE(Gba::regRef(Gba::R0) == 0x80000000);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armAdd(set_cond, 0, 0, 1);
        REQUIRE(Gba::regRef(Gba::R1) == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set C
        gba.armAdd(set_cond, 0xffffffff, 0x00000002, 2);
        REQUIRE(Gba::regRef(Gba::R2) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set V, set C
        gba.armAdd(set_cond, 0x80000000, 0x80000001, 3);
        REQUIRE(Gba::regRef(Gba::R3) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(Gba::vFlag());

        // set V, set N
        gba.armAdd(set_cond, 0x7fffffff, 0x00000001, 3);
        REQUIRE(Gba::regRef(Gba::R3) == 0x80000000);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(Gba::vFlag());
    }

    SECTION("AND, set_cond = false") {
        // set_cond = false, no flags set
        gba.armAnd(set_cond, 0xffffffff, 0x55555555, 0);
        REQUIRE(Gba::regRef(Gba::R0) == 0x55555555);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        gba.armAnd(set_cond, 0xffffffff, 0, 1);
        REQUIRE(Gba::regRef(Gba::R1) == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set N
        gba.armAnd(set_cond, 0xffffffff, 0xaaaaaaaa, 2);
        REQUIRE(Gba::regRef(Gba::R2) == 0xaaaaaaaa);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("AND, set_cond = true") {
        set_cond = true;

        // no flags set
        gba.armAnd(set_cond, 0xffffffff, 0x55555555, 0);
        REQUIRE(Gba::regRef(Gba::R0) == 0x55555555);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armAnd(set_cond, 0xffffffff, 0, 1);
        REQUIRE(Gba::regRef(Gba::R1) == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armAnd(set_cond, 0xffffffff, 0xaaaaaaaa, 2);
        REQUIRE(Gba::regRef(Gba::R2) == 0xaaaaaaaa);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("ASR, set_cond = false") {
        uint32_t op;

        // set_cond = false, ASR #0
        op = 0x7fffffff;
        gba.armAsr(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, ASR #0, set C
        op = 0x80000000;
        gba.armAsr(set_cond, 0, op);
        REQUIRE(op == 0xffffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, ASR #30
        op = 0x40000000;
        gba.armAsr(set_cond, 30, op);
        REQUIRE(op == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, ASR #31, set C
        op = 0x40000000;
        gba.armAsr(set_cond, 31, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, ASR #30, set C
        op = 0xbfffffff;
        gba.armAsr(set_cond, 30, op);
        REQUIRE(op == 0xfffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, ASR #31
        op = 0xbfffffff;
        gba.armAsr(set_cond, 31, op);
        REQUIRE(op == 0xffffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("ASR, set_cond = true") {
        set_cond = true;
        uint32_t op;

        // ASR #0, no flags set
        op = 0x7fffffff;
        gba.armAsr(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // ASR #0, set C
        op = 0x80000000;
        gba.armAsr(set_cond, 0, op);
        REQUIRE(op == 0xffffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // ASR #30, no flags set
        op = 0x40000000;
        gba.armAsr(set_cond, 30, op);
        REQUIRE(op == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // ASR #31, set C
        op = 0x40000000;
        gba.armAsr(set_cond, 31, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // ASR #30, set C
        op = 0xbfffffff;
        gba.armAsr(set_cond, 30, op);
        REQUIRE(op == 0xfffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // ASR #31, no flags set
        op = 0xbfffffff;
        gba.armAsr(set_cond, 31, op);
        REQUIRE(op == 0xffffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("B") {
        gba.armB(4);
        REQUIRE(Gba::reg[Gba::PC] == Gba::pak_rom0_offset + 4);

        gba.armB(4);
        REQUIRE(Gba::reg[Gba::PC] == Gba::pak_rom0_offset + 8);

        gba.armB(4);
        REQUIRE(Gba::reg[Gba::PC] == Gba::pak_rom0_offset + 12);

        gba.armB(4);
        REQUIRE(Gba::reg[Gba::PC] == Gba::pak_rom0_offset + 16);
    }

    SECTION("BIC, set_cond = false") {
        // set_cond = false, no flags set
        gba.armBic(set_cond, 0xffffffff, 0xfffffffe, 0);
        REQUIRE(Gba::regRef(Gba::R0) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, no flags set
        gba.armBic(set_cond, 1, 0xfffffffe, 0);
        REQUIRE(Gba::regRef(Gba::R0) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        gba.armBic(set_cond, 0xffffffff, 0xffffffff, 1);
        REQUIRE(Gba::regRef(Gba::R1) == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set N
        gba.armBic(set_cond, 0xffffffff, 0x7fffffff, 2);
        REQUIRE(Gba::regRef(Gba::R2) == 0x80000000);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("BIC, set_cond = true") {
        set_cond = true;

        // no flags set
        gba.armBic(set_cond, 0xffffffff, 0xfffffffe, 0);
        REQUIRE(Gba::regRef(Gba::R0) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // no flags set
        gba.armBic(set_cond, 1, 0xfffffffe, 0);
        REQUIRE(Gba::regRef(Gba::R0) == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armBic(set_cond, 0xffffffff, 0xffffffff, 1);
        REQUIRE(Gba::regRef(Gba::R1) == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armBic(set_cond, 0xffffffff, 0x7fffffff, 2);
        REQUIRE(Gba::regRef(Gba::R2) == 0x80000000);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("BL") {
        gba.armBl(4);
        REQUIRE(Gba::regRef(Gba::LR) == Gba::pak_rom0_offset);
        REQUIRE(Gba::reg[Gba::PC] == Gba::pak_rom0_offset + 4);

        gba.armBl(76);
        REQUIRE(Gba::regRef(Gba::LR) == Gba::pak_rom0_offset + 4);
        REQUIRE(Gba::reg[Gba::PC] == Gba::pak_rom0_offset + 80);
    }

    SECTION("BX") {
        gba.armBx(0);
        REQUIRE(Gba::reg[Gba::PC] == 0);
        REQUIRE(Gba::isArm());

        Gba::regRef(1) = Gba::pak_rom0_offset | 1;
        gba.armBx(1);
        REQUIRE(Gba::reg[Gba::PC] == (Gba::pak_rom0_offset | 1));
        REQUIRE(!Gba::isArm());
    }

    SECTION("CMN") {
        // no flags set
        gba.armCmn(0, 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armCmn(0x80000000, 1);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armCmn(0, 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set C
        gba.armCmn(0xffffffff, 2);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set V, set N
        gba.armCmn(0x7fffffff, 1);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(Gba::vFlag());

        // set V, set C
        gba.armCmn(0x80000000, 0x80000001);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(Gba::vFlag());
    }

    SECTION("CMP") {
        // set N
        gba.armCmp(0, 1);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z, set C
        gba.armCmp(0, 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set C
        gba.armCmp(1, 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set V, set C
        gba.armCmp(0x80000000, 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(Gba::vFlag());
    }

    SECTION("EOR, set_cond = false") {
        // set_cond = false, no flags set
        gba.armEor(set_cond, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R0] == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set N
        gba.armEor(set_cond, 0, 0x80000000, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0x80000000);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        gba.armEor(set_cond, 0, 0, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("EOR, set_cond = true") {
        set_cond = true;

        // no flags set
        gba.armEor(set_cond, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R0] == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armEor(set_cond, 0, 0x80000000, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0x80000000);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armEor(set_cond, 0, 0, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("LDR") {
        Gba::memRef(0) = 0x10;
        Gba::memRef(1) = 0x32;
        Gba::memRef(2) = 0x54;
        Gba::memRef(3) = 0x76;
        Gba::memRef(4) = 0x98;
        Gba::memRef(5) = 0xba;
        Gba::memRef(6) = 0xdc;
        Gba::memRef(7) = 0xfe;

        // word-aligned, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdr(false, false, false, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R1] == 0xfedcba98);

        // 1-byte offset, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R2] = 5;
        gba.armLdr(false, false, false, 2, 3, 0);
        REQUIRE(Gba::reg[Gba::R3] == 0x98fedcba);

        // 2-byte offset, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R4] = 2;
        gba.armLdr(false, false, false, 4, 5, 0);
        REQUIRE(Gba::reg[Gba::R5] == 0x32107654);

        // 3-byte offset, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R6] = 7;
        gba.armLdr(false, false, false, 6, 7, 0);
        REQUIRE(Gba::reg[Gba::R7] == 0xdcba98fe);

        // 3-byte offset, 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R6] = 7;
        gba.armLdr(false, false, true, 6, 7, 7);
        REQUIRE(Gba::reg[Gba::R6] == 0);
        REQUIRE(Gba::reg[Gba::R7] == 0xdcba98fe);

        // word-aligned, 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdr(false, false, true, 0, 1, 2);
        REQUIRE(Gba::reg[Gba::R0] == 2);
        REQUIRE(Gba::reg[Gba::R1] == 0xfedcba98);

        // word-aligned, 011 = no pre-index, add offset, write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdr(false, true, true, 0, 1, 2);
        REQUIRE(Gba::reg[Gba::R0] == 6);
        REQUIRE(Gba::reg[Gba::R1] == 0xfedcba98);

        // 2-byte offset, 111 = pre-index, add offset, write-back
        Gba::reg[Gba::R2] = 4;
        gba.armLdr(true, true, true, 2, 3, 2);
        REQUIRE(Gba::reg[Gba::R2] == 6);
        REQUIRE(Gba::reg[Gba::R3] == 0xba98fedc);

        // 2-byte offset, 110 = pre-index, add offset, no write-back
        Gba::reg[Gba::R2] = 4;
        gba.armLdr(true, true, false, 2, 3, 2);
        REQUIRE(Gba::reg[Gba::R2] == 4);
        REQUIRE(Gba::reg[Gba::R3] == 0xba98fedc);
    }

    SECTION("LDRB") {
        Gba::memRef(0) = 0x10;
        Gba::memRef(1) = 0x32;
        Gba::memRef(2) = 0x54;
        Gba::memRef(3) = 0x76;
        Gba::memRef(4) = 0x98;
        Gba::memRef(5) = 0xba;
        Gba::memRef(6) = 0xdc;
        Gba::memRef(7) = 0xfe;

        // 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdrb(false, false, false, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R1] == 0x00000098);

        // 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R2] = 5;
        gba.armLdrb(false, false, false, 2, 3, 0);
        REQUIRE(Gba::reg[Gba::R3] == 0x000000ba);

        // 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R6] = 7;
        gba.armLdrb(false, false, true, 6, 7, 7);
        REQUIRE(Gba::reg[Gba::R6] == 0);
        REQUIRE(Gba::reg[Gba::R7] == 0x000000fe);

        // 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdrb(false, false, true, 0, 1, 2);
        REQUIRE(Gba::reg[Gba::R0] == 2);
        REQUIRE(Gba::reg[Gba::R1] == 0x00000098);

        // 011 = no pre-index, add offset, write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdrb(false, true, true, 0, 1, 2);
        REQUIRE(Gba::reg[Gba::R0] == 6);
        REQUIRE(Gba::reg[Gba::R1] == 0x00000098);

        // 111 = pre-index, add offset, write-back
        Gba::reg[Gba::R2] = 4;
        gba.armLdrb(true, true, true, 2, 3, 2);
        REQUIRE(Gba::reg[Gba::R2] == 6);
        REQUIRE(Gba::reg[Gba::R3] == 0x000000dc);

        // 2-byte offset, 110 = pre-index, add offset, no write-back
        Gba::reg[Gba::R2] = 4;
        gba.armLdrb(true, true, false, 2, 3, 2);
        REQUIRE(Gba::reg[Gba::R2] == 4);
        REQUIRE(Gba::reg[Gba::R3] == 0x000000dc);
    }

    SECTION("LSL, set_cond = false") {
        uint32_t op;

        // set_cond = false, no flags set
        op = 1;
        gba.armLsl(set_cond, 0, op);
        REQUIRE(op == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, no flags set
        op = 1;
        gba.armLsl(set_cond, 1, op);
        REQUIRE(op == 2);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set C
        op = 0x80000000;
        gba.armLsl(set_cond, 1, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, retain C
        op = 0x80000000;
        gba.armLsl(set_cond, 0, op);
        REQUIRE(op == 0x80000000);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("LSL, set_cond = true") {
        set_cond = true;
        uint32_t op;

        // no flags set
        op = 1;
        gba.armLsl(set_cond, 0, op);
        REQUIRE(op == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // no flags set
        op = 1;
        gba.armLsl(set_cond, 1, op);
        REQUIRE(op == 2);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set C
        op = 0x80000000;
        gba.armLsl(set_cond, 1, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // retain C
        op = 0x80000000;
        gba.armLsl(set_cond, 0, op);
        REQUIRE(op == 0x80000000);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("LSR, set_cond = false") {
        uint32_t op;

        // set_cond = false, no flags set
        op = 2;
        gba.armLsr(set_cond, 1, op);
        REQUIRE(op == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, LSR #0, no flags set
        op = 0x7fffffff;
        gba.armLsr(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, LSR #0, set C
        op = 0x80000000;
        gba.armLsr(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, no flags set
        op = 0xbfffffff;
        gba.armLsr(set_cond, 31, op);
        REQUIRE(op == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set C
        op = 0x40000000;
        gba.armLsr(set_cond, 31, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("LSR, set_cond = true") {
        set_cond = true;
        uint32_t op;

        // no flags set
        op = 2;
        gba.armLsr(set_cond, 1, op);
        REQUIRE(op == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // LSR #0, no flags set
        op = 0x7fffffff;
        gba.armLsr(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // LSR #0, set C
        op = 0x80000000;
        gba.armLsr(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // no flags set
        op = 0xbfffffff;
        gba.armLsr(set_cond, 31, op);
        REQUIRE(op == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set C
        op = 0x40000000;
        gba.armLsr(set_cond, 31, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MLA, set_cond = false") {
        // set_cond = false, no flags set
        gba.armMla(set_cond, 1, 1, 0, 0);
        REQUIRE(Gba::reg[Gba::R0] == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set N
        gba.armMla(set_cond, 0x80000000, 1, 0, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0x80000000);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        gba.armMla(set_cond, 0xffffffff, 1, 1, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MLA, set_cond = true") {
        set_cond = true;

        // no flags set
        gba.armMla(set_cond, 1, 1, 0, 0);
        REQUIRE(Gba::reg[Gba::R0] == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armMla(set_cond, 0x80000000, 1, 0, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0x80000000);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armMla(set_cond, 0xffffffff, 1, 1, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MLAL, set_cond = false") {
        // set_cond = false, datasheet unsigned example, no flags set
        gba.armMlal(set_cond, false, 0xfffffff6, 0x00000014, 1, 0);
        REQUIRE(Gba::reg[Gba::R1] == 0x00000013);
        REQUIRE(Gba::reg[Gba::R0] == 0xffffff38);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, datasheet signed example, set N
        gba.armMlal(set_cond, true, 0xfffffff6, 0x00000014, 3, 2);
        REQUIRE(Gba::reg[Gba::R3] == 0xffffffff);
        REQUIRE(Gba::reg[Gba::R2] == 0xffffff38);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set N
        gba.armMlal(set_cond, true, -2, 1, 5, 4);
        REQUIRE(Gba::reg[Gba::R5] == 0xffffffff);
        REQUIRE(Gba::reg[Gba::R4] == 0xfffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        Gba::regRef(6) = 2;
        gba.armMlal(set_cond, true, -2, 1, 7, 6);
        REQUIRE(Gba::reg[Gba::R7] == 0);
        REQUIRE(Gba::reg[Gba::R6] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MLAL, set_cond = true") {
        set_cond = true;

        // datasheet unsigned example, no flags set
        gba.armMlal(set_cond, false, 0xfffffff6, 0x00000014, 1, 0);
        REQUIRE(Gba::reg[Gba::R1] == 0x00000013);
        REQUIRE(Gba::reg[Gba::R0] == 0xffffff38);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // datasheet signed example, set N
        gba.armMlal(set_cond, true, 0xfffffff6, 0x00000014, 3, 2);
        REQUIRE(Gba::reg[Gba::R3] == 0xffffffff);
        REQUIRE(Gba::reg[Gba::R2] == 0xffffff38);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armMlal(set_cond, true, -2, 1, 5, 4);
        REQUIRE(Gba::reg[Gba::R5] == 0xffffffff);
        REQUIRE(Gba::reg[Gba::R4] == 0xfffffffe);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        Gba::regRef(6) = 2;
        gba.armMlal(set_cond, true, -2, 1, 7, 6);
        REQUIRE(Gba::reg[Gba::R7] == 0);
        REQUIRE(Gba::reg[Gba::R6] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MOV, set_cond = false") {
        // set_cond = false, no flags set
        gba.armMov(set_cond, 0x0fffffff, 0);
        REQUIRE(Gba::reg[Gba::R0] == 0x0fffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set N
        gba.armMov(set_cond, 0x80000000, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0x80000000);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        gba.armMov(set_cond, 0, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MOV, set_cond = true") {
        set_cond = true;

        // no flags set
        gba.armMov(set_cond, 0x0fffffff, 0);
        REQUIRE(Gba::reg[Gba::R0] == 0x0fffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armMov(set_cond, 0x80000000, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0x80000000);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armMov(set_cond, 0, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MUL, set_cond = false") {
        uint32_t acc;

        // set_cond = false, no flags set
        acc = 5;
        gba.armMul(set_cond, 1, 1, acc, 0);
        REQUIRE(Gba::reg[Gba::R0] == 1);
        REQUIRE(acc == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set N
        acc = 5;
        gba.armMul(set_cond, -1, 1, acc, 1);
        REQUIRE(Gba::reg[Gba::R1] == -1);
        REQUIRE(acc == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        acc = 5;
        gba.armMul(set_cond, 500, 0, acc, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(acc == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MUL, set_cond = true") {
        set_cond = true;
        uint32_t acc;

        // no flags set
        acc = 5;
        gba.armMul(set_cond, 1, 1, acc, 0);
        REQUIRE(Gba::reg[Gba::R0] == 1);
        REQUIRE(acc == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        acc = 5;
        gba.armMul(set_cond, -1, 1, acc, 1);
        REQUIRE(Gba::reg[Gba::R1] == -1);
        REQUIRE(acc == 0);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        acc = 5;
        gba.armMul(set_cond, 500, 0, acc, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(acc == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MULL, set_cond = false") {
        // set_cond = false, datasheet unsigned example
        gba.armMull(set_cond, false, 0xfffffff6, 0x00000014, 1, 0);
        REQUIRE(Gba::reg[Gba::R1] == 0x00000013);
        REQUIRE(Gba::reg[Gba::R0] == 0xffffff38);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, datasheet signed example
        gba.armMull(set_cond, true, 0xfffffff6, 0x00000014, 3, 2);
        REQUIRE(Gba::reg[Gba::R3] == 0xffffffff);
        REQUIRE(Gba::reg[Gba::R2] == 0xffffff38);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set N
        gba.armMull(set_cond, true, -2, 1, 5, 4);
        REQUIRE(Gba::reg[Gba::R5] == 0xffffffff);
        REQUIRE(Gba::reg[Gba::R4] == 0xfffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        gba.armMull(set_cond, true, -3, 0, 7, 6);
        REQUIRE(Gba::reg[Gba::R7] == 0);
        REQUIRE(Gba::reg[Gba::R6] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MULL, set_cond = true") {
        set_cond = true;

        // datasheet unsigned example, no flags set
        gba.armMull(set_cond, false, 0xfffffff6, 0x00000014, 1, 0);
        REQUIRE(Gba::reg[Gba::R1] == 0x00000013);
        REQUIRE(Gba::reg[Gba::R0] == 0xffffff38);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // datasheet signed example, set N
        gba.armMull(set_cond, true, 0xfffffff6, 0x00000014, 3, 2);
        REQUIRE(Gba::reg[Gba::R3] == 0xffffffff);
        REQUIRE(Gba::reg[Gba::R2] == 0xffffff38);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armMull(set_cond, true, -2, 1, 5, 4);
        REQUIRE(Gba::reg[Gba::R5] == 0xffffffff);
        REQUIRE(Gba::reg[Gba::R4] == 0xfffffffe);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armMull(set_cond, true, -3, 0, 7, 6);
        REQUIRE(Gba::reg[Gba::R7] == 0);
        REQUIRE(Gba::reg[Gba::R6] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MRS") {
        gba.armMrs(Gba::CPSR, 0);
        REQUIRE(Gba::reg[Gba::R0] == 0x00000013);

        gba.armMrs(Gba::SPSR_fiq, 0);
        REQUIRE(Gba::reg[Gba::R0] == 0);
    }

    SECTION("MSR") {
        // Reg to PSR flags only
        Gba::regRef(0) = 0xafffffff;
        gba.armMsr(Gba::CPSR, 0x00000000);
        REQUIRE((Gba::reg[Gba::CPSR] & 0xf00000ff) == 0xa0000013);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // Rotated imm to PSR flags only
        gba.armMsr(Gba::CPSR, 0x02000207);
        REQUIRE((Gba::reg[Gba::CPSR] & 0xf00000ff) == 0xc0000013);
        REQUIRE(Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // Rotated imm to PSR flags only
        gba.armMsr(Gba::CPSR, 0x02000107);
        REQUIRE((Gba::reg[Gba::CPSR] & 0xf00000ff) == 0x80000013);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // Reg to PSR
        Gba::regRef(1) = 0x9fffff30;
        gba.armMsr(Gba::CPSR, 0x00010001);
        REQUIRE((Gba::reg[Gba::CPSR] & 0xf00000ff) == 0x90000030);
        REQUIRE(!Gba::isArm());
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(Gba::vFlag());

        // Reg to PSR
        Gba::regRef(2) = 0x6fffff13;
        gba.armMsr(Gba::CPSR, 0x00010002);
        REQUIRE((Gba::reg[Gba::CPSR] & 0xf00000ff) == 0x60000013);
        REQUIRE(Gba::isArm());
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MVN, set_cond = false") {
        // set_cond = false, no flags set
        gba.armMvn(set_cond, 0xfffffffe, 0);
        REQUIRE(Gba::reg[Gba::R0] == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set N
        gba.armMvn(set_cond, 1, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0xfffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        gba.armMvn(set_cond, 0xffffffff, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("MVN, set_cond = true") {
        set_cond = true;

        // no flags set
        gba.armMvn(set_cond, 0xfffffffe, 0);
        REQUIRE(Gba::reg[Gba::R0] == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armMvn(set_cond, 1, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0xfffffffe);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armMvn(set_cond, 0xffffffff, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("ORR, set_cond = false") {
        // set_cond = false, no flags set
        gba.armOrr(set_cond, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R0] == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set N
        gba.armOrr(set_cond, 0x80000000, 1, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0x80000001);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z
        gba.armOrr(set_cond, 0, 0, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("ORR, set_cond = true") {
        set_cond = true;
        
        // no flags set
        gba.armOrr(set_cond, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R0] == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armOrr(set_cond, 0x80000000, 1, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0x80000001);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armOrr(set_cond, 0, 0, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("ROR, set_cond = false") {
        uint32_t op;
        
        // set_cond = false, ROR #0, no flags set
        op = 0;
        gba.armRor(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, ROR #0, set C
        op = 1;
        gba.armRor(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, ROR #0, no flags set
        op = 0;
        gba.armRor(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, ROR #31, set C
        op = 0x7fffffff;
        gba.armRor(set_cond, 31, op);
        REQUIRE(op == 0xfffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, ROR #31, no flags set
        op = 0x80000000;
        gba.armRor(set_cond, 31, op);
        REQUIRE(op == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("ROR, set_cond = false") {
        set_cond = true;
        uint32_t op;
        
        // ROR #0, no flags set
        op = 0;
        gba.armRor(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // ROR #0, set C
        op = 1;
        gba.armRor(set_cond, 0, op);
        REQUIRE(op == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // ROR #0, no flags set
        op = 0;
        gba.armRor(set_cond, 0, op);
        REQUIRE(op == 0x80000000);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // ROR #31, set C
        op = 0x7fffffff;
        gba.armRor(set_cond, 31, op);
        REQUIRE(op == 0xfffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, ROR #31, no flags set
        op = 0x80000000;
        gba.armRor(set_cond, 31, op);
        REQUIRE(op == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("RSB, set_cond = false") {
        // set_cond = false, set N, set C
        gba.armRsb(set_cond, 0, 0xffffffff, 0);
        REQUIRE(Gba::reg[Gba::R0] == 0xffffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z, set C
        gba.armRsb(set_cond, -1, -1, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set C
        gba.armRsb(set_cond, 1, 2, 2);
        REQUIRE(Gba::reg[Gba::R2] == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set V
        gba.armRsb(set_cond, 2, 0x80000000, 3);
        REQUIRE(Gba::reg[Gba::R3] == 0x7ffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("RSB, set_cond = true") {
        set_cond = true;

        // set N, set C
        gba.armRsb(set_cond, 0, 0xffffffff, 0);
        REQUIRE(Gba::reg[Gba::R0] == 0xffffffff);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z, set C
        gba.armRsb(set_cond, -1, -1, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set C
        gba.armRsb(set_cond, 1, 2, 2);
        REQUIRE(Gba::reg[Gba::R2] == 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set V, set N
        gba.armRsb(set_cond, -1, 0x7fffffff, 3);
        REQUIRE(Gba::reg[Gba::R3] == 0x80000000);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(Gba::vFlag());
    }

    SECTION("RSC, set_cond = false") {
        // set_cond = false, set N
        gba.armRsc(set_cond, 2, 0, 0);
        REQUIRE(Gba::reg[Gba::R0] == -1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z, set C
        gba.armRsc(set_cond, 2, 1, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set V, set C
        gba.armRsc(set_cond, 2, 0x80000000, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0x7fffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("RSC, set_cond = true") {
        set_cond = true;

        // set N
        gba.armRsc(set_cond, 2, 0, 0);
        REQUIRE(Gba::reg[Gba::R0] == -1);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z, set C
        gba.armRsc(set_cond, 2, 1, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set V, set C
        gba.armRsc(set_cond, 2, 0x80000000, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0x7ffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(Gba::vFlag());
    }

    SECTION("SBC, set_cond = false") {
        // set_cond = false, set N
        gba.armSbc(set_cond, 0, 2, 0);
        REQUIRE(Gba::reg[Gba::R0] == -1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z, set C
        gba.armSbc(set_cond, 1, 2, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set V, set C
        gba.armSbc(set_cond, 0x80000000, 2, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0x7fffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("SBC, set_cond = true") {
        set_cond = true;

        // set N
        gba.armSbc(set_cond, 0, 2, 0);
        REQUIRE(Gba::reg[Gba::R0] == -1);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z, set C
        gba.armSbc(set_cond, 1, 2, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set V, set C
        gba.armSbc(set_cond, 0x80000000, 2, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0x7ffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(Gba::vFlag());
    }

    SECTION("STR") {
        Gba::reg[Gba::R0] = 0x76543210;
        Gba::reg[Gba::R2] = 0xfedcba98;

        // not word-aligned address, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R3] = 15;
        gba.armStr(false, false, false, 3, 0, 0);
        REQUIRE(Gba::memRef(12) == 0x10);
        REQUIRE(Gba::memRef(13) == 0x32);
        REQUIRE(Gba::memRef(14) == 0x54);
        REQUIRE(Gba::memRef(15) == 0x76);

        // 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R3] = 17;
        gba.armStr(false, false, true, 3, 0, 1);
        REQUIRE(Gba::reg[Gba::R3] == 16);
        REQUIRE(Gba::memRef(16) == 0x10);
        REQUIRE(Gba::memRef(17) == 0x32);
        REQUIRE(Gba::memRef(18) == 0x54);
        REQUIRE(Gba::memRef(19) == 0x76);

        // 011 = no pre-index, add offset, write-back
        Gba::reg[Gba::R4] = 14;
        gba.armStr(false, true, true, 4, 2, 2);
        REQUIRE(Gba::reg[Gba::R4] == 16);
        REQUIRE(Gba::memRef(12) == 0x98);
        REQUIRE(Gba::memRef(13) == 0xba);
        REQUIRE(Gba::memRef(14) == 0xdc);
        REQUIRE(Gba::memRef(15) == 0xfe);

        // 111 = pre-index, add offset, write-back
        Gba::reg[Gba::R4] = 14;
        gba.armStr(true, true, true, 4, 2, 2);
        REQUIRE(Gba::reg[Gba::R4] == 16);
        REQUIRE(Gba::memRef(16) == 0x98);
        REQUIRE(Gba::memRef(17) == 0xba);
        REQUIRE(Gba::memRef(18) == 0xdc);
        REQUIRE(Gba::memRef(19) == 0xfe);

        // 110 = pre-index, add offset, no write-back
        Gba::reg[Gba::R4] = 14;
        gba.armStr(true, true, false, 4, 0, 2);
        REQUIRE(Gba::reg[Gba::R4] == 14);
        REQUIRE(Gba::memRef(16) == 0x10);
        REQUIRE(Gba::memRef(17) == 0x32);
        REQUIRE(Gba::memRef(18) == 0x54);
        REQUIRE(Gba::memRef(19) == 0x76);
    }

    SECTION("STRB") {
        Gba::reg[Gba::R0] = 0x765432aa;
        Gba::reg[Gba::R2] = 0xfedcba55;

        // 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R3] = 21;
        gba.armStrb(false, false, false, 3, 0, 0);
        REQUIRE(Gba::memRef(21) == 0xaa);

        // 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R5] = 17;
        gba.armStrb(false, false, true, 5, 0, 1);
        REQUIRE(Gba::reg[Gba::R5] == 16);
        REQUIRE(Gba::memRef(17) == 0xaa);

        // 011 = no pre-index, add offset, write-back
        Gba::reg[Gba::R4] = 14;
        gba.armStrb(false, true, true, 4, 2, 2);
        REQUIRE(Gba::reg[Gba::R4] == 16);
        REQUIRE(Gba::memRef(14) == 0x55);

        // 111 = pre-index, add offset, write-back
        Gba::reg[Gba::R4] = 14;
        gba.armStrb(true, true, true, 4, 2, 2);
        REQUIRE(Gba::reg[Gba::R4] == 16);
        REQUIRE(Gba::memRef(16) == 0x55);

        // 110 = pre-index, add offset, no write-back
        Gba::reg[Gba::R4] = 14;
        gba.armStrb(true, true, false, 4, 0, 2);
        REQUIRE(Gba::reg[Gba::R4] == 14);
        REQUIRE(Gba::memRef(16) == 0xaa);
    }

    SECTION("SUB, set_cond = false") {
        // set_cond = false, set N
        gba.armSub(set_cond, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R0] == -1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set Z, set C
        gba.armSub(set_cond, 1, 1, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set_cond = false, set V, set C
        gba.armSub(set_cond, 0x80000000, 2, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0x7ffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("SUB, set_cond = true") {
        set_cond = true;

        // set N
        gba.armSub(set_cond, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R0] == -1);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z, set C
        gba.armSub(set_cond, 1, 1, 1);
        REQUIRE(Gba::reg[Gba::R1] == 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set V, set C
        gba.armSub(set_cond, 0x80000000, 2, 2);
        REQUIRE(Gba::reg[Gba::R2] == 0x7ffffffe);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(Gba::vFlag());
    }

    SECTION("TEQ") {
        // no flags set
        gba.armTeq(0x80000000, 0xffffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armTeq(0x80000000, 0);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armTeq(0xffffffff, 0xffffffff);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }

    SECTION("TST") {
        // no flags set
        gba.armTst(1, 1);
        REQUIRE(!Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set N
        gba.armTst(0x80000000, 0x8fffffff);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // set Z
        gba.armTst(0xffffffff, 0);
        REQUIRE(!Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());
    }
};