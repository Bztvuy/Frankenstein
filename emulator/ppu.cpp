#include "ppu.h"
#include "dependencies.h"

Ppu::Ppu(const Rom* rom, const Cpu* cpu)
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
    
    registers.controlRegister = cpu->Memory(0x2000);
    registers.maskRegister = cpu->Memory(0x2001);
    registers.processorStatus = cpu->Memory(0x2002);
    registers.oamAddress = cpu->Memory(0x2003);
    registers.oamData = cpu->Memory(0x2004);
    registers.vramAddress1 = cpu->Memory(0x2005);
    registers.vramAddress2 = cpu->Memory(0x2006);
    registers.vramIO = cpu->Memory(0x2007);
    registers.spriteDma = cpu->Memory(0x4014);
}

Ppu::Reset(){
    cycle = 340;
    scanline = 240;
    frame = 0;
    registers.controlRegister = 0;
    registers.maskRegister = 0;
    registers.oamAddress = 0;
}