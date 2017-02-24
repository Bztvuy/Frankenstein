#include "ppu.h"
#include "dependencies.h"

using namespace Frankenstein;

Ppu::Registers::Registers(Memory& ram) :
    controlRegister(ram[0x2000]),
    maskRegister(ram[0x2001]),
    processorStatus(ram[0x2002]),
    oamAddress(ram[0x2003]),
    oamData(ram[0x2004]),
    vramAddress1(ram[0x2005]),
    vramAddress2(ram[0x2006]),
    vramIO(ram[0x2007]),
    spriteDma(ram[0x4014]) {}

Ppu::Ppu(Memory& ram, Rom& rom) : registers(ram)
{
    const iNesHeader* header = rom.GetHeader();
    int prgRomBanks = header->prgRomBanks;
    int vRomBanks = header->vRomBanks;
    int trainerOffset = rom.GetTrainerOffset();
    int vRomBanksLocation = rom.headerSize + trainerOffset + prgRomBanks * PRGROM_BANK_SIZE;

    switch (vRomBanks) {
        case 1:
            memcpy(this->memory.patternTable0, rom.GetRaw() + vRomBanksLocation, VROM_BANK_SIZE);
            break;
        default: //TODO: implement multiple V-ROM banks
            break;
    }

    cycle = 0;
    scanline = 0;
    frame = 0;
}

void Ppu::Reset(){
    cycle = 340;
    scanline = 240;
    frame = 0;
    registers.controlRegister = 0;
    registers.maskRegister = 0;
    registers.oamAddress = 0;
}
