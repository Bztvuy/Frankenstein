#include "ppu.h"
#include "dependencies.h"
#include "nes.h"

using namespace Frankenstein;

Ppu::Ppu(Nes& pNes) : nes(pNes){
    front = new RGBColor*[256];
    back = new RGBColor*[256];

    for (int i = 0; i < 256; ++i)
    {
        front[i] = new RGBColor[240];
        back[i] = new RGBColor[240];

        for(int j = 0; j < 240; j++) {
            front[i][j] = {0, 0, 0};
            back[i][j] = {0, 0, 0};
        }
    }

    const iNesHeader header = nes.rom.GetHeader();
    int prgRomBanks = header.prgRomBanks;
    int trainerOffset = nes.rom.GetTrainerOffset();
    int vRomBanksLocation = IRom::HeaderSize + trainerOffset + prgRomBanks * PRGROM_BANK_SIZE;

    for (int i = 0; i < 0x2000; ++i){
        chrData[i] = nes.rom.GetRaw()[vRomBanksLocation + i];
    }

    Reset();
}

void Ppu::Reset() {
    Cycle = 340;
    ScanLine = 240;
    Frame = 0;
    writeControl(0);
    writeMask(0);
    writeOAMAddress(0);
}

u8 Ppu::Read(u16 address) {
    u16 temp = address % 0x4000;
    if (temp < 0x2000){
        return chrData[temp];
    } else if (temp < 0x3F00) {
        u8 mode = CheckBit<1>(nes.rom.GetHeader().controlByte1);
        return nameTableData[MirrorAddress(mode, temp)%2048];
    } else if (temp < 0x4000) {
        return readPalette(temp % 32);
    }
    return 0;
}

void Ppu::Write(u16 address, u8 value) {
    u16 temp = address % 0x4000;
    if (temp < 0x2000) {
        chrData[temp] = value;
    } else if (temp < 0x3F00) {
        u8 mode = CheckBit<1>(nes.rom.GetHeader().controlByte1);
        nameTableData[MirrorAddress(mode, temp)%2048] = value;
    } else if (temp < 0x4000) {
        writePalette(temp%32, value);
    }
}

u16 Ppu::MirrorAddress(u8 mode, u16 address) {
    u16 temp = (address - 0x2000) % 0x1000;
    u16 table = temp / 0x0400;
    u16 offset = temp % 0x0400;
    return 0x2000 + MirrorLookup[mode][table]*0x0400 + offset;
}

u8 Ppu::readPalette(u16 address) {
    if (address >= 16 && address % 4 == 0) {
        address -= 16;
    }
    return paletteData[address];
}

void Ppu::writePalette(u16 address, u8 value) {
    if (address >= 16 && address % 4 == 0) {
        address -= 16;
    }
    paletteData[address] = value;
}

u8 Ppu::readRegister(u16 address) {
    switch (address) {
        case 0x2002:
            return readStatus();
        case 0x2004:
            return readOAMData();
        case 0x2007:
            return readData();
    }
    return 0;
}

void Ppu::writeRegister(u16 address, u8 value) {
    reg = value;
    switch (address) {
        case 0x2000:
            writeControl(value);
        break;
        case 0x2001:
            writeMask(value);
        break;
        case 0x2003:
            writeOAMAddress(value);
        break;
        case 0x2004:
            writeOAMData(value);
        break;
        case 0x2005:
            writeScroll(value);
        break;
        case 0x2006:
            writeAddress(value);
        break;
        case 0x2007:
            writeData(value);
        break;
        case 0x4014:
            writeDMA(value);
        break;
    }
}

// $2000: PPUCTRL

void Ppu::writeControl(u8 value) {
    flagNameTable = (value >> 0) & 3;
    flagIncrement = (value >> 2) & 1;
    flagSpriteTable = (value >> 3) & 1;
    flagBackgroundTable = (value >> 4) & 1;
    flagSpriteSize = (value >> 5) & 1;
    flagMasterSlave = (value >> 6) & 1;
    nmiOutput = ((value >> 7)&1) == 1;
    nmiChange();
    // t: ....BA.. ........ = d: ......BA
    t = (t & 0xF3FF) | ((u16(value) & 0x03) << 10);
}

// $2001: PPUMASK

void Ppu::writeMask(u8 value) {
    flagGrayscale = (value >> 0) & 1;
    flagShowLeftBackground = (value >> 1) & 1;
    flagShowLeftSprites = (value >> 2) & 1;
    flagShowBackground = (value >> 3) & 1;
    flagShowSprites = (value >> 4) & 1;
    flagRedTint = (value >> 5) & 1;
    flagGreenTint = (value >> 6) & 1;
    flagBlueTint = (value >> 7) & 1;
}

// $2002: PPUSTATUS

u8 Ppu::readStatus() {
    u8 result = reg & 0x1F;
    result |= flagSpriteOverflow << 5;
    result |= flagSpriteZeroHit << 6;
    if (nmiOccurred) {
        result |= 1 << 7;
    }
    nmiOccurred = false;
    nmiChange();
    // w:                   = 0
    w = 0;
    return result;
}

// $2003: OAMADDR

void Ppu::writeOAMAddress(u8 value) {
    oamAddress = value;
}

// $2004: OAMDATA (read)

u8 Ppu::readOAMData() {
    return oamData[oamAddress];
}

// $2004: OAMDATA (write)

void Ppu::writeOAMData(u8 value) {
    oamData[oamAddress] = value;
    oamAddress++;
}

// $2005: PPUSCROLL

void Ppu::writeScroll(u8 value) {
    if (w == 0) {
        // t: ........ ...HGFED = d: HGFED...
        // x:               CBA = d: .....CBA
        // w:                   = 1
        t = (t & 0xFFE0) | (u16(value) >> 3);
        x = value & 0x07;
        w = 1;
    } else {
        // t: .CBA..HG FED..... = d: HGFEDCBA
        // w:                   = 0
        t = (t & 0x8FFF) | ((u16(value) & 0x07) << 12);
        t = (t & 0xFC1F) | ((u16(value) & 0xF8) << 2);
        w = 0;
    }
}

// $2006: PPUADDR

void Ppu::writeAddress(u8 value) {
    if (w == 0) {
        // t: ..FEDCBA ........ = d: ..FEDCBA
        // t: .X...... ........ = 0
        // w:                   = 1
        t = (t & 0x80FF) | ((u16(value) & 0x3F) << 8);
        w = 1;
    } else {
        // t: ........ HGFEDCBA = d: HGFEDCBA
        // v                    = t
        // w:                   = 0
        t = (t & 0xFF00) | u16(value);
        v = t;
        w = 0;
    }
}

// $2007: PPUDATA (read)

u8 Ppu::readData() {
    u8 value = Read(v);
    // emulate buffered reads
    if (v % 0x4000 < 0x3F00) {
        u8 buffered = bufferedData;
        bufferedData = value;
        value = buffered;
    } else {
        bufferedData = Read(v - 0x1000);
    }
    // increment address
    if (flagIncrement == 0) {
        v += 1;
    } else {
        v += 32;
    }
    return value;
}

// $2007: PPUDATA (write)

void Ppu::writeData(u8 value) {
    Write(v, value);
    if (flagIncrement == 0) {
        v += 1;
    } else {
        v += 32;
    }
}

// $4014: OAMDMA

void Ppu::writeDMA(u8 value) {
    u16 address = u16(value) << 8;
    for (u16 i = 0; i < 256; i++) {
        oamData[oamAddress] = nes.ram[address];
        oamAddress++;
        address++;
    }
    /**
     * When sprite DMA ($4014) is written to, 
     * the next instruction always begins on an odd cycle. 
     * If the $4014 write is on an odd cycle, 
     * it pauses the CPU for an additional 513 cycles, otherwise 514 cycles. 
     * We can use this aspect to partially compensate for NMI's variable delay.
     */
    nes.cpu.cycles += 513;
    if (nes.cpu.cycles%2 == 1) {
         nes.cpu.cycles++;
    }
}

// NTSC Timing Helper Functions

void Ppu::incrementX() {
    // increment hori(v)
    // if coarse X == 31
    if ((v & 0x001F) == 31) {
        // coarse X = 0
        v &= 0xFFE0;
        // switch horizontal nametable
        v ^= 0x0400;
    } else {
        // increment coarse X
        v++;
    }
}

void Ppu::incrementY() {
    // increment vert(v)
    // if fine Y < 7
    if ((v & 0x7000) != 0x7000) {
        // increment fine Y
        v += 0x1000;
    } else {
        // fine Y = 0
        v &= 0x8FFF;
        // let y = coarse Y
        u16 y = (v & 0x03E0) >> 5;
        if (y == 29) {
            // coarse Y = 0
            y = 0;
            // switch vertical nametable
            v ^= 0x0800;
        } else if (y == 31) {
            // coarse Y = 0, nametable not switched
            y = 0;
        } else {
            // increment coarse Y
            y++;
        }
        // put coarse Y back into v
        v = (v & 0xFC1F) | (y << 5);
    }
}

void Ppu::copyX() {
    // hori(v) = hori(t)
    // v: .....F.. ...EDCBA = t: .....F.. ...EDCBA
    v = (v & 0xFBE0) | (t & 0x041F);
}

void Ppu::copyY() {
    // vert(v) = vert(t)
    // v: .IHGF.ED CBA..... = t: .IHGF.ED CBA.....
    v = (v & 0x841F) | (t & 0x7BE0);
}

void Ppu::nmiChange() {
    bool nmi = nmiOutput && nmiOccurred;
    if (nmi && !nmiPrevious) {
        // TODO: this fixes some games but the delay shouldn't have to be so
        // long, so the timings are off somewhere
        nmiDelay = 15;
    }
    nmiPrevious = nmi;
}

void Ppu::setVerticalBlank() {
    auto temp = front;
    front = back;
    back = temp;
    nmiOccurred = true;
    nmiChange();
}

void Ppu::clearVerticalBlank() {
    nmiOccurred = false;
    nmiChange();
}

void Ppu::fetchNameTableByte() {
    u16 address = 0x2000 | (v & 0x0FFF);
    nameTableByte = Read(address);
}

void Ppu::fetchAttributeTableByte() {
    u16 address = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
    u16 shift = ((v >> 4) & 4) | (v & 2);
    attributeTableByte = ((Read(address) >> shift) & 3) << 2;
}

void Ppu::fetchLowTileByte() {
    u16 fineY = (v >> 12) & 7;
    u8 table = flagBackgroundTable;
    u8 tile = nameTableByte;
    u16 address = 0x1000 * u16(table) + u16(tile)*16 + fineY;
    lowTileByte = Read(address);
}

void Ppu::fetchHighTileByte() {
    u16 fineY = (v >> 12) & 7;
    u8 table = flagBackgroundTable;
    u8 tile = nameTableByte;
    u16 address = 0x1000 * u16(table) + u16(tile)*16 + fineY;
    highTileByte = Read(address + 8);
}

void Ppu::storeTileData() {
    u32 data = 0;
    for (u8 i = 0; i < 8; i++) {
        u8 a = attributeTableByte;
        u8 p1 = (lowTileByte & 0x80) >> 7;
        u8 p2 = (highTileByte & 0x80) >> 6;
        lowTileByte <<= 1;
        highTileByte <<= 1;
        data <<= 4;
        data |= u32(a | p1 | p2);
    }
    tileData |= u64(data);
}

u32 Ppu::fetchTileData() {
    return u32(tileData >> 32);
}

u8 Ppu::backgroundPixel() {
    if (flagShowBackground == 0) {
        return 0;
    }
    u32 data = fetchTileData() >> ((7 - x) * 4);
    return u8(data & 0x0F);
}

Ppu::BytePair Ppu::spritePixel() {
    if (flagShowSprites == 0) {
        return BytePair{0, 0};
    }
    for (u32 i = 0; i < spriteCount; i++) {
        u32 offset = (Cycle - 1) - u32(spritePositions[i]);
        if (offset < 0 || offset > 7) {
            continue;
        }
        offset = 7 - offset;
        u8 color = u8((spritePatterns[i] >> u8(offset * 4)) & 0x0F);
        if (color % 4 == 0) {
            continue;
        }
        return BytePair{u8(i), color};
    }
    return BytePair{0, 0};
}

void Ppu::renderPixel() {
    u32 x = Cycle - 1;
    u32 y = ScanLine;
    u8 background = backgroundPixel();
    BytePair indexAndSprite = spritePixel();
    u8 i = indexAndSprite.first;
    u8 sprite = indexAndSprite.second;
    if (x < 8 && flagShowLeftBackground == 0) {
        background = 0;
    }
    if (x < 8 && flagShowLeftSprites == 0) {
        sprite = 0;
    }
    bool b = background % 4 != 0;
    bool s = sprite % 4 != 0;
    u8 color;
    if (!b && !s) {
        color = 0;
    } else if (!b && s) {
        color = sprite | 0x10;
    } else if (b && !s) {
        color = background;
    } else {
        if (spriteIndexes[i] == 0 && x < 255) {
            flagSpriteZeroHit = 1;
        }
        if (spritePriorities[i] == 0) {
            color = sprite | 0x10;
        } else {
            color = background;
        }
    }
    RGBColor c = systemPalette[readPalette(u16(color)) % 64];
    back[x][y] = c;
}

u32 Ppu::fetchSpritePattern(u8 i, u32 row) {
    u8 tile = oamData[i * 4 + 1];
    u8 attributes = oamData[i * 4 + 2];
    u16 address = 0;
    if (flagSpriteSize == 0) {
        if ((attributes & 0x80) == 0x80) {
            row = 7 - row;
        }
        u8 table = flagSpriteTable;
        address = 0x1000 * u16(table) + u16(tile)*16 + u16(row);
    } else {
        if ((attributes & 0x80) == 0x80) {
            row = 15 - row;
        }
        u8 table = tile & 1;
        tile &= 0xFE;
        if (row > 7) {
            tile++;
            row -= 8;
        }
        address = 0x1000 * u16(table) + u16(tile)*16 + u16(row);
    }
    u8 a = (attributes & 3) << 2;
    lowTileByte = Read(address);
    highTileByte = Read(address + 8);
    u32 data = 0;
    for (u8 i = 0; i < 8; i++) {
        u8 p1, p2;
        if ((attributes & 0x40) == 0x40) {
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
        data |= u32(a | p1 | p2);
    }
    return data;
}

void Ppu::evaluateSprites() {
    u32 h;
    if (flagSpriteSize == 0) {
        h = 8;
    } else {
        h = 16;
    }
    u8 count = 0;
    for (u8 i = 0; i < 64; i++) {
        u8 y = oamData[i * 4 + 0];
        u8 a = oamData[i * 4 + 2];
        u8 x = oamData[i * 4 + 3];
        u32 row = ScanLine - u32(y);
        if (row < 0 || row >= h) {
            continue;
        }
        if (count < 8) {
            spritePatterns[count] = fetchSpritePattern(i, row);
            spritePositions[count] = x;
            spritePriorities[count] = (a >> 5) & 1;
            spriteIndexes[count] = u8(i);
        }
        count++;
    }
    if (count > 8) {
        count = 8;
        flagSpriteOverflow = 1;
    }
    spriteCount = count;
}

// tick updates Cycle, ScanLine and Frame counters

void Ppu::tick() {
    if (nmiDelay > 0) {
        nmiDelay--;
        if (nmiDelay == 0 && nmiOutput && nmiOccurred) {
            nes.cpu.nmiOccurred = true;
        }
    }

    if (flagShowBackground != 0 || flagShowSprites != 0) {
        if (f == 1 && ScanLine == 261 && Cycle == 339) {
            Cycle = 0;
            ScanLine = 0;
            Frame++;
            f ^= 1;
            return;
        }
    }
    Cycle++;
    if (Cycle > 340) {
        Cycle = 0;
        ScanLine++;
        if (ScanLine > 261) {
            ScanLine = 0;
            Frame++;
            f ^= 1;
        }
    }
}

// Step executes a single PPU cycle

void Ppu::Step() {
    tick();

    bool renderingEnabled = flagShowBackground != 0 || flagShowSprites != 0;
    bool preLine = ScanLine == 261;
    bool visibleLine = ScanLine < 240;
    // postLine := ScanLine == 240
    bool renderLine = preLine || visibleLine;
    bool preFetchCycle = Cycle >= 321 && Cycle <= 336;
    bool visibleCycle = Cycle >= 1 && Cycle <= 256;
    bool fetchCycle = preFetchCycle || visibleCycle;

    // background logic
    if (renderingEnabled) {
        if (visibleLine && visibleCycle) {
            renderPixel();
        }
        if (renderLine && fetchCycle) {
            tileData <<= 4;
            switch (Cycle % 8) {
                case 1:
                    fetchNameTableByte();
            break;
                case 3:
                    fetchAttributeTableByte();
            break;
                case 5:
                    fetchLowTileByte();
            break;
                case 7:
                    fetchHighTileByte();
            break;
                case 0:
                    storeTileData();
            break;
            }
        }
        if (preLine && Cycle >= 280 && Cycle <= 304) {
            copyY();
        }
        if (renderLine) {
            if (fetchCycle && Cycle % 8 == 0) {
                incrementX();
            }
            if (Cycle == 256) {
                incrementY();
            }
            if (Cycle == 257) {
                copyX();
            }
        }
    }

    // sprite logic
    if (renderingEnabled) {
        if (Cycle == 257) {
            if (visibleLine) {
                evaluateSprites();
            } else {
                spriteCount = 0;
            }
        }
    }

    // vblank logic
    if (ScanLine == 241 && Cycle == 1) {
        setVerticalBlank();
    }
    if (preLine && Cycle == 1) {
        clearVerticalBlank();
        flagSpriteZeroHit = 0;
        flagSpriteOverflow = 0;
    }
}

