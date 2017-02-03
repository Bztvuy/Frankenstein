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

void Cpu::BRK() {
}

/// Logical OR 
/// 2 bytes; 6 cycles

void Cpu::ORA_IND_X() {
    auto operand = this->memory->raw[this->registers->PC + 1];
    auto value = this->memory->raw[PreIndexedIndirect(operand, this->registers->X)];
    this->registers->A |= value;
}

void Cpu::ORA_ZP() {
    auto operand = this->memory->raw[this->registers->PC + 1];
    auto value = this->memory->raw[ZeroPage(operand)];
    this->registers->A |= value;
}

void Cpu::ASL_ZP() {
    auto operand = this->memory->raw[this->registers->PC + 1];
    auto address = ZeroPage(operand);
    auto value = this->memory->raw[address];
    SetFlag(this->flags->carry, CHECK_BIT(value, 7));
    this->memory->raw[address] <<= 1;
}

void Cpu::PHP() {
}

void Cpu::ORA_IMM() {
    auto operand = this->memory->raw[this->registers->PC + 1];
    auto value = Immediate(operand);
    this->registers->A |= value;
}

void Cpu::ASL_ACC() {
    auto value = this->registers->A;
    SetFlag(this->flags->carry, CHECK_BIT(value, 7));
    this->registers->A <<= 1;
}

void Cpu::ORA_ABS() {
    auto low = this->memory->raw[this->registers->PC + 1];
    auto high = this->memory->raw[this->registers->PC + 2];
    auto value = this->memory->raw[Absolute(low, high)];
    this->registers->A |= value;
}

void Cpu::ASL_ABS() {
    auto low = this->memory->raw[this->registers->PC + 1];
    auto high = this->memory->raw[this->registers->PC + 2];
    auto address = Absolute(low, high);
    auto value = this->memory->raw[address];
    SetFlag(this->flags->carry, CHECK_BIT(value, 7));
    this->memory->raw[address] <<= 1;
}

void Cpu::BPL() {
}

void Cpu::ORA_IND_Y() {
    auto operand = this->memory->raw[this->registers->PC + 1];
    auto value = this->memory->raw[PostIndexedIndirect(operand, this->registers->Y)];
    this->registers->A |= value;
}

void Cpu::ORA_ZP_X() {
    auto operand = this->memory->raw[this->registers->PC + 1];
    auto value = this->memory->raw[ZeroPageIndexed(operand, this->registers->X)];
    this->registers->A |= value;
}

void Cpu::ASL_ZP_X() {
    auto operand = this->memory->raw[this->registers->PC + 1];
    auto address = ZeroPageIndexed(operand, this->registers->X);
    auto value = this->memory->raw[address];
    SetFlag(this->flags->carry, CHECK_BIT(value, 7));
    this->memory->raw[address] <<= 1;
}

void Cpu::CLC() {
}

void Cpu::ORA_ABS_Y() {
    auto low = this->memory->raw[this->registers->PC + 1];
    auto high = this->memory->raw[this->registers->PC + 2];
    auto value = this->memory->raw[Indexed(low, high, this->registers->Y)];
    this->registers->A |= value;
}

void Cpu::ORA_ABS_X() {
    auto low = this->memory->raw[this->registers->PC + 1];
    auto high = this->memory->raw[this->registers->PC + 2];
    auto value = this->memory->raw[Indexed(low, high, this->registers->X)];
    this->registers->A |= value;
}

void Cpu::ASL_ABS_X() {
    auto low = this->memory->raw[this->registers->PC + 1];
    auto high = this->memory->raw[this->registers->PC + 2];
    auto address = Indexed(low, high, this->registers->X);
    auto value = this->memory->raw[address];
    SetFlag(this->flags->carry, CHECK_BIT(value, 7));
    this->memory->raw[address] <<= 1;
}

void Cpu::JSR() {
}

void Cpu::AND_IND_X() {
}

void Cpu::BIT_ZP() {
}

void Cpu::AND_ZP() {
}

void Cpu::ROL_ZP() {
}

void Cpu::PLP() {
}

void Cpu::AND_IMM() {
}

void Cpu::ROL_ACC() {
}

void Cpu::BIT_ABS() {
}

void Cpu::AND_ABS() {
}

void Cpu::ROL_ABS() {
}

void Cpu::BMI() {
}

void Cpu::AND_IND_Y() {
}

void Cpu::AND_ZP_X() {
}

void Cpu::ROL_ZP_X() {
}

void Cpu::SEC() {
}

void Cpu::AND_ABS_Y() {
}

void Cpu::AND_ABS_X() {
}

void Cpu::ROL_ABS_X() {
}

void Cpu::RTI() {
}

void Cpu::EOR_IND_X() {
}

void Cpu::EOR_ZP() {
}

void Cpu::LSR_ZP() {
}

void Cpu::PHA() {
}

void Cpu::EOR_IMM() {
}

void Cpu::LSR_ACC() {
}

void Cpu::JMP_ABS() {
}

void Cpu::EOR_ABS() {
}

void Cpu::LSR_ABS() {
}

void Cpu::BVC() {
}

void Cpu::EOR_IND_Y() {
}

void Cpu::EOR_ZP_X() {
}

void Cpu::LSR_ZP_X() {
}

void Cpu::CLI() {
}

void Cpu::EOR_ABS_Y() {
}

void Cpu::EOR_ABS_X() {
}

void Cpu::LSR_ABS_X() {
}

void Cpu::RTS() {
}

void Cpu::ADC_IND_X() {
}

void Cpu::ADC_ZP() {
}

void Cpu::ROR_ZP() {
}

void Cpu::PLA() {
}

void Cpu::ADC_IMM() {
}

void Cpu::ROR_ACC() {
}

void Cpu::JMP_IND() {
}

void Cpu::ADC_ABS() {
}

void Cpu::ROR_ABS() {
}

void Cpu::BVS() {
}

void Cpu::ADC_IND_Y() {
}

void Cpu::ADC_ZP_X() {
}

void Cpu::ROR_ZP_X() {
}

void Cpu::SEI() {
}

void Cpu::ADC_ABS_Y() {
}

void Cpu::ADC_ABS_X() {
}

void Cpu::ROR_ABS_X() {
}

void Cpu::STA_IND_X() {
}

void Cpu::STY_ZP() {
}

void Cpu::STA_ZP() {
}

void Cpu::STX_ZP() {
}

void Cpu::DEY() {
}

void Cpu::TXA() {
}

void Cpu::STY_ABS() {
}

void Cpu::STA_ABS() {
}

void Cpu::STX_ABS() {
}

void Cpu::BCC() {
}

void Cpu::STA_IND_Y() {
}

void Cpu::STY_ZP_X() {
}

void Cpu::STA_ZP_X() {
}

void Cpu::STX_ZP_Y() {
}

void Cpu::TYA() {
}

void Cpu::STA_ABS_Y() {
}

void Cpu::TXS() {
}

void Cpu::STA_ABS_X() {
}

void Cpu::LDY_IMM() {
}

void Cpu::LDA_IND_X() {
}

void Cpu::LDX_IMM() {
}

void Cpu::LDY_ZP() {
}

void Cpu::LDA_ZP() {
}

void Cpu::LDX_ZP() {
}

void Cpu::TAY() {
}

void Cpu::LDA_IMM() {
}

void Cpu::TAX() {
}

void Cpu::LDY_ABS() {
}

void Cpu::LDA_ABS() {
}

void Cpu::LDX_ABS() {
}

void Cpu::BCS() {
}

void Cpu::LDA_IND_Y() {
}

void Cpu::LDY_ZP_X() {
}

void Cpu::LDA_ZP_X() {
}

void Cpu::LDX_ZP_Y() {
}

void Cpu::CLV() {
}

void Cpu::LDA_ABS_Y() {
}

void Cpu::TSX() {
}

void Cpu::LDY_ABS_X() {
}

void Cpu::LDA_ABS_X() {
}

void Cpu::LDX_ABS_Y() {
}

void Cpu::CPY_IMM() {
}

void Cpu::CMP_IND_X() {
}

void Cpu::CPY_ZP() {
}

void Cpu::CMP_ZP() {
}

void Cpu::DEC_ZP() {
}

void Cpu::INY() {
}

void Cpu::CMP_IMM() {
}

void Cpu::DEX() {
}

void Cpu::CPY_ABS() {
}

void Cpu::CMP_ABS() {
}

void Cpu::DEC_ABS() {
}

void Cpu::BNE() {
}

void Cpu::CMP_IND_Y() {
}

void Cpu::CMP_ZP_X() {
}

void Cpu::DEC_ZP_X() {
}

void Cpu::CLD() {
}

void Cpu::CMP_ABS_Y() {
}

void Cpu::CMP_ABS_X() {
}

void Cpu::DEC_ABS_X() {
}

void Cpu::CPX_IMM() {
}

void Cpu::SBC_IND_X() {
}

void Cpu::CPX_ZP() {
}

void Cpu::SBC_ZP() {
}

void Cpu::INC_ZP() {
}

void Cpu::INX() {
}

void Cpu::SBC_IMM() {
}

void Cpu::NOP() {
}

void Cpu::CPX_ABS() {
}

void Cpu::SBC_ABS() {
}

void Cpu::INC_ABS() {
}

void Cpu::BEQ() {
}

void Cpu::SBC_IND_Y() {
}

void Cpu::SBC_ZP_X() {
}

void Cpu::INC_ZP_X() {
}

void Cpu::SED() {
}

void Cpu::SBC_ABS_Y() {
}

void Cpu::SBC_ABS_X() {
}

void Cpu::INC_ABS_X() {
}

void Cpu::UNIMP() {
}

void Cpu::SetFlag(u8 flag, u8 value){
    ASSIGN_BIT(this->registers->P, flag, value);
}