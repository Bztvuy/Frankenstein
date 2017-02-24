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

void Ppu::EvaluateSprites(){
    u8 h;
    if (!Get<ControlFlags::SpriteSize>()) {
            h = 8;
    } else {
            h = 16;
    }
    u8 count = 0;
    for (u8 i = 0; i < 64; i++) {
        Sprite sprite = this->primaryOAM[i];
            u8 y = sprite.Y;
            u8 a = sprite.attributes;
            u8 x = sprite.X;
            u8 row = this->scanline - y;
            if (row < 0 || row >= h) {
                    continue;
            }
            if (count < 8) {
                    spritePatterns[count] = FetchSpritePattern(i, row);
                    spritePositions[count] = x;
                    spritePriorities[count] = (a >> 5) & 1;
                    spriteIndexes[count] = i;
            }
            ++count;
    }
    if (count > 8) {
            count = 8;
            Set<StatusFlags::ScanlineSpriteCount>(true);
    }
    this->sprite = count;
}

u32 Ppu::FetchSpritePattern(u16 x, u16 y){
    u8 tile = this->primaryOAM[x].index;
    u8 attributes = this->primaryOAM[x].attributes;
    u16 address;
    if (!Get<ControlFlags::SpriteSize>()) {
        
            if (this->primaryOAM[x].Get<SpriteFlags::FlipVertical>()) {
                    y = 7 - y;
            }
            bool table = Get<ControlFlags::SpriteTable>();
            address = (0x1000 * table) + (tile * 16) + y;
    } else {
            if (this->primaryOAM[x].Get<SpriteFlags::FlipVertical>()) {
                    y = 15 - y;
            }
            bool table = tile & 1;
            tile &= 0xFE;
            if (y > 7) {
                    tile++;
                    y -= 8;
            }
            address = (0x1000 * table) + (tile * 16) + y;
    }
    u8 a = (attributes & 3) << 2;
    u8 lowTileByte = this->memory.raw[address];
    u8 highTileByte = this->memory.raw[address+8];
    u32 data = 0;
    for (u8 i = 0; i < 8; i++) {
            u8 p1, p2;
            if (this->primaryOAM[x].Get<SpriteFlags::FlipHorizontal>()) {
                    p1 = (lowTileByte & 1) << 0;
                    p2 = (highTileByte & 1) << 1;
                    lowTileByte >>= 1;
                    highTileByte >>= 1;
            } else {
                    p1 = (lowTileByte & 0x80) >> 7;
                    p2 = (highTileByte & 0x80) >> 6;
                    lowTileByte <<= 1;
                    highTileByte <<= 1;
            }
            data <<= 4;
            data |= a | p1 | p2;
    }
    return data;
}

