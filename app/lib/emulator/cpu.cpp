#include "cpu.h"
#include <circle/util.h>

Cpu::Cpu(const Rom* rom) {
    const iNesHeader* header = rom->GetHeader();
    int prgRomBanks = header->prgRomBanks;
    int trainerOffset = rom->GetTrainerOffset();
    int prgRomBanksLocation = rom->headerSize + trainerOffset;

    switch (prgRomBanks) {
        case 2:
            memcpy(this->memory.prgRomLowerBank, rom->GetRaw() + prgRomBanksLocation + prgRomBankSize, prgRomBankSize);
        case 1:
            memcpy(this->memory.prgRomLowerBank, rom->GetRaw() + prgRomBanksLocation, prgRomBankSize);
            break;
        default: //TODO: implement multiple PRG-ROM banks
            break;
    }
}

void Cpu::SetFlag(u8 flag, u8 value){
    ASSIGN_BIT(this->registers.P, flag, value);
}

u8 Cpu::GetFlag(u8 flag){
    return CHECK_BIT(this->registers.P, flag);
}

inline u8& Cpu::Memory(const u16 address) {
    return this->memory.raw[address];
}

inline void Cpu::PushOnStack(u8& value){
    this->memory->stack[this->registers->SP] = value;
    this->registers->SP -= 1;
}

inline u8& Cpu::PopFromStack(){
    this->registers->SP += 1;
    return this->memory->stack[this->registers->SP];
}

inline u8& Cpu::Operand(int number) {
    return Memory(this->registers.PC + number);
}

////////////////////////////////////////////////////////////////////////////////
/// Addressing
////////////////////////////////////////////////////////////////////////////////

u16 Cpu::FromValues(const u8 low) const {
    u16 res = 0 | low;
    return res;
}

u16 Cpu::FromValues(const u8 low, const u8 high) const {
    u16 res = high;
    res <<= 8;
    res |= low;
    return res;
}

u16 Cpu::ZeroPage(const u8 low) const {
    return FromValues(low);
}

u16 Cpu::Absolute(const u8 low, const u8 high) const {
    return FromValues(low, high);
}

u16 Cpu::Indexed(const u8 low, const u8 high, const u8 index) const {
    return FromValues(low, high) + index;
}

u16 Cpu::ZeroPageIndexed(const u8 low, const u8 index) const {
    return FromValues(low) + index;
}

u16 Cpu::Indirect(const u8 low, const u8 high) {
    auto memAddrAddr = FromValues(low, high);
    auto valLow = Memory(memAddrAddr);
    auto valHigh = Memory(memAddrAddr+1);
    return FromValues(valLow, valHigh);
}

u16 Cpu::PreIndexedIndirect(const u8 low, const u8 index) {
    return Indirect((low + index) % (0xFF+1), 0);
}

u16 Cpu::PostIndexedIndirect(const u8 low, const u8 index) {
    return Indirect(low, 0) + index;
}

////////////////////////////////////////////////////////////////////////////////
/// Binary Operations Definition 
////////////////////////////////////////////////////////////////////////////////

void Cpu::AND(u8& value){
    // CONFIRM THIS PLEASE
    this->registers.A &= value;
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
    SetFlag(Z, value & this->registers.A);
    SetFlag(S, CHECK_BIT(value, 7));
    SetFlag(V, CHECK_BIT(value, 6));
}

void Cpu::EOR(u8& value){
    this->registers.A ^= value;
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
    this->registers.A |= value;
    SetFlag(Z, this->registers.A);
    SetFlag(S, CHECK_BIT(this->registers.A, 7));
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

////////////////////////////////////////////////////////////////////////////////
/// Arithmetic Operations Definition 
////////////////////////////////////////////////////////////////////////////////

void Cpu::ADC(u8& value){
    u16 result = value + this->registers->A + GetFlag(C);
    SetFlag(Z, result & 0xFF);
    SetFlag(S, CHECK_BIT(value, 7));
    SetFlag(V, !((this->registers->A ^ value) & 0x80) && ((this->registers->A ^ result) & 0x80));
    SetFlag(C, result > 0xFF);
    this->registers->A = (u8) result;
}

void Cpu::DEC(u8& value){
    value -= 1;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::INC(u8& value){
    value += 1;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::SBC(u8& value){
    u16 result = this->registers->A - value - GetFlag(C);
    SetFlag(Z, result & 0xFF);
    SetFlag(S, CHECK_BIT(value, 7));
    SetFlag(V, !((this->registers->A ^ value) & 0x80) && ((this->registers->A ^ result) & 0x80));
    SetFlag(C, result < 0x100);
    this->registers->A = (result & 0xFF);  
}

////////////////////////////////////////////////////////////////////////////////
/// Memory Operations Definition 
////////////////////////////////////////////////////////////////////////////////

void Cpu::LDA(u8& value){
    this->registers.A = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::LDX(u8& value){
    this->registers.X = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::LDY(u8& value){
    this->registers.Y = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::STA(u8& value){
    value = this->registers.A;
}

void Cpu::STX(u8& value){
    value = this->registers.X;
}

void Cpu::STY(u8& value){
    value = this->registers.Y;
}

////////////////////////////////////////////////////////////////////////////////
/// Branch Operations
////////////////////////////////////////////////////////////////////////////////

void Cpu::BPL() {
    // TODO
}

void Cpu::BMI() {
    // TODO
}

void Cpu::BVC() {
    // TODO
}

void Cpu::BVS() {
    // TODO
}

void Cpu::BCC() {
    // TODO
}

void Cpu::BCS() {
    // TODO
}

void Cpu::BNE() {
    // TODO
}

void Cpu::BEQ() {
    // TODO
}

////////////////////////////////////////////////////////////////////////////////
/// Register Operations Definition 
/// T[1][2] -> Transfert register 1 to register 2
/// DE[1] -> DEC register 1
/// IN[1] -> INC register 1
////////////////////////////////////////////////////////////////////////////////

void Cpu::TAX() {
    auto& value = this->registers->A
    this->registers.X = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::TXA() {
    auto& value = this->registers->X
    this->registers.A = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::DEX() {
    auto& value = this->registers->X
    value -= 1 ;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::INX() {
    auto& value = this->registers->X
    value += 1 ;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::TAY() {
    auto& value = this->registers->A
    this->registers.Y = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::TYA() {
    auto& value = this->registers->Y
    this->registers.A = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::DEY() {
    auto& value = this->registers->Y
    value -= 1 ;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

void Cpu::INY() {
    auto& value = this->registers->Y
    value += 1 ;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

////////////////////////////////////////////////////////////////////////////////
/// Stack Operations Definition 
////////////////////////////////////////////////////////////////////////////////

// Transfert X to Stack ptr
void Cpu::TXS() {
    PushOnStack(this->registers->X);
}

void Cpu::TSX() {
    auto& value = PopFromStack();
    this->registers->X = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

// Push A
void Cpu::PHA() {
    PushOnStack(this->registers->A);
}

// Pop A
void Cpu::PLA() {
    auto& value = PopFromStack();
    this->registers->A = value;
    SetFlag(Z, value);
    SetFlag(S, CHECK_BIT(value, 7));
}

// Push Processor Status
void Cpu::PHP() {
    PushOnStack(this->registers->P);
}

// Pull Processor Status
void Cpu::PLP() {
    this->registers->P = PopFromStack();
}

////////////////////////////////////////////////////////////////////////////////
/// PC Operations Definition 
////////////////////////////////////////////////////////////////////////////////

void Cpu::BRK(){
    this->registers->PC += 1;
    PushOnStack((this->registers->PC >> 8) & 0xFF);     /* Push return address onto the stack. */
    PushOnStack(this->registers->PC & 0xFF)
    SetFlag(B, 1);                                      /* Set BFlag before pushing */
    PushOnStack(this->registers->P);
    SetFlag(I, 1);
    this->registers->PC = (Memory(0xFFFE) | Memory(0xFFFF) << 8);
}

void Cpu::JMP(u8& value){
    // Complex stuff with page boundary?
    this->registers->PC = value;
}

void Cpu::JSR() {
    this->registers->PC -= 1;
    PushOnStack((this->registers->PC >> 8) & 0xFF);	/* Push return address onto the stack. */
    PushOnStack(this->registers->PC & 0xFF);
    this->registers->PC = Operand(1);
}

void Cpu::RTI() {
    auto processorStatus = PopFromStack();
    this->registers->P = processorStatus;
    auto returnAddress = PopFromStack();
    returnAddress |= (PopFromStack() << 8);	/* Load return address from stack. */
    this->registers->PC = returnAddress;
}

void Cpu::RTS() {
    auto returnAddress = PopFromStack();
    returnAddress += ((PopFromStack) << 8) + 1;	/* Load return address from stack and add 1. */
    this->registers->PC = returnAddress;
}

///////////////////////////////////////////////////////////////////////////////
/// Flag Instructions
///////////////////////////////////////////////////////////////////////////////

void Cpu::CMP(u8& value){
    u16 result = this->registers.A - value;
    SetFlag(C, result < 0x100);
    SetFlag(S, CHECK_BIT(result, 7));
    SetFlag(Z, result &= 0xFF);
}

void Cpu::CPX(u8& value){
    u16 result = this->registers.X - value;
    SetFlag(C, result < 0x100);
    SetFlag(S, CHECK_BIT(result, 7));
    SetFlag(Z, result &= 0xFF);
}

void Cpu::CPY(u8& value){
    u16 result = this->registers.Y - value;
    SetFlag(C, result < 0x100);
    SetFlag(S, CHECK_BIT(result, 7));
    SetFlag(Z, result &= 0xFF);
}

void Cpu::CLC() {
    SetFlag(C, 0);
}

void Cpu::SEC() {
    SetFlag(C, 1);
}

void Cpu::CLI() {
    SetFlag(I, 0);
}

void Cpu::SEI() {
    SetFlag(I, 1);
}

void Cpu::CLV() {
    SetFlag(V, 0);
}

void Cpu::CLD() {
    SetFlag(D, 0);
}

void Cpu::SED() {
    SetFlag(D, 1);
}

////////////////////////////////////////////////////////////////////////////////
/// Other operations
////////////////////////////////////////////////////////////////////////////////

void Cpu::NOP() {
    // TODO
}

void Cpu::UNIMP() {
    // TODO
}

////////////////////////////////////////////////////////////////////////////////
/// Operations variant with addressing 
////////////////////////////////////////////////////////////////////////////////

/// Logical OR 
/// 2 bytes; 6 cycles
void Cpu::ORA_IND_X() {
    ORA(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
}

/// 2 bytes; 3 cycles
void Cpu::ORA_ZP() {
    ORA(Memory(ZeroPage(Operand(1))));
}

void Cpu::ASL_ZP() {
    ASL(Memory(ZeroPage(Operand(1))));
}

void Cpu::ORA_IMM() {
    ORA(Operand(1));
}

void Cpu::ASL_ACC() {
    ASL(this->registers.A);
}

void Cpu::ORA_ABS() {
    ORA(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::ASL_ABS() {
    ASL(Memory(Absolute(Operand(1), Operand(2))));    
}

void Cpu::ORA_IND_Y() {
    ORA(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
}

void Cpu::ORA_ZP_X() {
    ORA(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::ASL_ZP_X() {
    ASL(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::ORA_ABS_Y() {
    ORA(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
}

void Cpu::ORA_ABS_X() {
    ORA(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::ASL_ABS_X() {
    ASL(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

// 6 cycles
void Cpu::AND_IND_X() {
    AND(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
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

void Cpu::AND_IMM() {
    AND(Operand(1));
}

void Cpu::ROL_ACC() {
    ROL(this->registers.A);
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

void Cpu::AND_IND_Y() {
    AND(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
}

void Cpu::AND_ZP_X() {
    AND(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::ROL_ZP_X() {
    ROL(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::AND_ABS_Y() {
    AND(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
}

void Cpu::AND_ABS_X() {
    AND(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::ROL_ABS_X() {
    ROL(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::EOR_IND_X() {
    EOR(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
}

void Cpu::EOR_ZP() {
    EOR(Memory(ZeroPage(Operand(1))));
}

void Cpu::LSR_ZP() {
    LSR(Memory(ZeroPage(Operand(1))));
}

void Cpu::EOR_IMM() {
    EOR(Operand(1));
}

void Cpu::LSR_ACC() {
    LSR(this->registers.A);
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

void Cpu::EOR_IND_Y() {
    EOR(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
}

void Cpu::EOR_ZP_X() {
    EOR(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::LSR_ZP_X() {
    LSR(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::EOR_ABS_Y() {
    EOR(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
}

void Cpu::EOR_ABS_X() {
    EOR(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::LSR_ABS_X() {
    LSR(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::ADC_IND_X() {
    ADC(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
}

void Cpu::ADC_ZP() {
    ADC(Memory(ZeroPage(Operand(1))));
}

void Cpu::ROR_ZP() {
    ROR(Memory(ZeroPage(Operand(1))));
}

void Cpu::ADC_IMM() {
    ADC(Operand(1));
}

void Cpu::ROR_ACC() {
    ROR(this->registers.A);
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

void Cpu::ADC_IND_Y() {
    ADC(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
}

void Cpu::ADC_ZP_X() {
    ADC(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::ROR_ZP_X() {
    ROR(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::ADC_ABS_Y() {
    ADC(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
}

void Cpu::ADC_ABS_X() {
    ADC(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::ROR_ABS_X() {
    ROR(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::STA_IND_X() {
    STA(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
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

void Cpu::STY_ABS() {
    STY(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::STA_ABS() {
    STA(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::STX_ABS() {
    STX(Memory(Absolute(Operand(1), Operand(2))));
}

void Cpu::STA_IND_Y() {
    STA(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
}

void Cpu::STY_ZP_X() {
    STY(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::STA_ZP_X() {
    STA(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::STX_ZP_Y() {
    STX(Memory(ZeroPageIndexed(Operand(1), this->registers.Y)));
}

void Cpu::STA_ABS_Y() {
    STA(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
}

void Cpu::STA_ABS_X() {
    STA(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::LDY_IMM() {
    LDY(Operand(1));
}

void Cpu::LDA_IND_X() {
    LDA(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
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

void Cpu::LDA_IMM() {
    LDA(Operand(1));
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

void Cpu::LDA_IND_Y() {
    LDA(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
}

void Cpu::LDY_ZP_X() {
    LDY(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::LDA_ZP_X() {
    LDA(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::LDX_ZP_Y() {
    LDX(Memory(ZeroPageIndexed(Operand(1), this->registers.Y)));
}

void Cpu::LDA_ABS_Y() {
    LDA(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
}

void Cpu::LDY_ABS_X() {
    LDY(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::LDA_ABS_X() {
    LDA(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::LDX_ABS_Y() {
    LDX(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
}

void Cpu::CPY_IMM() {
    CPY(Operand(1));
}

void Cpu::CMP_IND_X() {
    CMP(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
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

void Cpu::CMP_IMM() {
    CMP(Operand(1));
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

void Cpu::CMP_IND_Y() {
    CMP(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
}

void Cpu::CMP_ZP_X() {
    CMP(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::DEC_ZP_X() {
    DEC(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::CMP_ABS_Y() {
    CMP(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
}

void Cpu::CMP_ABS_X() {
    CMP(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::DEC_ABS_X() {
    DEC(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::CPX_IMM() {
    CPX(Operand(1));
}

void Cpu::SBC_IND_X() {
    SBC(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
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

void Cpu::SBC_IMM() {
    SBC(Operand(1));
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

void Cpu::SBC_IND_Y() {
    SBC(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
}

void Cpu::SBC_ZP_X() {
    SBC(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::INC_ZP_X() {
    INC(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
}

void Cpu::SBC_ABS_Y() {
    SBC(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
}

void Cpu::SBC_ABS_X() {
    SBC(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}

void Cpu::INC_ABS_X() {
    INC(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
}
