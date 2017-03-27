#pragma once

#include "memory.h"
#include "rom.h"
#include "util.h"

namespace Frankenstein {

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
    NesMemory& memory;

    typedef u8 (Cpu::*Instruction)(void);

    struct InstructionInfo {
        const char *const name;
        const Instruction fct;
        const u8 size;
    };

    InstructionInfo instructions[256]{
        {"BRK",       &Cpu::BRK,       0}, {"ORA_IND_X", &Cpu::ORA_IND_X, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"ORA_ZP",    &Cpu::ORA_ZP,    2},
        {"ASL_ZP",    &Cpu::ASL_ZP,    2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"PHP",       &Cpu::PHP,       1}, {"ORA_IMM",   &Cpu::ORA_IMM,   2},
        {"ASL_ACC",   &Cpu::ASL_ACC,   1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"ORA_ABS",   &Cpu::ORA_ABS,   3},
        {"ASL_ABS",   &Cpu::ASL_ABS,   3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"BPL",       &Cpu::BPL,       2}, {"ORA_IND_Y", &Cpu::ORA_IND_Y, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"ORA_ZP_X",  &Cpu::ORA_ZP_X,  2},
        {"ASL_ZP_X",  &Cpu::ASL_ZP_X,  2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"CLC",       &Cpu::CLC,       1}, {"ORA_ABS_Y", &Cpu::ORA_ABS_Y, 3},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"ORA_ABS_X", &Cpu::ORA_ABS_X, 3},
        {"ASL_ABS_X", &Cpu::ASL_ABS_X, 3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"JSR",       &Cpu::JSR,       0}, {"AND_IND_X", &Cpu::AND_IND_X, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"BIT_ZP",    &Cpu::BIT_ZP,    2}, {"AND_ZP",    &Cpu::AND_ZP,    2},
        {"ROL_ZP",    &Cpu::ROL_ZP,    2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"PLP",       &Cpu::PLP,       1}, {"AND_IMM",   &Cpu::AND_IMM,   2},
        {"ROL_ACC",   &Cpu::ROL_ACC,   1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"BIT_ABS",   &Cpu::BIT_ABS,   3}, {"AND_ABS",   &Cpu::AND_ABS,   3},
        {"ROL_ABS",   &Cpu::ROL_ABS,   3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"BMI",       &Cpu::BMI,       2}, {"AND_IND_Y", &Cpu::AND_IND_Y, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"AND_ZP_X",  &Cpu::AND_ZP_X,  2},
        {"ROL_ZP_X",  &Cpu::ROL_ZP_X,  2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"SEC",       &Cpu::SEC,       1}, {"AND_ABS_Y", &Cpu::AND_ABS_Y, 3},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"AND_ABS_X", &Cpu::AND_ABS_X, 3},
        {"ROL_ABS_X", &Cpu::ROL_ABS_X, 3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"RTI",       &Cpu::RTI,       1}, {"EOR_IND_X", &Cpu::EOR_IND_X, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"EOR_ZP",    &Cpu::EOR_ZP,    2},
        {"LSR_ZP",    &Cpu::LSR_ZP,    2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"PHA",       &Cpu::PHA,       1}, {"EOR_IMM",   &Cpu::EOR_IMM,   2},
        {"LSR_ACC",   &Cpu::LSR_ACC,   1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"JMP_ABS",   &Cpu::JMP_ABS,   0}, {"EOR_ABS",   &Cpu::EOR_ABS,   3},
        {"LSR_ABS",   &Cpu::LSR_ABS,   3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"BVC",       &Cpu::BVC,       2}, {"EOR_IND_Y", &Cpu::EOR_IND_Y, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"EOR_ZP_X",  &Cpu::EOR_ZP_X,  2},
        {"LSR_ZP_X",  &Cpu::LSR_ZP_X,  2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"CLI",       &Cpu::CLI,       1}, {"EOR_ABS_Y", &Cpu::EOR_ABS_Y, 3},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"EOR_ABS_X", &Cpu::EOR_ABS_X, 3},
        {"LSR_ABS_X", &Cpu::LSR_ABS_X, 3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"RTS",       &Cpu::RTS,       1}, {"ADC_IND_X", &Cpu::ADC_IND_X, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"ADC_ZP",    &Cpu::ADC_ZP,    2},
        {"ROR_ZP",    &Cpu::ROR_ZP,    2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"PLA",       &Cpu::PLA,       1}, {"ADC_IMM",   &Cpu::ADC_IMM,   2},
        {"ROR_ACC",   &Cpu::ROR_ACC,   1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"JMP_IND",   &Cpu::JMP_IND,   0}, {"ADC_ABS",   &Cpu::ADC_ABS,   3},
        {"ROR_ABS",   &Cpu::ROR_ABS,   3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"BVS",       &Cpu::BVS,       2}, {"ADC_IND_Y", &Cpu::ADC_IND_Y, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"ADC_ZP_X",  &Cpu::ADC_ZP_X,  2},
        {"ROR_ZP_X",  &Cpu::ROR_ZP_X,  2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"SEI",       &Cpu::SEI,       1}, {"ADC_ABS_Y", &Cpu::ADC_ABS_Y, 3},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"ADC_ABS_X", &Cpu::ADC_ABS_X, 3},
        {"ROR_ABS_X", &Cpu::ROR_ABS_X, 3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"STA_IND_X", &Cpu::STA_IND_X, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"STY_ZP",    &Cpu::STY_ZP,    2}, {"STA_ZP",    &Cpu::STA_ZP,    2},
        {"STX_ZP",    &Cpu::STX_ZP,    2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"DEY",       &Cpu::DEY,       1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"TXA",       &Cpu::TXA,       1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"STY_ABS",   &Cpu::STY_ABS,   3}, {"STA_ABS",   &Cpu::STA_ABS,   3},
        {"STX_ABS",   &Cpu::STX_ABS,   3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"BCC",       &Cpu::BCC,       2}, {"STA_IND_Y", &Cpu::STA_IND_Y, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"STY_ZP_X",  &Cpu::STY_ZP_X,  2}, {"STA_ZP_X",  &Cpu::STA_ZP_X,  2},
        {"STX_ZP_Y",  &Cpu::STX_ZP_Y,  2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"TYA",       &Cpu::TYA,       1}, {"STA_ABS_Y", &Cpu::STA_ABS_Y, 3},
        {"TXS",       &Cpu::TXS,       1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"STA_ABS_X", &Cpu::STA_ABS_X, 3},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"LDY_IMM",   &Cpu::LDY_IMM,   2}, {"LDA_IND_X", &Cpu::LDA_IND_X, 2},
        {"LDX_IMM",   &Cpu::LDX_IMM,   2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"LDY_ZP",    &Cpu::LDY_ZP,    2}, {"LDA_ZP",    &Cpu::LDA_ZP,    2},
        {"LDX_ZP",    &Cpu::LDX_ZP,    2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"TAY",       &Cpu::TAY,       1}, {"LDA_IMM",   &Cpu::LDA_IMM,   2},
        {"TAX",       &Cpu::TAX,       1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"LDY_ABS",   &Cpu::LDY_ABS,   3}, {"LDA_ABS",   &Cpu::LDA_ABS,   3},
        {"LDX_ABS",   &Cpu::LDX_ABS,   3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"BCS",       &Cpu::BCS,       2}, {"LDA_IND_Y", &Cpu::LDA_IND_Y, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"LDY_ZP_X",  &Cpu::LDY_ZP_X,  2}, {"LDA_ZP_X",  &Cpu::LDA_ZP_X,  2},
        {"LDX_ZP_Y",  &Cpu::LDX_ZP_Y,  2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"CLV",       &Cpu::CLV,       1}, {"LDA_ABS_Y", &Cpu::LDA_ABS_Y, 3},
        {"TSX",       &Cpu::TSX,       1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"LDY_ABS_X", &Cpu::LDY_ABS_X, 3}, {"LDA_ABS_X", &Cpu::LDA_ABS_X, 3},
        {"LDX_ABS_Y", &Cpu::LDX_ABS_Y, 3}, {"CPY_IMM",   &Cpu::CPY_IMM,   1},
        {"CMP_IND_X", &Cpu::CMP_IND_X, 2}, {"UNIMP",     &Cpu::UNIMP,     2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"CPY_ZP",    &Cpu::CPY_ZP,    2}, {"CMP_ZP",    &Cpu::CMP_ZP,    2},
        {"DEC_ZP",    &Cpu::DEC_ZP,    2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"INY",       &Cpu::INY,       1}, {"CMP_IMM",   &Cpu::CMP_IMM,   2},
        {"DEX",       &Cpu::DEX,       1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"CPY_ABS",   &Cpu::CPY_ABS,   3}, {"CMP_ABS",   &Cpu::CMP_ABS,   3},
        {"DEC_ABS",   &Cpu::DEC_ABS,   3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"BNE",       &Cpu::BNE,       2}, {"CMP_IND_Y", &Cpu::CMP_IND_Y, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"CMP_ZP_X",  &Cpu::CMP_ZP_X,  2},
        {"DEC_ZP_X",  &Cpu::DEC_ZP_X,  2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"CLD",       &Cpu::CLD,       1}, {"CMP_ABS_Y", &Cpu::CMP_ABS_Y, 3},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"CMP_ABS_X", &Cpu::CMP_ABS_X, 3},
        {"DEC_ABS_X", &Cpu::DEC_ABS_X, 3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"CPX_IMM",   &Cpu::CPX_IMM,   2}, {"SBC_IND_X", &Cpu::SBC_IND_X, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"CPX_ZP",    &Cpu::CPX_ZP,    2}, {"SBC_ZP",    &Cpu::SBC_ZP,    2},
        {"INC_ZP",    &Cpu::INC_ZP,    2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"INX",       &Cpu::INX,       1}, {"SBC_IMM",   &Cpu::SBC_IMM,   2},
        {"NOP",       &Cpu::NOP,       1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"CPX_ABS",   &Cpu::CPX_ABS,   3}, {"SBC_ABS",   &Cpu::SBC_ABS,   3},
        {"INC_ABS",   &Cpu::INC_ABS,   3}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"BEQ",       &Cpu::BEQ,       2}, {"SBC_IND_Y", &Cpu::SBC_IND_Y, 2},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"SBC_ZP_X",  &Cpu::SBC_ZP_X,  2},
        {"INC_ZP_X",  &Cpu::INC_ZP_X,  2}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"SED",       &Cpu::SED,       1}, {"SBC_ABS_Y", &Cpu::SBC_ABS_Y, 3},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"UNIMP",     &Cpu::UNIMP,     1},
        {"UNIMP",     &Cpu::UNIMP,     1}, {"SBC_ABS_X", &Cpu::SBC_ABS_X, 3},
        {"INC_ABS_X", &Cpu::INC_ABS_X, 3}, {"UNIMP",     &Cpu::UNIMP,     1},
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
    inline const u8 OpCode() const
    {
        return this->memory[this->registers.PC];
    }

    /**
     * Fetch the operand at memory[PC + number]
     */
    inline const u8 Operand(int number) const
    {
        return this->memory[this->registers.PC + number];
    }

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

    Cpu(NesMemory& ram);
    Cpu(NesMemory& ram, const IRom& rom);

    void LoadRom(const IRom& rom);

    u8 cycles;
    bool nmiOccurred;
    u16 previousPC;
    u8 currentOpcode;
    u8 nextOpcode;

};

}
