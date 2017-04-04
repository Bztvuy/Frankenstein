#include "rom.h"

using namespace Frankenstein;

Rom::Rom(u8 const* const raw, u64 size)
    : raw(raw)
    , length(size)
{
    this->header = MakeHeader();
    this->PRG = MakePRG();
    this->CHR = MakeCHR();
    this->SRAM = MakeSRAM();
}

const iNesHeader Rom::GetHeader() const {
    return this->header;
}

const u8* const Rom::GetRaw() const {
    return this->raw;
};

u32 Rom::GetLength() const {
    return this->length;
}

u8* Rom::GetPRG() const {
    return this->PRG;
}

u8* Rom::GetCHR() const {
    return this->CHR;
}

u8* Rom::GetSRAM() const {
    return this->SRAM;
}

u8* Rom::MakePRG() const
{
    iNesHeader header = GetHeader();
    u8 prgRomBanks = header.prgRomBanks;
    u32 trainerOffset = GetTrainerOffset();
    u32 prgRomBanksLocation = Rom::HeaderSize + trainerOffset;
    u8* PRG = new u8[PRGROM_BANK_SIZE * prgRomBanks];
    for (u32 i = 0; i < PRGROM_BANK_SIZE * prgRomBanks; ++i) {
        PRG[i] = GetRaw()[i + prgRomBanksLocation];
    }
    return PRG;
}

u8* Rom::MakeCHR() const
{
    iNesHeader header = GetHeader();
    u8 prgRomBanks = header.prgRomBanks;
    u8 vRomBanks = header.vRomBanks;
    u32 trainerOffset = GetTrainerOffset();
    u32 vRomBanksLocation = Rom::HeaderSize + trainerOffset + prgRomBanks * PRGROM_BANK_SIZE;
    u8* CHR = new u8[VROM_BANK_SIZE * vRomBanks];
    for (u32 i = 0; i < VROM_BANK_SIZE * vRomBanks; ++i) {
        CHR[i] = GetRaw()[i + vRomBanksLocation];
    }
    return CHR;
}

u8* Rom::MakeSRAM() const
{
    return new u8[0x2000];
}

iNesHeader Rom::MakeHeader() const
{
    iNesHeader header;
    u8 const* const raw = this->GetRaw();
    for (u32 i = 0; i < sizeof(iNesHeader); ++i) {
        header.raw[i] = raw[i];
    }
    return header;
}

u32 Rom::GetTrainerOffset() const
{
    return CheckBit<3>(this->GetHeader().controlByte1) * Rom::TrainerSize;
}
