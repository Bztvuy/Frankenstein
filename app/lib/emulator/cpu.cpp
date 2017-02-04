#include "cpu.h"
#include <circle/util.h>

Cpu::Cpu(const Rom* rom) {
    const iNesHeader* header = rom->GetHeader();
    int prgRomBanks = header->prgRomBanks;
    int trainerOffset = rom->GetTrainerOffset();
    int prgRomBanksLocation = rom->headerSize + trainerOffset;

    switch (prgRomBanks) {
        case 2:
            memcpy(memory->prgRomUpperBank, rom->GetRaw() + prgRomBanksLocation + prgRomBankSize, prgRomBankSize);
        case 1:
            memcpy(memory->prgRomLowerBank, rom->GetRaw() + prgRomBanksLocation, prgRomBankSize);
            break;
        default: //TODO: implement multiple PRG-ROM banks
            break;
    }
}

inline u8& Cpu::Memory(const u16 address) const {
    return this->memory->raw[address];
}

inline u8& Cpu::Operand(int number) const {
    return Memory(this->registers->PC + number);
}

void Cpu::ADC(u8& value){
    
}

void Cpu::AND(u8& value){
    this->registers->A &= value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::ASL(u8& value){
    // 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry. 
    SetFlag(C, CHECK_BIT(value, 7));
    value <<= 1;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::BIT(u8& value){
    SetFlag(Z, value & this->registers->A);
    SetFlag(S, CHECK_BIT(value, 7));
    SetFlag(V, CHECK_BIT(value, 6));
}

void Cpu::CMP(u8& value){
    SetFlag(C, this->registers->A >= value);
    SetFlag(Z, this->registers->A == value);
    SetFlag(S, this->registers->A < value);
}

void Cpu::CPX(u8& value){
    SetFlag(C, this->registers->X >= value);
    SetFlag(Z, this->registers->X == value);
    SetFlag(S, this->registers->X < value);
}

void Cpu::CPY(u8& value){
    SetFlag(C, this->registers->Y >= value);
    SetFlag(Z, this->registers->Y == value);
    SetFlag(S, this->registers->Y < value);
}

void Cpu::DEC(u8& value){
    
}

void Cpu::EOR(u8& value){
    this->registers->A ^= value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::INC(u8& value){
    
}

void Cpu::JMP(u8& value){
    // Complex stuff with page boundary?
}

void Cpu::LDA(u8& value){
    this->registers->A = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::LDX(u8& value){
    this->registers->X = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::LDY(u8& value){
    this->registers->Y = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::LSR(u8& value){
    // 0 is shifted into bit 7 and the original bit 0 is shifted into the Carry. 
    SetFlag(C, CHECK_BIT(value, 0));
    value >>= 1;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::ORA(u8& value){
    this->registers->A |= value;
    SetFlag(Z, this->registers->A);
    SetFlag(S, CHECK_BIT(this->registers->A, 7));
}

void Cpu::ROL(u8& value){
    auto carry = GetFlag(C);
    SetFlag(C, CHECK_BIT(value, 7));
    value <<= 1;
    ASSIGN_BIT(value, 0, carry);
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::ROR(u8& value){
    auto carry = GetFlag(C);
    SetFlag(C, CHECK_BIT(value, 0));
    value >>= 1;
    ASSIGN_BIT(value, 7, carry);
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::SBC(u8& value){
    
}

void Cpu::STA(u8& value){
    value = this->registers->A;
}

void Cpu::STX(u8& value){
    value = this->registers->X;
}

void Cpu::STY(u8& value){
    value = this->registers->Y;
}

void Cpu::BRK() {
    // TODO
}

/// Logical OR 
/// 2 bytes; 6 cycles
void Cpu::ORA_IND_X() {
    ORA(Memory(PreIndexedIndirect(Operand(1), this->registers->X)));
}

/// 2 bytes; 3 cycles
void Cpu::ORA_ZP() {
    ORA(Memory(ZeroPage(Operand(1))));
}

// 
void Cpu::ASL_ZP() {
    ASL(Memory(ZeroPage(Operand(1))));
}

void Cpu::PHP() {
    // TODO
}

void Cpu::ORA_IMM() {
    ORA(Operand(1));
}

void Cpu::ASL_ACC() {
    ASL(this->registers->A);
}

void Cpu::ORA_ABS() {
    ORA(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::ASL_ABS() {
    ASL(Memory(Absolute(Operand(1), Operand(2))));    
}

void Cpu::BPL() {
    // TODO
}

void Cpu::ORA_IND_Y() {
    ORA(Memory(PostIndexedIndirect(Operand(1), this->registers->Y)));
}

void Cpu::ORA_ZP_X() {
    ORA(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::ASL_ZP_X() {
    ASL(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::CLC() {
    // TODO
}

void Cpu::ORA_ABS_Y() {
    ORA(Memory(Indexed(Operand(1), Operand(2), this->registers->Y)));
}

void Cpu::ORA_ABS_X() {
    ORA(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::ASL_ABS_X() {
    ASL(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::JSR() {
    // TODO
}

// 6 cycles
void Cpu::AND_IND_X() {
    AND(Memory(PreIndexedIndirect(Operand(1), this->registers->X)));
}

void Cpu::BIT_ZP() {
    BIT(Memory(ZeroPage(Operand(1))));
}

void Cpu::AND_ZP() {
    AND(Memory(ZeroPage(Operand(1))));
}

void Cpu::ROL_ZP() {
    ROL(Memory(ZeroPage(Operand(1))));
}

void Cpu::PLP() {
    // TODO
}

void Cpu::AND_IMM() {
    AND(Operand(1));
}

void Cpu::ROL_ACC() {
    ROL(this->registers->A);
}

void Cpu::BIT_ABS() {
    BIT(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::AND_ABS() {
    AND(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::ROL_ABS() {
    ROL(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::BMI() {
    // TODO
}

void Cpu::AND_IND_Y() {
    AND(Memory(PostIndexedIndirect(Operand(1), this->registers->Y)));
}

void Cpu::AND_ZP_X() {
    AND(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::ROL_ZP_X() {
    ROL(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::SEC() {
    // TODO
}

void Cpu::AND_ABS_Y() {
    AND(Memory(Indexed(Operand(1), Operand(2), this->registers->Y)));
}

void Cpu::AND_ABS_X() {
    AND(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::ROL_ABS_X() {
    ROL(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::RTI() {
    // TODO
}

void Cpu::EOR_IND_X() {
    EOR(Memory(PreIndexedIndirect(Operand(1), this->registers->X)));
}

void Cpu::EOR_ZP() {
    EOR(Memory(ZeroPage(Operand(1))));
}

void Cpu::LSR_ZP() {
    LSR(Memory(ZeroPage(Operand(1))));
}

void Cpu::PHA() {
    // TODO
}

void Cpu::EOR_IMM() {
    EOR(Operand(1));
}

void Cpu::LSR_ACC() {
    LSR(this->registers->A);
}

void Cpu::JMP_ABS() {
    JMP(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::EOR_ABS() {
    EOR(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::LSR_ABS() {
    LSR(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::BVC() {
    // TODO
}

void Cpu::EOR_IND_Y() {
    EOR(Memory(PostIndexedIndirect(Operand(1), this->registers->Y)));
}

void Cpu::EOR_ZP_X() {
    EOR(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::LSR_ZP_X() {
    LSR(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::CLI() {
    // TODO
}

void Cpu::EOR_ABS_Y() {
    EOR(Memory(Indexed(Operand(1), Operand(2), this->registers->Y)));
}

void Cpu::EOR_ABS_X() {
    EOR(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::LSR_ABS_X() {
    LSR(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::RTS() {
    // TODO
}

void Cpu::ADC_IND_X() {
    ADC(Memory(PreIndexedIndirect(Operand(1), this->registers->X)));
}

void Cpu::ADC_ZP() {
    ADC(Memory(ZeroPage(Operand(1))));
}

void Cpu::ROR_ZP() {
    ROR(Memory(ZeroPage(Operand(1))));
}

void Cpu::PLA() {
    // TODO
}

void Cpu::ADC_IMM() {
    ADC(Operand(1));
}

void Cpu::ROR_ACC() {
    ROR(this->registers->A);
}

void Cpu::JMP_IND() {
    JMP(Memory(Indirect(Operand(1), Operand(2))));
}

void Cpu::ADC_ABS() {
    ADC(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::ROR_ABS() {
    ROR(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::BVS() {
    // TODO
}

void Cpu::ADC_IND_Y() {
    ADC(Memory(PostIndexedIndirect(Operand(1), this->registers->Y)));
}

void Cpu::ADC_ZP_X() {
    ADC(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::ROR_ZP_X() {
    ROR(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::SEI() {
    // TODO
}

void Cpu::ADC_ABS_Y() {
    ADC(Memory(Indexed(Operand(1), Operand(2), this->registers->Y)));
}

void Cpu::ADC_ABS_X() {
    ADC(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::ROR_ABS_X() {
    ROR(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::STA_IND_X() {
    STA(Memory(PreIndexedIndirect(Operand(1), this->registers->X)));
}

void Cpu::STY_ZP() {
    STY(Memory(ZeroPage(Operand(1))));
}

void Cpu::STA_ZP() {
    STA(Memory(ZeroPage(Operand(1))));
}

void Cpu::STX_ZP() {
    STX(Memory(ZeroPage(Operand(1))));
}

void Cpu::DEY() {
    // TODO
}

void Cpu::TXA() {
    // TODO
}

void Cpu::STY_ABS() {
    STY(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::STA_ABS() {
    STA(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::STX_ABS() {
    STX(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::BCC() {
    // TODO
}

void Cpu::STA_IND_Y() {
    STA(Memory(PostIndexedIndirect(Operand(1), this->registers->Y)));
}

void Cpu::STY_ZP_X() {
    STY(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::STA_ZP_X() {
    STA(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::STX_ZP_Y() {
    STX(Memory(ZeroPageIndexed(Operand(1), this->registers->Y)));
}

void Cpu::TYA() {
    // TODO
}

void Cpu::STA_ABS_Y() {
    STA(Memory(Indexed(Operand(1), Operand(2), this->registers->Y)));
}

void Cpu::TXS() {
    // TODO
}

void Cpu::STA_ABS_X() {
    STA(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::LDY_IMM() {
    LDY(Operand(1));
}

void Cpu::LDA_IND_X() {
    LDA(Memory(PreIndexedIndirect(Operand(1), this->registers->X)));
}

void Cpu::LDX_IMM() {
    LDX(Operand(1));
}

void Cpu::LDY_ZP() {
    LDY(Memory(ZeroPage(Operand(1))));
}

void Cpu::LDA_ZP() {
    LDA(Memory(ZeroPage(Operand(1))));
}

void Cpu::LDX_ZP() {
    LDX(Memory(ZeroPage(Operand(1))));
}

void Cpu::TAY() {
    // TODO
}

void Cpu::LDA_IMM() {
    LDA(Operand(1));
}

void Cpu::TAX() {
    // TODO
}

void Cpu::LDY_ABS() {
    LDY(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::LDA_ABS() {
    LDA(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::LDX_ABS() {
    LDX(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::BCS() {
    // TODO
}

void Cpu::LDA_IND_Y() {
    LDA(Memory(PostIndexedIndirect(Operand(1), this->registers->Y)));
}

void Cpu::LDY_ZP_X() {
    LDY(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::LDA_ZP_X() {
    LDA(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::LDX_ZP_Y() {
    LDX(Memory(ZeroPageIndexed(Operand(1), this->registers->Y)));
}

void Cpu::CLV() {
    // TODO
}

void Cpu::LDA_ABS_Y() {
    LDA(Memory(Indexed(Operand(1), Operand(2), this->registers->Y)));
}

void Cpu::TSX() {
    // TODO
}

void Cpu::LDY_ABS_X() {
    LDY(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::LDA_ABS_X() {
    LDA(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::LDX_ABS_Y() {
    LDX(Memory(Indexed(Operand(1), Operand(2), this->registers->Y)));
}

void Cpu::CPY_IMM() {
    CPY(Operand(1));
}

void Cpu::CMP_IND_X() {
    CMP(Memory(PreIndexedIndirect(Operand(1), this->registers->X)));
}

void Cpu::CPY_ZP() {
    CPY(Memory(ZeroPage(Operand(1))));
}

void Cpu::CMP_ZP() {
    CMP(Memory(ZeroPage(Operand(1))));
}

void Cpu::DEC_ZP() {
    DEC(Memory(ZeroPage(Operand(1))));
}

void Cpu::INY() {
    // TODO
}

void Cpu::CMP_IMM() {
    CMP(Operand(1));
}

void Cpu::DEX() {
    // TODO
}

void Cpu::CPY_ABS() {
    CPY(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::CMP_ABS() {
    CMP(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::DEC_ABS() {
    DEC(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::BNE() {
    // TODO
}

void Cpu::CMP_IND_Y() {
    CMP(Memory(PostIndexedIndirect(Operand(1), this->registers->Y)));
}

void Cpu::CMP_ZP_X() {
    CMP(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::DEC_ZP_X() {
    DEC(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::CLD() {
    // TODO
}

void Cpu::CMP_ABS_Y() {
    CMP(Memory(Indexed(Operand(1), Operand(2), this->registers->Y)));
}

void Cpu::CMP_ABS_X() {
    CMP(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::DEC_ABS_X() {
    DEC(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::CPX_IMM() {
    CPX(Operand(1));
}

void Cpu::SBC_IND_X() {
    SBC(Memory(PreIndexedIndirect(Operand(1), this->registers->X)));
}

void Cpu::CPX_ZP() {
    CPX(Memory(ZeroPage(Operand(1))));
}

void Cpu::SBC_ZP() {
    SBC(Memory(ZeroPage(Operand(1))));
}

void Cpu::INC_ZP() {
    INC(Memory(ZeroPage(Operand(1))));
}

void Cpu::INX() {
    // TODO
}

void Cpu::SBC_IMM() {
    SBC(Operand(1));
}

void Cpu::NOP() {
    // TODO
}

void Cpu::CPX_ABS() {
    CPX(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::SBC_ABS() {
    SBC(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::INC_ABS() {
    INC(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::BEQ() {
    // TODO
}

void Cpu::SBC_IND_Y() {
    SBC(Memory(PostIndexedIndirect(Operand(1), this->registers->Y)));
}

void Cpu::SBC_ZP_X() {
    SBC(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::INC_ZP_X() {
    INC(Memory(ZeroPageIndexed(Operand(1), this->registers->X)));
}

void Cpu::SED() {
    // TODO
}

void Cpu::SBC_ABS_Y() {
    SBC(Memory(Indexed(Operand(1), Operand(2), this->registers->Y)));
}

void Cpu::SBC_ABS_X() {
    SBC(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::INC_ABS_X() {
    INC(Memory(Indexed(Operand(1), Operand(2), this->registers->X)));
}

void Cpu::UNIMP() {
}

void Cpu::SetFlag(u8 flag, u8 value){
    ASSIGN_BIT(this->registers->P, flag, value);
}

u8 Cpu::GetFlag(u8 flag){
    return CHECK_BIT(this->registers->P, flag);
}