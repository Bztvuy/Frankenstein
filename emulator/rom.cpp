#include "rom.h"

using namespace Frankenstein;

u8* IRom::MakePRG() const {
    iNesHeader header = GetHeader();
    u8 prgRomBanks = header.prgRomBanks;
    u32 trainerOffset = GetTrainerOffset();
    u32 prgRomBanksLocation = IRom::HeaderSize + trainerOffset;
    u8* PRG = new u8[PRGROM_BANK_SIZE * prgRomBanks];
    for(u32 i = 0; i < PRGROM_BANK_SIZE * prgRomBanks; ++i) {
	PRG[i] = GetRaw()[i + prgRomBanksLocation];
    }
    return PRG;
}

u8* IRom::MakeCHR() const {
    iNesHeader header = GetHeader();
    u8 prgRomBanks = header.prgRomBanks;
    u8 vRomBanks = header.vRomBanks;
    u32 trainerOffset = GetTrainerOffset();
    u32 vRomBanksLocation = IRom::HeaderSize + trainerOffset + prgRomBanks * PRGROM_BANK_SIZE;
    u8* CHR = new u8[VROM_BANK_SIZE * vRomBanks];
    for(u32 i = 0; i < VROM_BANK_SIZE * vRomBanks; ++i) {
	CHR[i] = GetRaw()[i + vRomBanksLocation];
    }
    return CHR;
}

u8* IRom::MakeSRAM() const {
    return new u8[0x2000];
}

const iNesHeader IRom::MakeHeader() const {
    iNesHeader header;
    u8 const *const raw = this->GetRaw();
    for (u32 i = 0; i < sizeof(iNesHeader); ++i){
        header.raw[i] = raw[i];
    }
    return header;
}

u32 IRom::GetTrainerOffset() const {
    return CheckBit<3>(this->GetHeader().controlByte1) * IRom::TrainerSize;
}
