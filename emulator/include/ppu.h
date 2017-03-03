#ifndef PPU_H
#define PPU_H

#include "rom.h"
#include "cpu.h"

namespace Frankenstein {
class Ppu
{
public:
    struct BytePair{
        u8 first;
        u8 second;
    };
    enum ControlFlags {
        LowerNameTable,         //Name table address, changes between the four name tables at 0x2000 (0), 0x2400 (1), 0x2800 (2) and 0x2C00 (3).
        UpperNameTable,         
        Increment,              //Specifies amount to increment address by, either 1 if this is 0 or 32 if this is 1.
        SpriteTable,            //Identifies which pattern table sprites are stored in, either 0x0000 (0) or 0x1000 (1). 
        BackgroundTable,        //Identifies which pattern table the background is stored in, either 0x0000 (0) or 0x1000 (1). 
        SpriteSize,             //Specifies the size of sprites in pixels, 8x8 if this is 0, otherwise 8x16.
        SlaveMode,              //Changes PPU between master and slave modes.  This is not used by the NES.
        NMI                     //Indicates whether a NMI should occur upon V-Blank. 
    };
    
    enum MaskFlags {
        Monochrome,             //Indicates whether the system is in colour (0) or monochrome mode (1)
        ClipBackground,         //Specifies whether to clip the background, that is whether to hide the background in the left 8 pixels on screen (0) or to show them (1).
        ClipSprites,            //Specifies whether to clip the sprites, that is whether to hide sprites in the left 8 pixels on screen (0) or to show them (1).
        ShowBackground,         //If this is 0, the background should not be displayed.
        ShowSprites,            //If this is 0, sprites should not be displayed. 
        RedTint,                //Indicates background colour in monochrome mode or colour intensity in colour mode. 
        GreenTint,              
        BlueTint                
    };
    
    enum StatusFlags {
        IgnoreWrite = 4,            //If set, indicates that writes to VRAM should be ignored. 
        ScanlineSpriteCount = 5,    //Scanline sprite count, if set, indicates more than 8 sprites on the current scanline.
        SpriteZeroHit = 6,          //Sprite 0 hit flag, set when a non-transparent pixel of sprite 0 overlaps a non-transparent background pixel.
        VBlank = 7                  //Indicates whether V-Blank is occurring.
    };

    enum class SpriteFlags {
        LowerColor,             //Most significant two bits of the color. 
        UpperColor,
        Priority = 5,           //Indicates whether this sprite has priority over the background. 
        FlipHorizontal = 6,     //Indicates whether to flip the sprite horizontally. 
        FlipVertical = 7        //Indicates whether to flip the sprite vertically.
    };
    
    union Tile {
        struct {
            u8 low[8];
            u8 high[8];
        };
        u8 raw[16];
    };
    
    union MemoryMap {
        struct {
            union {
                u8 patternTable0[0x1000];
                Tile tiles0[256];
            };
            union {
                u8 patternTable1[0x1000];
                Tile tiles1[256];
            };
            u8 nameTable0[0x03C0];
            u8 attributeTable0[0x0040];
            u8 nameTable1[0x03C0];
            u8 attributeTable1[0x0040];
            u8 nameTable2[0x03C0];
            u8 attributeTable2[0x0040];
            u8 nameTable3[0x03C0];
            u8 attributeTable3[0x0040];
            u8 mirrors1[0x0F00];
            u8 imagePalette[0x0010];
            u8 spritePalette[0x0010];
            u8 mirrors2[0x00E0];
            u8 mirrors3[0xC000];
        };
        u8 raw[0x10000];
    };

    struct Registers {
        u8& controlRegister;    //0x2000
        u8& maskRegister;       //0x2001
        u8& processorStatus;    //0x2002
        u8& oamAddress;         //0x2003
        u8& oamData;            //0x2004
        u8& vramAddress1;       //0x2005
        u8& vramAddress2;       //0x2006
        u8& vramIO;             //0x2007
        u8& spriteDma;          //0x4014

        Registers(Memory& cpu);
    };
    
    union Sprite {
        struct {
            u8 Y;
            u8 index;
            u8 attributes;
            u8 X;
        };
        u8 raw[4];
        
        template<typename Ppu::SpriteFlags f>
        void Set(bool value){
            AssignBit<static_cast<int>(f)>(this->attributes, value);
        }

        template<typename Ppu::SpriteFlags f>
        bool Get(){
            return CheckBit<static_cast<int>(f)>(this->attributes);
        }
    };

    struct RGBColor{
        u8 red;
        u8 green;
        u8 blue;
    };

    const RGBColor systemPalette[0x40] = {
            {0x80,0x80,0x80},
            {0x00,0x3D,0xA6},
            {0x00,0x12,0xB0},
            {0x44,0x00,0x96},
            {0xA1,0x00,0x5E},
            {0xC7,0x00,0x28},
            {0xBA,0x06,0x00},
            {0x8C,0x17,0x00},
            {0x5C,0x2F,0x00},
            {0x10,0x45,0x00},
            {0x05,0x4A,0x00},
            {0x00,0x47,0x2E},
            {0x00,0x41,0x66},
            {0x00,0x00,0x00},
            {0x05,0x05,0x05},
            {0x05,0x05,0x05},
            {0xC7,0xC7,0xC7},
            {0x00,0x77,0xFF},
            {0x21,0x55,0xFF},
            {0x82,0x37,0xFA},
            {0xEB,0x2F,0xB5},
            {0xFF,0x29,0x50},
            {0xFF,0x22,0x00},
            {0xD6,0x32,0x00},
            {0xC4,0x62,0x00},
            {0x35,0x80,0x00},
            {0x05,0x8F,0x00},
            {0x00,0x8A,0x55},
            {0x00,0x99,0xCC},
            {0x21,0x21,0x21},
            {0x09,0x09,0x09},
            {0x09,0x09,0x09},
            {0xFF,0xFF,0xFF},
            {0x0F,0xD7,0xFF},
            {0x69,0xA2,0xFF},
            {0xD4,0x80,0xFF},
            {0xFF,0x45,0xF3},
            {0xFF,0x61,0x8B},
            {0xFF,0x88,0x33},
            {0xFF,0x9C,0x12},
            {0xFA,0xBC,0x20},
            {0x9F,0xE3,0x0E},
            {0x2B,0xF0,0x35},
            {0x0C,0xF0,0xA4},
            {0x05,0xFB,0xFF},
            {0x5E,0x5E,0x5E},
            {0x0D,0x0D,0x0D},
            {0x0D,0x0D,0x0D},
            {0xFF,0xFF,0xFF},
            {0xA6,0xFC,0xFF},
            {0xB3,0xEC,0xFF},
            {0xDA,0xAB,0xEB},
            {0xFF,0xA8,0xF9},
            {0xFF,0xAB,0xB3},
            {0xFF,0xD2,0xB0},
            {0xFF,0xEF,0xA6},
            {0xFF,0xF7,0x9C},
            {0xD7,0xE8,0x95},
            {0xA6,0xED,0xAF},
            {0xA2,0xF2,0xDA},
            {0x99,0xFF,0xFC},
            {0xDD,0xDD,0xDD},
            {0x11,0x11,0x11},
            {0x11,0x11,0x11}
    };
    
    Registers registers;
    MemoryMap memory;

    RGBColor front[256][240];
    RGBColor back[256][240];

    Sprite primaryOAM[64];
    Sprite secondaryOAM[8];
    
    u32 Cycle;      // 0-340
    u32 ScanLine;   // 0-261, 0-239=visible, 240=post, 241-260=vblank, 261=pre
    u64 Frame;      // frame counter

    // storage variables
    u8 paletteData[32];
    u8 nameTableData[2048];
    u8 oamData[256];

    // PPU registers
    u16 v;      // current vram address (15 bit)
    u16 t;      // temporary vram address (15 bit)
    u8 x;       // fine x scroll (3 bit)
    u8 w;       // write toggle (1 bit)
    u8 f;       // even/odd frame flag (1 bit)

    u8 reg;

    // NMI flags
    bool nmiOccurred;
    bool nmiOutput;
    bool nmiPrevious;
    u8 nmiDelay;

    // background temporary variables
    u8 nameTableByte;
    u8 attributeTableByte;
    u8 lowTileByte;
    u8 highTileByte;
    u64 tileData;

    // sprite temporary variables
    u32 spriteCount;
    u32 spritePatterns[8];
    u8 spritePositions[8];
    u8 spritePriorities[8];
    u8 spriteIndexes[8];

    // $2000 PPUCTRL
    u8 flagNameTable;        // 0: $2000; 1: $2400; 2: $2800; 3: $2C00
    u8 flagIncrement;        // 0: add 1; 1: add 32
    u8 flagSpriteTable;      // 0: $0000; 1: $1000; ignored in 8x16 mode
    u8 flagBackgroundTable;  // 0: $0000; 1: $1000
    u8 flagSpriteSize;       // 0: 8x8; 1: 8x16
    u8 flagMasterSlave;      // 0: read EXT; 1: write EXT

    // $2001 PPUMASK
    u8 flagGrayscale;           // 0: color; 1: grayscale
    u8 flagShowLeftBackground;  // 0: hide; 1: show
    u8 flagShowLeftSprites;     // 0: hide; 1: show
    u8 flagShowBackground;      // 0: hide; 1: show
    u8 flagShowSprites;         // 0: hide; 1: show
    u8 flagRedTint;             // 0: normal; 1: emphasized
    u8 flagGreenTint;           // 0: normal; 1: emphasized
    u8 flagBlueTint;            // 0: normal; 1: emphasized

    // $2002 PPUSTATUS
    u8 flagSpriteZeroHit;
    u8 flagSpriteOverflow;

    // $2003 OAMADDR
    u8 oamAddress;

    // $2007 PPUDATA
    u8 bufferedData;  // for buffered reads

    Ppu(Memory& ram, Rom& rom);

    template<typename Ppu::ControlFlags f>
    void Set(bool value){
        AssignBit<static_cast<int>(f)>(this->registers.controlRegister, value);
    }

    template<typename Ppu::ControlFlags f>
    bool Get(){
        return CheckBit<static_cast<int>(f)>(this->registers.controlRegister);
    }
    
    template<typename Ppu::MaskFlags f>
    void Set(bool value){
        AssignBit<static_cast<int>(f)>(this->registers.maskRegister, value);
    }

    template<typename Ppu::MaskFlags f>
    bool Get(){
        return CheckBit<static_cast<int>(f)>(this->registers.maskRegister);
    }
    
    template<typename Ppu::StatusFlags f>
    void Set(bool value){
        AssignBit<static_cast<int>(f)>(this->registers.processorStatus, value);
    }

    template<typename Ppu::StatusFlags f>
    bool Get(){
        return CheckBit<static_cast<int>(f)>(this->registers.processorStatus);
    }

    void Reset();
    u8 readPalette(u16 address);
    void writePalette(u16 address, u8 value);
    u8 readRegister(u16 address);
    void writeRegister(u16 address, u8 value);
    void writeControl(u8 value);
    void writeMask(u8 value);
    u8 readStatus();
    void writeOAMAddress(u8 value);
    u8 readOAMData();
    void writeOAMData(u8 value);
    void writeScroll(u8 value);
    void writeAddress(u8 value);
    u8 readData();
    void writeData(u8 value);
    void writeDMA(u8 value);
    void incrementX();
    void incrementY();
    void copyX();
    void copyY();
    void nmiChange();
    void setVerticalBlank();
    void clearVerticalBlank();
    void fetchNameTableByte();
    void fetchAttributeTableByte();
    void fetchLowTileByte();
    void fetchHighTileByte();
    void storeTileData();
    u32 fetchTileData();
    u8 backgroundPixel();
    BytePair spritePixel();
    void renderPixel();
    u32 fetchSpritePattern(u8 i, u32 row);
    void evaluateSprites();
    void tick();
    void Step();
};
}
#endif // PPU_Hd
