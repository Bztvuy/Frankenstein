#ifndef CPU_H
#define CPU_H

#include "rom.h"

union NESCPUMemoryMap{
    struct {
        unsigned char zeroPage[0x0100];
        unsigned char stack[0x0100];
        unsigned char ram[0x0600];
        unsigned char mirrors1[0x1800];
        unsigned char ioRegisters1[0x0008];
        unsigned char mirrors2[0x1FF8];
        unsigned char ioRegisters2[0x0020];
        unsigned char expansionRom[0x1FE0];
        unsigned char sram[0x2000];
        unsigned char prgRomLowerBank[0x4000];
        unsigned char prgRomUpperBank[0x4000];
    };
    unsigned char raw[0x10000];
};

union NESCPURegisters{
    struct {
        unsigned short programCounter;
        unsigned char stackPointer;
        unsigned char accumulator;
        unsigned char indexX;
        unsigned char indexY;
        unsigned char processorStatus;
    };
    struct {
        unsigned short PC;
        unsigned char SP;
        unsigned char A;
        unsigned char X;
        unsigned char Y;
        unsigned char P;
    };
};

class Cpu
{
private:
    NESCPURegisters* registers;
    NESCPUMemoryMap* memory;

    //sizes related to hardware (in bytes) :
    const unsigned int prgRomBankSize = 16 * KILOBYTE;
    const unsigned int vRomBankSize = 8 * KILOBYTE;
    const unsigned int prgRamBankSize = 8 * KILOBYTE;
    const unsigned char instructionSizes[256] =
    {   1, 2, 1, 1, 1, 2, 2, 1, 1, 2, 1, 1, 1, 3, 3, 1, 2, 2, 
        1, 1, 1, 2, 2, 1, 1, 3, 1, 1, 1, 3, 3, 1, 3, 2, 1, 1, 
        2, 2, 2, 1, 1, 2, 1, 1, 3, 3, 3, 1, 2, 2, 1, 1, 1, 2, 
        2, 1, 1, 3, 1, 1, 1, 3, 3, 1, 1, 2, 1, 1, 1, 2, 2, 1, 
        1, 2, 1, 1, 3, 3, 3, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1, 3, 
        1, 1, 1, 3, 3, 1, 1, 2, 1, 1, 1, 2, 2, 1, 1, 2, 1, 1, 
        3, 3, 3, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1, 3, 1, 1, 1, 3, 
        3, 1, 1, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 3, 3, 3, 1, 
        2, 2, 1, 1, 2, 2, 2, 1, 1, 3, 1, 1, 1, 3, 1, 1, 2, 2, 
        2, 1, 2, 2, 2, 1, 1, 2, 1, 1, 3, 3, 3, 1, 2, 2, 1, 1, 
        2, 2, 2, 1, 1, 3, 1, 1, 3, 3, 3, 1, 2, 2, 1, 1, 2, 2, 
        2, 1, 1, 2, 1, 1, 3, 3, 3, 1, 2, 2, 1, 1, 1, 2, 2, 1, 
        1, 3, 1, 1, 1, 3, 3, 1, 2, 2, 1, 1, 2, 2, 2, 1, 1, 2, 
        1, 1, 3, 3, 3, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1, 3, 1, 1, 
        1, 3, 3, 1  };

    void BRK();
    void ORA_IND_X();
    void ORA_ZP();
    void ASL_ZP();
    void PHP();
    void ORA_IMM();
    void ASL_ACC();
    void ORA_ABS();
    void ASL_ABS();
    void BPL();
    void ORA_IND_Y();
    void ORA_ZP_X();
    void ASL_ZP_X();
    void CLC();
    void ORA_ABS_Y();
    void ORA_ABS_X();
    void ASL_ABS_X();
    void JSR();
    void AND_IND_X();
    void BIT_ZP();
    void AND_ZP();
    void ROL_ZP();
    void PLP();
    void AND_IMM();
    void ROL_ACC();
    void BIT_ABS();
    void AND_ABS();
    void ROL_ABS();
    void BMI();
    void AND_IND_Y();
    void AND_ZP_X();
    void ROL_ZP_X();
    void SEC();
    void AND_ABS_Y();
    void AND_ABS_X();
    void ROL_ABS_X();
    void RTI();
    void EOR_IND_X();
    void EOR_ZP();
    void LSR_ZP();
    void PHA();
    void EOR_IMM();
    void LSR_ACC();
    void JMP_ABS();
    void EOR_ABS();
    void LSR_ABS();
    void BVC();
    void EOR_IND_Y();
    void EOR_ZP_X();
    void LSR_ZP_X();
    void CLI();
    void EOR_ABS_Y();
    void EOR_ABS_X();
    void LSR_ABS_X();
    void RTS();
    void ADC_IND_X();
    void ADC_ZP();
    void ROR_ZP();
    void PLA();
    void ADC_IMM();
    void ROR_ACC();
    void JMP_IND();
    void ADC_ABS();
    void ROR_ABS();
    void BVS();
    void ADC_IND_Y();
    void ADC_ZP_X();
    void ROR_ZP_X();
    void SEI();
    void ADC_ABS_Y();
    void ADC_ABS_X();
    void ROR_ABS_X();
    void STA_IND_X();
    void STY_ZP();
    void STA_ZP();
    void STX_ZP();
    void DEY();
    void TXA();
    void STY_ABS();
    void STA_ABS();
    void STX_ABS();
    void BCC();
    void STA_IND_Y();
    void STY_ZP_X();
    void STA_ZP_X();
    void STX_ZP_Y();
    void TYA();
    void STA_ABS_Y();
    void TXS();
    void STA_ABS_X();
    void LDY_IMM();
    void LDA_IND_X();
    void LDX_IMM();
    void LDY_ZP();
    void LDA_ZP();
    void LDX_ZP();
    void TAY();
    void LDA_IMM();
    void TAX();
    void LDY_ABS();
    void LDA_ABS();
    void LDX_ABS();
    void BCS();
    void LDA_IND_Y();
    void LDY_ZP_X();
    void LDA_ZP_X();
    void LDX_ZP_Y();
    void CLV();
    void LDA_ABS_Y();
    void TSX();
    void LDY_ABS_X();
    void LDA_ABS_X();
    void LDX_ABS_Y();
    void CPY_IMM();
    void CMP_IND_X();
    void CPY_ZP();
    void CMP_ZP();
    void DEC_ZP();
    void INY();
    void CMP_IMM();
    void DEX();
    void CPY_ABS();
    void CMP_ABS();
    void DEC_ABS();
    void BNE();
    void CMP_IND_Y();
    void CMP_ZP_X();
    void DEC_ZP_X();
    void CLD();
    void CMP_ABS_Y();
    void CMP_ABS_X();
    void DEC_ABS_X();
    void CPX_IMM();
    void SBC_IND_X();
    void CPX_ZP();
    void SBC_ZP();
    void INC_ZP();
    void INX();
    void SBC_IMM();
    void NOP();
    void CPX_ABS();
    void SBC_ABS();
    void INC_ABS();
    void BEQ();
    void SBC_IND_Y();
    void SBC_ZP_X();
    void INC_ZP_X();
    void SED();
    void SBC_ABS_Y();
    void SBC_ABS_X();
    void INC_ABS_X();
    void UNIMP();
    
    typedef void (Cpu::*func)(void);
    
    const func instructions[256]{
        &Cpu::BRK,      &Cpu::ORA_IND_X,&Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::UNIMP,
        &Cpu::ORA_ZP,   &Cpu::ASL_ZP,   &Cpu::UNIMP,    &Cpu::PHP,      &Cpu::ORA_IMM,
        &Cpu::ASL_ACC,  &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::ORA_ABS,  &Cpu::ASL_ABS,
        &Cpu::UNIMP,    &Cpu::BPL,      &Cpu::ORA_IND_Y,&Cpu::UNIMP,    &Cpu::UNIMP,
        &Cpu::UNIMP,    &Cpu::ORA_ZP_X, &Cpu::ASL_ZP_X, &Cpu::UNIMP,    &Cpu::CLC,
        &Cpu::ORA_ABS_Y,&Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::ORA_ABS_X,
        &Cpu::ASL_ABS_X,&Cpu::UNIMP,    &Cpu::JSR,      &Cpu::AND_IND_X,&Cpu::UNIMP,
        &Cpu::UNIMP,    &Cpu::BIT_ZP,   &Cpu::AND_ZP,   &Cpu::ROL_ZP,   &Cpu::UNIMP, 
        &Cpu::PLP,      &Cpu::AND_IMM,  &Cpu::ROL_ACC,  &Cpu::UNIMP,    &Cpu::BIT_ABS, 
        &Cpu::AND_ABS,  &Cpu::ROL_ABS,  &Cpu::UNIMP,    &Cpu::BMI,      &Cpu::AND_IND_Y,
        &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::AND_ZP_X, &Cpu::ROL_ZP_X,
        &Cpu::UNIMP,    &Cpu::SEC,      &Cpu::AND_ABS_Y,&Cpu::UNIMP,    &Cpu::UNIMP,
        &Cpu::UNIMP,    &Cpu::AND_ABS_X,&Cpu::ROL_ABS_X,&Cpu::UNIMP,    &Cpu::RTI,
        &Cpu::EOR_IND_X,&Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::EOR_ZP,
        &Cpu::LSR_ZP,   &Cpu::UNIMP,    &Cpu::PHA,      &Cpu::EOR_IMM,  &Cpu::LSR_ACC,
        &Cpu::UNIMP,    &Cpu::JMP_ABS,  &Cpu::EOR_ABS,  &Cpu::LSR_ABS,  &Cpu::UNIMP,
        &Cpu::BVC,      &Cpu::EOR_IND_Y,&Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::UNIMP,
        &Cpu::EOR_ZP_X, &Cpu::LSR_ZP_X, &Cpu::UNIMP,    &Cpu::CLI,      &Cpu::EOR_ABS_Y,
        &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::EOR_ABS_X,&Cpu::LSR_ABS_X,
        &Cpu::UNIMP,    &Cpu::RTS,      &Cpu::ADC_IND_X,&Cpu::UNIMP,    &Cpu::UNIMP,
        &Cpu::UNIMP,    &Cpu::ADC_ZP,   &Cpu::ROR_ZP,   &Cpu::UNIMP,    &Cpu::PLA,
        &Cpu::ADC_IMM,  &Cpu::ROR_ACC,  &Cpu::UNIMP,    &Cpu::JMP_IND,  &Cpu::ADC_ABS,
        &Cpu::ROR_ABS,  &Cpu::UNIMP,    &Cpu::BVS,      &Cpu::ADC_IND_Y,&Cpu::UNIMP,
        &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::ADC_ZP_X, &Cpu::ROR_ZP_X, &Cpu::UNIMP,
        &Cpu::SEI,      &Cpu::ADC_ABS_Y,&Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::UNIMP,
        &Cpu::ADC_ABS_X,&Cpu::ROR_ABS_X,&Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::STA_IND_X,
        &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::STY_ZP,   &Cpu::STA_ZP,   &Cpu::STX_ZP,   
        &Cpu::UNIMP,    &Cpu::DEY,      &Cpu::UNIMP,    &Cpu::TXA,      &Cpu::UNIMP,    
        &Cpu::STY_ABS,  &Cpu::STA_ABS,  &Cpu::STX_ABS,  &Cpu::UNIMP,    &Cpu::BCC,      
        &Cpu::STA_IND_Y,&Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::STY_ZP_X, &Cpu::STA_ZP_X, 
        &Cpu::STX_ZP_Y, &Cpu::UNIMP,    &Cpu::TYA,      &Cpu::STA_ABS_Y,&Cpu::TXS,      
        &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::STA_ABS_X,&Cpu::UNIMP,    &Cpu::UNIMP,    
        &Cpu::LDY_IMM,  &Cpu::LDA_IND_X,&Cpu::LDX_IMM,  &Cpu::UNIMP,    &Cpu::LDY_ZP,   
        &Cpu::LDA_ZP,   &Cpu::LDX_ZP,   &Cpu::UNIMP,    &Cpu::TAY,      &Cpu::LDA_IMM,  
        &Cpu::TAX,      &Cpu::UNIMP,    &Cpu::LDY_ABS,  &Cpu::LDA_ABS,  &Cpu::LDX_ABS,  
        &Cpu::UNIMP,    &Cpu::BCS,      &Cpu::LDA_IND_Y,&Cpu::UNIMP,    &Cpu::UNIMP,    
        &Cpu::LDY_ZP_X, &Cpu::LDA_ZP_X, &Cpu::LDX_ZP_Y, &Cpu::UNIMP,    &Cpu::CLV,      
        &Cpu::LDA_ABS_Y,&Cpu::TSX,      &Cpu::UNIMP,    &Cpu::LDY_ABS_X,&Cpu::LDA_ABS_X,
        &Cpu::LDX_ABS_Y,&Cpu::CPY_IMM,  &Cpu::CMP_IND_X,&Cpu::UNIMP,    &Cpu::UNIMP,    
        &Cpu::UNIMP,    &Cpu::CPY_ZP,   &Cpu::CMP_ZP,   &Cpu::DEC_ZP,   &Cpu::UNIMP,    
        &Cpu::INY,      &Cpu::CMP_IMM,  &Cpu::DEX,      &Cpu::UNIMP,    &Cpu::CPY_ABS,  
        &Cpu::CMP_ABS,  &Cpu::DEC_ABS,  &Cpu::UNIMP,    &Cpu::BNE,      &Cpu::CMP_IND_Y,
        &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::CMP_ZP_X, &Cpu::DEC_ZP_X, 
        &Cpu::UNIMP,    &Cpu::CLD,      &Cpu::CMP_ABS_Y,&Cpu::UNIMP,    &Cpu::UNIMP,    
        &Cpu::UNIMP,    &Cpu::CMP_ABS_X,&Cpu::DEC_ABS_X,&Cpu::UNIMP,    &Cpu::CPX_IMM,  
        &Cpu::SBC_IND_X,&Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::CPX_ZP,   &Cpu::SBC_ZP,   
        &Cpu::INC_ZP,   &Cpu::UNIMP,    &Cpu::INX,      &Cpu::SBC_IMM,  &Cpu::NOP,      
        &Cpu::UNIMP,    &Cpu::CPX_ABS,  &Cpu::SBC_ABS,  &Cpu::INC_ABS,  &Cpu::UNIMP,    
        &Cpu::BEQ,      &Cpu::SBC_IND_Y,&Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::UNIMP,    
        &Cpu::SBC_ZP_X, &Cpu::INC_ZP_X, &Cpu::UNIMP,    &Cpu::SED,      &Cpu::SBC_ABS_Y,
        &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::UNIMP,    &Cpu::SBC_ABS_X,&Cpu::INC_ABS_X,
        &Cpu::UNIMP
    };

public:
	
    Cpu(const Rom* rom);
};

#endif // CPU_H
