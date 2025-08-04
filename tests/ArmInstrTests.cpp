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

    SECTION("LDM") {
        Gba::memRef(0) = 0x10;
        Gba::memRef(1) = 0x32;
        Gba::memRef(2) = 0x54;
        Gba::memRef(3) = 0x76;
        Gba::memRef(4) = 0x98;
        Gba::memRef(5) = 0xba;
        Gba::memRef(6) = 0xdc;
        Gba::memRef(7) = 0xfe;
        Gba::memRef(8) = 0x88;
        Gba::memRef(9) = 0x99;
        Gba::memRef(10) = 0xaa;
        Gba::memRef(11) = 0xbb;
        Gba::memRef(12) = 0xcc;
        Gba::memRef(13) = 0xdd;
        Gba::memRef(14) = 0xee;
        Gba::memRef(15) = 0xff;

        // 0100 = post-index, add offset, !s, no write-back
        Gba::reg[Gba::R0] = 0;
        gba.armLdm(false, true, false, false, 0, 6);
        REQUIRE(Gba::reg[Gba::R0] == 0);
        REQUIRE(Gba::reg[Gba::R1] == 0x76543210);
        REQUIRE(Gba::reg[Gba::R2] == 0xfedcba98);

        // 0101 = post-index, add offset, !s, write-back
        Gba::reg[Gba::R0] = 5;
        gba.armLdm(false, true, false, true, 0, 6);
        REQUIRE(Gba::reg[Gba::R0] == 12);
        REQUIRE(Gba::reg[Gba::R1] == 0xfedcba98);
        REQUIRE(Gba::reg[Gba::R2] == 0xbbaa9988);

        // 1001 = pre-index, subtract offset, !s, write-back
        Gba::reg[Gba::R1] = 13;
        gba.armLdm(true, false, false, true, 1, 12);
        REQUIRE(Gba::reg[Gba::R1] == 4);
        REQUIRE(Gba::reg[Gba::R2] == 0xfedcba98);
        REQUIRE(Gba::reg[Gba::R3] == 0xbbaa9988);

        // 0001 = post-index, subtract offset, !s, write-back
        Gba::reg[Gba::R1] = 13;
        gba.armLdm(false, false, false, true, 1, 12);
        REQUIRE(Gba::reg[Gba::R1] == 4);
        REQUIRE(Gba::reg[Gba::R2] == 0xbbaa9988);
        REQUIRE(Gba::reg[Gba::R3] == 0xffeeddcc);

        // 0011 = post-index, subtract offset, s, write-back; S bit set and PC not in reg_list
        REQUIRE(Gba::reg[Gba::SP] == 0); // User reg 13 is SP
        REQUIRE(Gba::reg[Gba::R13_svc] == 0);
        Gba::reg[Gba::R1] = 9;
        gba.armLdm(false, false, true, true, 1, 0x2004);
        REQUIRE(Gba::reg[Gba::R1] == 9);
        REQUIRE(Gba::reg[Gba::R2] == 0xfedcba98);
        REQUIRE(Gba::reg[Gba::R13_svc] == 0);
        REQUIRE(Gba::reg[Gba::SP] == 0xbbaa9988);

        // 0000 = post-index, subtract offset, !s, no write-back
        Gba::reg[Gba::SP] = 0;
        Gba::reg[Gba::R13_svc] = 0;
        Gba::reg[Gba::R1] = 9;
        gba.armLdm(false, false, false, false, 1, 0x2004);
        REQUIRE(Gba::reg[Gba::R1] == 9);
        REQUIRE(Gba::reg[Gba::R2] == 0xfedcba98);
        REQUIRE(Gba::reg[Gba::R13_svc] == 0xbbaa9988);
        REQUIRE(Gba::reg[Gba::SP] == 0);

        // 1111 = pre-index, add offset, s, write-back; S bit set and PC in reg_list
        REQUIRE(Gba::reg[Gba::SPSR_svc] == 0);
        REQUIRE(Gba::reg[Gba::CPSR] == 0x000000d3);
        Gba::reg[Gba::SPSR_svc] = 0xf00000d3;
        Gba::reg[Gba::R1] = 6;
        gba.armLdm(true, true, true, true, 1, 0x8004);
        REQUIRE(Gba::reg[Gba::R1] == 12);
        REQUIRE(Gba::reg[Gba::R2] == 0xbbaa9988);
        REQUIRE(Gba::reg[Gba::PC] == 0xffeeddcc);
        REQUIRE(Gba::reg[Gba::CPSR] == 0xf00000d3);
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
        REQUIRE(Gba::reg[Gba::R0] == 4);
        REQUIRE(Gba::reg[Gba::R1] == 0xfedcba98);

        // 1-byte offset, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R2] = 5;
        gba.armLdr(false, false, false, 2, 3, 0);
        REQUIRE(Gba::reg[Gba::R2] == 5);
        REQUIRE(Gba::reg[Gba::R3] == 0x98fedcba);

        // 2-byte offset, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R4] = 2;
        gba.armLdr(false, false, false, 4, 5, 1);
        REQUIRE(Gba::reg[Gba::R4] == 1);
        REQUIRE(Gba::reg[Gba::R5] == 0x32107654);

        // 3-byte offset, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R6] = 7;
        gba.armLdr(false, false, false, 6, 7, 3);
        REQUIRE(Gba::reg[Gba::R6] == 4);
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
        REQUIRE(Gba::reg[Gba::R0] == 4);
        REQUIRE(Gba::reg[Gba::R1] == 0x00000098);

        // 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R2] = 5;
        gba.armLdrb(false, false, false, 2, 3, 1);
        REQUIRE(Gba::reg[Gba::R2] == 4);
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

    SECTION("LDRH") {
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
        gba.armLdrh(false, false, false, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R0] == 4);
        REQUIRE(Gba::reg[Gba::R1] == 0x0000ba98);

        // 1-byte offset, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R2] = 5;
        gba.armLdrh(false, false, false, 2, 3, 0);
        REQUIRE(Gba::reg[Gba::R2] == 5);
        REQUIRE(Gba::reg[Gba::R3] == 0x0000ba98);

        // 2-byte / half-word offset, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R4] = 2;
        gba.armLdrh(false, false, false, 4, 5, 1);
        REQUIRE(Gba::reg[Gba::R4] == 1);
        REQUIRE(Gba::reg[Gba::R5] == 0x00007654);

        // 3-byte offset, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R6] = 7;
        gba.armLdrh(false, false, false, 6, 7, 3);
        REQUIRE(Gba::reg[Gba::R6] == 4);
        REQUIRE(Gba::reg[Gba::R7] == 0x0000fedc);

        // 3-byte offset, 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R6] = 7;
        gba.armLdrh(false, false, true, 6, 7, 7);
        REQUIRE(Gba::reg[Gba::R6] == 0);
        REQUIRE(Gba::reg[Gba::R7] == 0x0000fedc);

        // word-aligned, 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdrh(false, false, true, 0, 1, 2);
        REQUIRE(Gba::reg[Gba::R0] == 2);
        REQUIRE(Gba::reg[Gba::R1] == 0x0000ba98);

        // word-aligned, 011 = no pre-index, add offset, write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdrh(false, true, true, 0, 1, 2);
        REQUIRE(Gba::reg[Gba::R0] == 6);
        REQUIRE(Gba::reg[Gba::R1] == 0x0000ba98);

        // 2-byte / half-word offset, 111 = pre-index, add offset, write-back
        Gba::reg[Gba::R2] = 4;
        gba.armLdrh(true, true, true, 2, 3, 2);
        REQUIRE(Gba::reg[Gba::R2] == 6);
        REQUIRE(Gba::reg[Gba::R3] == 0x0000fedc);

        // 2-byte offset, 110 = pre-index, add offset, no write-back
        Gba::reg[Gba::R2] = 4;
        gba.armLdrh(true, true, false, 2, 3, 2);
        REQUIRE(Gba::reg[Gba::R2] == 4);
        REQUIRE(Gba::reg[Gba::R3] == 0x0000fedc);
    }

    SECTION("LDRSB") {
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
        gba.armLdrsb(false, false, false, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R0] == 4);
        REQUIRE(Gba::reg[Gba::R1] == 0xffffff98);

        // 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R2] = 5;
        gba.armLdrsb(false, false, false, 2, 3, 1);
        REQUIRE(Gba::reg[Gba::R2] == 4);
        REQUIRE(Gba::reg[Gba::R3] == 0xffffffba);

        // 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R6] = 3;
        gba.armLdrsb(false, false, true, 6, 7, 3);
        REQUIRE(Gba::reg[Gba::R6] == 0);
        REQUIRE(Gba::reg[Gba::R7] == 0x00000076);

        // 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdrsb(false, false, true, 0, 1, 2);
        REQUIRE(Gba::reg[Gba::R0] == 2);
        REQUIRE(Gba::reg[Gba::R1] == 0xffffff98);

        // 011 = no pre-index, add offset, write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdrsb(false, true, true, 0, 1, 2);
        REQUIRE(Gba::reg[Gba::R0] == 6);
        REQUIRE(Gba::reg[Gba::R1] == 0xffffff98);

        // 111 = pre-index, add offset, write-back
        Gba::reg[Gba::R2] = 4;
        gba.armLdrsb(true, true, true, 2, 3, 2);
        REQUIRE(Gba::reg[Gba::R2] == 6);
        REQUIRE(Gba::reg[Gba::R3] == 0xffffffdc);

        // 2-byte offset, 110 = pre-index, add offset, no write-back
        Gba::reg[Gba::R2] = 4;
        gba.armLdrsb(true, true, false, 2, 3, 2);
        REQUIRE(Gba::reg[Gba::R2] == 4);
        REQUIRE(Gba::reg[Gba::R3] == 0xffffffdc);
    }

    SECTION("LDRSH") {
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
        gba.armLdrsh(false, false, false, 0, 1, 0);
        REQUIRE(Gba::reg[Gba::R0] == 4);
        REQUIRE(Gba::reg[Gba::R1] == 0xffffba98);

        // 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R2] = 1;
        gba.armLdrsh(false, false, false, 2, 3, 1);
        REQUIRE(Gba::reg[Gba::R2] == 0);
        REQUIRE(Gba::reg[Gba::R3] == 0x00003210);

        // 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R6] = 3;
        gba.armLdrsh(false, false, true, 6, 7, 3);
        REQUIRE(Gba::reg[Gba::R6] == 0);
        REQUIRE(Gba::reg[Gba::R7] == 0x00007654);

        // 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdrsh(false, false, true, 0, 1, 2);
        REQUIRE(Gba::reg[Gba::R0] == 2);
        REQUIRE(Gba::reg[Gba::R1] == 0xffffba98);

        // 011 = no pre-index, add offset, write-back
        Gba::reg[Gba::R0] = 4;
        gba.armLdrsh(false, true, true, 0, 1, 2);
        REQUIRE(Gba::reg[Gba::R0] == 6);
        REQUIRE(Gba::reg[Gba::R1] == 0xffffba98);

        // 111 = pre-index, add offset, write-back
        Gba::reg[Gba::R2] = 4;
        gba.armLdrsh(true, true, true, 2, 3, 2);
        REQUIRE(Gba::reg[Gba::R2] == 6);
        REQUIRE(Gba::reg[Gba::R3] == 0xfffffedc);

        // 2-byte / half-word offset, 110 = pre-index, add offset, no write-back
        Gba::reg[Gba::R2] = 4;
        gba.armLdrsh(true, true, false, 2, 3, 2);
        REQUIRE(Gba::reg[Gba::R2] == 4);
        REQUIRE(Gba::reg[Gba::R3] == 0xfffffedc);
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
        REQUIRE(Gba::reg[Gba::R0] == 0x000000d3);

        gba.armMrs(Gba::SPSR_fiq, 0);
        REQUIRE(Gba::reg[Gba::R0] == 0);
    }

    SECTION("MSR") {
        // Reg to PSR flags only
        Gba::regRef(0) = 0xafffffff;
        gba.armMsr(Gba::CPSR, 0x00000000);
        REQUIRE((Gba::reg[Gba::CPSR] & 0xf00000ff) == 0xa00000d3);
        REQUIRE(Gba::nFlag());
        REQUIRE(!Gba::zFlag());
        REQUIRE(Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // Rotated imm to PSR flags only
        gba.armMsr(Gba::CPSR, 0x02000207);
        REQUIRE((Gba::reg[Gba::CPSR] & 0xf00000ff) == 0xc00000d3);
        REQUIRE(Gba::nFlag());
        REQUIRE(Gba::zFlag());
        REQUIRE(!Gba::cFlag());
        REQUIRE(!Gba::vFlag());

        // Rotated imm to PSR flags only
        gba.armMsr(Gba::CPSR, 0x02000107);
        REQUIRE((Gba::reg[Gba::CPSR] & 0xf00000ff) == 0x800000d3);
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

    SECTION("STM") {
        // 0000 = post-index, subtract offset, !s, no write-back
        Gba::reg[Gba::R0] = 37;
        Gba::reg[Gba::R1] = 0x33221100;
        Gba::reg[Gba::R2] = 0x77665544;
        gba.armStm(false, false, false, false, 0, 6);
        REQUIRE(Gba::reg[Gba::R0] == 37);
        REQUIRE(Gba::memRef(36) == 0x44);
        REQUIRE(Gba::memRef(37) == 0x55);
        REQUIRE(Gba::memRef(38) == 0x66);
        REQUIRE(Gba::memRef(39) == 0x77);
        REQUIRE(Gba::memRef(32) == 0);
        REQUIRE(Gba::memRef(33) == 0x11);
        REQUIRE(Gba::memRef(34) == 0x22);
        REQUIRE(Gba::memRef(35) == 0x33);

        // 0001 = post-index, subtract offset, !s, write-back
        Gba::reg[Gba::R0] = 23;
        Gba::reg[Gba::R2] = 0xbbaa9988;
        Gba::reg[Gba::R3] = 0xffeeddcc;
        gba.armStm(false, false, false, true, 0, 0xc);
        REQUIRE(Gba::reg[Gba::R0] == 12);
        REQUIRE(Gba::memRef(20) == 0xcc);
        REQUIRE(Gba::memRef(21) == 0xdd);
        REQUIRE(Gba::memRef(22) == 0xee);
        REQUIRE(Gba::memRef(23) == 0xff);
        REQUIRE(Gba::memRef(16) == 0x88);
        REQUIRE(Gba::memRef(17) == 0x99);
        REQUIRE(Gba::memRef(18) == 0xaa);
        REQUIRE(Gba::memRef(19) == 0xbb);

        // 0101 = post-index, add offset, !s, write-back
        Gba::reg[Gba::R0] = 23;
        Gba::reg[Gba::R2] = 0xbbaa9988;
        Gba::reg[Gba::R3] = 0xffeeddcc;
        gba.armStm(false, true, false, true, 0, 0xc);
        REQUIRE(Gba::reg[Gba::R0] == 28);
        REQUIRE(Gba::memRef(24) == 0xcc);
        REQUIRE(Gba::memRef(25) == 0xdd);
        REQUIRE(Gba::memRef(26) == 0xee);
        REQUIRE(Gba::memRef(27) == 0xff);
        REQUIRE(Gba::memRef(20) == 0x88);
        REQUIRE(Gba::memRef(21) == 0x99);
        REQUIRE(Gba::memRef(22) == 0xaa);
        REQUIRE(Gba::memRef(23) == 0xbb);

        // 1101 = pre-index, add offset, !s, write-back
        Gba::reg[Gba::R0] = 52;
        Gba::reg[Gba::R2] = 0xfedcba98;
        Gba::reg[Gba::R3] = 0x76543210;
        gba.armStm(true, true, false, true, 0, 0xc);
        REQUIRE(Gba::reg[Gba::R0] == 60);
        REQUIRE(Gba::memRef(60) == 0x10);
        REQUIRE(Gba::memRef(61) == 0x32);
        REQUIRE(Gba::memRef(62) == 0x54);
        REQUIRE(Gba::memRef(63) == 0x76);
        REQUIRE(Gba::memRef(56) == 0x98);
        REQUIRE(Gba::memRef(57) == 0xba);
        REQUIRE(Gba::memRef(58) == 0xdc);
        REQUIRE(Gba::memRef(59) == 0xfe);

        // 1111 = pre-index, add offset, s, write-back
        Gba::reg[Gba::R0] = 40;
        Gba::reg[Gba::R1] = 0x33221100;
        Gba::reg[Gba::R13_svc] = 0x77665544;
        Gba::reg[Gba::SP] = 0xbbaa9988;
        gba.armStm(true, true, true, true, 0, 0x2002);
        REQUIRE(Gba::reg[Gba::R0] == 40);
        REQUIRE(Gba::memRef(48) == 0x88);
        REQUIRE(Gba::memRef(49) == 0x99);
        REQUIRE(Gba::memRef(50) == 0xaa);
        REQUIRE(Gba::memRef(51) == 0xbb);
        REQUIRE(Gba::memRef(44) == 0);
        REQUIRE(Gba::memRef(45) == 0x11);
        REQUIRE(Gba::memRef(46) == 0x22);
        REQUIRE(Gba::memRef(47) == 0x33);

        // 1101 = pre-index, add offset, !s, write-back
        Gba::reg[Gba::R0] = 40;
        Gba::reg[Gba::R1] = 0x33221100;
        Gba::reg[Gba::R13_svc] = 0x77665544;
        Gba::reg[Gba::SP] = 0xbbaa9988;
        gba.armStm(true, true, false, true, 0, 0x2002);
        REQUIRE(Gba::reg[Gba::R0] == 48);
        REQUIRE(Gba::memRef(48) == 0x44);
        REQUIRE(Gba::memRef(49) == 0x55);
        REQUIRE(Gba::memRef(50) == 0x66);
        REQUIRE(Gba::memRef(51) == 0x77);
        REQUIRE(Gba::memRef(44) == 0);
        REQUIRE(Gba::memRef(45) == 0x11);
        REQUIRE(Gba::memRef(46) == 0x22);
        REQUIRE(Gba::memRef(47) == 0x33);
    }

    SECTION("STR") {
        Gba::reg[Gba::R0] = 0x76543210;
        Gba::reg[Gba::R2] = 0xfedcba98;

        // not word-aligned address, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R3] = 15;
        gba.armStr(false, false, false, 3, 0, 1);
        REQUIRE(Gba::reg[Gba::R3] == 14);
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
        gba.armStrb(false, false, false, 3, 0, 1);
        REQUIRE(Gba::reg[Gba::R3] == 20);
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

    SECTION("STRH") {
        Gba::reg[Gba::R0] = 0x76543210;
        Gba::reg[Gba::R2] = 0xfedcba98;

        // not half-word-aligned address, 000 = no pre-index, subtract offset, no write-back
        Gba::reg[Gba::R3] = 15;
        gba.armStrh(false, false, false, 3, 0, 1);
        REQUIRE(Gba::reg[Gba::R3] == 14);
        REQUIRE(Gba::memRef(14) == 0x10);
        REQUIRE(Gba::memRef(15) == 0x32);

        // 001 = no pre-index, subtract offset, write-back
        Gba::reg[Gba::R3] = 17;
        gba.armStrh(false, false, true, 3, 0, 1);
        REQUIRE(Gba::reg[Gba::R3] == 16);
        REQUIRE(Gba::memRef(16) == 0x10);
        REQUIRE(Gba::memRef(17) == 0x32);

        // 011 = no pre-index, add offset, write-back
        Gba::reg[Gba::R4] = 14;
        gba.armStrh(false, true, true, 4, 2, 2);
        REQUIRE(Gba::reg[Gba::R4] == 16);
        REQUIRE(Gba::memRef(14) == 0x98);
        REQUIRE(Gba::memRef(15) == 0xba);

        // 111 = pre-index, add offset, write-back
        Gba::reg[Gba::R4] = 14;
        gba.armStrh(true, true, true, 4, 2, 2);
        REQUIRE(Gba::reg[Gba::R4] == 16);
        REQUIRE(Gba::memRef(16) == 0x98);
        REQUIRE(Gba::memRef(17) == 0xba);

        // 110 = pre-index, add offset, no write-back
        Gba::reg[Gba::R4] = 14;
        gba.armStrh(true, true, false, 4, 0, 2);
        REQUIRE(Gba::reg[Gba::R4] == 14);
        REQUIRE(Gba::memRef(16) == 0x10);
        REQUIRE(Gba::memRef(17) == 0x32);
    }

    SECTION("SWP") {
        Gba::reg[Gba::R0] = 0x33221100;
        Gba::reg[Gba::R1] = 0x77665544;
        Gba::reg[Gba::R2] = 0xbbaa9988;
        Gba::reg[Gba::R3] = 0xffeeddcc;
        Gba::memRef(0) = 0x10;
        Gba::memRef(1) = 0x32;
        Gba::memRef(2) = 0x54;
        Gba::memRef(3) = 0x76;
        Gba::memRef(4) = 0x98;
        Gba::memRef(5) = 0xba;
        Gba::memRef(6) = 0xdc;
        Gba::memRef(7) = 0xfe;

        // word swap, 3-byte offset
        Gba::reg[Gba::R4] = 3;
        gba.armSwp(true, 4, 6, 1);
        REQUIRE(Gba::reg[Gba::R4] == 3);
        REQUIRE(Gba::temp_reg == 0x54321076);
        REQUIRE(Gba::reg[Gba::R6] == 0x54321076);
        REQUIRE(Gba::memRef(0) == 0x44);
        REQUIRE(Gba::memRef(1) == 0x55);
        REQUIRE(Gba::memRef(2) == 0x66);
        REQUIRE(Gba::memRef(3) == 0x77);

        // word swap, 2-byte offset
        Gba::reg[Gba::R4] = 2;
        gba.armSwp(true, 4, 6, 0);
        REQUIRE(Gba::reg[Gba::R4] == 2);
        REQUIRE(Gba::temp_reg == 0x55447766);
        REQUIRE(Gba::reg[Gba::R6] == 0x55447766);
        REQUIRE(Gba::memRef(0) == 0);
        REQUIRE(Gba::memRef(1) == 0x11);
        REQUIRE(Gba::memRef(2) == 0x22);
        REQUIRE(Gba::memRef(3) == 0x33);

        // byte swap
        Gba::reg[Gba::R5] = 1;
        gba.armSwp(false, 5, 7, 2);
        REQUIRE(Gba::reg[Gba::R5] == 1);
        REQUIRE(Gba::temp_reg == 0x00000011);
        REQUIRE(Gba::reg[Gba::R7] == 0x00000011);
        REQUIRE(Gba::memRef(0) == 0);
        REQUIRE(Gba::memRef(1) == 0x88);
        REQUIRE(Gba::memRef(2) == 0x22);
        REQUIRE(Gba::memRef(3) == 0x33);

        // word swap, 1-byte offset, source reg = dest reg
        Gba::reg[Gba::R4] = 5;
        gba.armSwp(true, 4, 6, 6);
        REQUIRE(Gba::reg[Gba::R4] == 5);
        REQUIRE(Gba::temp_reg == 0x98fedcba);
        REQUIRE(Gba::reg[Gba::R6] == 0x98fedcba);
        REQUIRE(Gba::memRef(4) == 0x66);
        REQUIRE(Gba::memRef(5) == 0x77);
        REQUIRE(Gba::memRef(6) == 0x44);
        REQUIRE(Gba::memRef(7) == 0x55);

        // byte swap, source reg = dest reg
        Gba::reg[Gba::R7] = 6;
        gba.armSwp(false, 7, 6, 6);
        REQUIRE(Gba::reg[Gba::R7] == 6);
        REQUIRE(Gba::temp_reg == 0x00000044);
        REQUIRE(Gba::reg[Gba::R6] == 0x00000044);
        REQUIRE(Gba::memRef(4) == 0x66);
        REQUIRE(Gba::memRef(5) == 0x77);
        REQUIRE(Gba::memRef(6) == 0xba);
        REQUIRE(Gba::memRef(7) == 0x55);
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