#include "ppu.h"
#include <circle/util.h>

Ppu::Ppu(const Rom* rom)
{
    const iNesHeader* header = rom->GetHeader();
    int prgRomBanks = header->prgRomBanks;
    int vRomBanks = header->vRomBanks;
    int trainerOffset = rom->GetTrainerOffset();
    int vRomBanksLocation = rom->headerSize + trainerOffset + prgRomBanks * PRGROM_BANK_SIZE;

    switch (vRomBanks) {
        case 1:
            memcpy(this->memory.patternTable0, rom->GetRaw() + vRomBanksLocation, VROM_BANK_SIZE);
            break;
        default: //TODO: implement multiple V-ROM banks
            break;
    }
}
