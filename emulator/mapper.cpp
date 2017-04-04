#include "mapper.h"

using namespace Frankenstein;

/**********************************************/
/***************** MAPPER 1 *******************/
/**********************************************/

u8 Mapper1::Read(u16 address)
{
    if (address < 0x2000) {
        u16 bank = address / 0x1000;
        u16 offset = address % 0x1000;
        return rom.GetCHR()[chrOffsets[bank] + offset];
    } else if (address >= 0x8000) {
        address -= 0x8000;
        u16 bank = address / 0x4000;
        u16 offset = address % 0x4000;
        return rom.GetPRG()[prgOffsets[bank] + offset];
    } else if (address >= 0x6000) {
        return rom.GetSRAM()[address - 0x6000];
    }
    return 0;
}

void Mapper1::Write(u16 address, u8 value)
{
    if (address < 0x2000) {
        u16 bank = address / 0x1000;
        u16 offset = address % 0x1000;
        rom.GetCHR()[chrOffsets[bank] + offset] = value;
    } else if (address >= 0x8000) {
        loadRegister(address, value);
    } else if (address >= 0x6000) {
        rom.GetSRAM()[address - 0x6000] = value;
    }
}

void Mapper1::Step()
{
    //do nothing
}

void Mapper1::loadRegister(u16 address, u8 value)
{
    if ((value & 0x80) == 0x80) {
        shiftRegister = 0x10;
        writeControl(control | 0x0C);
    } else {
        bool complete = (shiftRegister & 1) == 1;
        shiftRegister >>= 1;
        shiftRegister |= (value & 1) << 4;
        if (complete) {
            writeRegister(address, shiftRegister);
            shiftRegister = 0x10;
        }
    }
}

void Mapper1::writeRegister(u16 address, u8 value)
{
    if (address <= 0x9FFF) {
        writeControl(value);
    } else if (address <= 0xBFFF) {
        writeCHRBank0(value);
    } else if (address <= 0xDFFF) {
        writeCHRBank1(value);
    } else if (address <= 0xFFFF) {
        writePRGBank(value);
    }
}

// Control (internal, $8000-$9FFF)
void Mapper1::writeControl(u8 value)
{
    control = value;
    chrMode = (value >> 4) & 1;
    prgMode = (value >> 2) & 3;
    u8 mirror = value & 3;
    switch (mirror) {
    case 0:
        mirrorMode = MirrorSingle0;
        break;
    case 1:
        mirrorMode = MirrorSingle1;
        break;
    case 2:
        mirrorMode = MirrorVertical;
        break;
    case 3:
        mirrorMode = MirrorHorizontal;
        break;
    }
    updateOffsets();
}

// CHR bank 0 (internal, $A000-$BFFF)
void Mapper1::writeCHRBank0(u8 value)
{
    chrBank0 = value;
    updateOffsets();
}

// CHR bank 1 (internal, $C000-$DFFF)
void Mapper1::writeCHRBank1(u8 value)
{
    chrBank1 = value;
    updateOffsets();
}

// PRG bank (internal, $E000-$FFFF)
void Mapper1::writePRGBank(u8 value)
{
    prgBank = value & 0x0F;
    updateOffsets();
}

s32 Mapper1::prgBankOffset(s32 index)
{
    if (index >= 0x80) {
        index -= 0x100;
    }
    u32 PRGSize = rom.GetHeader().prgRomBanks * PRGROM_BANK_SIZE;
    index %= PRGSize / 0x4000;
    s32 offset = index * 0x4000;
    if (offset < 0) {
        offset += PRGSize;
    }
    return offset;
}

s32 Mapper1::chrBankOffset(s32 index)
{
    if (index >= 0x80) {
        index -= 0x100;
    }
    u32 CHRSize = rom.GetHeader().vRomBanks * VROM_BANK_SIZE;
    index %= CHRSize / 0x1000;
    s32 offset = index * 0x1000;
    if (offset < 0) {
        offset += CHRSize;
    }
    return offset;
}

// PRG ROM bank mode (0, 1: switch 32 KB at $8000, ignoring low bit of bank number;
//                    2: fix first bank at $8000 and switch 16 KB bank at $C000;
//                    3: fix last bank at $C000 and switch 16 KB bank at $8000)
// CHR ROM bank mode (0: switch 8 KB at a time; 1: switch two separate 4 KB banks)
void Mapper1::updateOffsets()
{
    switch (prgMode) {
    case 0:
    case 1:
        prgOffsets[0] = prgBankOffset(prgBank & 0xFE);
        prgOffsets[1] = prgBankOffset(prgBank | 0x01);
        break;
    case 2:
        prgOffsets[0] = 0;
        prgOffsets[1] = prgBankOffset(prgBank);
        break;
    case 3:
        prgOffsets[0] = prgBankOffset(prgBank);
        prgOffsets[1] = prgBankOffset(-1);
        break;
    }
    switch (chrMode) {
    case 0:
        chrOffsets[0] = chrBankOffset(chrBank0 & 0xFE);
        chrOffsets[1] = chrBankOffset(chrBank0 | 0x01);
        break;
    case 1:
        chrOffsets[0] = chrBankOffset(chrBank0);
        chrOffsets[1] = chrBankOffset(chrBank1);
        break;
    }
}

Mapper1::Mapper1(Rom& pRom)
    : rom(pRom)
{
    shiftRegister = 0x10;
    prgOffsets[1] = prgBankOffset(-1);
}

Mapper1::~Mapper1() {}

/**********************************************/
/***************** MAPPER 2 *******************/
/**********************************************/

u8 Mapper2::Read(u16 address)
{
    u16 index;
    if (address < 0x2000) {
        return rom.GetCHR()[address];
    } else if (address >= 0xC000) {
        index = prgBank2 * 0x4000 + (address - 0xC000);
        return rom.GetPRG()[index];
    } else if (address >= 0x8000) {
        index = prgBank1 * 0x4000 + (address - 0x8000);
        return rom.GetPRG()[index];
    } else if (address >= 0x6000) {
        index = address - 0x6000;
        return rom.GetSRAM()[index];
    } else {
        return 0;
    }
}

void Mapper2::Write(u16 address, u8 value)
{
    u16 index;
    if (address < 0x2000) {
        rom.GetCHR()[address] = value;
    } else if (address >= 0x8000) {
        prgBank1 = value % prgBanks;
    } else if (address >= 0x6000) {
        index = address - 0x6000;
        rom.GetSRAM()[index] = value;
    }
}

void Mapper2::Step()
{
    //nothing to do
}

Mapper2::Mapper2(Rom& pRom)
    : rom(pRom)
{
    prgBanks = rom.GetHeader().prgRomBanks;
    prgBank1 = 0;
    prgBank2 = prgBanks - 1;
}

Mapper2::~Mapper2() {}

/**********************************************/
/***************** MAPPER 3 *******************/
/**********************************************/

u8 Mapper3::Read(u16 address)
{
    return 0;
}

void Mapper3::Write(u16 address, u8 value)
{
}

void Mapper3::Step()
{
}

Mapper3::~Mapper3() {}

/**********************************************/
/***************** MAPPER 4 *******************/
/**********************************************/

u8 Mapper4::Read(u16 address)
{
    return 0;
}

void Mapper4::Write(u16 address, u8 value)
{
}

void Mapper4::Step()
{
}

Mapper4::~Mapper4() {}

/**********************************************/
/***************** MAPPER 7 *******************/
/**********************************************/

u8 Mapper7::Read(u16 address)
{
    return 0;
}

void Mapper7::Write(u16 address, u8 value)
{
}

void Mapper7::Step()
{
}

Mapper7::~Mapper7() {}
