#include "cpu.h"
#include "dependencies.h"

Cpu::Cpu(){}

Cpu::Cpu(const Rom* rom) {
    const iNesHeader* header = rom->GetHeader();
    int prgRomBanks = header->prgRomBanks;
    int trainerOffset = rom->GetTrainerOffset();
    int prgRomBanksLocation = rom->headerSize + trainerOffset;

    switch (prgRomBanks) {
        case 1:
            memcpy(this->memory.prgRomLowerBank, rom->GetRaw() + prgRomBanksLocation, PRGROM_BANK_SIZE);
            memcpy(this->memory.prgRomUpperBank, rom->GetRaw() + prgRomBanksLocation, PRGROM_BANK_SIZE);
            break;
        case 2:
            memcpy(this->memory.prgRomLowerBank, rom->GetRaw() + prgRomBanksLocation, PRGROM_BANK_SIZE);
            memcpy(this->memory.prgRomUpperBank, rom->GetRaw() + prgRomBanksLocation + PRGROM_BANK_SIZE, PRGROM_BANK_SIZE);
            break;
        default: //TODO: implement multiple PRG-ROM banks
            break;
    }

    this->registers.PC = (Memory(0xFFFC) | Memory(0xFFFD) << 8);
}

bool Cpu::IsPageCrossed(u16 startAddress, u16 endAddress){
    return (startAddress & 0xFF00) != (endAddress & 0xFF00);
}

void Cpu::Execute(){
    auto opCode = OpCode();
    this->cycles = (this->*instructions[opCode])();
    this->registers.PC += this->instructionSizes[opCode];
}

inline u8& Cpu::OpCode(){
    return Memory(this->registers.PC);
}

inline u8& Cpu::Memory(const u16 address) {
    return this->memory.raw[address];
}

inline void Cpu::PushOnStack(u8 value){
    this->memory.stack[this->registers.SP] = value;
    this->registers.SP -= 1;
}

inline u8 Cpu::PopFromStack(){
    this->registers.SP += 1;
    return this->memory.stack[this->registers.SP];
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
    return FromValues(low + index);
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
    this->registers.A &= value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
}

void Cpu::ASL(u8& value){
    // 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
    Set<Flags::C>(CheckBit<8>(value));
    value <<= 1;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
}

void Cpu::BIT(u8& value){
    Set<Flags::Z>(CheckZero(value & this->registers.A));
    Set<Flags::S>(CheckSign(value));
    Set<Flags::V>(CheckBit<7>(value));
}

void Cpu::EOR(u8& value){
    this->registers.A ^= value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
}

void Cpu::LSR(u8& value){
    // 0 is shifted into bit 7 and the original bit 0 is shifted into the Carry.
    Set<Flags::C>(CheckBit<1>(value));
    value >>= 1;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
}

void Cpu::ORA(u8& value){
    this->registers.A |= value;
    Set<Flags::Z>(CheckZero(this->registers.A));
    Set<Flags::S>(CheckSign(this->registers.A));
}

void Cpu::ROL(u8& value){
    bool carry = Get<Flags::C>();
    Set<Flags::C>(CheckSign(value));
    value <<= 1;
    AssignBit<1>(value, carry);
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
}

void Cpu::ROR(u8& value){
    auto carry = Get<Flags::C>();
    Set<Flags::C>(CheckBit<1>(value));
    value >>= 1;
    AssignBit<8>(value, carry);
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
}

////////////////////////////////////////////////////////////////////////////////
/// Arithmetic Operations Definition
////////////////////////////////////////////////////////////////////////////////

void Cpu::ADC(u8& value){
    u16 result = value + this->registers.A + Get<Flags::C>();
    Set<Flags::Z>(CheckZero(result));
    Set<Flags::S>(CheckSign(value));
    Set<Flags::V>(CheckOverflow<>(this->registers.A, value, result));
    Set<Flags::C>(result > 0xFF);
    this->registers.A = (u8) result;
}

void Cpu::DEC(u8& value){
    value -= 1;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
}

void Cpu::INC(u8& value){
    value += 1;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
}

void Cpu::SBC(u8& value){
    u16 result = this->registers.A - value - Get<Flags::C>();
    Set<Flags::Z>(CheckZero(result));
    Set<Flags::S>(CheckSign(value));
    Set<Flags::V>(CheckOverflow<>(this->registers.A, value, result));
    Set<Flags::C>(result < 0x100);
    this->registers.A = (result & 0xFF);
}

////////////////////////////////////////////////////////////////////////////////
/// Memory Operations Definition
////////////////////////////////////////////////////////////////////////////////

void Cpu::LDA(u8& value){
    this->registers.A = value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
}

void Cpu::LDX(u8& value){
    this->registers.X = value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
}

void Cpu::LDY(u8& value){
    this->registers.Y = value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
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

//Branch on plus
u8 Cpu::BPL() {
    if (!Get<Flags::S>()) {
        auto pageCrossed = IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on minus
u8 Cpu::BMI() {
    if (Get<Flags::S>()) {
    auto pageCrossed = IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on overflow clear
u8 Cpu::BVC() {
    if (!Get<Flags::V>()) {
    auto pageCrossed = IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on overflow set
u8 Cpu::BVS() {
    if (Get<Flags::V>()) {
    auto pageCrossed = IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on carry clear
u8 Cpu::BCC() {
    if (!Get<Flags::C>()) {
    auto pageCrossed = IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on carry set
u8 Cpu::BCS() {
    if (Get<Flags::C>()) {
    auto pageCrossed = IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on not equal
u8 Cpu::BNE() {
    if (!Get<Flags::Z>()) {
    auto pageCrossed = IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on equal
u8 Cpu::BEQ() {
    if (Get<Flags::Z>()) {
    auto pageCrossed = IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

////////////////////////////////////////////////////////////////////////////////
/// Register Operations Definition
/// T[1][2] -> Transfert register 1 to register 2
/// DE[1] -> DEC register 1
/// IN[1] -> INC register 1
////////////////////////////////////////////////////////////////////////////////

u8 Cpu::TAX() {
    auto& value = this->registers.A;
    this->registers.X = value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
    return 2;
}

u8 Cpu::TXA() {
    auto& value = this->registers.X;
    this->registers.A = value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
    return 2;
}

u8 Cpu::DEX() {
    auto& value = this->registers.X;
    value -= 1 ;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
    return 2;
}

u8 Cpu::INX() {
    auto& value = this->registers.X;
    value += 1 ;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
    return 2;
}

u8 Cpu::TAY() {
    auto& value = this->registers.A;
    this->registers.Y = value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
    return 2;
}

u8 Cpu::TYA() {
    auto& value = this->registers.Y;
    this->registers.A = value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
    return 2;
}

u8 Cpu::DEY() {
    auto& value = this->registers.Y;
    value -= 1 ;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
    return 2;
}

u8 Cpu::INY() {
    auto& value = this->registers.Y;
    value += 1 ;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
    return 2;
}

////////////////////////////////////////////////////////////////////////////////
/// Stack Operations Definition
////////////////////////////////////////////////////////////////////////////////

// Transfert X to Stack ptr
u8 Cpu::TXS() {
    PushOnStack(this->registers.X);
    return 2;
}

u8 Cpu::TSX() {
    auto value = PopFromStack();
    this->registers.X = value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
    return 2;
}

// Push A
u8 Cpu::PHA() {
    PushOnStack(this->registers.A);
    return 3;
}

// Pop A
u8 Cpu::PLA() {
    auto value = PopFromStack();
    this->registers.A = value;
    Set<Flags::Z>(CheckZero(value));
    Set<Flags::S>(CheckSign(value));
    return 4;
}

// Push Processor Status
u8 Cpu::PHP() {
    PushOnStack(this->registers.P);
    return 3;
}

// Pull Processor Status
u8 Cpu::PLP() {
    this->registers.P = PopFromStack();
    return 4;
}

////////////////////////////////////////////////////////////////////////////////
/// PC Operations Definition
////////////////////////////////////////////////////////////////////////////////

u8 Cpu::BRK(){
    this->registers.PC += 1;
    PushOnStack((this->registers.PC >> 8) & 0xFF);     /* Push return address onto the stack. */
    PushOnStack(this->registers.PC & 0xFF);
    Set<Flags::B>(true);                           /* Set BFlag before pushing */
    PushOnStack(this->registers.P);
    Set<Flags::I>(true);
    this->registers.PC = (Memory(0xFFFE) | Memory(0xFFFF) << 8);
    return 7;
}

void Cpu::JMP(u8& value){
    this->registers.PC = value;
}

u8 Cpu::JSR() {
    auto address = Absolute(Operand(1), Operand(2));
    this->registers.PC += 2;
    PushOnStack((this->registers.PC >> 8) & 0xFF);	/* Push return address onto the stack. */
    PushOnStack(this->registers.PC & 0xFF);
    this->registers.PC = address;
    return 6;
}

u8 Cpu::RTI() {
    auto processorStatus = PopFromStack();
    this->registers.P = processorStatus;
    u16 address = PopFromStack();
    address |= (PopFromStack() << 8);	/* Load return address from stack. */
    this->registers.PC = address;
    return 6;
}

u8 Cpu::RTS() {
    u16 address = PopFromStack();
    address |= ((PopFromStack()) << 8);	/* Load return address from stack. */
    this->registers.PC = address;
    return 6;
}

///////////////////////////////////////////////////////////////////////////////
/// Flag Instructions
///////////////////////////////////////////////////////////////////////////////

void Cpu::CMP(u8& value){
    u16 result = this->registers.A - value;
    Set<Flags::C>(result < 0x100);
    Set<Flags::S>(CheckSign(result));
    Set<Flags::Z>(CheckZero(result));
}

void Cpu::CPX(u8& value){
    u16 result = this->registers.X - value;
    Set<Flags::C>(result < 0x100);
    Set<Flags::S>(CheckSign(result));
    Set<Flags::Z>(CheckZero(result));
}

void Cpu::CPY(u8& value){
    u16 result = this->registers.Y - value;
    Set<Flags::C>(result < 0x100);
    Set<Flags::S>(CheckSign(result));
    Set<Flags::Z>(CheckZero(result));
}

u8 Cpu::CLC() {
    Set<Flags::C>(false);
    return 2;
}

u8 Cpu::SEC() {
    Set<Flags::C>(true);
    return 2;
}

u8 Cpu::CLI() {
    Set<Flags::I>(false);
    return 2;
}

u8 Cpu::SEI() {
    Set<Flags::I>(true);
    return 2;
}

u8 Cpu::CLV() {
    Set<Flags::V>(false);
    return 2;
}

u8 Cpu::CLD() {
    Set<Flags::D>(false);
    return 2;
}

u8 Cpu::SED() {
    Set<Flags::D>(false);
    return 2;
}

////////////////////////////////////////////////////////////////////////////////
/// Other operations
////////////////////////////////////////////////////////////////////////////////

u8 Cpu::NOP() {
    return 2;
}

u8 Cpu::UNIMP() {
    return 2;
}

////////////////////////////////////////////////////////////////////////////////
/// Operations variant with addressing
////////////////////////////////////////////////////////////////////////////////

/// Logical OR
/// 2 bytes; 6 cycles
u8 Cpu::ORA_IND_X() {
    ORA(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
    return 6;
}

/// 2 bytes; 3 cycles
u8 Cpu::ORA_ZP() {
    ORA(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::ASL_ZP() {
    ASL(Memory(ZeroPage(Operand(1))));
    return 5;
}

u8 Cpu::ORA_IMM() {
    ORA(Operand(1));
    return 2;
}

u8 Cpu::ASL_ACC() {
    ASL(this->registers.A);
    return 2;
}

u8 Cpu::ORA_ABS() {
    ORA(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::ASL_ABS() {
    ASL(Memory(Absolute(Operand(1), Operand(2))));
    return 6;
}

u8 Cpu::ORA_IND_Y() {
    ORA(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
    auto address = Indirect(Operand(1), 0);
    return 5 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::ORA_ZP_X() {
    ORA(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 4;
}

u8 Cpu::ASL_ZP_X() {
    ASL(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::ORA_ABS_Y() {
    ORA(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::ORA_ABS_X() {
    ORA(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::ASL_ABS_X() {
    ASL(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    return 7;
}

u8 Cpu::AND_IND_X() {
    AND(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::BIT_ZP() {
    BIT(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::AND_ZP() {
    AND(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::ROL_ZP() {
    ROL(Memory(ZeroPage(Operand(1))));
    return 5;
}

u8 Cpu::AND_IMM() {
    AND(Operand(1));
    return 2;
}

u8 Cpu::ROL_ACC() {
    ROL(this->registers.A);
    return 2;
}

u8 Cpu::BIT_ABS() {
    BIT(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::AND_ABS() {
    AND(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::ROL_ABS() {
    ROL(Memory(Absolute(Operand(1), Operand(2))));
    return 6;
}

u8 Cpu::AND_IND_Y() {
    AND(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
    auto address = Indirect(Operand(1), 0);
    return 5 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::AND_ZP_X() {
    AND(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 4;
}

u8 Cpu::ROL_ZP_X() {
    ROL(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::AND_ABS_Y() {
    AND(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::AND_ABS_X() {
    AND(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::ROL_ABS_X() {
    ROL(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    return 7;
}

u8 Cpu::EOR_IND_X() {
    EOR(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::EOR_ZP() {
    EOR(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::LSR_ZP() {
    LSR(Memory(ZeroPage(Operand(1))));
    return 5;
}

u8 Cpu::EOR_IMM() {
    EOR(Operand(1));
    return 2;
}

u8 Cpu::LSR_ACC() {
    LSR(this->registers.A);
    return 2;
}

u8 Cpu::JMP_ABS() {
    JMP(Memory(Absolute(Operand(1), Operand(2))));
    return 3;
}

u8 Cpu::EOR_ABS() {
    EOR(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::LSR_ABS() {
    LSR(Memory(Absolute(Operand(1), Operand(2))));
    return 6;
}

u8 Cpu::EOR_IND_Y() {
    EOR(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
    auto address = Indirect(Operand(1), 0);
    return 5 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::EOR_ZP_X() {
    EOR(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 4;
}

u8 Cpu::LSR_ZP_X() {
    LSR(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::EOR_ABS_Y() {
    EOR(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::EOR_ABS_X() {
    EOR(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::LSR_ABS_X() {
    LSR(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    return 7;
}

u8 Cpu::ADC_IND_X() {
    ADC(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::ADC_ZP() {
    ADC(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::ROR_ZP() {
    ROR(Memory(ZeroPage(Operand(1))));
    return 5;
}

u8 Cpu::ADC_IMM() {
    ADC(Operand(1));
    return 2;
}

u8 Cpu::ROR_ACC() {
    ROR(this->registers.A);
    return 2;
}

u8 Cpu::JMP_IND() {
    if (IsPageCrossed(this->registers.PC + 1, this->registers.PC + 2))
        JMP(Memory(Indirect(Operand(1), Operand(-0xFE)))); //wrap around
    else
        JMP(Memory(Indirect(Operand(1), Operand(2))));
    return 5;
}

u8 Cpu::ADC_ABS() {
    ADC(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::ROR_ABS() {
    ROR(Memory(Absolute(Operand(1), Operand(2))));
    return 6;
}

u8 Cpu::ADC_IND_Y() {
    ADC(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
    auto address = Indirect(Operand(1), 0);
    return 5 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::ADC_ZP_X() {
    ADC(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 4;
}

u8 Cpu::ROR_ZP_X() {
    ROR(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::ADC_ABS_Y() {
    ADC(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::ADC_ABS_X() {
    ADC(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::ROR_ABS_X() {
    ROR(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    return 7;
}

u8 Cpu::STA_IND_X() {
    STA(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::STY_ZP() {
    STY(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::STA_ZP() {
    STA(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::STX_ZP() {
    STX(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::STY_ABS() {
    STY(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::STA_ABS() {
    STA(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::STX_ABS() {
    STX(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::STA_IND_Y() {
    STA(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
    return 6;
}

u8 Cpu::STY_ZP_X() {
    STY(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 4;
}

u8 Cpu::STA_ZP_X() {
    STA(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 4;
}

u8 Cpu::STX_ZP_Y() {
    STX(Memory(ZeroPageIndexed(Operand(1), this->registers.Y)));
    return 4;
}

u8 Cpu::STA_ABS_Y() {
    STA(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
    return 5;
}

u8 Cpu::STA_ABS_X() {
    STA(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    return 5;
}

u8 Cpu::LDY_IMM() {
    LDY(Operand(1));
    return 2;
}

u8 Cpu::LDA_IND_X() {
    LDA(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::LDX_IMM() {
    LDX(Operand(1));
    return 2;
}

u8 Cpu::LDY_ZP() {
    LDY(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::LDA_ZP() {
    LDA(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::LDX_ZP() {
    LDX(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::LDA_IMM() {
    LDA(Operand(1));
    return 2;
}

u8 Cpu::LDY_ABS() {
    LDY(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::LDA_ABS() {
    LDA(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::LDX_ABS() {
    LDX(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::LDA_IND_Y() {
    LDA(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
    auto address = Indirect(Operand(1), 0);
    return 5 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::LDY_ZP_X() {
    LDY(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 4;
}

u8 Cpu::LDA_ZP_X() {
    LDA(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 4;
}

u8 Cpu::LDX_ZP_Y() {
    LDX(Memory(ZeroPageIndexed(Operand(1), this->registers.Y)));
    return 4;
}

u8 Cpu::LDA_ABS_Y() {
    LDA(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::LDY_ABS_X() {
    LDY(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::LDA_ABS_X() {
    LDA(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::LDX_ABS_Y() {
    LDX(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::CPY_IMM() {
    CPY(Operand(1));
    return 2;
}

u8 Cpu::CMP_IND_X() {
    CMP(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::CPY_ZP() {
    CPY(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::CMP_ZP() {
    CMP(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::DEC_ZP() {
    DEC(Memory(ZeroPage(Operand(1))));
    return 5;
}

u8 Cpu::CMP_IMM() {
    CMP(Operand(1));
    return 2;
}

u8 Cpu::CPY_ABS() {
    CPY(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::CMP_ABS() {
    CMP(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::DEC_ABS() {
    DEC(Memory(Absolute(Operand(1), Operand(2))));
    return 6;
}

u8 Cpu::CMP_IND_Y() {
    CMP(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
    auto address = Indirect(Operand(1), 0);
    return 5 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::CMP_ZP_X() {
    CMP(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 4;
}

u8 Cpu::DEC_ZP_X() {
    DEC(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::CMP_ABS_Y() {
    CMP(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::CMP_ABS_X() {
    CMP(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::DEC_ABS_X() {
    DEC(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    return 7;
}

u8 Cpu::CPX_IMM() {
    CPX(Operand(1));
    return 2;
}

u8 Cpu::SBC_IND_X() {
    SBC(Memory(PreIndexedIndirect(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::CPX_ZP() {
    CPX(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::SBC_ZP() {
    SBC(Memory(ZeroPage(Operand(1))));
    return 3;
}

u8 Cpu::INC_ZP() {
    INC(Memory(ZeroPage(Operand(1))));
    return 5;
}

u8 Cpu::SBC_IMM() {
    SBC(Operand(1));
    return 2;
}

u8 Cpu::CPX_ABS() {
    CPX(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::SBC_ABS() {
    SBC(Memory(Absolute(Operand(1), Operand(2))));
    return 4;
}

u8 Cpu::INC_ABS() {
    INC(Memory(Absolute(Operand(1), Operand(2))));
    return 6;
}

u8 Cpu::SBC_IND_Y() {
    SBC(Memory(PostIndexedIndirect(Operand(1), this->registers.Y)));
    auto address = Indirect(Operand(1), 0);
    return 5 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::SBC_ZP_X() {
    SBC(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 4;
}

u8 Cpu::INC_ZP_X() {
    INC(Memory(ZeroPageIndexed(Operand(1), this->registers.X)));
    return 6;
}

u8 Cpu::SBC_ABS_Y() {
    SBC(Memory(Indexed(Operand(1), Operand(2), this->registers.Y)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::SBC_ABS_X() {
    SBC(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    auto address = FromValues(Operand(1), Operand(2));
    return 4 + IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::INC_ABS_X() {
    INC(Memory(Indexed(Operand(1), Operand(2), this->registers.X)));
    return 7;
}
