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
    uint32_t r = regRef(rn);
    reg[PC] = r;
    reg[CPSR] = ((r & 0x00000001) == 0x00000000 ? reg[CPSR] & 0xffffffdf : reg[CPSR] | 0x00000020); // If bit 0 == 0, then ARM; else, THUMB
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

void Gba::armMla(bool set_cond, uint32_t op1, uint32_t op2, uint32_t acc, uint8_t rd) {
    uint32_t res = (op1 * op2) + acc;
    regRef(rd) = res;

    if (set_cond) { // N and Z flags affected
        if (res == 0) reg[CPSR] = (reg[CPSR] & 0x7fffffff) | 0x40000000; // Un-set N, set Z
        else if ((res & 0x80000000) == 0x80000000) reg[CPSR] = (reg[CPSR] & 0xbfffffff) | 0x80000000; // Un-set Z, set N
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