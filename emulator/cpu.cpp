#include "cpu.h"
#include "memory.h"
#include "dependencies.h"

using namespace Frankenstein;
using Mode = Frankenstein::Memory::Addressing;

Cpu::Cpu(Memory& ram) : memory(ram){}

Cpu::Cpu(Memory& ram, Rom& rom) : memory(ram) {
    const iNesHeader* header = rom.GetHeader();
    int prgRomBanks = header->prgRomBanks;
    int trainerOffset = rom.GetTrainerOffset();
    int prgRomBanksLocation = rom.headerSize + trainerOffset;

    switch (prgRomBanks) {
        case 1:
            memcpy(&this->memory[Memory::ADDR_PRG_ROM_LOWER_BANK], rom.GetRaw() + prgRomBanksLocation, PRGROM_BANK_SIZE);
            memcpy(&this->memory[Memory::ADDR_PRG_ROM_UPPER_BANK], rom.GetRaw() + prgRomBanksLocation, PRGROM_BANK_SIZE);
            break;
        case 2:
            memcpy(&this->memory[Memory::ADDR_PRG_ROM_LOWER_BANK], rom.GetRaw() + prgRomBanksLocation, PRGROM_BANK_SIZE);
            memcpy(&this->memory[Memory::ADDR_PRG_ROM_UPPER_BANK], rom.GetRaw() + prgRomBanksLocation + PRGROM_BANK_SIZE, PRGROM_BANK_SIZE);
            break;
        default: //TODO: implement multiple PRG-ROM banks
            break;
    }

    this->registers.PC = (memory[0xFFFC] | memory[0xFFFD] << 8);
}

void Cpu::Execute(){
    auto opCode = OpCode();
    this->cycles = (this->*instructions[opCode])();
    this->registers.PC += this->instructionSizes[opCode];
}

inline u8& Cpu::OpCode(){
    return this->memory[this->registers.PC];
}

inline void Cpu::PushOnStack(u8 value){
    this->memory[Memory::ADDR_STACK + this->registers.SP] = value;
    this->registers.SP -= 1;
}

inline u8 Cpu::PopFromStack(){
    this->registers.SP += 1;
    return this->memory[Memory::ADDR_STACK + this->registers.SP];
}

inline u8& Cpu::Operand(int number) {
    return this->memory[this->registers.PC + number];
}

////////////////////////////////////////////////////////////////////////////////
/// Binary Operations Definition
////////////////////////////////////////////////////////////////////////////////

void Cpu::AND(u8& value){
    this->registers.A &= value;
    Set<Flags::Z>(CheckZero(this->registers.A));
    Set<Flags::S>(CheckSign(this->registers.A));
}

void Cpu::ASL(u8& value){
    // 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
    Set<Flags::C>(CheckBit<8>(value));
    value <<= 1;
    Set<Flags::Z>(CheckZero(this->registers.A));
    Set<Flags::S>(CheckSign(this->registers.A));
}

void Cpu::BIT(u8& value){
    auto result = value & this->registers.A;
    Set<Flags::Z>(CheckZero(result));
    Set<Flags::V>(CheckBit<7>(value));
    Set<Flags::S>(CheckBit<8>(value));
}

void Cpu::EOR(u8& value){
    this->registers.A ^= value;
    Set<Flags::Z>(CheckZero(this->registers.A));
    Set<Flags::S>(CheckSign(this->registers.A));
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
    Set<Flags::C>(CheckBit<8>(value));
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
    u8 truncResult = static_cast<u8>(result);
    
    Set<Flags::Z>(CheckZero(truncResult));
    Set<Flags::S>(CheckSign(truncResult));
    Set<Flags::C>(CheckBit<9, u16>(result));
    Set<Flags::V>(CheckOverflow<>(this->registers.A, value, truncResult));
    
    this->registers.A = truncResult;
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
    u8 truncResult = static_cast<u8>(result);
    
    Set<Flags::Z>(CheckZero(truncResult));
    Set<Flags::S>(CheckSign(truncResult));
    Set<Flags::C>(CheckBit<9, u16>(result));
    Set<Flags::V>(CheckOverflow<>(this->registers.A, value, truncResult));
    this->registers.A = (result & 0xFF);
}

////////////////////////////////////////////////////////////////////////////////
/// Memory Operations Definition
////////////////////////////////////////////////////////////////////////////////

void Cpu::LDA(u8& value){
    this->registers.A = value;
    Set<Flags::Z>(CheckZero(this->registers.A));
    Set<Flags::S>(CheckSign(this->registers.A));
}

void Cpu::LDX(u8& value){
    this->registers.X = value;
    Set<Flags::Z>(CheckZero(this->registers.X));
    Set<Flags::S>(CheckSign(this->registers.X));
}

void Cpu::LDY(u8& value){
    this->registers.Y = value;
    Set<Flags::Z>(CheckZero(this->registers.Y));
    Set<Flags::S>(CheckSign(this->registers.Y));
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
        auto pageCrossed = Memory::IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on minus
u8 Cpu::BMI() {
    if (Get<Flags::S>()) {
    auto pageCrossed = Memory::IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on overflow clear
u8 Cpu::BVC() {
    if (!Get<Flags::V>()) {
    auto pageCrossed = Memory::IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on overflow set
u8 Cpu::BVS() {
    if (Get<Flags::V>()) {
    auto pageCrossed = Memory::IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on carry clear
u8 Cpu::BCC() {
    if (!Get<Flags::C>()) {
    auto pageCrossed = Memory::IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on carry set
u8 Cpu::BCS() {
    if (Get<Flags::C>()) {
    auto pageCrossed = Memory::IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on not equal
u8 Cpu::BNE() {
    if (!Get<Flags::Z>()) {
    auto pageCrossed = Memory::IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
	this->registers.PC = (this->registers.PC + Operand(1)) & 0xFF;
        return 3 + pageCrossed;
    }
    return 2;
}

//Branch on equal
u8 Cpu::BEQ() {
    if (Get<Flags::Z>()) {
    auto pageCrossed = Memory::IsPageCrossed(this->registers.PC + 2, (this->registers.PC + 2 + Operand(1)) & 0xFF);
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
    Set<Flags::Z>(CheckZero(this->registers.X));
    Set<Flags::S>(CheckSign(this->registers.X));
    return 2;
}

u8 Cpu::TXA() {
    this->registers.A = this->registers.X;
    Set<Flags::Z>(CheckZero(this->registers.A));
    Set<Flags::S>(CheckSign(this->registers.A));
    return 2;
}

u8 Cpu::DEX() {
    this->registers.X -= 1;
    Set<Flags::Z>(CheckZero(this->registers.X));
    Set<Flags::S>(CheckSign(this->registers.X));
    return 2;
}

u8 Cpu::INX() {
    this->registers.X += 1;
    Set<Flags::Z>(CheckZero(this->registers.X));
    Set<Flags::S>(CheckSign(this->registers.X));
    return 2;
}

u8 Cpu::TAY() {
    this->registers.Y = this->registers.A;
    Set<Flags::Z>(CheckZero(this->registers.Y));
    Set<Flags::S>(CheckSign(this->registers.Y));
    return 2;
}

u8 Cpu::TYA() {
    this->registers.A = this->registers.Y;
    Set<Flags::Z>(CheckZero(this->registers.A));
    Set<Flags::S>(CheckSign(this->registers.A));
    return 2;
}

u8 Cpu::DEY() {
    this->registers.Y -= 1;
    Set<Flags::Z>(CheckZero(this->registers.Y));
    Set<Flags::S>(CheckSign(this->registers.Y));
    return 2;
}

u8 Cpu::INY() {
    this->registers.Y += 1;
    Set<Flags::Z>(CheckZero(this->registers.Y));
    Set<Flags::S>(CheckSign(this->registers.Y));
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
    this->registers.PC = (memory[0xFFFE] | memory[0xFFFF] << 8);
    return 7;
}

void Cpu::JMP(u8& value){
    this->registers.PC = value;
}

u8 Cpu::JSR() {
    auto address = Memory::Absolute(Operand(1), Operand(2));
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
    ORA(memory.get<Mode::PreIndexedIndirect>(Operand(1), this->registers.X));
    return 6;
}

/// 2 bytes; 3 cycles
u8 Cpu::ORA_ZP() {
    ORA(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::ASL_ZP() {
    ASL(memory.get<Mode::ZeroPage>(Operand(1)));
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
    ORA(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::ASL_ABS() {
    ASL(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 6;
}

u8 Cpu::ORA_IND_Y() {
    ORA(memory.get<Mode::PostIndexedIndirect>(Operand(1), this->registers.Y));
    auto address = memory.Indirect(Operand(1), 0);
    return 5 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::ORA_ZP_X() {
    ORA(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 4;
}

u8 Cpu::ASL_ZP_X() {
    ASL(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::ORA_ABS_Y() {
    ORA(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.Y));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::ORA_ABS_X() {
    ORA(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::ASL_ABS_X() {
    ASL(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    return 7;
}

u8 Cpu::AND_IND_X() {
    AND(memory.get<Mode::PreIndexedIndirect>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::BIT_ZP() {
    BIT(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::AND_ZP() {
    AND(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::ROL_ZP() {
    ROL(memory.get<Mode::ZeroPage>(Operand(1)));
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
    BIT(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::AND_ABS() {
    AND(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::ROL_ABS() {
    ROL(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 6;
}

u8 Cpu::AND_IND_Y() {
    AND(memory.get<Mode::PostIndexedIndirect>(Operand(1), this->registers.Y));
    auto address = memory.Indirect(Operand(1), 0);
    return 5 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::AND_ZP_X() {
    AND(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 4;
}

u8 Cpu::ROL_ZP_X() {
    ROL(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::AND_ABS_Y() {
    AND(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.Y));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::AND_ABS_X() {
    AND(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::ROL_ABS_X() {
    ROL(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    return 7;
}

u8 Cpu::EOR_IND_X() {
    EOR(memory.get<Mode::PreIndexedIndirect>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::EOR_ZP() {
    EOR(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::LSR_ZP() {
    LSR(memory.get<Mode::ZeroPage>(Operand(1)));
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
    JMP(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 3;
}

u8 Cpu::EOR_ABS() {
    EOR(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::LSR_ABS() {
    LSR(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 6;
}

u8 Cpu::EOR_IND_Y() {
    EOR(memory.get<Mode::PostIndexedIndirect>(Operand(1), this->registers.Y));
    auto address = memory.Indirect(Operand(1), 0);
    return 5 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::EOR_ZP_X() {
    EOR(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 4;
}

u8 Cpu::LSR_ZP_X() {
    LSR(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::EOR_ABS_Y() {
    EOR(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.Y));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::EOR_ABS_X() {
    EOR(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::LSR_ABS_X() {
    LSR(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    return 7;
}

u8 Cpu::ADC_IND_X() {
    ADC(memory.get<Mode::PreIndexedIndirect>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::ADC_ZP() {
    ADC(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::ROR_ZP() {
    ROR(memory.get<Mode::ZeroPage>(Operand(1)));
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
    if (Memory::IsPageCrossed(this->registers.PC + 1, this->registers.PC + 2))
        JMP(memory.get<Mode::Indirect>(Operand(1), Operand(-0xFE))); //wrap around
    else
        JMP(memory.get<Mode::Indirect>(Operand(1), Operand(2)));
    return 5;
}

u8 Cpu::ADC_ABS() {
    ADC(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::ROR_ABS() {
    ROR(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 6;
}

u8 Cpu::ADC_IND_Y() {
    ADC(memory.get<Mode::PostIndexedIndirect>(Operand(1), this->registers.Y));
    auto address = memory.Indirect(Operand(1), 0);
    return 5 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::ADC_ZP_X() {
    ADC(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 4;
}

u8 Cpu::ROR_ZP_X() {
    ROR(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::ADC_ABS_Y() {
    ADC(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.Y));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::ADC_ABS_X() {
    ADC(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::ROR_ABS_X() {
    ROR(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    return 7;
}

u8 Cpu::STA_IND_X() {
    STA(memory.get<Mode::PreIndexedIndirect>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::STY_ZP() {
    STY(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::STA_ZP() {
    STA(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::STX_ZP() {
    STX(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::STY_ABS() {
    STY(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::STA_ABS() {
    STA(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::STX_ABS() {
    STX(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::STA_IND_Y() {
    STA(memory.get<Mode::PostIndexedIndirect>(Operand(1), this->registers.Y));
    return 6;
}

u8 Cpu::STY_ZP_X() {
    STY(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 4;
}

u8 Cpu::STA_ZP_X() {
    STA(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 4;
}

u8 Cpu::STX_ZP_Y() {
    STX(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.Y));
    return 4;
}

u8 Cpu::STA_ABS_Y() {
    STA(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.Y));
    return 5;
}

u8 Cpu::STA_ABS_X() {
    STA(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    return 5;
}

u8 Cpu::LDY_IMM() {
    LDY(Operand(1));
    return 2;
}

u8 Cpu::LDA_IND_X() {
    LDA(memory.get<Mode::PreIndexedIndirect>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::LDX_IMM() {
    LDX(Operand(1));
    return 2;
}

u8 Cpu::LDY_ZP() {
    LDY(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::LDA_ZP() {
    LDA(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::LDX_ZP() {
    LDX(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::LDA_IMM() {
    LDA(Operand(1));
    return 2;
}

u8 Cpu::LDY_ABS() {
    LDY(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::LDA_ABS() {
    LDA(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::LDX_ABS() {
    LDX(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::LDA_IND_Y() {
    LDA(memory.get<Mode::PostIndexedIndirect>(Operand(1), this->registers.Y));
    auto address = memory.Indirect(Operand(1), 0);
    return 5 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::LDY_ZP_X() {
    LDY(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 4;
}

u8 Cpu::LDA_ZP_X() {
    LDA(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 4;
}

u8 Cpu::LDX_ZP_Y() {
    LDX(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.Y));
    return 4;
}

u8 Cpu::LDA_ABS_Y() {
    LDA(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.Y));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::LDY_ABS_X() {
    LDY(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::LDA_ABS_X() {
    LDA(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::LDX_ABS_Y() {
    LDX(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.Y));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::CPY_IMM() {
    CPY(Operand(1));
    return 2;
}

u8 Cpu::CMP_IND_X() {
    CMP(memory.get<Mode::PreIndexedIndirect>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::CPY_ZP() {
    CPY(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::CMP_ZP() {
    CMP(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::DEC_ZP() {
    DEC(memory.get<Mode::ZeroPage>(Operand(1)));
    return 5;
}

u8 Cpu::CMP_IMM() {
    CMP(Operand(1));
    return 2;
}

u8 Cpu::CPY_ABS() {
    CPY(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::CMP_ABS() {
    CMP(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::DEC_ABS() {
    DEC(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 6;
}

u8 Cpu::CMP_IND_Y() {
    CMP(memory.get<Mode::PostIndexedIndirect>(Operand(1), this->registers.Y));
    auto address = memory.Indirect(Operand(1), 0);
    return 5 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::CMP_ZP_X() {
    CMP(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 4;
}

u8 Cpu::DEC_ZP_X() {
    DEC(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::CMP_ABS_Y() {
    CMP(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.Y));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::CMP_ABS_X() {
    CMP(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::DEC_ABS_X() {
    DEC(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    return 7;
}

u8 Cpu::CPX_IMM() {
    CPX(Operand(1));
    return 2;
}

u8 Cpu::SBC_IND_X() {
    SBC(memory.get<Mode::PreIndexedIndirect>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::CPX_ZP() {
    CPX(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::SBC_ZP() {
    SBC(memory.get<Mode::ZeroPage>(Operand(1)));
    return 3;
}

u8 Cpu::INC_ZP() {
    INC(memory.get<Mode::ZeroPage>(Operand(1)));
    return 5;
}

u8 Cpu::SBC_IMM() {
    SBC(Operand(1));
    return 2;
}

u8 Cpu::CPX_ABS() {
    CPX(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::SBC_ABS() {
    SBC(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 4;
}

u8 Cpu::INC_ABS() {
    INC(memory.get<Mode::Absolute>(Operand(1), Operand(2)));
    return 6;
}

u8 Cpu::SBC_IND_Y() {
    SBC(memory.get<Mode::PostIndexedIndirect>(Operand(1), this->registers.Y));
    auto address = memory.Indirect(Operand(1), 0);
    return 5 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::SBC_ZP_X() {
    SBC(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 4;
}

u8 Cpu::INC_ZP_X() {
    INC(memory.get<Mode::ZeroPageIndexed>(Operand(1), this->registers.X));
    return 6;
}

u8 Cpu::SBC_ABS_Y() {
    SBC(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.Y));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.Y);
}

u8 Cpu::SBC_ABS_X() {
    SBC(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    auto address = Memory::FromValues(Operand(1), Operand(2));
    return 4 + Memory::IsPageCrossed(address, address + this->registers.X);
}

u8 Cpu::INC_ABS_X() {
    INC(memory.get<Mode::Indexed>(Operand(1), Operand(2), this->registers.X));
    return 7;
}
