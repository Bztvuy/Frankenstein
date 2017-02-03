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

void Cpu::BRK() {
    // TODO
}

/// Logical OR 
/// 2 bytes; 6 cycles
void Cpu::ORA_IND_X() {
    auto operand = Memory(this->registers->PC + 1);
    auto value = Memory(PreIndexedIndirect(operand, this->registers->Y));
    this->registers->A |= value;
}

void Cpu::ORA_ZP() {
    auto operand = Memory(this->registers->PC + 1);
    auto value = Memory(ZeroPage(operand));
    this->registers->A |= value;
}

void Cpu::ASL_ZP() {
    auto operand = Memory(this->registers->PC + 1);
    auto address = ZeroPage(operand);
    auto value = Memory(address);
    //this->flags->carry = CHECK_BIT(value, 7);
    Memory(address) <<= 1;
}

void Cpu::PHP() {
    // TODO
}

void Cpu::ORA_IMM() {
    auto operand = Memory(this->registers->PC + 1);
    auto value = Immediate(operand);
    this->registers->A |= value;
}

void Cpu::ASL_ACC() {
    auto value = this->registers->A;
    //this->flags->carry = CHECK_BIT(value, 7);
    this->registers->A <<= 1;
}

void Cpu::ORA_ABS() {
    auto low = Memory(this->registers->PC + 1);
    auto high = Memory(this->registers->PC + 2);
    auto value = Memory(Absolute(low, high));
    this->registers->A |= value;
}

void Cpu::ASL_ABS() {
    auto low = Memory(this->registers->PC + 1);
    auto high = Memory(this->registers->PC + 2);
    auto address = Absolute(low, high);
    auto value = Memory(address);
    //this->flags->carry = CHECK_BIT(value, 7);
    Memory(address) <<= 1;
}

void Cpu::BPL() {
    // TODO
}

void Cpu::ORA_IND_Y() {
    auto operand = Memory(this->registers->PC + 1);
    auto value = Memory(PostIndexedIndirect(operand, this->registers->Y));
    this->registers->A |= value;
}

void Cpu::ORA_ZP_X() {
    auto operand = Memory(this->registers->PC + 1);
    auto value = Memory(ZeroPageIndexed(operand, this->registers->X));
    this->registers->A |= value;
}

void Cpu::ASL_ZP_X() {
    auto operand = Memory(this->registers->PC + 1);
    auto address = ZeroPageIndexed(operand, this->registers->X);
    auto value = Memory(address);
    //this->flags->carry = CHECK_BIT(value, 7);
    Memory(address) <<= 1;
}

void Cpu::CLC() {
    // TODO
}

void Cpu::ORA_ABS_Y() {
    auto low = Memory(this->registers->PC + 1);
    auto high = Memory(this->registers->PC + 2);
    auto value = Memory(Indexed(low, high, this->registers->Y));
    this->registers->A |= value;
}

void Cpu::ORA_ABS_X() {
    auto low = Memory(this->registers->PC + 1);
    auto high = Memory(this->registers->PC + 2);
    auto value = Memory(Indexed(low, high, this->registers->X));
    this->registers->A |= value;
}

void Cpu::ASL_ABS_X() {
    auto low = Memory(this->registers->PC + 1);
    auto high = Memory(this->registers->PC + 2);
    auto address = Indexed(low, high, this->registers->X);
    auto value = Memory(address);
//    this->flags->carry = CHECK_BIT(value, 7);
    Memory(address) <<= 1;
}

void Cpu::JSR() {
    // TODO
}

void Cpu::AND_IND_X() {
    // TODO
}

void Cpu::BIT_ZP() {
    // TODO
}

void Cpu::AND_ZP() {
    // TODO
}

void Cpu::ROL_ZP() {
    // TODO
}

void Cpu::PLP() {
    // TODO
}

void Cpu::AND_IMM() {
    // TODO
}

void Cpu::ROL_ACC() {
    // TODO
}

void Cpu::BIT_ABS() {
    // TODO
}

void Cpu::AND_ABS() {
    // TODO
}

void Cpu::ROL_ABS() {
    // TODO
}

void Cpu::BMI() {
    // TODO
}

void Cpu::AND_IND_Y() {
    // TODO
}

void Cpu::AND_ZP_X() {
    // TODO
}

void Cpu::ROL_ZP_X() {
    // TODO
}

void Cpu::SEC() {
    // TODO
}

void Cpu::AND_ABS_Y() {
    // TODO
}

void Cpu::AND_ABS_X() {
    // TODO
}

void Cpu::ROL_ABS_X() {
    // TODO
}

void Cpu::RTI() {
    // TODO
}

void Cpu::EOR_IND_X() {
    // TODO
}

void Cpu::EOR_ZP() {
    // TODO
}

void Cpu::LSR_ZP() {
    // TODO
}

void Cpu::PHA() {
    // TODO
}

void Cpu::EOR_IMM() {
    // TODO
}

void Cpu::LSR_ACC() {
    // TODO
}

void Cpu::JMP_ABS() {
    // TODO
}

void Cpu::EOR_ABS() {
    // TODO
}

void Cpu::LSR_ABS() {
    // TODO
}

void Cpu::BVC() {
    // TODO
}

void Cpu::EOR_IND_Y() {
    // TODO
}

void Cpu::EOR_ZP_X() {
    // TODO
}

void Cpu::LSR_ZP_X() {
    // TODO
}

void Cpu::CLI() {
    // TODO
}

void Cpu::EOR_ABS_Y() {
    // TODO
}

void Cpu::EOR_ABS_X() {
    // TODO
}

void Cpu::LSR_ABS_X() {
    // TODO
}

void Cpu::RTS() {
    // TODO
}

void Cpu::ADC_IND_X() {
    // TODO
}

void Cpu::ADC_ZP() {
    // TODO
}

void Cpu::ROR_ZP() {
    // TODO
}

void Cpu::PLA() {
    // TODO
}

void Cpu::ADC_IMM() {
    // TODO
}

void Cpu::ROR_ACC() {
    // TODO
}

void Cpu::JMP_IND() {
    // TODO
}

void Cpu::ADC_ABS() {
    // TODO
}

void Cpu::ROR_ABS() {
    // TODO
}

void Cpu::BVS() {
    // TODO
}

void Cpu::ADC_IND_Y() {
    // TODO
}

void Cpu::ADC_ZP_X() {
    // TODO
}

void Cpu::ROR_ZP_X() {
    // TODO
}

void Cpu::SEI() {
    // TODO
}

void Cpu::ADC_ABS_Y() {
    // TODO
}

void Cpu::ADC_ABS_X() {
    // TODO
}

void Cpu::ROR_ABS_X() {
    // TODO
}

void Cpu::STA_IND_X() {
    // TODO
}

void Cpu::STY_ZP() {
    // TODO
}

void Cpu::STA_ZP() {
    // TODO
}

void Cpu::STX_ZP() {
    // TODO
}

void Cpu::DEY() {
    // TODO
}

void Cpu::TXA() {
    // TODO
}

void Cpu::STY_ABS() {
    // TODO
}

void Cpu::STA_ABS() {
    // TODO
}

void Cpu::STX_ABS() {
    // TODO
}

void Cpu::BCC() {
    // TODO
}

void Cpu::STA_IND_Y() {
    // TODO
}

void Cpu::STY_ZP_X() {
    // TODO
}

void Cpu::STA_ZP_X() {
    // TODO
}

void Cpu::STX_ZP_Y() {
    // TODO
}

void Cpu::TYA() {
    // TODO
}

void Cpu::STA_ABS_Y() {
    // TODO
}

void Cpu::TXS() {
    // TODO
}

void Cpu::STA_ABS_X() {
    // TODO
}

void Cpu::LDY_IMM() {
    // TODO
}

void Cpu::LDA_IND_X() {
    // TODO
}

void Cpu::LDX_IMM() {
    // TODO
}

void Cpu::LDY_ZP() {
    // TODO
}

void Cpu::LDA_ZP() {
    // TODO
}

void Cpu::LDX_ZP() {
    // TODO
}

void Cpu::TAY() {
    // TODO
}

void Cpu::LDA_IMM() {
    // TODO
}

void Cpu::TAX() {
    // TODO
}

void Cpu::LDY_ABS() {
    // TODO
}

void Cpu::LDA_ABS() {
    // TODO
}

void Cpu::LDX_ABS() {
    // TODO
}

void Cpu::BCS() {
    // TODO
}

void Cpu::LDA_IND_Y() {
    // TODO
}

void Cpu::LDY_ZP_X() {
    // TODO
}

void Cpu::LDA_ZP_X() {
    // TODO
}

void Cpu::LDX_ZP_Y() {
    // TODO
}

void Cpu::CLV() {
    // TODO
}

void Cpu::LDA_ABS_Y() {
    // TODO
}

void Cpu::TSX() {
    // TODO
}

void Cpu::LDY_ABS_X() {
    // TODO
}

void Cpu::LDA_ABS_X() {
    // TODO
}

void Cpu::LDX_ABS_Y() {
    // TODO
}

void Cpu::CPY_IMM() {
    // TODO
}

void Cpu::CMP_IND_X() {
    // TODO
}

void Cpu::CPY_ZP() {
    // TODO
}

void Cpu::CMP_ZP() {
    // TODO
}

void Cpu::DEC_ZP() {
    // TODO
}

void Cpu::INY() {
    // TODO
}

void Cpu::CMP_IMM() {
    // TODO
}

void Cpu::DEX() {
    // TODO
}

void Cpu::CPY_ABS() {
    // TODO
}

void Cpu::CMP_ABS() {
    // TODO
}

void Cpu::DEC_ABS() {
    // TODO
}

void Cpu::BNE() {
    // TODO
}

void Cpu::CMP_IND_Y() {
    // TODO
}

void Cpu::CMP_ZP_X() {
    // TODO
}

void Cpu::DEC_ZP_X() {
    // TODO
}

void Cpu::CLD() {
    // TODO
}

void Cpu::CMP_ABS_Y() {
    // TODO
}

void Cpu::CMP_ABS_X() {
    // TODO
}

void Cpu::DEC_ABS_X() {
    // TODO
}

void Cpu::CPX_IMM() {
    // TODO
}

void Cpu::SBC_IND_X() {
    // TODO
}

void Cpu::CPX_ZP() {
    // TODO
}

void Cpu::SBC_ZP() {
    // TODO
}

void Cpu::INC_ZP() {
    // TODO
}

void Cpu::INX() {
    // TODO
}

void Cpu::SBC_IMM() {
    // TODO
}

void Cpu::NOP() {
    // TODO
}

void Cpu::CPX_ABS() {
    // TODO
}

void Cpu::SBC_ABS() {
    // TODO
}

void Cpu::INC_ABS() {
    // TODO
}

void Cpu::BEQ() {
    // TODO
}

void Cpu::SBC_IND_Y() {
    // TODO
}

void Cpu::SBC_ZP_X() {
    // TODO
}

void Cpu::INC_ZP_X() {
    // TODO
}

void Cpu::SED() {
    // TODO
}

void Cpu::SBC_ABS_Y() {
    // TODO
}

void Cpu::SBC_ABS_X() {
    // TODO
}

void Cpu::INC_ABS_X() {
    // TODO
}

void Cpu::UNIMP() {
    // TODO
}