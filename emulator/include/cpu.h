#pragma once

#include "memory.h"
#include "rom.h"
#include "util.h"

namespace Frankenstein {

class Nes;
class Cpu {

    using Memory = NesMemory::Ref;

public:
    struct Registers {
        u16 PC;
        u8 SP;
        u8 A;
        u8 X;
        u8 Y;
        u8 P;
    };

    enum class Flags : int {
        C = 1, //Carry
        Z, //Zero
        I, //Interrupt
        D, //Decimal
        B, //Break
        A, //Always
        V, //Overflow
        S //Sign : 1 is negative
    };

    Registers registers;

    typedef u8 (Cpu::*Instruction)(void);

    struct InstructionInfo {
        const char *const name;
        const Instruction fct;
        const u8 size;
    };

    InstructionInfo instructions[256]{
        {"BRK",       &Cpu::BRK,       0},	//0x0
	{"ORA_IND_X", &Cpu::ORA_IND_X, 2},	//0x1
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x2
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x3
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x4
	{"ORA_ZP",    &Cpu::ORA_ZP,    2},	//0x5
	{"ASL_ZP",    &Cpu::ASL_ZP,    2},	//0x6
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x7
	{"PHP",       &Cpu::PHP,       1},	//0x8
	{"ORA_IMM",   &Cpu::ORA_IMM,   2},	//0x9
	{"ASL_ACC",   &Cpu::ASL_ACC,   1},	//0xA
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xB
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xC
	{"ORA_ABS",   &Cpu::ORA_ABS,   3},	//0xD
	{"ASL_ABS",   &Cpu::ASL_ABS,   3},	//0xE
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xF
	{"BPL",       &Cpu::BPL,       2},	//0x10
	{"ORA_IND_Y", &Cpu::ORA_IND_Y, 2},	//0x11
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x12
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x13
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x14
	{"ORA_ZP_X",  &Cpu::ORA_ZP_X,  2},	//0x15
	{"ASL_ZP_X",  &Cpu::ASL_ZP_X,  2},	//0x16
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x17
	{"CLC",       &Cpu::CLC,       1},	//0x18
	{"ORA_ABS_Y", &Cpu::ORA_ABS_Y, 3},	//0x19
	{"NOP",       &Cpu::NOP,       1},	//0x1A
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x1B
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x1C
	{"ORA_ABS_X", &Cpu::ORA_ABS_X, 3},	//0x1D
	{"ASL_ABS_X", &Cpu::ASL_ABS_X, 3},	//0x1E
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x1F
	{"JSR",       &Cpu::JSR,       0},	//0x20
	{"AND_IND_X", &Cpu::AND_IND_X, 2},	//0x21
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x22
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x23
	{"BIT_ZP",    &Cpu::BIT_ZP,    2},	//0x24
	{"AND_ZP",    &Cpu::AND_ZP,    2},	//0x25
	{"ROL_ZP",    &Cpu::ROL_ZP,    2},	//0x26
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x27
	{"PLP",       &Cpu::PLP,       1},	//0x28
	{"AND_IMM",   &Cpu::AND_IMM,   2},	//0x29
	{"ROL_ACC",   &Cpu::ROL_ACC,   1},	//0x2A
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x2B
	{"BIT_ABS",   &Cpu::BIT_ABS,   3},	//0x2C
	{"AND_ABS",   &Cpu::AND_ABS,   3},	//0x2D
	{"ROL_ABS",   &Cpu::ROL_ABS,   3},	//0x2E
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x2F
	{"BMI",       &Cpu::BMI,       2},	//0x30
	{"AND_IND_Y", &Cpu::AND_IND_Y, 2},	//0x31
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x32
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x33
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x34
	{"AND_ZP_X",  &Cpu::AND_ZP_X,  2},	//0x35
	{"ROL_ZP_X",  &Cpu::ROL_ZP_X,  2},	//0x36
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x37
	{"SEC",       &Cpu::SEC,       1},	//0x38
	{"AND_ABS_Y", &Cpu::AND_ABS_Y, 3},	//0x39
	{"NOP",       &Cpu::NOP,       1},	//0x3A
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x3B
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x3C
	{"AND_ABS_X", &Cpu::AND_ABS_X, 3},	//0x3D
	{"ROL_ABS_X", &Cpu::ROL_ABS_X, 3},	//0x3E
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x3F
	{"RTI",       &Cpu::RTI,       0},	//0x40
	{"EOR_IND_X", &Cpu::EOR_IND_X, 2},	//0x41
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x42
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x43
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x44
	{"EOR_ZP",    &Cpu::EOR_ZP,    2},	//0x45
	{"LSR_ZP",    &Cpu::LSR_ZP,    2},	//0x46
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x47
	{"PHA",       &Cpu::PHA,       1},	//0x48
	{"EOR_IMM",   &Cpu::EOR_IMM,   2},	//0x49
	{"LSR_ACC",   &Cpu::LSR_ACC,   1},	//0x4A
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x4B
	{"JMP_ABS",   &Cpu::JMP_ABS,   0},	//0x4C
	{"EOR_ABS",   &Cpu::EOR_ABS,   3},	//0x4D
	{"LSR_ABS",   &Cpu::LSR_ABS,   3},	//0x4E
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x4F
	{"BVC",       &Cpu::BVC,       2},	//0x50
	{"EOR_IND_Y", &Cpu::EOR_IND_Y, 2},	//0x51
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x52
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x53
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x54
	{"EOR_ZP_X",  &Cpu::EOR_ZP_X,  2},	//0x55
	{"LSR_ZP_X",  &Cpu::LSR_ZP_X,  2},	//0x56
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x57
	{"CLI",       &Cpu::CLI,       1},	//0x58
	{"EOR_ABS_Y", &Cpu::EOR_ABS_Y, 3},	//0x59
	{"NOP",       &Cpu::NOP,       1},	//0x5A
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x5B
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x5C
	{"EOR_ABS_X", &Cpu::EOR_ABS_X, 3},	//0x5D
	{"LSR_ABS_X", &Cpu::LSR_ABS_X, 3},	//0x5E
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x5F
	{"RTS",       &Cpu::RTS,       1},	//0x60
	{"ADC_IND_X", &Cpu::ADC_IND_X, 2},	//0x61
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x62
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x63
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x64
	{"ADC_ZP",    &Cpu::ADC_ZP,    2},	//0x65
	{"ROR_ZP",    &Cpu::ROR_ZP,    2},	//0x66
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x67
	{"PLA",       &Cpu::PLA,       1},	//0x68
	{"ADC_IMM",   &Cpu::ADC_IMM,   2},	//0x69
	{"ROR_ACC",   &Cpu::ROR_ACC,   1},	//0x6A
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x6B
	{"JMP_IND",   &Cpu::JMP_IND,   0},	//0x6C
	{"ADC_ABS",   &Cpu::ADC_ABS,   3},	//0x6D
	{"ROR_ABS",   &Cpu::ROR_ABS,   3},	//0x6E
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x6F
	{"BVS",       &Cpu::BVS,       2},	//0x70
	{"ADC_IND_Y", &Cpu::ADC_IND_Y, 2},	//0x71
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x72
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x73
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x74
	{"ADC_ZP_X",  &Cpu::ADC_ZP_X,  2},	//0x75
	{"ROR_ZP_X",  &Cpu::ROR_ZP_X,  2},	//0x76
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x77
	{"SEI",       &Cpu::SEI,       1},	//0x78
	{"ADC_ABS_Y", &Cpu::ADC_ABS_Y, 3},	//0x79
	{"NOP",       &Cpu::NOP,       1},	//0x7A
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x7B
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x7C
	{"ADC_ABS_X", &Cpu::ADC_ABS_X, 3},	//0x7D
	{"ROR_ABS_X", &Cpu::ROR_ABS_X, 3},	//0x7E
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x7F
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x80
	{"STA_IND_X", &Cpu::STA_IND_X, 2},	//0x81
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x82
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x83
	{"STY_ZP",    &Cpu::STY_ZP,    2},	//0x84
	{"STA_ZP",    &Cpu::STA_ZP,    2},	//0x85
	{"STX_ZP",    &Cpu::STX_ZP,    2},	//0x86
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x87
	{"DEY",       &Cpu::DEY,       1},	//0x88
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x89
	{"TXA",       &Cpu::TXA,       1},	//0x8A
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x8B
	{"STY_ABS",   &Cpu::STY_ABS,   3},	//0x8C
	{"STA_ABS",   &Cpu::STA_ABS,   3},	//0x8D
	{"STX_ABS",   &Cpu::STX_ABS,   3},	//0x8E
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x8F
	{"BCC",       &Cpu::BCC,       2},	//0x90
	{"STA_IND_Y", &Cpu::STA_IND_Y, 2},	//0x91
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x92
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x93
	{"STY_ZP_X",  &Cpu::STY_ZP_X,  2},	//0x94
	{"STA_ZP_X",  &Cpu::STA_ZP_X,  2},	//0x95
	{"STX_ZP_Y",  &Cpu::STX_ZP_Y,  2},	//0x96
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x97
	{"TYA",       &Cpu::TYA,       1},	//0x98
	{"STA_ABS_Y", &Cpu::STA_ABS_Y, 3},	//0x99
	{"TXS",       &Cpu::TXS,       1},	//0x9A
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x9B
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x9C
	{"STA_ABS_X", &Cpu::STA_ABS_X, 3},	//0x9D
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x9E
	{"UNIMP",     &Cpu::UNIMP,     1},	//0x9F
	{"LDY_IMM",   &Cpu::LDY_IMM,   2},	//0xA0
	{"LDA_IND_X", &Cpu::LDA_IND_X, 2},	//0xA1
	{"LDX_IMM",   &Cpu::LDX_IMM,   2},	//0xA2
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xA3
	{"LDY_ZP",    &Cpu::LDY_ZP,    2},	//0xA4
	{"LDA_ZP",    &Cpu::LDA_ZP,    2},	//0xA5
	{"LDX_ZP",    &Cpu::LDX_ZP,    2},	//0xA6
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xA7
	{"TAY",       &Cpu::TAY,       1},	//0xA8
	{"LDA_IMM",   &Cpu::LDA_IMM,   2},	//0xA9
	{"TAX",       &Cpu::TAX,       1},	//0xAA
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xAB
	{"LDY_ABS",   &Cpu::LDY_ABS,   3},	//0xAC
	{"LDA_ABS",   &Cpu::LDA_ABS,   3},	//0xAD
	{"LDX_ABS",   &Cpu::LDX_ABS,   3},	//0xAE
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xAF
	{"BCS",       &Cpu::BCS,       2},	//0xB0
	{"LDA_IND_Y", &Cpu::LDA_IND_Y, 2},	//0xB1
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xB2
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xB3
	{"LDY_ZP_X",  &Cpu::LDY_ZP_X,  2},	//0xB4
	{"LDA_ZP_X",  &Cpu::LDA_ZP_X,  2},	//0xB5
	{"LDX_ZP_Y",  &Cpu::LDX_ZP_Y,  2},	//0xB6
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xB7
	{"CLV",       &Cpu::CLV,       1},	//0xB8
	{"LDA_ABS_Y", &Cpu::LDA_ABS_Y, 3},	//0xB9
	{"TSX",       &Cpu::TSX,       1},	//0xBA
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xBB
	{"LDY_ABS_X", &Cpu::LDY_ABS_X, 3},	//0xBC
	{"LDA_ABS_X", &Cpu::LDA_ABS_X, 3},	//0xBD
	{"LDX_ABS_Y", &Cpu::LDX_ABS_Y, 3},	//0xBE
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xBF
	{"CPY_IMM",   &Cpu::CPY_IMM,   2},	//0xC0
	{"CMP_IND_X", &Cpu::CMP_IND_X, 2},	//0xC1
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xC2
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xC3
	{"CPY_ZP",    &Cpu::CPY_ZP,    2},	//0xC4
	{"CMP_ZP",    &Cpu::CMP_ZP,    2},	//0xC5
	{"DEC_ZP",    &Cpu::DEC_ZP,    2},	//0xC6
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xC7
	{"INY",       &Cpu::INY,       1},	//0xC8
	{"CMP_IMM",   &Cpu::CMP_IMM,   2},	//0xC9
	{"DEX",       &Cpu::DEX,       1},	//0xCA
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xCB
	{"CPY_ABS",   &Cpu::CPY_ABS,   3},	//0xCC
	{"CMP_ABS",   &Cpu::CMP_ABS,   3},	//0xCD
	{"DEC_ABS",   &Cpu::DEC_ABS,   3},	//0xCE
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xCF
	{"BNE",       &Cpu::BNE,       2},	//0xD0
	{"CMP_IND_Y", &Cpu::CMP_IND_Y, 2},	//0xD1
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xD2
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xD3
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xD4
	{"CMP_ZP_X",  &Cpu::CMP_ZP_X,  2},	//0xD5
	{"DEC_ZP_X",  &Cpu::DEC_ZP_X,  2},	//0xD6
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xD7
	{"CLD",       &Cpu::CLD,       1},	//0xD8
	{"CMP_ABS_Y", &Cpu::CMP_ABS_Y, 3},	//0xD9
	{"NOP",       &Cpu::NOP,       1},	//0xDA
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xDB
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xDC
	{"CMP_ABS_X", &Cpu::CMP_ABS_X, 3},	//0xDD
	{"DEC_ABS_X", &Cpu::DEC_ABS_X, 3},	//0xDE
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xDF
	{"CPX_IMM",   &Cpu::CPX_IMM,   2},	//0xE0
	{"SBC_IND_X", &Cpu::SBC_IND_X, 2},	//0xE1
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xE2
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xE3
	{"CPX_ZP",    &Cpu::CPX_ZP,    2},	//0xE4
	{"SBC_ZP",    &Cpu::SBC_ZP,    2},	//0xE5
	{"INC_ZP",    &Cpu::INC_ZP,    2},	//0xE6
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xE7
	{"INX",       &Cpu::INX,       1},	//0xE8
	{"SBC_IMM",   &Cpu::SBC_IMM,   2},	//0xE9
	{"NOP",       &Cpu::NOP,       1},	//0xEA
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xEB
	{"CPX_ABS",   &Cpu::CPX_ABS,   3},	//0xEC
	{"SBC_ABS",   &Cpu::SBC_ABS,   3},	//0xED
	{"INC_ABS",   &Cpu::INC_ABS,   3},	//0xEE
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xEF
	{"BEQ",       &Cpu::BEQ,       2},	//0xF0
	{"SBC_IND_Y", &Cpu::SBC_IND_Y, 2},	//0xF1
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xF2
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xF3
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xF4
	{"SBC_ZP_X",  &Cpu::SBC_ZP_X,  2},	//0xF5
	{"INC_ZP_X",  &Cpu::INC_ZP_X,  2},	//0xF6
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xF7
	{"SED",       &Cpu::SED,       1},	//0xF8
	{"SBC_ABS_Y", &Cpu::SBC_ABS_Y, 3},	//0xF9
	{"NOP",       &Cpu::NOP,       1},	//0xFA
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xFB
	{"UNIMP",     &Cpu::UNIMP,     1},	//0xFC
	{"SBC_ABS_X", &Cpu::SBC_ABS_X, 3},	//0xFD
	{"INC_ABS_X", &Cpu::INC_ABS_X, 3},	//0xFE
	{"UNIMP",     &Cpu::UNIMP,     1}	//0xFF

    };

    //
    // The instructions operations, either on a value or a memory cell
    //
    void ADC(const u8 value);
    void AND(const u8 value);
    void ASL(u8& value);
    void ASL(const Memory value);
    void BIT(const u8 value);
    void CMP(const u8 value);
    void CPX(const u8 value);
    void CPY(const u8 value);
    void DEC(u8& value);
    void DEC(const Memory value);
    void EOR(const u8 value);
    void INC(u8& value);
    void INC(const Memory value);
    void JMP(const u16 value);
    void LDA(const u8 value);
    void LDX(const u8 value);
    void LDY(const u8 value);
    void LSR(u8& value);
    void LSR(const Memory value);
    void ORA(const u8 value);
    void ROL(u8& value);
    void ROL(const Memory value);
    void ROR(u8& value);
    void ROR(const Memory value);
    void SBC(const u8 value);
    void STA(const Memory value);
    void STX(const Memory value);
    void STY(const Memory value);

    //
    // The instructions functions using the memory mode
    //

    u8 BRK();
    u8 ORA_IND_X();
    u8 ORA_ZP();
    u8 ASL_ZP();
    u8 PHP();
    u8 ORA_IMM();
    u8 ASL_ACC();
    u8 ORA_ABS();
    u8 ASL_ABS();
    u8 BPL();
    u8 ORA_IND_Y();
    u8 ORA_ZP_X();
    u8 ASL_ZP_X();
    u8 CLC();
    u8 ORA_ABS_Y();
    u8 ORA_ABS_X();
    u8 ASL_ABS_X();
    u8 JSR();
    u8 AND_IND_X();
    u8 BIT_ZP();
    u8 AND_ZP();
    u8 ROL_ZP();
    u8 PLP();
    u8 AND_IMM();
    u8 ROL_ACC();
    u8 BIT_ABS();
    u8 AND_ABS();
    u8 ROL_ABS();
    u8 BMI();
    u8 AND_IND_Y();
    u8 AND_ZP_X();
    u8 ROL_ZP_X();
    u8 SEC();
    u8 AND_ABS_Y();
    u8 AND_ABS_X();
    u8 ROL_ABS_X();
    u8 RTI();
    u8 EOR_IND_X();
    u8 EOR_ZP();
    u8 LSR_ZP();
    u8 PHA();
    u8 EOR_IMM();
    u8 LSR_ACC();
    u8 JMP_ABS();
    u8 EOR_ABS();
    u8 LSR_ABS();
    u8 BVC();
    u8 EOR_IND_Y();
    u8 EOR_ZP_X();
    u8 LSR_ZP_X();
    u8 CLI();
    u8 EOR_ABS_Y();
    u8 EOR_ABS_X();
    u8 LSR_ABS_X();
    u8 RTS();
    u8 ADC_IND_X();
    u8 ADC_ZP();
    u8 ROR_ZP();
    u8 PLA();
    u8 ADC_IMM();
    u8 ROR_ACC();
    u8 JMP_IND();
    u8 ADC_ABS();
    u8 ROR_ABS();
    u8 BVS();
    u8 ADC_IND_Y();
    u8 ADC_ZP_X();
    u8 ROR_ZP_X();
    u8 SEI();
    u8 ADC_ABS_Y();
    u8 ADC_ABS_X();
    u8 ROR_ABS_X();
    u8 STA_IND_X();
    u8 STY_ZP();
    u8 STA_ZP();
    u8 STX_ZP();
    u8 DEY();
    u8 TXA();
    u8 STY_ABS();
    u8 STA_ABS();
    u8 STX_ABS();
    u8 BCC();
    u8 STA_IND_Y();
    u8 STY_ZP_X();
    u8 STA_ZP_X();
    u8 STX_ZP_Y();
    u8 TYA();
    u8 STA_ABS_Y();
    u8 TXS();
    u8 STA_ABS_X();
    u8 LDY_IMM();
    u8 LDA_IND_X();
    u8 LDX_IMM();
    u8 LDY_ZP();
    u8 LDA_ZP();
    u8 LDX_ZP();
    u8 TAY();
    u8 LDA_IMM();
    u8 TAX();
    u8 LDY_ABS();
    u8 LDA_ABS();
    u8 LDX_ABS();
    u8 BCS();
    u8 LDA_IND_Y();
    u8 LDY_ZP_X();
    u8 LDA_ZP_X();
    u8 LDX_ZP_Y();
    u8 CLV();
    u8 LDA_ABS_Y();
    u8 TSX();
    u8 LDY_ABS_X();
    u8 LDA_ABS_X();
    u8 LDX_ABS_Y();
    u8 CPY_IMM();
    u8 CMP_IND_X();
    u8 CPY_ZP();
    u8 CMP_ZP();
    u8 DEC_ZP();
    u8 INY();
    u8 CMP_IMM();
    u8 DEX();
    u8 CPY_ABS();
    u8 CMP_ABS();
    u8 DEC_ABS();
    u8 BNE();
    u8 CMP_IND_Y();
    u8 CMP_ZP_X();
    u8 DEC_ZP_X();
    u8 CLD();
    u8 CMP_ABS_Y();
    u8 CMP_ABS_X();
    u8 DEC_ABS_X();
    u8 CPX_IMM();
    u8 SBC_IND_X();
    u8 CPX_ZP();
    u8 SBC_ZP();
    u8 INC_ZP();
    u8 INX();
    u8 SBC_IMM();
    u8 NOP();
    u8 CPX_ABS();
    u8 SBC_ABS();
    u8 INC_ABS();
    u8 BEQ();
    u8 SBC_IND_Y();
    u8 SBC_ZP_X();
    u8 INC_ZP_X();
    u8 SED();
    u8 SBC_ABS_Y();
    u8 SBC_ABS_X();
    u8 INC_ABS_X();
    u8 UNIMP();

    u8 NMI();

    void Interrupt();

    void Reset();

    /**
     * Executes the next instruction at memory[PC] if no interrupt occured.
     * Increments the PC accordingly
     */
    void Step();

    /**
    * Fetch the opcode at memory[PC]
    */
    u8 OpCode();

    /**
    * Fetch the operand at memory[PC + number]
    */
    u8 Operand(int number);

    /**
     * Store the byte at stack[SP]
     * and decrement the stack pointer
     */
    void PushOnStack(u8 value);

    /**
     * Fetch the byte at stack[SP]
     * and increment the stack pointer
     */
    u8 PopFromStack();

    template <Cpu::Flags f>
    void Set(bool value)
    {
        AssignBit<static_cast<int>(f)>(this->registers.P, value);
    }

    template <Cpu::Flags f>
    bool Get()
    {
        return CheckBit<static_cast<int>(f)>(this->registers.P);
    }

    Cpu(Nes& pNes);

    void LoadRom(const IRom& rom);

    u8 cycles;
    u16 stall;
    bool nmiOccurred;
    u16 previousPC;
    u8 currentOpcode;
    u8 nextOpcode;

    Nes& nes;
};

}


