#include "ppu.h"
#include "dependencies.h"

Ppu::Registers::Registers(Cpu *const cpu) :
    controlRegister(cpu->Memory(0x2000)),
    maskRegister(cpu->Memory(0x2001)),
    processorStatus(cpu->Memory(0x2002)),
    oamAddress(cpu->Memory(0x2003)),
    oamData(cpu->Memory(0x2004)),
    vramAddress1(cpu->Memory(0x2005)),
    vramAddress2(cpu->Memory(0x2006)),
    vramIO(cpu->Memory(0x2007)),
    spriteDma(cpu->Memory(0x4014)) {}

Ppu::Ppu(const Rom* rom, Cpu *const cpu) : registers(cpu)
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
