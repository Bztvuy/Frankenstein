#ifndef PPU_H
#define PPU_H

union NESPPUMemoryMap{
    struct {
        char patternTable0[0x1000];
        char patternTable1[0x1000];
        char nameTable0[0x03C0];
        char attributeTable0[0x0040];
        char nameTable1[0x03C0];
        char attributeTable1[0x0040];
        char nameTable2[0x03C0];
        char attributeTable2[0x0040];
        char nameTable3[0x03C0];
        char attributeTable3[0x0040];
        char mirrors1[0x0F00];
        char imagePalette[0x0010];
        char spritePalette[0x0010];
        char mirrors2[0x00E0];
        char mirrors3[0xC000];
    };
    char raw[0x10000];
};

struct NESPPURegisters{
    struct {
        unsigned char controlRegister1;
        unsigned char controlRegister2;
        unsigned char processorStatus;
        unsigned char sprRamAddress;
        unsigned char sprRamIO;
        unsigned char vramAddress1;
        unsigned char vramAddress2;
        unsigned char vramIO;
        //TODO: add register above 0x4000
    };
    struct {
        unsigned char C1;
        unsigned char C2;
        unsigned char P;
        unsigned char SRA;
        unsigned char SRIO;
        unsigned char VRA1;
        unsigned char VRA2;
        unsigned char VRIO;
    };
};

class PPU
{
public:
    PPU();
};

#endif // PPU_H
