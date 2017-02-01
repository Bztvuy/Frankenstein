#ifndef CPU_H
#define CPU_H

#include "rom.h"
#include <circle/types.h>

union NESCPUMemoryMap{
    struct {
        u8 zeroPage[0x0100];
        u8 stack[0x0100];
        u8 ram[0x0600];
        u8 mirrors1[0x1800];
        u8 ioRegisters1[0x0008];
        u8 mirrors2[0x1FF8];
        u8 ioRegisters2[0x0020];
        u8 expansionRom[0x1FE0];
        u8 sram[0x2000];
        u8 prgRomLowerBank[0x4000];
        u8 prgRomUpperBank[0x4000];
    };
    u8 raw[0x10000];
};

union NESCPURegisters{
    struct {
        u16 programCounter;
        u8 stackPointer;
        u8 accumulator;
        u8 indexX;
        u8 indexY;
        u8 processorStatus;
    };
    struct {
        u16 PC;
        u8 SP;
        u8 A;
        u8 X;
        u8 Y;
        u8 P;
    };
};

class Cpu
{
private:
    NESCPURegisters* registers;
    NESCPUMemoryMap* memory;

    //sizes related to hardware (in bytes) :
    u32 prgRomBankSize = 16 * KILOBYTE;
    u32 vRomBankSize = 8 * KILOBYTE;
    u32 prgRamBankSize = 8 * KILOBYTE;
    u64 instructions[256];
    u8 instructionSizes[256] =
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

    
    u16 Immediate();
    u16 ZeroPage();
    u16 Absolute();
    u16 Implied();
    u16 Accumulator();
    u16 Indexed();
    u16 ZeroPageIndexed();
    u16 Indirect();
    u16 PreIndexedIndirect();
    u16 PostIndexedIndirect();
    u16 Relative();
   
    u16 FromValues(u8 low);
    u16 FromValues(u8 low, u8 high);
public:
	
    Cpu(const Rom* rom);
};

#endif // CPU_H
