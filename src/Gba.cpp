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

// * See 3.11: Reset; SVC mode, sets I, F, and clears T bit in CPSR, PC = 0
void Gba::init(bool debug) {
    Gba::debug = debug;

    // Clear registers
    for(uint8_t i = 0; i < REG_COUNT; ++i)
        reg[i] = 0;

    // Clear memory
    uint32_t i;
    for(i = 0; i < ob_wram_size; ++i) { // ob_wram is the largest in size
        if (i < sys_rom_size) sys_rom[i] = 0;
        if (i < ob_wram_size) ob_wram[i] = 0;
        if (i < oc_wram_size) oc_wram[i] = 0;
        if (i < io_ram_size) io_ram[i] = 0;
        if (i < palette_ram_size) palette_ram[i] = 0;
        if (i < vram_size) vram[i] = 0;
        if (i < oam_size) oam[i] = 0;
        if (i < pak_sram_size) pak_sram[i] = 0;
    }

    reg[PC] = pak_rom0_offset;
    reg[CPSR] = 0x00000013; // N, Z, C, V flags 0; SVC Mode
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

uint32_t Gba::armFetch() {
    // Little endian
    // uint32_t w = (pak_rom[0][reg[PC] + 3] << 24) | (pak_rom[0][reg[PC] + 2] << 16) | (pak_rom[0][reg[PC] + 1] << 8) | pak_rom[0][reg[PC]];
    // if ((reg[PC] + 4) < pak_size) reg[PC] += 4;

    uint32_t w = (memRef(reg[PC] + 3) << 24) | (memRef(reg[PC] + 2) << 16) | (memRef(reg[PC] + 1) << 8) | memRef(reg[PC]);
    if (((reg[PC] - pak_rom0_offset) + 4) < pak_size) reg[PC] += 4;
    return(w);
};

uint16_t Gba::thumbFetch() {
    // Little endian
    // uint16_t half_w = (pak_rom[0][reg[PC] + 1] << 8) | pak_rom[0][reg[PC]];
    // if ((reg[PC] + 2) < pak_size) reg[PC] += 2;

    uint16_t half_w = (memRef(reg[PC] + 1) << 8) | memRef(reg[PC]);
    if (((reg[PC] - pak_rom0_offset) + 2) < pak_size) reg[PC] += 2;
    return(half_w);
};

void Gba::armDecode(uint32_t w, std::string* ret_instr_fmt = nullptr) {
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

    if (debug) printf("w = %x, ARM state\n", w);

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
            if ((cpsr_cond & 0x4) == 0x0 && ((cpsr_cond & 0x9) == 0x9 || (cpsr_cond & 0x9) == 0x0))
                cond = true;
            break;

        case 0xd: // Z set || (N != V)
            if ((cpsr_cond & 0x4) == 0x4 && ((cpsr_cond & 0x9) == 0x1 || (cpsr_cond & 0x9) == 0x8))
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
        
        if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Branch and Exchange";
        if (cond) armBx(static_cast<uint8_t>(w & 0x0000000f));

    }else {
        instr_code = instr_code & 0x0ff00ff0;

        if ((instr_code & 0x0fb00ff0) == 0x01000090) {
            // Single Data Swap / Half-Word Data Transfer: Register Offset, SWP
            if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Half-Word Data Transfer: Register Offset, SWP";

            if (cond) {

            }
        }else if ((instr_code & 0x0e400f90) == 0x00000090 && (instr_code & 0x00000060) != 0) {
            // Half-Word Data Transfer: Register Offset, Not SWP; [6:5] must not equal 0b00
            if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Half-Word Data Transfer: Register Offset, Not SWP";

            if (cond) {
                bool pre_ind = (w & 0x01000000) == 0x01000000; // true = pre-index, false = post-index
                bool add_offset = (w & 0x00800000) == 0x00800000; // true = add offset, false = subtract offset
                bool write_back = (w & 0x00200000) == 0x00200000; // true = write back to base reg, false = don't write back
                bool load = (w & 0x00100000) == 0x00100000; // true = load from mem, false = store to mem
                bool s = (w & 0x00000040) == 0x00000040; // true = signed, false = unsigned
                bool h = (w & 0x00000020) == 0x00000020; // true = half-word, false = byte
                uint8_t rn = static_cast<uint8_t>((w & 0x000f0000) >> 16);
                uint8_t rd = static_cast<uint8_t>((w & 0x0000f000) >> 12);
                uint32_t offset = regRef(static_cast<uint8_t>(w & 0x0000000f));

                if (load) {
                    if (!s && h) armLdrh(pre_ind, add_offset, write_back, rn, rd, offset);
                    else if (s && !h) armLdrsb(pre_ind, add_offset, write_back, rn, rd, offset);
                    else armLdrsh(pre_ind, add_offset, write_back, rn, rd, offset);
                }else {
                    if (!s) { // * See 4.10.3, Signed byte and half-word loads: If storing into memory, should be unsigned
                        if (h) armStrh(pre_ind, add_offset, write_back, rn, rd, offset);
                        else armStrb(pre_ind, add_offset, write_back, rn, rd, offset);
                    }
                }
            }
        }else {
            instr_code = instr_code & 0x0ff000f0;

            if ((instr_code & 0x0fc000f0) == 0x00000090) {
                // Multiply: MUL and MLA
                // Only executes if condition is true

                if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Multiply";

                if (cond) {
                    uint32_t a = w & 0x00200000;        // Accumulate field
                    uint32_t set_cond = w & 0x00100000; // Set condition field
                    uint8_t rd = (w & 0x000f0000) >> 16;
                    uint32_t op1 = regRef(static_cast<uint8_t>(w & 0x0000000f));
                    uint32_t op2 = regRef(static_cast<uint8_t>((w & 0x00000f00) >> 8));
                    uint32_t& acc = regRef(static_cast<uint8_t>((w & 0x0000f000) >> 12));

                    if (a == 0x00000000) armMul((set_cond == 0x00100000), op1, op2, acc, rd); // MUL
                    else armMla((set_cond == 0x00100000), op1, op2, acc, rd); // MLA
                }

            }else if ((instr_code & 0x0f8000f0) == 0x00800090) {
                // Multiply Long: MULL and MLAL
                // Only executes if condition is true

                if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Multiply Long";

                if (cond) {
                    bool sign = (w & 0x00400000) == 0x00400000; // Unsigned field; true = signed operands
                    bool acc = (w & 0x00200000) == 0x00200000;  // Accumulate field
                    bool set_cond = (w & 0x00100000) == 0x00100000;
                    uint8_t rd_h = (w & 0x000f0000) >> 16; // High destination reg
                    uint8_t rd_l = (w & 0x0000f000) >> 12; // Low destination reg
                    uint32_t op1 = regRef(static_cast<uint8_t>(w & 0x0000000f));
                    uint32_t op2 = regRef(static_cast<uint8_t>((w & 0x00000f00) >> 8));

                    if (acc) armMlal(set_cond, sign, op1, op2, rd_h, rd_l);
                    else armMull(set_cond, sign, op1, op2, rd_h, rd_l);
                }

            }else if ((instr_code & 0x0e400090) == 0x00400090) {
                // Single Data Swap / Half-Word Data Transfer: Immediate Offset, SWP
                if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Half-Word Data Transfer: Immediate Offset, SWP";
            }else if ((instr_code & 0x0e400090) == 0x00400090 && (instr_code & 0x00000060) != 0) {
                // Half-Word Data Transfer: Immediate Offset, Not SWP
                if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Half-Word Data Transfer: Immediate Offset, Not SWP";
            }else {
                instr_code = instr_code & 0x0f000010;

                if (instr_code == 0x0e000000) {
                    // Co-Processor Data Operation
                    // * Don't need, GBA doesn't have co-processor
                    if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Co-Processor Data Operation";
                }else if (instr_code == 0x0e000010) {
                    // Co-Processor Register Transfer
                    // * Don't need, GBA doesn't have co-processor
                    if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Co-Processor Register Transfer";
                }else if ((instr_code & 0x0e000010) == 0x06000010) {
                    // Undefined
                    if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Undefined";
                }else {
                    instr_code = instr_code & 0x0f000000;

                    if (instr_code == 0x0f000000) {
                        // Software Interrupt
                        if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Software Interrupt";
                    }else if ((instr_code & 0x0e000000) == 0x08000000) {
                        // Block Data Transfer
                        if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Block Data Transfer";

                        if (cond) {
                            
                        }
                    }else if ((instr_code & 0x0e000000) == 0x0a000000) {
                        // Branch and Branch with Link
                        // Only executes if condition is true
                        // * prefetch?

                        if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Branch and Branch with Link";
                        
                        if (cond) {
                            uint32_t offset = (w & 0x00ffffff) << 2;    // uint or int, doesn't matter; should overflow into the correct value either way
                            offset = ((offset & 0x02000000) == 0x02000000) ? offset | 0xfc000000 : offset;  // Extending the sign if it is negative

                            if ((w & 0x01000000) == 0x01000000) armBl(offset);
                            else armB(offset);
                        }

                    }else if ((instr_code & 0x0e000000) == 0x0c000000) {
                        // Co-Processor Data Transfer
                        // * Don't need, GBA doesn't have co-processor
                        if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Co-Processor Data Transfer";
                    }else if ((instr_code & 0x0c000000) == 0x00000000) {
                        // Data Processing / PSR Transfer

                        if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Data Processing / PSR Transfer";

                        if (cond) {
                            bool set_cond = (w & 0x00100000) == 0x00100000;
                            uint8_t opcode = (w & 0x01e00000) >> 21;
                            uint32_t op1 = regRef(static_cast<uint8_t>((w & 0x000f0000) >> 16));
                            uint32_t op2 = armDpOp2(w);
                            uint8_t rd = (w & 0x0000f000) >> 12; // Destination reg                            

                            switch(opcode) {
                                case 0x0: armAnd(set_cond, op1, op2, rd); break;    // AND
                                case 0x1: armEor(set_cond, op1, op2, rd); break;    // EOR
                                case 0x2: armSub(set_cond, op1, op2, rd); break;    // SUB
                                case 0x3: armRsb(set_cond, op1, op2, rd); break;    // RSB
                                case 0x4: armAdd(set_cond, op1, op2, rd); break;    // ADD
                                case 0x5: armAdc(set_cond, op1, op2, rd); break;    // ADC
                                case 0x6: armSbc(set_cond, op1, op2, rd); break;    // SBC
                                case 0x7: armRsc(set_cond, op1, op2, rd); break;    // RSC
                                case 0x8: 
                                    if (set_cond) armTst(op1, op2); // TST; AND operation 
                                    else armPsrTransfer(w);         // * See 4.6: PSR Transfer; "TEQ, TST, CMN, CMP instructions without S flag set"
                                    break;
                                case 0x9: 
                                    if (set_cond) armTeq(op1, op2); // TEQ; EOR operation
                                    else armPsrTransfer(w);
                                    break;
                                case 0xa:
                                    if (set_cond) armCmp(op1, op2); // CMP; SUB operation
                                    else armPsrTransfer(w);
                                    break;
                                case 0xb: 
                                    if (set_cond) armCmn(op1, op2); // CMN; ADD operation
                                    else armPsrTransfer(w);
                                    break;
                                case 0xc: armOrr(set_cond, op1, op2, rd); break;    // ORR
                                case 0xd: armMov(set_cond, op2, rd); break;         // MOV
                                case 0xe: armBic(set_cond, op1, op2, rd); break;    // BIC
                                case 0xf: armMvn(set_cond, op2, rd); break;         // MVN
                                default: break;
                            }
                        }

                    }else if ((instr_code & 0x0c000000) == 0x04000000) {
                        // Single Data Transfer
                        if (ret_instr_fmt != nullptr) *ret_instr_fmt = "Single Data Transfer";

                        if (cond) {
                            bool imm_offset = (w & 0x02000000) == 0x00000000; // true = imm, false = from reg
                            bool pre_ind = (w & 0x01000000) == 0x01000000; // true = pre, false = post
                            bool add_offset = (w & 0x00800000) == 0x00800000; // true = add offset, false = subtract offset
                            bool word_transfer = (w & 0x00400000) == 0x00000000; // true = word transfer, false = byte transfer
                            bool write_back = (w & 0x00200000) == 0x00200000; // true = write address into base, false = no write-back
                            bool load = (w & 0x00100000) == 0x00100000; // true = load from memory, false = store to memory
                            uint8_t rn = (w & 0x000f0000) >> 16; // base reg
                            uint8_t rd = (w & 0x0000f000) >> 12; // source / dest reg
                            uint32_t offset = 0;

                            if (imm_offset)
                                offset = w & 0x00000fff;
                            else {
                                offset = regRef(static_cast<uint8_t>(w & 0x0000000f));
                                uint8_t shift = (w & 0x00000fe0) >> 4; // * See 4.9.2, Shifted register offset: "fe" because shift amount cannot be reg-specified; bit 0 should always be 0
                                shiftOp(false, shift, offset); // offset passed by ref
                            }

                            // * See 4.9.3: Bytes and words; ignore BIGEND signal because GBA is always little endian
                            if (load) {
                                if (word_transfer) armLdr(pre_ind, add_offset, write_back, rn, rd, offset);
                                else armLdrb(pre_ind, add_offset, write_back, rn, rd, offset);
                            }else {
                                if (word_transfer) armStr(pre_ind, add_offset, write_back, rn, rd, offset);
                                else armStrb(pre_ind, add_offset, write_back, rn, rd, offset);
                            }
                        }
                    }
                }
            }
        }
    }
};

void Gba::thumbDecode(uint16_t half_w) {
    // Only branch instruction is capable of conditional execution

    if (debug) printf("half_w = %x, THUMB state\n", half_w);

    uint16_t instr_code = half_w & 0xff00;

    if (instr_code == 0xdf00) {
        // Software Interrupt
        if (debug) std::cout << "Software Interrupt" << std::endl;
    }else if (instr_code == 0xb000) {
        // Add offset to stack pointer
        if (debug) std::cout << "Add offset to stack pointer" << std::endl;
    }else if ((instr_code & 0xf600) == 0xb400) {
        // Push/pop registers
        if (debug) std::cout << "Push/pop registers" << std::endl;
    }else if ((instr_code & 0xf200) == 0x5000) {
        // Load/store with register offset
        if (debug) std::cout << "Load/store with register offset" << std::endl;
    }else if ((instr_code & 0xf200) == 0x5200) {
        // Load/store sign-extended byte/half-word
        if (debug) std::cout << "Load/store sign-extended byte/half-word" << std::endl;
    }else if ((instr_code & 0xfc00) == 0x4000) {
        // ALU operations
        if (debug) std::cout << "ALU operations" << std::endl;
    }else if ((instr_code & 0xfc00) == 0x4400) {
        // High register operations / branch exchange
        if (debug) std::cout << "High register operations / branch exchange" << std::endl;
    }else if ((instr_code & 0xf800) == 0x1800) {
        // Add/subtract
        if (debug) std::cout << "Add/subtract" << std::endl;
    }else if ((instr_code & 0xf800) == 0x4800) {
        // PC-relative load
        if (debug) std::cout << "PC-relative load" << std::endl;
    }else if ((instr_code & 0xf800) == 0xe000) {
        // Unconditional branch
        if (debug) std::cout << "Unconditional branch" << std::endl;
    }else {
        instr_code = instr_code & 0xf000;

        if (instr_code == 0xf000) {
            // Long branch with link
            if (debug) std::cout << "Long branch with link" << std::endl;
        }else if (instr_code == 0xd000) {
            // Conditional branch
            if (debug) std::cout << "Conditional branch" << std::endl;
        }else if (instr_code == 0xa000) {
            // Load address
            if (debug) std::cout << "Load address" << std::endl;
        }else if (instr_code == 0x9000) {
            // SP-relative load/store
            if (debug) std::cout << "SP-relative load/store" << std::endl;
        }else if (instr_code == 0x8000) {
            // Load/store half-word
            if (debug) std::cout << "Load/store half-word" << std::endl;
        }else {
            instr_code = instr_code & 0xe000;

            if (instr_code == 0x6000) {
                // Load/store with immediate offset
                if (debug) std::cout << "Load/store with immediate offset" << std::endl;
            }else if (instr_code == 0x2000) {
                // Move/compare/add/subtract immediate
                if (debug) std::cout << "Move/compare/add/subtract immediate" << std::endl;
            }else if (instr_code == 0x0000) {
                // Move shifted register
                if (debug) std::cout << "Move shifted register" << std::endl;
            }
        }
    }
};

void Gba::cycle() {
    bool thumb = ((reg[CPSR] & 0x00000020) == 0x00000020);
    if (thumb)
        thumbDecode(thumbFetch());
    else
        armDecode(armFetch());
};

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