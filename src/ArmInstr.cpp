#include "../include/Gba.hpp"

void Gba::armAdc(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    op2 = op2 + ((reg[CPSR] & 0x20000000) == 0x20000000 ? 1 : 0);
    uint64_t res_e = op1 + op2;
    bool c_flag = (res_e >= 0x100000000);
    bool v_flag = false;
    uint32_t res = res_e;

    if ((op1 & 0x80000000) == (op2 & 0x80000000) && (op1 & 0x80000000) != (res & 0x80000000))
        v_flag = true; // Set V
    
    regRef(rd) = res;
    if (set_cond) armAriSetCond(res, c_flag, v_flag, rd);
};

void Gba::armAdd(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    uint64_t res_e = op1 + op2;
    bool c_flag = (res_e >= 0x100000000);
    bool v_flag = false;
    uint32_t res = res_e;

    if ((op1 & 0x80000000) == (op2 & 0x80000000) && (op1 & 0x80000000) != (res & 0x80000000))
        v_flag = true; // Set V

    regRef(rd) = res;
    if (set_cond) armAriSetCond(res, c_flag, v_flag, rd);
};

void Gba::armAnd(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    uint32_t res = op1 & op2;
    regRef(rd) = res;
    if (set_cond) armLogSetCond(res, rd);
};

void Gba::armAsr(bool set_cond, uint8_t shift_am, uint32_t& op) {
    uint8_t sign = (op & 0x80000000) == 0x00000000 ? 0x00000000 : 0x80000000;

    if (shift_am == 0) { // ASR #0 becomes ASR #32; all 0s or 1s result and bit 31 becomes carry out
        if (set_cond) reg[CPSR] = sign == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
        op = sign == 0x00000000 ? 0x00000000 : 0xffffffff;
    }else {
        for(uint8_t i = 0; i < (shift_am - 1); ++i) // Most significant shifted out bit needs to be saved in CPSR as the carry out
            op = (op >> 1) | sign;

        if (set_cond) reg[CPSR] = (op & 0x00000001) == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
        op = (op >> 1) | sign;
    }
};

void Gba::armB(uint32_t offset) { reg[PC] += offset; };

void Gba::armBic(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    uint32_t res = op1 & ~op2;
    regRef(rd) = res;
    if (set_cond) armLogSetCond(res, rd);
};

void Gba::armBl(uint32_t offset) {
    regRef(LR) = reg[PC] & 0xfffffffc;
    reg[PC] += offset;
};

void Gba::armBx(uint8_t rn) {
    uint32_t w = regRef(rn);
    reg[PC] = w;
    reg[CPSR] = ((w & 0x00000001) == 0x00000000 ? reg[CPSR] & 0xffffffdf : reg[CPSR] | 0x00000020); // If bit 0 == 0, then ARM; else, THUMB
};

void Gba::armCmn(uint32_t op1, uint32_t op2) {
    uint64_t res_e = op1 + op2;
    bool c_flag = (res_e >= 0x100000000);
    bool v_flag = false;
    uint32_t res = res_e;

    if ((op1 & 0x80000000) == (op2 & 0x80000000) && (op1 & 0x80000000) != (res & 0x80000000))
        v_flag = true; // Set V

    armAriSetCond(res, c_flag, v_flag, 0); // rd = 0 will allow the setting / un-setting of condition bits
};

void Gba::armCmp(uint32_t op1, uint32_t op2) {
    bool c_flag = (op2 <= op1);
    bool v_flag = false;
    uint32_t res = op1 - op2;

    if ((op1 & 0x80000000) != (op2 & 0x80000000) && (res & 0x80000000) != (op1 & 0x80000000)) // res must have different sign from minuend, which is op1
        v_flag = true;

    armAriSetCond(res, c_flag, v_flag, 0); // rd = 0 will allow the setting / un-setting of condition bits
};

void Gba::armEor(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    uint32_t res = op1 ^ op2;
    regRef(rd) = res;
    if (set_cond) armLogSetCond(res, rd);
};

void Gba::armLdr(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset) {
    uint32_t base = regRef(rn);
    uint32_t res_addr = base;

    if (add_offset)
        res_addr += offset;
    else // Subtract the offset
        res_addr -= offset;

    if (pre_ind) { // Add offset before transfer (use res_addr)
        uint32_t w = 
            static_cast<uint32_t>(memRef(res_addr + 3) << 24) |
            static_cast<uint32_t>(memRef(res_addr + 2) << 16) |
            static_cast<uint32_t>(memRef(res_addr + 1) << 8) |
            static_cast<uint32_t>(memRef(res_addr));

        regRef(rd) = w;
    }else { // Add offset after transfer (use base)
        uint32_t w = 
            static_cast<uint32_t>(memRef(base + 3) << 24) |
            static_cast<uint32_t>(memRef(base + 2) << 16) |
            static_cast<uint32_t>(memRef(base + 1) << 8) |
            static_cast<uint32_t>(memRef(base));

        regRef(rd) = w;
    }

    if (write_back)
        regRef(rn) = res_addr;
};

void Gba::armLdrb(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset) {

};

void Gba::armLsl(bool set_cond, uint8_t shift_am, uint32_t& op) {
    if (shift_am > 0) {
        op = op << (shift_am - 1); // Least significant shifted out bit needs to be saved in CPSR as the carry out
        if (set_cond) reg[CPSR] = (op & 0x80000000) == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
        op = op << 1;
    } // Else, use op2 directly (LSL #0)
};

void Gba::armLsr(bool set_cond, uint8_t shift_am, uint32_t& op) {
    if (shift_am == 0) { // LSR #0 becomes LSR #32; zero result and bit 31 becomes carry out
        if (set_cond) reg[CPSR] = (op & 0x80000000) == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
        op = 0;
    }else {
        op = op >> (shift_am - 1); // Most significant shifted out bit needs to be saved in CPSR as the carry out
        if (set_cond) reg[CPSR] = (op & 0x00000001) == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
        op = op >> 1;
    }
};

void Gba::armMla(bool set_cond, uint32_t op1, uint32_t op2, uint32_t acc, uint8_t rd) {
    uint32_t res = (op1 * op2) + acc;
    regRef(rd) = res;

    if (set_cond) { // N and Z flags affected
        if (res == 0) reg[CPSR] = (reg[CPSR] & 0x7fffffff) | 0x40000000; // Un-set N, set Z
        else if ((res & 0x80000000) == 0x80000000) reg[CPSR] = (reg[CPSR] & 0xbfffffff) | 0x80000000; // Un-set Z, set N
    }
};

void Gba::armMlal(bool set_cond, bool sign, uint32_t op1, uint32_t op2, uint8_t rd_h, uint8_t rd_l) {
    if (sign) {
        int64_t acc = (static_cast<int64_t>(regRef(rd_h)) << 32) | static_cast<int64_t>(regRef(rd_l));
        int64_t res = static_cast<int64_t>(op1) * static_cast<int64_t>(op2) + acc;
        regRef(rd_h) = static_cast<int32_t>(res >> 32);
        regRef(rd_l) = static_cast<int32_t>(res);

        if (set_cond) { // Only N and Z flags are affected
            if (res == 0) reg[CPSR] = (reg[CPSR] & 0x7fffffff) | 0x40000000; // Un-set N, set Z
            else if ((res & 0x8000000000000000) == 0x8000000000000000) reg[CPSR] = (reg[CPSR] & 0xbfffffff) | 0x80000000; // Un-set Z, set N
        }
    }else {
        uint64_t acc = (static_cast<uint64_t>(regRef(rd_h)) << 32) | static_cast<uint64_t>(regRef(rd_l));
        uint64_t res = op1 * op2 + acc;
        regRef(rd_h) = static_cast<uint32_t>(res >> 32);
        regRef(rd_l) = static_cast<uint32_t>(res);

        if (set_cond) { // Only N and Z flags are affected
            if (res == 0) reg[CPSR] = (reg[CPSR] & 0x7fffffff) | 0x40000000; // Un-set N, set Z
            else if ((res & 0x8000000000000000) == 0x8000000000000000) reg[CPSR] = (reg[CPSR] & 0xbfffffff) | 0x80000000; // Un-set Z, set N
        }
    }
};

void Gba::armMov(bool set_cond, uint32_t op2, uint8_t rd) {
    regRef(rd) = op2;
    if (set_cond) armLogSetCond(op2, rd);
};

void Gba::armMul(bool set_cond, uint32_t op1, uint32_t op2, uint32_t& acc, uint8_t rd) {
    acc = 0; // Accumulator register should be zero'd
    uint32_t res = op1 * op2;
    regRef(rd) = res;

    if (set_cond) { // Only N and Z flags are affected
        if (res == 0) reg[CPSR] = (reg[CPSR] & 0x7fffffff) | 0x40000000; // Un-set N, set Z
        else if ((res & 0x80000000) == 0x80000000) reg[CPSR] = (reg[CPSR] & 0xbfffffff) | 0x80000000; // Un-set Z, set N
    }
};

void Gba::armMull(bool set_cond, bool sign, uint32_t op1, uint32_t op2, uint8_t rd_h, uint8_t rd_l) {
    if (sign) {
        int64_t res = static_cast<int64_t>(op1) * static_cast<int64_t>(op2);
        regRef(rd_h) = static_cast<int32_t>(res >> 32);
        regRef(rd_l) = static_cast<int32_t>(res);

        if (set_cond) { // Only N and Z flags are affected
            if (res == 0) reg[CPSR] = (reg[CPSR] & 0x7fffffff) | 0x40000000; // Un-set N, set Z
            else if ((res & 0x8000000000000000) == 0x8000000000000000) reg[CPSR] = (reg[CPSR] & 0xbfffffff) | 0x80000000; // Un-set Z, set N
        }
    }else {
        uint64_t res = op1 * op2;
        regRef(rd_h) = static_cast<uint32_t>(res >> 32);
        regRef(rd_l) = static_cast<uint32_t>(res);

        if (set_cond) { // Only N and Z flags are affected
            if (res == 0) reg[CPSR] = (reg[CPSR] & 0x7fffffff) | 0x40000000; // Un-set N, set Z
            else if ((res & 0x8000000000000000) == 0x8000000000000000) reg[CPSR] = (reg[CPSR] & 0xbfffffff) | 0x80000000; // Un-set Z, set N
        }
    }
};

void Gba::armMrs(uint8_t psr_num, uint8_t rd) { regRef(rd) = reg[psr_num]; };

void Gba::armMsr(uint8_t psr_num, uint32_t w) {
    // * See Figure 4-11: PSR Transfer; bit 16 for distinguishing between MSR (reg contents to PSR) and MSR (reg contents / imm val to PSR flag bits only)
    if ((w & 0x00010000) == 0x00000000) { // Reg contents / imm val to PSR flags only
        if ((w & 0x02000000) == 0x00000000) { // Operand is a reg
            reg[psr_num] = (reg[psr_num] & 0x0fffffff) | (regRef(static_cast<uint8_t>(w & 0x0000000f)) & 0xf0000000);
        }else { // Operand is an immediate rotated value
            uint8_t rotate = (w & 0x00000f00) >> 8;
            uint32_t imm_val = w & 0x000000ff;
            armRor(false, rotate, imm_val);
            reg[psr_num] = (reg[psr_num] & 0x0fffffff) | (imm_val & 0xf0000000);
        }
    }else // Reg contents to PSR
        reg[psr_num] = (reg[psr_num] & 0x0fffff00) | (regRef(static_cast<uint8_t>(w & 0x0000000f)) & 0xf00000ff);
};

void Gba::armMvn(bool set_cond, uint32_t op2, uint8_t rd) {
    uint32_t res = ~op2;
    regRef(rd) = res;
    if (set_cond) armLogSetCond(res, rd);
};

void Gba::armOrr(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    uint32_t res = op1 | op2;
    regRef(rd) = res;
    if (set_cond) armLogSetCond(res, rd);
};

void Gba::armRor(bool set_cond, uint8_t shift_am, uint32_t& op) {
    if (shift_am == 0) { // ROR #0 becomes RRX (rotate right extended); bit 0 carried out, and CPSR's carry is carried into bit 31
        uint8_t lsb = op & 0x00000001;
        op = (op >> 1) | ((reg[CPSR] & 0x2000000) == 0x00000000 ? 0x00000000 : 0x80000000);
        if (set_cond) reg[CPSR] = lsb == 0x00 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
    }else {
        for(uint8_t i = 0; i < (shift_am - 1); ++i) { // Most significant shifted out bit needs to be saved in CPSR as the carry out
            uint8_t lsb = op & 0x00000001;
            op = (op >> 1) | (lsb == 0x00 ? 0x00000000 : 0x80000000);
        }

        uint8_t lsb = op & 0x00000001;
        if (set_cond) reg[CPSR] = lsb == 0x00 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
        op = (op >> 1) | (lsb == 0x00 ? 0x00000000 : 0x80000000);
    }
};

void Gba::armRsb(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    bool c_flag = (op1 <= op2); // True when borrow does NOT occur or when subtrahend <= minuend
    bool v_flag = false;
    uint32_t res = op2 - op1;

    if ((op1 & 0x80000000) != (op2 & 0x80000000) && (res & 0x80000000) != (op2 & 0x80000000)) // res must have different sign from minuend, which is op2
        v_flag = true;

    regRef(rd) = res;
    if (set_cond) armAriSetCond(res, c_flag, v_flag, rd);
};

void Gba::armRsc(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    op1 = op1 - (1 - ((reg[CPSR] & 0x20000000) == 0x20000000 ? 1 : 0));
    bool c_flag = (op1 <= op2);
    bool v_flag = false;
    uint32_t res = op2 - op1;

    if ((op1 & 0x80000000) != (op2 & 0x80000000) && (res & 0x80000000) != (op2 & 0x80000000)) // res must have different sign from minuend, which is op2
        v_flag = true;

    regRef(rd) = res;
    if (set_cond) armAriSetCond(res, c_flag, v_flag, rd);
};

void Gba::armSbc(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    op2 = op2 - (1 - ((reg[CPSR] & 0x20000000) == 0x20000000 ? 1 : 0));
    bool c_flag = (op2 <= op1);
    bool v_flag = false;
    uint32_t res = op1 - op2;

    if ((op1 & 0x80000000) != (op2 & 0x80000000) && (res & 0x80000000) != (op1 & 0x80000000)) // res must have different sign from minuend, which is op1
        v_flag = true;

    regRef(rd) = res;
    if (set_cond) armAriSetCond(res, c_flag, v_flag, rd);
};

void Gba::armSub(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    bool c_flag = (op2 <= op1); // True when borrow does NOT occur or when subtrahend <= minuend
    bool v_flag = false;
    uint32_t res = op1 - op2;

    if ((op1 & 0x80000000) != (op2 & 0x80000000) && (res & 0x80000000) != (op1 & 0x80000000)) // res must have different sign from minuend, which is op1
        v_flag = true;

    regRef(rd) = res;
    if (set_cond) armAriSetCond(res, c_flag, v_flag, rd);
};

void Gba::armTeq(uint32_t op1, uint32_t op2) {
    uint32_t res = op1 ^ op2;
    armLogSetCond(res, 0); // rd = 0 will allow the setting / un-setting of condition bits
};

void Gba::armTst(uint32_t op1, uint32_t op2) {
    uint32_t res = op1 & op2;
    armLogSetCond(res, 0); // rd = 0 will allow the setting / un-setting of condition bits
};