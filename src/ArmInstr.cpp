#include "../include/Gba.hpp"

void Gba::armAdc(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    op2 = op2 + (cFlag() ? 1 : 0);
    uint64_t res_e = static_cast<uint64_t>(op1) + static_cast<uint64_t>(op2);
    bool c_flag = ((res_e & 0x0000000100000000) == 0x100000000);
    bool v_flag = false;
    uint32_t res = res_e;

    if ((op1 & 0x80000000) == (op2 & 0x80000000) && (op1 & 0x80000000) != (res & 0x80000000))
        v_flag = true; // Set V
    
    regRef(rd) = res;
    if (set_cond) armAriSetCond(res, c_flag, v_flag, rd);
};

void Gba::armAdd(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    uint64_t res_e = static_cast<uint64_t>(op1) + static_cast<uint64_t>(op2);
    bool c_flag = ((res_e & 0x0000000100000000) == 0x100000000);
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
    uint32_t sign = (op & 0x80000000) == 0 ? 0 : 0x80000000;

    if (shift_am == 0) { // ASR #0 becomes ASR #32; all 0s or 1s result and bit 31 becomes carry out
        if (set_cond) reg[CPSR] = (sign == 0 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000);
        op = (sign == 0 ? 0 : 0xffffffff);
    }else {
        for(uint8_t i = 0; i < (shift_am - 1); ++i) // Most significant shifted out bit needs to be saved in CPSR as the carry out
            op = (op >> 1) | sign;

        if (set_cond) reg[CPSR] = (op & 0x00000001) == 0 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
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
    reg[CPSR] = ((w & 0x00000001) == 0 ? reg[CPSR] & 0xffffffdf : reg[CPSR] | 0x00000020); // If bit 0 == 0, then ARM; else, THUMB
};

void Gba::armCmn(uint32_t op1, uint32_t op2) {
    uint64_t res_e = static_cast<uint64_t>(op1) + static_cast<uint64_t>(op2);
    bool c_flag = ((res_e & 0x0000000100000000) == 0x100000000);
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

void Gba::armLdm(bool pre_ind, bool add_offset, bool s, bool write_back, uint8_t rn, uint16_t reg_list) {
    uint32_t addr = regRef(rn) & 0xfffffffc; // word-aligned address from base reg

    bool revert_mode = false;
    uint8_t cur_mode = static_cast<uint8_t>(reg[CPSR] & 0x0000001f);
    if (s && (reg_list & 0x8000) == 0x0000) {
        setMode(USR); // * See 4.11.4, Use of the S bit: PC not in reg_list and S set means user bank registers are used
        write_back = false;
        revert_mode = true;
    }else if (s && (reg_list & 0x8000) == 0x8000) {
        switch(cur_mode) {
            case FIQ:
                reg[CPSR] = reg[SPSR_fiq];
                break;

            case SVC:
                reg[CPSR] = reg[SPSR_svc];
                break;

            case ABT:
                reg[CPSR] = reg[SPSR_abt];
                break;

            case IRQ:
                reg[CPSR] = reg[SPSR_irq];
                break;

            case UND:
                reg[CPSR] = reg[SPSR_und];
                break;

            default:
                revert_mode = true;
                break;
        }
    }

    for(uint8_t i = 0; i < 16; ++i) {
        uint32_t offset_addr = addr;
        uint8_t reg_num;
        uint32_t w;

        if (add_offset) {
            bool in_list = (reg_list & 0x0001) == 0x0001;
            reg_list = reg_list >> 1;

            if (!in_list) continue; // Register not in list; go to next iteration

            reg_num = i;
            offset_addr += 4;
        }else{
            bool in_list = (reg_list & 0x8000) == 0x8000;
            reg_list = reg_list << 1;

            if (!in_list) continue;

            reg_num = 15 - i;
            offset_addr -= 4;
        }

        if (reg_num == rn)
            write_back = false; // * See 4.11.2, Addressing modes: Any write back to the base register is overwritten by the load operation

        if (pre_ind) {
            w = 
                (static_cast<uint32_t>(memRef(offset_addr + 3)) << 24) |
                (static_cast<uint32_t>(memRef(offset_addr + 2)) << 16) |
                (static_cast<uint32_t>(memRef(offset_addr + 1)) << 8) |
                static_cast<uint32_t>(memRef(offset_addr));
        }else {
            w = 
                (static_cast<uint32_t>(memRef(addr + 3)) << 24) |
                (static_cast<uint32_t>(memRef(addr + 2)) << 16) |
                (static_cast<uint32_t>(memRef(addr + 1)) << 8) |
                static_cast<uint32_t>(memRef(addr));
        }

        regRef(reg_num) = w;
        addr = offset_addr;
    }

    if (write_back)
        regRef(rn) = addr;

    if (revert_mode)
        setMode(cur_mode);
}

void Gba::armLdr(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset) {
    uint32_t base = regRef(rn);
    uint32_t res_addr = base;

    if (add_offset)
        res_addr += offset;
    else // Subtract the offset
        res_addr -= offset;

    if (pre_ind) { // Add / subtract offset before transfer (use res_addr)
        // Need to rotate the bytes if res_addr is not word-aligned
        uint32_t wa_addr = (res_addr - (res_addr % 4)); // wa_addr = word-aligned address

        uint32_t w = 
            (static_cast<uint32_t>(memRef(wa_addr + ((res_addr + 3) % 4))) << 24) |
            (static_cast<uint32_t>(memRef(wa_addr + ((res_addr + 2) % 4))) << 16) |
            (static_cast<uint32_t>(memRef(wa_addr + ((res_addr + 1) % 4))) << 8) |
            static_cast<uint32_t>(memRef(wa_addr + (res_addr % 4)));

        regRef(rd) = w;
    }else { // Add offset after transfer (use base)
        // Need to rotate the bytes if base is not word-aligned
        uint32_t wa_addr = (base - (base % 4));

        uint32_t w = 
            (static_cast<uint32_t>(memRef(wa_addr + ((base + 3) % 4))) << 24) |
            (static_cast<uint32_t>(memRef(wa_addr + ((base + 2) % 4))) << 16) |
            (static_cast<uint32_t>(memRef(wa_addr + ((base + 1) % 4))) << 8) |
            static_cast<uint32_t>(memRef(wa_addr + (base % 4)));

        regRef(rd) = w;
    }

    if (write_back || !pre_ind) // * See 4.9.1, Offsets and auto-indexing: post-indexing always writes back to the base reg
        regRef(rn) = res_addr;
};

void Gba::armLdrb(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset) {
    uint32_t base = regRef(rn);
    uint32_t res_addr = base;

    if (add_offset)
        res_addr += offset;
    else // Subtract the offset
        res_addr -= offset;

    // * See 4.9.3, Bytes and words: Selected byte goes to bottom 8 bits of destination reg, rest of bits are 0.
    if (pre_ind) { // Add / subtract offset before transfer (use res_addr)
        uint32_t w = static_cast<uint32_t>(memRef(res_addr));
        regRef(rd) = w;
    }else { // Add offset after transfer (use base)
        uint32_t w = static_cast<uint32_t>(memRef(base));
        regRef(rd) = w;
    }

    if (write_back || !pre_ind)
        regRef(rn) = res_addr;
};

void Gba::armLdrh(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset) {
    uint32_t base = regRef(rn);
    uint32_t res_addr = base;

    if (add_offset)
        res_addr += offset;
    else // Subtract the offset
        res_addr -= offset;

    if (pre_ind) { // Add / subtract offset before transfer (use res_addr)
        uint32_t a_addr = res_addr & 0xfffffffe; // a_addr = aligned address; must be word or half-word aligned (bit 0 must be 0)

        uint32_t w = 
            (static_cast<uint32_t>(memRef(a_addr + 1)) << 8) |
            static_cast<uint32_t>(memRef(a_addr));

        regRef(rd) = w;
    }else { // Add offset after transfer (use base)
        uint32_t a_addr = base & 0xfffffffe;

        uint32_t w =
            (static_cast<uint32_t>(memRef(a_addr + 1)) << 8) |
            static_cast<uint32_t>(memRef(a_addr));

        regRef(rd) = w;
    }

    if (write_back || !pre_ind) // * See 4.9.1, Offsets and auto-indexing: post-indexing always writes back to the base reg
        regRef(rn) = res_addr;
};

void Gba::armLdrsb(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset) {
    uint32_t base = regRef(rn);
    uint32_t res_addr = base;

    if (add_offset)
        res_addr += offset;
    else // Subtract the offset
        res_addr -= offset;

    if (pre_ind) { // Add / subtract offset before transfer (use res_addr)
        int32_t w = static_cast<int32_t>(static_cast<int8_t>(memRef(res_addr)));
        regRef(rd) = w;
    }else { // Add offset after transfer (use base)
        int32_t w = static_cast<int32_t>(static_cast<int8_t>(memRef(base)));
        regRef(rd) = w;
    }

    if (write_back || !pre_ind) // * See 4.9.1, Offsets and auto-indexing: post-indexing always writes back to the base reg
        regRef(rn) = res_addr;
};

void Gba::armLdrsh(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset) {
    uint32_t base = regRef(rn);
    uint32_t res_addr = base;

    if (add_offset)
        res_addr += offset;
    else // Subtract the offset
        res_addr -= offset;

    if (pre_ind) { // Add / subtract offset before transfer (use res_addr)
        uint32_t a_addr = res_addr & 0xfffffffe; // a_addr = aligned address; must be word or half-word aligned (bit 0 must be 0)

        int32_t w = static_cast<int32_t>(static_cast<int16_t>( 
            (static_cast<uint16_t>(memRef(a_addr + 1)) << 8) |
            static_cast<uint16_t>(memRef(a_addr))
        ));

        regRef(rd) = w;
    }else { // Add offset after transfer (use base)
        uint32_t a_addr = base & 0xfffffffe;

        int32_t w = static_cast<int32_t>(static_cast<int16_t>(
            (static_cast<uint32_t>(memRef(a_addr + 1)) << 8) |
            static_cast<uint32_t>(memRef(a_addr))
        ));

        regRef(rd) = w;
    }

    if (write_back || !pre_ind) // * See 4.9.1, Offsets and auto-indexing: post-indexing always writes back to the base reg
        regRef(rn) = res_addr;
};

void Gba::armLsl(bool set_cond, uint8_t shift_am, uint32_t& op) {
    if (shift_am > 0) {
        op = op << (shift_am - 1); // Least significant shifted out bit needs to be saved in CPSR as the carry out
        if (set_cond) reg[CPSR] = (op & 0x80000000) == 0 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
        op = op << 1;
    } // Else, use op2 directly (LSL #0)
};

void Gba::armLsr(bool set_cond, uint8_t shift_am, uint32_t& op) {
    if (shift_am == 0) { // LSR #0 becomes LSR #32; zero result and bit 31 becomes carry out
        if (set_cond) reg[CPSR] = ((op & 0x80000000) == 0 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000);
        op = 0;
    }else {
        op = op >> (shift_am - 1); // Most significant shifted out bit needs to be saved in CPSR as the carry out
        if (set_cond) reg[CPSR] = ((op & 0x00000001) == 0 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000);
        op = op >> 1;
    }
};

void Gba::armMla(bool set_cond, uint32_t op1, uint32_t op2, uint32_t acc, uint8_t rd) {
    uint32_t res = (op1 * op2) + acc;
    regRef(rd) = res;

    if (set_cond) { // N and Z flags affected
        if (res == 0)
            reg[CPSR] = reg[CPSR] | 0x40000000; // Set Z
        else
            reg[CPSR] = reg[CPSR] & 0xbfffffff; // Un-set Z

        if ((res & 0x80000000) == 0x80000000)
            reg[CPSR] = reg[CPSR] | 0x80000000; // Set N
        else
            reg[CPSR] = reg[CPSR] & 0x7fffffff; // Un-set N
    }
};

void Gba::armMlal(bool set_cond, bool sign, uint32_t op1, uint32_t op2, uint8_t rd_h, uint8_t rd_l) {
    if (sign) {
        int64_t op1_e = static_cast<int64_t>(static_cast<int32_t>(op1));
        int64_t op2_e = static_cast<int64_t>(static_cast<int32_t>(op2));

        int64_t acc = (static_cast<int64_t>(regRef(rd_h)) << 32) | static_cast<int64_t>(regRef(rd_l));
        int64_t res = op1_e * op2_e + acc;
        regRef(rd_h) = static_cast<int32_t>(res >> 32);
        regRef(rd_l) = static_cast<int32_t>(res);

        if (set_cond) { // Only N and Z flags are affected
            if (res == 0)
                reg[CPSR] = reg[CPSR] | 0x40000000; // Set Z
            else
                reg[CPSR] = reg[CPSR] & 0xbfffffff; // Un-set Z
            
            if ((res & 0x8000000000000000) == 0x8000000000000000)
                reg[CPSR] = reg[CPSR] | 0x80000000; // Set N
            else
                reg[CPSR] = reg[CPSR] & 0x7fffffff; // Un-set N
        }
    }else {
        uint64_t acc = (static_cast<uint64_t>(regRef(rd_h)) << 32) | static_cast<uint64_t>(regRef(rd_l));
        uint64_t res = static_cast<uint64_t>(op1) * static_cast<uint64_t>(op2) + acc;
        regRef(rd_h) = static_cast<uint32_t>(res >> 32);
        regRef(rd_l) = static_cast<uint32_t>(res);

        if (set_cond) { // Only N and Z flags are affected
            if (res == 0)
                reg[CPSR] = reg[CPSR] | 0x40000000; // Set Z
            else
                reg[CPSR] = reg[CPSR] & 0xbfffffff; // Un-set Z
            
            if ((res & 0x8000000000000000) == 0x8000000000000000)
                reg[CPSR] = reg[CPSR] | 0x80000000; // Set N
            else
                reg[CPSR] = reg[CPSR] & 0x7fffffff; // Un-set N
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
        if (res == 0) 
            reg[CPSR] = reg[CPSR] | 0x40000000; // Set Z
        else
            reg[CPSR] = reg[CPSR] & 0xbfffffff; // Un-set Z
        
        if ((res & 0x80000000) == 0x80000000)
            reg[CPSR] = reg[CPSR] | 0x80000000; // Set N
        else
            reg[CPSR] = reg[CPSR] & 0x7fffffff; // Un-set N
    }
};

void Gba::armMull(bool set_cond, bool sign, uint32_t op1, uint32_t op2, uint8_t rd_h, uint8_t rd_l) {
    if (sign) {
        int64_t op1_e = static_cast<int64_t>(static_cast<int32_t>(op1));
        int64_t op2_e = static_cast<int64_t>(static_cast<int32_t>(op2));

        int64_t res = op1_e * op2_e;
        regRef(rd_h) = static_cast<int32_t>(res >> 32);
        regRef(rd_l) = static_cast<int32_t>(res);

        if (set_cond) { // Only N and Z flags are affected
            if (res == 0)
                reg[CPSR] = reg[CPSR] | 0x40000000; // Set Z
            else
                reg[CPSR] = reg[CPSR] & 0xbfffffff; // Un-set Z
            
            if ((res & 0x8000000000000000) == 0x8000000000000000)
                reg[CPSR] = reg[CPSR] | 0x80000000; // Set N
            else
                reg[CPSR] = reg[CPSR] & 0x7fffffff; // Un-set N
        }
    }else {
        uint64_t res = static_cast<uint64_t>(op1) * static_cast<uint64_t>(op2);
        regRef(rd_h) = static_cast<uint32_t>(res >> 32);
        regRef(rd_l) = static_cast<uint32_t>(res);

        if (set_cond) { // Only N and Z flags are affected
            if (res == 0)
                reg[CPSR] = reg[CPSR] | 0x40000000; // Set Z
            else
                reg[CPSR] = reg[CPSR] & 0xbfffffff; // Un-set Z
            
            if ((res & 0x8000000000000000) == 0x8000000000000000)
                reg[CPSR] = reg[CPSR] | 0x80000000; // Set N
            else
                reg[CPSR] = reg[CPSR] & 0x7fffffff; // Un-set N
        }
    }
};

void Gba::armMrs(uint8_t psr_num, uint8_t rd) { regRef(rd) = reg[psr_num]; };

void Gba::armMsr(uint8_t psr_num, uint32_t w) {
    // * See Figure 4-11: PSR Transfer; bit 16 for distinguishing between MSR (reg contents to PSR) and MSR (reg contents / imm val to PSR flag bits only)
    if ((w & 0x00010000) == 0) { // Reg contents / imm val to PSR flags only
        if ((w & 0x02000000) == 0) { // Operand is a reg
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
        op = (op >> 1) | (cFlag() ? 0x80000000 : 0);
        if (set_cond) reg[CPSR] = (lsb == 0 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000);
    }else {
        for(uint8_t i = 0; i < (shift_am - 1); ++i) { // Most significant shifted out bit needs to be saved in CPSR as the carry out
            uint8_t lsb = op & 0x00000001;
            op = (op >> 1) | (lsb == 0 ? 0 : 0x80000000);
        }

        uint8_t lsb = op & 0x00000001;
        if (set_cond) reg[CPSR] = (lsb == 0 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000);
        op = (op >> 1) | (lsb == 0 ? 0 : 0x80000000);
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
    op1 = op1 - (1 - (cFlag() ? 1 : 0));
    bool c_flag = (op1 <= op2);
    bool v_flag = false;
    uint32_t res = op2 - op1;

    if ((op1 & 0x80000000) != (op2 & 0x80000000) && (res & 0x80000000) != (op2 & 0x80000000)) // res must have different sign from minuend, which is op2
        v_flag = true;

    regRef(rd) = res;
    if (set_cond) armAriSetCond(res, c_flag, v_flag, rd);
};

void Gba::armSbc(bool set_cond, uint32_t op1, uint32_t op2, uint8_t rd) {
    op2 = op2 - (1 - (cFlag() ? 1 : 0));
    bool c_flag = (op2 <= op1);
    bool v_flag = false;
    uint32_t res = op1 - op2;

    if ((op1 & 0x80000000) != (op2 & 0x80000000) && (res & 0x80000000) != (op1 & 0x80000000)) // res must have different sign from minuend, which is op1
        v_flag = true;

    regRef(rd) = res;
    if (set_cond) armAriSetCond(res, c_flag, v_flag, rd);
};

void Gba::armStm(bool pre_ind, bool add_offset, bool s, bool write_back, uint8_t rn, uint16_t reg_list) {
    uint32_t addr = regRef(rn) & 0xfffffffc; // word-aligned address from base reg

    uint8_t cur_mode = static_cast<uint8_t>(reg[CPSR] & 0x0000001f);
    if (s) {
        setMode(USR); // * See 4.11.4, Use of the S bit: S set means user bank registers are used
        write_back = false;
    }

    for(uint8_t i = 0; i < 16; ++i) {
        uint32_t offset_addr = addr;
        uint8_t reg_num;

        if (add_offset) {
            bool in_list = (reg_list & 0x0001) == 0x0001;
            reg_list = reg_list >> 1;

            if (!in_list) continue; // Register not in list; go to next iteration

            reg_num = i;
            offset_addr += 4;
        }else{
            bool in_list = (reg_list & 0x8000) == 0x8000;
            reg_list = reg_list << 1;

            if (!in_list) continue;

            reg_num = 15 - i;
            offset_addr -= 4;
        }

        uint32_t w = regRef(reg_num);

        if (pre_ind) {
            memRef(offset_addr + 3) = static_cast<uint8_t>(w >> 24);
            memRef(offset_addr + 2) = static_cast<uint8_t>(w >> 16);
            memRef(offset_addr + 1) = static_cast<uint8_t>(w >> 8);
            memRef(offset_addr) = static_cast<uint8_t>(w);
        }else {
            memRef(addr + 3) = static_cast<uint8_t>(w >> 24);
            memRef(addr + 2) = static_cast<uint8_t>(w >> 16);
            memRef(addr + 1) = static_cast<uint8_t>(w >> 8);
            memRef(addr) = static_cast<uint8_t>(w);
        }

        addr = offset_addr;
    }

    if (write_back)
        regRef(rn) = addr;

    setMode(cur_mode);
}

void Gba::armStr(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset) {
    uint32_t base = regRef(rn);
    uint32_t res_addr = base;
    uint32_t w = regRef(rd);

    if (add_offset)
        res_addr += offset;
    else // Subtract the offset
        res_addr -= offset;

    if (pre_ind) { // Add / subtract offset before transfer (use res_addr)
        // * See 4.9.3, Bytes and words: STR must generate a word-aligned address
        uint32_t wa_addr = (res_addr - (res_addr % 4)); // wa_addr = word-aligned address

        memRef(wa_addr) = static_cast<uint8_t>(w & 0x000000ff);
        memRef(wa_addr + 1) = static_cast<uint8_t>((w >> 8) & 0x000000ff);
        memRef(wa_addr + 2) = static_cast<uint8_t>((w >> 16) & 0x000000ff);
        memRef(wa_addr + 3) = static_cast<uint8_t>((w >> 24) & 0x000000ff);
    }else { // Add offset after transfer (use base)
        uint32_t wa_addr = (base - (base % 4));

        memRef(wa_addr) = static_cast<uint8_t>(w & 0x000000ff);
        memRef(wa_addr + 1) = static_cast<uint8_t>((w >> 8) & 0x000000ff);
        memRef(wa_addr + 2) = static_cast<uint8_t>((w >> 16) & 0x000000ff);
        memRef(wa_addr + 3) = static_cast<uint8_t>((w >> 24) & 0x000000ff);
    }

    if (write_back || !pre_ind)
        regRef(rn) = res_addr;
};


void Gba::armStrb(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset) {
    uint32_t base = regRef(rn);
    uint32_t res_addr = base;
    uint8_t b = static_cast<uint8_t>(regRef(rd) & 0x000000ff);

    if (add_offset)
        res_addr += offset;
    else // Subtract the offset
        res_addr -= offset;

    if (pre_ind) // Add / subtract offset before transfer (use res_addr)
        memRef(res_addr) = b;
    else // Add offset after transfer (use base)
        memRef(base) = b;

    if (write_back || !pre_ind)
        regRef(rn) = res_addr;
};

void Gba::armStrh(bool pre_ind, bool add_offset, bool write_back, uint8_t rn, uint8_t rd, uint32_t offset) {
    uint32_t base = regRef(rn);
    uint32_t res_addr = base;
    uint16_t hw = static_cast<uint16_t>(regRef(rd));

    if (add_offset)
        res_addr += offset;
    else // Subtract the offset
        res_addr -= offset;

    if (pre_ind) { // Add / subtract offset before transfer (use res_addr)
        uint32_t a_addr = res_addr & 0xfffffffe; // a_addr = aligned address; must be word or half-word aligned (bit 0 must be 0)
        memRef(a_addr) = static_cast<uint8_t>(hw & 0x00ff);
        memRef(a_addr + 1) = static_cast<uint8_t>((hw >> 8) & 0x00ff);
    }else { // Add offset after transfer (use base)
        uint32_t a_addr = base & 0xfffffffe;
        memRef(a_addr) = static_cast<uint8_t>(hw & 0x00ff);
        memRef(a_addr + 1) = static_cast<uint8_t>((hw >> 8) & 0x00ff);
    }

    if (write_back || !pre_ind)
        regRef(rn) = res_addr;
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

void Gba::armSwp(bool word, uint8_t rn, uint8_t rd, uint8_t rm) {
    if (word) {
        armLdr(true, false, false, rn, 16, 0);  // Load from memory into temp_reg
        armStr(true, false, false, rn, rm, 0);  // Write to memory from source reg
        regRef(rd) = temp_reg;                  // Put temp_reg value into dest reg
    }else {
        armLdrb(true, false, false, rn, 16, 0);
        armStrb(true, false, false, rn, rm, 0);
        regRef(rd) = temp_reg;
    }
}

void Gba::armTeq(uint32_t op1, uint32_t op2) {
    uint32_t res = op1 ^ op2;
    armLogSetCond(res, 0); // rd = 0 will allow the setting / un-setting of condition bits
};

void Gba::armTst(uint32_t op1, uint32_t op2) {
    uint32_t res = op1 & op2;
    armLogSetCond(res, 0); // rd = 0 will allow the setting / un-setting of condition bits
};