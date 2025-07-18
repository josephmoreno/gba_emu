#include <fstream>

#include "../include/Gba.hpp"

int Gba::disp_scale = 1;
uint16_t Gba::display[disp_w * disp_h];

// Internal memory
uint8_t Gba::sys_rom[sys_rom_size];
uint8_t Gba::ob_wram[ob_wram_size];
uint8_t Gba::oc_wram[oc_wram_size];
uint8_t Gba::io_ram[io_ram_size];
uint8_t Gba::palette_ram[palette_ram_size];
uint8_t Gba::vram[vram_size];
uint8_t Gba::oam[oam_size];

// External memory
uint8_t* Gba::pak_rom[3] = {nullptr, nullptr, nullptr};   // 0x0800_0000 - 0x09ff_ffff, 0x0a00_0000 - 0x0bff_ffff, 0x0c00_0000 - 0x0dff_ffff, 32MB each, game pak ROM; going to dynamically allocate because of large max size and pak size varies
uint8_t Gba::pak_sram[pak_sram_size]; // 0x0e00_0000 - 0x0e00_ffff, 64kB, for saved data
uint32_t Gba::pak_size = 0;

// ARM CPU Registers
uint32_t Gba::reg[Reg::REG_COUNT];

bool Gba::debug = false;

void Gba::init(bool debug) {
    Gba::debug = debug;
    reg[CPSR] = 0x00000010; // N, Z, C, V flags 0; USR Mode
};

bool Gba::insertRom(std::string path) {
	// Variables used for loading a ROM.
	FILE *file = NULL;

	file = fopen(path.c_str(), "rb");

    if (file == NULL)
        return(false);
    else {
        // Determine the ROM's size.
        fseek(file, 0L, SEEK_END);
        pak_size = ftell(file);
        fseek(file, 0L, SEEK_SET);

        // Dynamically allocate and put contents in pak_rom.
        pak_rom[0] = new uint8_t[pak_size];
        // pak_rom[1] = new uint8_t[file_size];
        // pak_rom[2] = new uint8_t[file_size];

        for(unsigned int i = 0; i < pak_size; ++i) {
            uint8_t c = fgetc(file);
            
            pak_rom[0][i] = c;
            // pak_rom[1][i] = c;
            // pak_rom[2][i] = c;
        }

        fclose(file);

        return(true);
    }
};

uint32_t Gba::fetch() {
    // Little endian
    uint32_t w = (pak_rom[0][reg[PC] + 3] << 24) | (pak_rom[0][reg[PC] + 2] << 16) | (pak_rom[0][reg[PC] + 1] << 8) | pak_rom[0][reg[PC]];
    if ((reg[PC] + 4) < pak_size) reg[PC] += 4;
    return(w);
};

void Gba::armDecode(uint32_t w) {
    // PSR[31:28] = [N, Z, C, V]; N = negative / less than, Z = zero, C = carry / borrow / extend, V = overflow
    // All ARM instructions have a condition bit-field (most significant nibble)
    /* 
        0x0 = "Z set"
        0x1 = "Z clear"
        0x2 = "C set"
        0x3 = "C clear"
        0x4 = "N set"
        0x5 = "N clear"
        0x6 = "V set"
        0x7 = "V clear"
        0x8 = "C set && Z clear"
        0x9 = "C clear || Z set"
        0xa = "N == V"
        0xb = "N != V"
        0xc = "Z clear && (N == V)"
        0xd = "Z set || (N != V)"
        0xe = "always true"
    */

    if (debug) printf("w = %x\n", w);

    uint32_t instr_code = w & 0x0ffffff0;
    uint8_t cpsr_cond = reg[CPSR] >> 28;
    bool cond = false;

    switch(static_cast<uint8_t>(w >> 28)) {
        case 0x0: if ((cpsr_cond & 0x4) == 0x4) cond = true; break; // Z set
        case 0x1: if ((cpsr_cond & 0x4) == 0x0) cond = true; break; // Z clear
        case 0x2: if ((cpsr_cond & 0x2) == 0x2) cond = true; break; // C set
        case 0x3: if ((cpsr_cond & 0x2) == 0x0) cond = true; break; // C clear
        case 0x4: if ((cpsr_cond & 0x8) == 0x8) cond = true; break; // N set
        case 0x5: if ((cpsr_cond & 0x8) == 0x0) cond = true; break; // N clear
        case 0x6: if ((cpsr_cond & 0x1) == 0x1) cond = true; break; // V set
        case 0x7: if ((cpsr_cond & 0x1) == 0x0) cond = true; break; // V clear
        case 0x8: if ((cpsr_cond & 0x6) == 0x2) cond = true; break; // C set && Z clear
        case 0x9: if ((cpsr_cond & 0x2) == 0x0 || (cpsr_cond & 0x4) == 0x4) cond = true; break; // C clear || Z set
        case 0xa: if ((cpsr_cond & 0x9) == 0x9 || (cpsr_cond & 0x9) == 0x0) cond = true; break; // N == V
        case 0xb: if ((cpsr_cond & 0x9) == 0x1 || (cpsr_cond & 0x9) == 0x8) cond = true; break; // N != V
        case 0xc: // Z clear && (N == V)
            if (
                (cpsr_cond & 0x4) == 0x0 && 
                ((cpsr_cond & 0x9) == 0x9 || (cpsr_cond & 0x9) == 0x0)
            )
                cond = true;
            
            break;

        case 0xd: // Z set || (N != V)
            if (
                (cpsr_cond & 0x4) == 0x4 &&
                ((cpsr_cond & 0x9) == 0x1 || (cpsr_cond & 0x9) == 0x8)
            )
                cond = true;

            break;

        case 0xe: cond = true; break; // Always true
        case 0xf: // Reserved; not used by programmer
        default:
            break;
    }

    if (debug) std::cout << "cond = " << (cond ? "true" : "false") << std::endl;

    if (instr_code == 0x012fff10) {
        // Branch and Exchange: BX
        // Only executes if condition is true
        // If operand register's bit 0 == 0, subsequent instructions are ARM; else if 1, THUMB
        
        if (debug) std::cout << "Branch and Exchange" << std::endl;
        // if (cond) armBx(static_cast<uint8_t>(w & 0x0000000f));

    }else {
        instr_code = instr_code & 0x0ff00ff0;

        if ((instr_code & 0x0fb00ff0) == 0x01000090) {
            // Single Data Swap / Half-Word Data Transfer: Register Offset, SWP
            if (debug) std::cout << "Single Data Swap" << std::endl;
        }else if ((instr_code & 0x0e400f90) == 0x00000090 && (instr_code & 0x00000060) != 0) {
            // Half-Word Data Transfer: Register Offset, Not SWP; [6:5] must not equal 0b00
            if (debug) std::cout << "Half-Word Data Transfer: Register Offset" << std::endl;
        }else {
            instr_code = instr_code & 0x0ff000f0;

            if ((instr_code & 0x0fc000f0) == 0x00000090) {
                // Multiply: MUL and MLA
                // Only executes if condition is true

                if (debug) std::cout << "Multiply" << std::endl;

                // if (cond) {
                //     uint32_t a = w & 0x00200000;        // Accumulate field
                //     uint32_t set_cond = w & 0x00100000; // Set condition field
                //     uint8_t rd = (w & 0x000f0000) >> 16;
                //     uint32_t op1 = regRef(static_cast<uint8_t>(w & 0x0000000f));
                //     uint32_t op2 = regRef(static_cast<uint8_t>((w & 0x00000f00) >> 8));
                //     uint32_t& acc = regRef(static_cast<uint8_t>((w & 0x0000f000) >> 12));

                //     if (a == 0x00000000) armMul((set_cond == 0x00100000), op1, op2, acc, rd); // MUL
                //     else armMla((set_cond == 0x00100000), op1, op2, acc, rd); // MLA
                // }

            }else if ((instr_code & 0x0f8000f0) == 0x00800090) {
                // Multiply Long: MULL and MLAL
                // Only executes if condition is true

                if (debug) std::cout << "Multiply Long" << std::endl;

                // if (cond) {
                //     uint32_t u = w & 0x00400000; // Unsigned field
                //     uint32_t a = w & 0x00200000; // Accumulate field
                //     uint32_t set_cond = w & 0x00100000;
                //     uint8_t rd_h = (w & 0x000f0000) >> 16; // High destination reg
                //     uint8_t rd_l = (w & 0x0000f000) >> 12; // Low destination reg
                //     uint32_t op1 = regRef(static_cast<uint8_t>(w & 0x0000000f));
                // }

            }else if ((instr_code & 0x0e400090) == 0x00400090) {
                // Half-Word Data Transfer: Immediate Offset
                if (debug) std::cout << "Half-Word Data Transfer: Immediate Offset" << std::endl;
            }else {
                instr_code = instr_code & 0x0f000010;

                if (instr_code == 0x0e000000) {
                    // Co-Processor Data Operation
                    // * Probably don't need, GBA doesn't have co-processor
                    if (debug) std::cout << "Co-Processor Data Operation" << std::endl;
                }else if (instr_code == 0x0e000010) {
                    // Co-Processor Register Transfer
                    // * Probably don't need, GBA doesn't have co-processor
                    if (debug) std::cout << "Co-Processor Register Transfer" << std::endl;
                }else if ((instr_code & 0x0e000010) == 0x06000010) {
                    // Undefined
                    if (debug) std::cout << "Undefined" << std::endl;
                }else {
                    instr_code = instr_code & 0x0f000000;

                    if (instr_code == 0x0f000000) {
                        // Software Interrupt
                        if (debug) std::cout << "Software Interrupt" << std::endl;
                    }else if ((instr_code & 0x0e000000) == 0x08000000) {
                        // Block Data Transfer
                        if (debug) std::cout << "Block Data Transfer" << std::endl;
                    }else if ((instr_code & 0x0e000000) == 0x0a000000) {
                        // Branch and Branch with Link
                        // Only executes if condition is true
                        // * prefetch?

                        if (debug) std::cout << "Branch and Branch with Link" << std::endl;
                        
                        // if (cond) {
                        //     uint32_t offset = (w & 0x00ffffff) << 2;    // uint or int, doesn't matter; should overflow into the correct value either way
                        //     offset = ((offset & 0x02000000) == 0x02000000) ? offset | 0xfc000000 : offset;  // Extending the sign if it is negative

                        //     if ((w & 0x01000000) == 0x01000000) armBl(offset);
                        //     else armB(offset);
                        // }

                    }else if ((instr_code & 0x0e000000) == 0x0c000000) {
                        // Co-Processor Data Transfer
                        // * Probably don't need, GBA doesn't have co-processor
                        if (debug) std::cout << "Co-Processor Data Transfer" << std::endl;
                    }else if ((instr_code & 0x0c000000) == 0x00000000) {
                        // Data Processing / PSR Transfer
                        // * Ignoring 4.6: PSR Transfer for now

                        if (debug) std::cout << "Data Processing / PSR Transfer" << std::endl;

                        // if (cond) {
                        //     bool set_cond = (w & 0x00100000) == 0x00100000;
                        //     uint8_t opcode = (w & 0x01e00000) >> 21;
                        //     uint32_t op1 = regRef(static_cast<uint8_t>((w & 0x000f0000) >> 16));
                        //     uint32_t op2 = armDpOp2(w);
                        //     uint8_t rd = (w & 0x0000f000) >> 12; // Destination reg                            

                        //     switch(opcode) {
                        //         case 0x0: armAnd(set_cond, op1, op2, rd); break;    // AND
                        //         case 0x1: armEor(set_cond, op1, op2, rd); break;    // EOR
                        //         case 0x2: armSub(set_cond, op1, op2, rd); break;    // SUB
                        //         case 0x3: armRsb(set_cond, op1, op2, rd); break;    // RSB
                        //         case 0x4: armAdd(set_cond, op1, op2, rd); break;    // ADD
                        //         case 0x5: armAdc(set_cond, op1, op2, rd); break;    // ADC
                        //         case 0x6: armSbc(set_cond, op1, op2, rd); break;    // SBC
                        //         case 0x7: armRsc(set_cond, op1, op2, rd); break;    // RSC
                        //         case 0x8: armTst(op1, op2); break;                  // TST; AND operation, result not written to destination reg, S bit is always set
                        //         case 0x9: armTeq(op1, op2); break;                  // TEQ; EOR operation, result not written to destination reg, S bit is always set
                        //         case 0xa: armCmp(op1, op2); break;                  // CMP; SUB operation, result not written to destination reg, S bit is always set
                        //         case 0xb: armCmn(op1, op2); break;                  // CMN; ADD operation, result not written to destination reg, S bit is always set
                        //         case 0xc: armOrr(set_cond, op1, op2, rd); break;    // ORR
                        //         case 0xd: armMov(set_cond, op2, rd); break;         // MOV
                        //         case 0xe: armBic(set_cond, op1, op2, rd); break;    // BIC
                        //         case 0xf: armMvn(set_cond, op2, rd); break;         // MVN
                        //         default: break;
                        //     }
                        // }

                    }else if ((instr_code & 0x0c000000) == 0x04000000) {
                        // Single Data Transfer
                        if (debug) std::cout << "Single Data Transfer" << std::endl;
                    }
                }
            }
        }
    }
};

void Gba::thumbDecode(uint16_t half_w) {
    // Only branch instruction is capable of conditional execution
};

void Gba::cycle() {
    armDecode(fetch());
};

// -- BEGIN Helper Functions

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

uint32_t Gba::armDpOp2(uint32_t w) {
    bool set_cond = (w & 0x00100000) == 0x00100000;
    uint32_t imm = w & 0x02000000; // For checking if operand 2 is an immediate value

    if (imm == 0x00000000) { // Operand 2 is a reg
        // uint8_t rm = w & 0x0000000f;
        uint8_t shift = (w & 0x00000ff0) >> 4;
        uint8_t shift_t = (shift & 0x06) >> 1;
        uint8_t shift_am = 0;
        
        if ((shift & 0x01) == 0x00) // Immediate value
            shift_am = (shift & 0xf8) >> 3;
        else // Value is from a reg
            shift_am = static_cast<uint8_t>(regRef((shift & 0xf0) >> 4) & 0x000000ff);

        uint32_t op2 = regRef(static_cast<uint8_t>(w & 0x0000000f));

        switch(shift_t) {
            case 0x00: // Logical left
                if (shift_am > 0) {
                    op2 = op2 << (shift_am - 1); // Least significant shifted out bit needs to be saved in CPSR as the carry out
                    if (set_cond) reg[CPSR] = (op2 & 0x80000000) == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
                    op2 = op2 << 1;
                } // Else, use op2 directly (LSL #0)
                break;

            case 0x01: // Logical right
                if (shift_am == 0) { // LSR #0 becomes LSR #32; zero result and bit 31 becomes carry out
                    if (set_cond) reg[CPSR] = (op2 & 0x80000000) == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
                    op2 = 0;
                }else {
                    op2 = op2 >> (shift_am - 1); // Most significant shifted out bit needs to be saved in CPSR as the carry out
                    if (set_cond) reg[CPSR] = (op2 & 0x00000001) == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
                    op2 = op2 >> 1;
                }

                break;

            case 0x02: { // Arithmetic right; logical right while keeping the signedness
                    uint8_t sign = (op2 & 0x80000000) == 0x00000000 ? 0x00000000 : 0x80000000;

                    if (shift_am == 0) { // ASR #0 becomes ASR #32; all 0s or 1s result and bit 31 becomes carry out
                        if (set_cond) reg[CPSR] = sign == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
                        op2 = sign == 0x00000000 ? 0x00000000 : 0xffffffff;
                    }else {
                        for(uint8_t i = 0; i < (shift_am - 1); ++i) // Most significant shifted out bit needs to be saved in CPSR as the carry out
                            op2 = (op2 >> 1) | sign;

                        if (set_cond) reg[CPSR] = (op2 & 0x00000001) == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
                        op2 = (op2 >> 1) | sign;
                    }
                } // Brackets here to prevent cross initialization

                break;

            case 0x03: // Rotate right
                if (shift_am == 0) { // ROR #0 becomes RRX (rotate right extended); bit 0 carried out, and CPSR's carry is carried into bit 31
                    uint8_t lsb = op2 & 0x00000001;
                    op2 = (op2 >> 1) | ((reg[CPSR] & 0x2000000) == 0x00000000 ? 0x00000000 : 0x80000000);
                    if (set_cond) reg[CPSR] = lsb == 0x00 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
                }else {
                    for(uint8_t i = 0; i < (shift_am - 1); ++i) { // Most significant shifted out bit needs to be saved in CPSR as the carry out
                        uint8_t lsb = op2 & 0x00000001;
                        op2 = (op2 >> 1) | (lsb == 0x00 ? 0x00000000 : 0x80000000);
                    }

                    uint8_t lsb = op2 & 0x00000001;
                    if (set_cond) reg[CPSR] = lsb == 0x00 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000;
                    op2 = (op2 >> 1) | (lsb == 0x00 ? 0x00000000 : 0x80000000);
                }

                break;

            default:
                break;
        }

        return(op2);
    }else { // Operand 2 is an immediate value
        uint8_t rotate = ((w & 0x00000f00) >> 7) % 32; // Shift word only 7 because rotate value is multiplied by 2 anyways; mod 32 since rotating by 32 will yield the same number
        uint32_t op2 = w & 0x000000ff;
        
        if (rotate > 0) {
            for(uint8_t i = 0; i < rotate - 1; ++i) {
                uint32_t lsb = op2 & 0x00000001;
                op2 = (op2 >> 1) | (lsb == 0x00000000 ? 0x00000000 : 0x80000000);
            }

            uint32_t lsb = op2 & 0x00000001;
            reg[CPSR] = (lsb == 0x00000000 ? reg[CPSR] & 0xdfffffff : reg[CPSR] | 0x20000000);
            op2 = (op2 >> 1) | (lsb == 0x00000000 ? 0x00000000 : 0x80000000);
        }

        return(op2);
    }
};

void Gba::armLogSetCond(uint32_t res, uint8_t rd) {
    if (rd != PC) { // C affected by op2 barrel shifting, N and Z affected by operation
        if (res == 0) reg[CPSR] = (reg[CPSR] & 0x7fffffff) | 0x40000000; // Un-set N, set Z
        else if ((res & 0x80000000) == 0x80000000) reg[CPSR] = (reg[CPSR] & 0xbfffffff) | 0x80000000; // Un-set Z, set N
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
        if (res == 0) reg[CPSR] = (reg[CPSR] & 0x7fffffff) | 0x40000000; // Un-set N, set Z
        else if ((res & 0x80000000) == 0x80000000) reg[CPSR] = (reg[CPSR] & 0xbfffffff) | 0x80000000; // Un-set Z, set N

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

// -- END Helper Functions

// -- BEGIN Debugging Functions

void Gba::printDisp() {
    printf("\n\nSTART DISPLAY\n");

    for(uint8_t i = 0; i < disp_h; ++i) {
        for(uint8_t j = 0; j < disp_w; ++j) {
            printf("%x", display[(i * disp_w) + j]);
            std::cout << (j < disp_w - 1 ? " " : "\n");
        }
    }

    printf("END DISPLAY\n\n");
};

void Gba::printReg() {
    printf("\n\nSTART REG\n");

    for(uint8_t i = 0; i < REG_COUNT; ++i) {
        switch(i) {
            case R0: printf("R0 = %x", reg[R0]); break;
            case R1: printf("R1 = %x", reg[R1]); break;
            case R2: printf("R2 = %x", reg[R2]); break;
            case R3: printf("R3 = %x", reg[R3]); break;
            case R4: printf("R4 = %x", reg[R4]); break;
            case R5: printf("R5 = %x", reg[R5]); break;
            case R6: printf("R6 = %x", reg[R6]); break;
            case R7: printf("R7 = %x", reg[R7]); break;
            case R8: printf("R8 = %x", reg[R8]); break;
            case R9: printf("R9 = %x", reg[R9]); break;
            case R10: printf("R10 = %x", reg[R10]); break;
            case R11: printf("R11 = %x", reg[R11]); break;
            case R12: printf("R12 = %x", reg[R12]); break;
            case SP: printf("SP (R13) = %x", reg[SP]); break;
            case LR: printf("LR (R14) = %x", reg[LR]); break;
            case PC: printf("PC (R15) = %x", reg[PC]); break;
            case R8_fiq: printf("R8_fiq = %x", reg[R8_fiq]); break;
            case R9_fiq: printf("R9_fiq = %x", reg[R9_fiq]); break;
            case R10_fiq: printf("R10_fiq = %x", reg[R10_fiq]); break;
            case R11_fiq: printf("R11_fiq = %x", reg[R11_fiq]); break;
            case R12_fiq: printf("R12_fiq = %x", reg[R12_fiq]); break;
            case R13_fiq: printf("R13_fiq = %x", reg[R13_fiq]); break;
            case R14_fiq: printf("R14_fiq = %x", reg[R14_fiq]); break;
            case R13_svc: printf("R13_svc = %x", reg[R13_svc]); break;
            case R14_svc: printf("R14_svc = %x", reg[R14_svc]); break;
            case R13_abt: printf("R13_abt = %x", reg[R13_abt]); break;
            case R14_abt: printf("R14_abt = %x", reg[R14_abt]); break;
            case R13_irq: printf("R13_irq = %x", reg[R13_irq]); break;
            case R14_irq: printf("R14_irq = %x", reg[R14_irq]); break;
            case R13_und: printf("R13_und = %x", reg[R13_und]); break;
            case R14_und: printf("R14_und = %x", reg[R14_und]); break;
            case CPSR: printf("CPSR = %x", reg[CPSR]); break;
            case SPSR_fiq: printf("SPSR_fiq = %x", reg[SPSR_fiq]); break;
            case SPSR_svc: printf("SPSR_svc = %x", reg[SPSR_svc]); break;
            case SPSR_abt: printf("SPSR_abt = %x", reg[SPSR_abt]); break;
            case SPSR_irq: printf("SPSR_irq = %x", reg[SPSR_irq]); break;
            case SPSR_und: printf("SPSR_und = %x", reg[SPSR_und]); break;
            
            default:
                break;
        }

        std::cout << std::endl;
    }

    printf("END REG\n\n");
};

void Gba::printAll() {
    printDisp();
    printReg();
};

// -- END Debugging Functions

// Destructor
Gba::~Gba() {
    for(unsigned int i = 0; i < 3; ++i) {
        if (pak_rom[i] != nullptr)
            delete[] pak_rom[i];
    }
};