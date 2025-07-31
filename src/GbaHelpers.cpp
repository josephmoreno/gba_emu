#include "../include/Gba.hpp"

// * See figure 3-3: ARM State General Registers and Program Counter
// All registers EXCEPT the program status registers and PC should be read and written to through this function
uint32_t& Gba::regRef(uint8_t reg_num) {
    uint8_t op_mode = static_cast<uint8_t>(reg[CPSR] & 0x0000001f);

    if (
        reg_num == PC ||
        reg_num >= CPSR ||
        (reg_num <= 0x0f && (op_mode == SYS || op_mode == USR)) ||
        (reg_num <= 0x07 && (op_mode >= FIQ && op_mode <= UND)) ||
        (reg_num <= 0x0c && (op_mode >= IRQ && op_mode <= UND))
    )
        return(reg[reg_num]);
    else if (reg_num >= 0x08 && reg_num <= 0x0e && op_mode == FIQ)
        return(reg[R8_fiq + (reg_num - 0x08)]);
    else if ((reg_num == 0x0d || reg_num == 0x0e) && (op_mode >= IRQ && op_mode <= UND)) {
        switch(op_mode) {
            case IRQ: return(reg[R13_irq + (reg_num - 0x0d)]);
            case SVC: return(reg[R13_svc + (reg_num - 0x0d)]);
            case ABT: return(reg[R13_abt + (reg_num - 0x0d)]);
            case UND: return(reg[R13_und + (reg_num - 0x0d)]);
            default: return(reg[reg_num]);
        }
    }else
        return(reg[reg_num]);
};

uint8_t& Gba::memRef(uint32_t addr) {
    if (addr >= sys_rom_offset && addr <= sys_rom_offset + sys_rom_size)
        return(sys_rom[addr]);
    else if (addr >= ob_wram_offset && addr <= ob_wram_offset + ob_wram_size)
        return(ob_wram[addr - ob_wram_offset]);
    else if (addr >= oc_wram_offset && addr <= oc_wram_offset + oc_wram_size)
        return(oc_wram[addr - oc_wram_offset]);
    else if (addr >= io_ram_offset && addr <= io_ram_offset + io_ram_size)
        return(io_ram[addr - io_ram_offset]);
    else if (addr >= palette_ram_offset && addr <= palette_ram_offset + palette_ram_size)
        return(palette_ram[addr - palette_ram_offset]);
    else if (addr >= vram_offset && addr <= vram_offset + vram_size)
        return(vram[addr - vram_offset]);
    else if (addr >= oam_offset && addr <= oam_offset + oam_size)
        return(oam[addr - oam_offset]);
    else if (addr >= pak_rom0_offset && addr <= pak_rom0_offset + pak_size)
        return(pak_rom[0][addr - pak_rom0_offset]);
    else if (addr >= pak_rom1_offset && addr <= pak_rom1_offset + pak_size)
        return(pak_rom[1][addr - pak_rom1_offset]);
    else if (addr >= pak_rom2_offset && addr <= pak_rom2_offset + pak_size)
        return(pak_rom[2][addr - pak_rom2_offset]);
    else
        return(pak_sram[addr - pak_sram_offset]);
};

uint32_t Gba::armDpOp2(uint32_t w) {
    bool set_cond = (w & 0x00100000) == 0x00100000;
    uint32_t imm = w & 0x02000000; // For checking if operand 2 is an immediate value

    if (imm == 0x00000000) { // Operand 2 is a reg
        // uint8_t rm = w & 0x0000000f;
        uint32_t op2 = regRef(static_cast<uint8_t>(w & 0x0000000f));
        uint8_t shift = (w & 0x00000ff0) >> 4;
        shiftOp(set_cond, shift, op2); // op2 passed by ref
        return(op2);
    }else { // Operand 2 is an immediate value
        uint8_t rotate = (w & 0x00000f00) >> 7; // Shift word only 7 because rotate value is multiplied by 2 anyways
        uint32_t op2 = w & 0x000000ff;
        armRor(set_cond, rotate, op2);  // Assuming this works the same as when operand 2 is register-specified

        // if (rotate > 0) {
        //     for(uint8_t i = 0; i < rotate - 1; ++i) {
        //         uint32_t lsb = op2 & 0x00000001;
        //         op2 = (op2 >> 1) | (lsb == 0x00000000 ? 0x00000000 : 0x80000000);
        //     }

        //     uint32_t lsb = op2 & 0x00000001;
        //     if (set_cond) reg[CPSR] = (lsb == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000);
        //     op2 = (op2 >> 1) | (lsb == 0x00000000 ? 0x00000000 : 0x80000000);
        // }

        return(op2);
    }
};

void Gba::shiftOp(bool set_cond, uint8_t shift, uint32_t& op) {
    uint8_t shift_t = (shift & 0x06) >> 1;
    uint8_t shift_am = 0;
    bool instr_spec = (shift & 0x01) == 0; // "instruction specified" shift amount
    
    if (instr_spec) // Immediate value
        shift_am = (shift & 0xf8) >> 3;
    else // Value is from a reg
        shift_am = static_cast<uint8_t>(regRef((shift & 0xf0) >> 4) & 0x000000ff);

    if (instr_spec || (shift_am > 0 && !instr_spec)) { // No special operations (e.g. ASR #0, LSR #0) if shift_am == 0 AND shift_am is reg-specified
        switch(shift_t) { // op passed by reference
            case 0x00: armLsl(set_cond, shift_am, op); break; // LSL, logical shift left
            case 0x01: armLsr(set_cond, shift_am, op); break; // LSR, logical shift right
            case 0x02: armAsr(set_cond, shift_am, op); break; // ASR, arithmetic shift right; shift right while keeping signedness
            case 0x03: armRor(set_cond, shift_am, op); break; // ROR, rotate right
            default: break;
        }
    }
};

void Gba::armLogSetCond(uint32_t res, uint8_t rd) {
    if (rd != PC) { // C affected by op2 barrel shifting, N and Z affected by operation
        if (res == 0)
            reg[CPSR] = reg[CPSR] | 0x40000000; // Set Z
        else
            reg[CPSR] = reg[CPSR] & 0xbfffffff; // Un-set Z
        
        if ((res & 0x80000000) == 0x80000000)
            reg[CPSR] = reg[CPSR] | 0x80000000; // Set N
        else
            reg[CPSR] = reg[CPSR] & 0x7fffffff; // Un-set N
    }else {
        uint8_t op_mode = reg[CPSR] & 0x0000001f;
        
        switch(op_mode) {
            case FIQ: reg[CPSR] = reg[SPSR_fiq]; break;
            case IRQ: reg[CPSR] = reg[SPSR_irq]; break;
            case SVC: reg[CPSR] = reg[SPSR_svc]; break;
            case ABT: reg[CPSR] = reg[SPSR_abt]; break;
            case UND: reg[CPSR] = reg[SPSR_und]; break;
            default: break;
        }
    }
};

void Gba::armAriSetCond(uint32_t res, bool c_flag, bool v_flag, uint8_t rd) {
    // N, Z, C, V all affected
    if (rd != PC) {
        if ((res & 0x80000000) == 0x80000000) // N flag
            reg[CPSR] = reg[CPSR] | 0x80000000; // Set N
        else
            reg[CPSR] = reg[CPSR] & 0x7fffffff; // Un-set N

        if (res == 0) // Z flag
            reg[CPSR] = reg[CPSR] | 0x40000000; // Set Z
        else
            reg[CPSR] = reg[CPSR] & 0xbfffffff; // Un-set Z

        reg[CPSR] = c_flag ? reg[CPSR] | 0x20000000 : reg[CPSR] & 0xdfffffff; // Set / un-set C
        reg[CPSR] = v_flag ? reg[CPSR] | 0x10000000 : reg[CPSR] & 0xefffffff; // Set / un-set V
    }else {
        uint8_t op_mode = reg[CPSR] & 0x0000001f;
        
        switch(op_mode) {
            case FIQ: reg[CPSR] = reg[SPSR_fiq]; break;
            case IRQ: reg[CPSR] = reg[SPSR_irq]; break;
            case SVC: reg[CPSR] = reg[SPSR_svc]; break;
            case ABT: reg[CPSR] = reg[SPSR_abt]; break;
            case UND: reg[CPSR] = reg[SPSR_und]; break;
            default: break;
        }
    }
};

void Gba::armPsrTransfer(uint32_t w) {
    uint8_t psr_num = CPSR;
    if ((w & 0x00400000) == 0x00400000) {
        uint8_t op_mode = static_cast<uint8_t>(reg[CPSR] & 0x0000001f);

        switch(op_mode) {
            case FIQ: psr_num = SPSR_fiq; break;
            case IRQ: psr_num = SPSR_irq; break;
            case SVC: psr_num = SPSR_svc; break;
            case ABT: psr_num = SPSR_abt; break;
            case UND: psr_num = SPSR_und; break;
            default: break;
        }
    }

    // * See Figure 4-11: PSR Transfer; bit 21 for distinguishing between MRS and MSR

    if ((w & 0x00200000) == 0x00000000) // MRS
        armMrs(psr_num, static_cast<uint8_t>((w & 0x0000f000) >> 12));
    else
        armMsr(psr_num, w);

};

bool Gba::nFlag() { return((reg[CPSR] & 0x80000000) == 0x80000000); }
bool Gba::zFlag() { return((reg[CPSR] & 0x40000000) == 0x40000000); }
bool Gba::cFlag() { return((reg[CPSR] & 0x20000000) == 0x20000000); }
bool Gba::vFlag() { return((reg[CPSR] & 0x10000000) == 0x10000000); }
bool Gba::isArm() { return((reg[CPSR] & 0x00000020) == 0); }