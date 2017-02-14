#ifndef CPU_H
#define CPU_H

#include <circle/types.h>
#include "util.h"
#include "rom.h"

class Cpu {
    public:
    union NESCPUMemoryMap {
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

    struct NESCPURegisters {
        u16 PC;
        u8 SP = 0xFF;
        u8 A;
        u8 X;
        u8 Y;
        u8 P = 0b00100000;
    };

    enum NESCPUFlags {
        C,      //Carry
        Z,      //Zero
        I,      //Interrupt
        D,      //Decimal
        B,      //Break
        A,      //Always
        V,      //Overflow
        S       //Sign : 1 is negative
    };

    NESCPURegisters registers;
    NESCPUMemoryMap memory;

    const u8 instructionSizes[256] ={
        0, //BRK
        2, //ORA_IND_X
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        2, //ORA_ZP
        2, //ASL_ZP
        1, //UNIMP
        1, //PHP
        2, //ORA_IMM
        1, //ASL_ACC
        1, //UNIMP
        1, //UNIMP
        3, //ORA_ABS
        3, //ASL_ABS
        1, //UNIMP
        2, //BPL
        2, //ORA_IND_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        2, //ORA_ZP_X
        2, //ASL_ZP_X
        1, //UNIMP
        1, //CLC
        3, //ORA_ABS_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        3, //ORA_ABS_X
        3, //ASL_ABS_X
        1, //UNIMP
        0, //JSR
        2, //AND_IND_X
        1, //UNIMP
        1, //UNIMP
        2, //BIT_ZP
        2, //AND_ZP
        2, //ROL_ZP
        1, //UNIMP
        1, //PLP
        2, //AND_IMM
        1, //ROL_ACC
        1, //UNIMP
        3, //BIT_ABS
        3, //AND_ABS
        3, //ROL_ABS
        1, //UNIMP
        2, //BMI
        2, //AND_IND_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        2, //AND_ZP_X
        2, //ROL_ZP_X
        1, //UNIMP
        1, //SEC
        3, //AND_ABS_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        3, //AND_ABS_X
        3, //ROL_ABS_X
        1, //UNIMP
        1, //RTI
        2, //EOR_IND_X
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        2, //EOR_ZP
        2, //LSR_ZP
        1, //UNIMP
        1, //PHA
        2, //EOR_IMM
        1, //LSR_ACC
        1, //UNIMP
        0, //JMP_ABS
        3, //EOR_ABS
        3, //LSR_ABS
        1, //UNIMP
        2, //BVC
        2, //EOR_IND_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        2, //EOR_ZP_X
        2, //LSR_ZP_X
        1, //UNIMP
        1, //CLI
        3, //EOR_ABS_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        3, //EOR_ABS_X
        3, //LSR_ABS_X
        1, //UNIMP
        1, //RTS
        2, //ADC_IND_X
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        2, //ADC_ZP
        2, //ROR_ZP
        1, //UNIMP
        1, //PLA
        2, //ADC_IMM
        1, //ROR_ACC
        1, //UNIMP
        0, //JMP_IND
        3, //ADC_ABS
        3, //ROR_ABS
        1, //UNIMP
        2, //BVS
        2, //ADC_IND_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        2, //ADC_ZP_X
        2, //ROR_ZP_X
        1, //UNIMP
        1, //SEI
        3, //ADC_ABS_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        3, //ADC_ABS_X
        3, //ROR_ABS_X
        1, //UNIMP
        1, //UNIMP
        2, //STA_IND_X
        1, //UNIMP
        1, //UNIMP
        2, //STY_ZP
        2, //STA_ZP
        2, //STX_ZP
        1, //UNIMP
        1, //DEY
        1, //UNIMP
        1, //TXA
        1, //UNIMP
        3, //STY_ABS
        3, //STA_ABS
        3, //STX_ABS
        1, //UNIMP
        2, //BCC
        2, //STA_IND_Y
        1, //UNIMP
        1, //UNIMP
        2, //STY_ZP_X
        2, //STA_ZP_X
        2, //STX_ZP_Y
        1, //UNIMP
        1, //TYA
        3, //STA_ABS_Y
        1, //TXS
        1, //UNIMP
        1, //UNIMP
        3, //STA_ABS_X
        1, //UNIMP
        1, //UNIMP
        2, //LDY_IMM
        2, //LDA_IND_X
        2, //LDX_IMM
        1, //UNIMP
        2, //LDY_ZP
        2, //LDA_ZP
        2, //LDX_ZP
        1, //UNIMP
        1, //TAY
        2, //LDA_IMM
        1, //TAX
        1, //UNIMP
        3, //LDY_ABS
        3, //LDA_ABS
        3, //LDX_ABS
        1, //UNIMP
        2, //BCS
        2, //LDA_IND_Y
        1, //UNIMP
        1, //UNIMP
        2, //LDY_ZP_X
        2, //LDA_ZP_X
        2, //LDX_ZP_Y
        1, //UNIMP
        1, //CLV
        3, //LDA_ABS_Y
        1, //TSX
        1, //UNIMP
        3, //LDY_ABS_X
        3, //LDA_ABS_X
        3, //LDX_ABS_Y
        1, //CPY_IMM
        2, //CMP_IND_X
        2, //UNIMP
        1, //UNIMP
        1, //UNIMP
        2, //CPY_ZP
        2, //CMP_ZP
        2, //DEC_ZP
        1, //UNIMP
        1, //INY
        2, //CMP_IMM
        1, //DEX
        1, //UNIMP
        3, //CPY_ABS
        3, //CMP_ABS
        3, //DEC_ABS
        1, //UNIMP
        2, //BNE
        2, //CMP_IND_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        2, //CMP_ZP_X
        2, //DEC_ZP_X
        1, //UNIMP
        1, //CLD
        3, //CMP_ABS_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        3, //CMP_ABS_X
        3, //DEC_ABS_X
        1, //UNIMP
        2, //CPX_IMM
        2, //SBC_IND_X
        1, //UNIMP
        1, //UNIMP
        2, //CPX_ZP
        2, //SBC_ZP
        2, //INC_ZP
        1, //UNIMP
        1, //INX
        2, //SBC_IMM
        1, //NOP
        1, //UNIMP
        3, //CPX_ABS
        3, //SBC_ABS
        3, //INC_ABS
        1, //UNIMP
        2, //BEQ
        2, //SBC_IND_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        2, //SBC_ZP_X
        2, //INC_ZP_X
        1, //UNIMP
        1, //SED
        3, //SBC_ABS_Y
        1, //UNIMP
        1, //UNIMP
        1, //UNIMP
        3, //SBC_ABS_X
        3, //INC_ABS_X
        1  //UNIMP
    };
    
    void ADC(u8& value);
    void AND(u8& value);
    void ASL(u8& value);
    void BIT(u8& value);
    void CMP(u8& value);
    void CPX(u8& value);
    void CPY(u8& value);
    void DEC(u8& value);
    void EOR(u8& value);
    void INC(u8& value);
    void JMP(u8& value);
    void LDA(u8& value);
    void LDX(u8& value);
    void LDY(u8& value);
    void LSR(u8& value);
    void ORA(u8& value);
    void ROL(u8& value);
    void ROR(u8& value);
    void SBC(u8& value);
    void STA(u8& value);
    void STX(u8& value);
    void STY(u8& value);

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

    typedef u8 (Cpu::*func)(void);

    const func instructions[256]{
        &Cpu::BRK, &Cpu::ORA_IND_X, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::ORA_ZP, &Cpu::ASL_ZP, &Cpu::UNIMP, &Cpu::PHP, &Cpu::ORA_IMM,
        &Cpu::ASL_ACC, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::ORA_ABS, &Cpu::ASL_ABS,
        &Cpu::UNIMP, &Cpu::BPL, &Cpu::ORA_IND_Y, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::UNIMP, &Cpu::ORA_ZP_X, &Cpu::ASL_ZP_X, &Cpu::UNIMP, &Cpu::CLC,
        &Cpu::ORA_ABS_Y, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::ORA_ABS_X,
        &Cpu::ASL_ABS_X, &Cpu::UNIMP, &Cpu::JSR, &Cpu::AND_IND_X, &Cpu::UNIMP,
        &Cpu::UNIMP, &Cpu::BIT_ZP, &Cpu::AND_ZP, &Cpu::ROL_ZP, &Cpu::UNIMP,
        &Cpu::PLP, &Cpu::AND_IMM, &Cpu::ROL_ACC, &Cpu::UNIMP, &Cpu::BIT_ABS,
        &Cpu::AND_ABS, &Cpu::ROL_ABS, &Cpu::UNIMP, &Cpu::BMI, &Cpu::AND_IND_Y,
        &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::AND_ZP_X, &Cpu::ROL_ZP_X,
        &Cpu::UNIMP, &Cpu::SEC, &Cpu::AND_ABS_Y, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::UNIMP, &Cpu::AND_ABS_X, &Cpu::ROL_ABS_X, &Cpu::UNIMP, &Cpu::RTI,
        &Cpu::EOR_IND_X, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::EOR_ZP,
        &Cpu::LSR_ZP, &Cpu::UNIMP, &Cpu::PHA, &Cpu::EOR_IMM, &Cpu::LSR_ACC,
        &Cpu::UNIMP, &Cpu::JMP_ABS, &Cpu::EOR_ABS, &Cpu::LSR_ABS, &Cpu::UNIMP,
        &Cpu::BVC, &Cpu::EOR_IND_Y, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::EOR_ZP_X, &Cpu::LSR_ZP_X, &Cpu::UNIMP, &Cpu::CLI, &Cpu::EOR_ABS_Y,
        &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::EOR_ABS_X, &Cpu::LSR_ABS_X,
        &Cpu::UNIMP, &Cpu::RTS, &Cpu::ADC_IND_X, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::UNIMP, &Cpu::ADC_ZP, &Cpu::ROR_ZP, &Cpu::UNIMP, &Cpu::PLA,
        &Cpu::ADC_IMM, &Cpu::ROR_ACC, &Cpu::UNIMP, &Cpu::JMP_IND, &Cpu::ADC_ABS,
        &Cpu::ROR_ABS, &Cpu::UNIMP, &Cpu::BVS, &Cpu::ADC_IND_Y, &Cpu::UNIMP,
        &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::ADC_ZP_X, &Cpu::ROR_ZP_X, &Cpu::UNIMP,
        &Cpu::SEI, &Cpu::ADC_ABS_Y, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::ADC_ABS_X, &Cpu::ROR_ABS_X, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::STA_IND_X,
        &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::STY_ZP, &Cpu::STA_ZP, &Cpu::STX_ZP,
        &Cpu::UNIMP, &Cpu::DEY, &Cpu::UNIMP, &Cpu::TXA, &Cpu::UNIMP,
        &Cpu::STY_ABS, &Cpu::STA_ABS, &Cpu::STX_ABS, &Cpu::UNIMP, &Cpu::BCC,
        &Cpu::STA_IND_Y, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::STY_ZP_X, &Cpu::STA_ZP_X,
        &Cpu::STX_ZP_Y, &Cpu::UNIMP, &Cpu::TYA, &Cpu::STA_ABS_Y, &Cpu::TXS,
        &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::STA_ABS_X, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::LDY_IMM, &Cpu::LDA_IND_X, &Cpu::LDX_IMM, &Cpu::UNIMP, &Cpu::LDY_ZP,
        &Cpu::LDA_ZP, &Cpu::LDX_ZP, &Cpu::UNIMP, &Cpu::TAY, &Cpu::LDA_IMM,
        &Cpu::TAX, &Cpu::UNIMP, &Cpu::LDY_ABS, &Cpu::LDA_ABS, &Cpu::LDX_ABS,
        &Cpu::UNIMP, &Cpu::BCS, &Cpu::LDA_IND_Y, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::LDY_ZP_X, &Cpu::LDA_ZP_X, &Cpu::LDX_ZP_Y, &Cpu::UNIMP, &Cpu::CLV,
        &Cpu::LDA_ABS_Y, &Cpu::TSX, &Cpu::UNIMP, &Cpu::LDY_ABS_X, &Cpu::LDA_ABS_X,
        &Cpu::LDX_ABS_Y, &Cpu::CPY_IMM, &Cpu::CMP_IND_X, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::UNIMP, &Cpu::CPY_ZP, &Cpu::CMP_ZP, &Cpu::DEC_ZP, &Cpu::UNIMP,
        &Cpu::INY, &Cpu::CMP_IMM, &Cpu::DEX, &Cpu::UNIMP, &Cpu::CPY_ABS,
        &Cpu::CMP_ABS, &Cpu::DEC_ABS, &Cpu::UNIMP, &Cpu::BNE, &Cpu::CMP_IND_Y,
        &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::CMP_ZP_X, &Cpu::DEC_ZP_X,
        &Cpu::UNIMP, &Cpu::CLD, &Cpu::CMP_ABS_Y, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::UNIMP, &Cpu::CMP_ABS_X, &Cpu::DEC_ABS_X, &Cpu::UNIMP, &Cpu::CPX_IMM,
        &Cpu::SBC_IND_X, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::CPX_ZP, &Cpu::SBC_ZP,
        &Cpu::INC_ZP, &Cpu::UNIMP, &Cpu::INX, &Cpu::SBC_IMM, &Cpu::NOP,
        &Cpu::UNIMP, &Cpu::CPX_ABS, &Cpu::SBC_ABS, &Cpu::INC_ABS, &Cpu::UNIMP,
        &Cpu::BEQ, &Cpu::SBC_IND_Y, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::UNIMP,
        &Cpu::SBC_ZP_X, &Cpu::INC_ZP_X, &Cpu::UNIMP, &Cpu::SED, &Cpu::SBC_ABS_Y,
        &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::UNIMP, &Cpu::SBC_ABS_X, &Cpu::INC_ABS_X,
        &Cpu::UNIMP
    };
    
    /**
     * Executes the next instruction at memory[PC]
     * Increments the PC accordingly
     */
    void Execute();
    
    /**
     * Fetch the opcode at memory[PC]
     */
    inline u8& OpCode();
    
    /**
     * Fetch the operand at memory[PC + number]
     */
    inline u8& Operand(int number);
    
    /**
     * Fetch the byte at memory[address]
     */
    inline u8& Memory(const u16 address);
    
    /**
     * Store the byte at stack[SP]
     * and decrement the stack pointer
     */
    inline void PushOnStack(u8 value);

    /**
     * Fetch the byte at stack[SP]
     * and increment the stack pointer
     */
    inline u8 PopFromStack();
    
    /**
     * Build an address using only the least significative byte. The first half of 
     * the address is assumed to be zero.
     * 
     * @param low the least significant byte of the address
     * @return the two bytes address
     */
    u16 FromValues(const u8 low) const;

    /**
     * Build an address using the most and the least significative bytes. Useful 
     * because the memory is stored as little endian.
     * 
     * @param low  the least significant byte of the address
     * @param high the most significant byte of the address
     * @return the two bytes address
     */
    u16 FromValues(const u8 low, const u8 high) const;

    /**
     * Build the address [00][low]. 
     * @param low the least significant byte of the address
     * @return the builded address
     */
    u16 ZeroPage(const u8 low) const;

    /**
     * Build the address [high][low].
     * @param low  the least significant byte of the address
     * @param high the most significant byte of the address
     * @return the builded address
     */
    u16 Absolute(const u8 low, const u8 high) const;

    //u16 Implied();
    //u16 Accumulator();

    /**
     * Compute the complete address [high][low] + index
     * @param low   the least significant byte of the address
     * @param high  the most significant byte of the address
     * @param index a value to add to the constructed address
     * @return the computed address
     */
    u16 Indexed(const u8 low, const u8 high, const u8 reg) const;

    /**
     * Compute the ZeroPage address [00][low] + index.
     * @param low   the least significant byte of the address
     * @param index a value to add to the constructed address
     * @return the computed address
     */
    u16 ZeroPageIndexed(const u8 low, const u8 reg) const;

    /**
     * Fetch an address at memory [high][low].
     * @param  low  the least significant byte 
     * @param  high the most significant byte
     * @return the stored address
     */
    u16 Indirect(const u8 low, const u8 high);

    /**
     * Fetch an address at memory [00][low + index]. If [low + index] overflow, only
     * the lowest byte is kept.
     * 
     * @param  low   ZeroPage address of the memory containing the address
     * @param  index a value to add to low to get the actual address
     * @return the pre-indexed address
     */
    u16 PreIndexedIndirect(const u8 low, const u8 reg);

    /**
     * Fetch an address at memory [00][low] then increase the address by index.
     * @param  low   ZeroPage address of the memory containing the address 
     * @param  index an index to add to the loaded address. Usually a register [X|Y]
     * @return the post-indexed address
     */
    u16 PostIndexedIndirect(const u8 low, const u8 reg);
    //u16 Relative();
    
    /**
     * Detects if two addresses are on the same page 
     * @param startAddress
     * @param endAddress
     * @return if a page is crossed
     */
    boolean IsPageCrossed(u16 startAddress, u16 endAddress);

    void SetFlag(u8 flag, u8 value);
    u8 GetFlag(u8 flag);
    
    Cpu();
    Cpu(const Rom* rom);
};

#endif // CPU_H
