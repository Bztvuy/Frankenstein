#ifndef CPU_H
#define CPU_H

#include "rom.h"

union NESCPUMemoryMap{
    struct {
        unsigned char zeroPage[0x0100];
        unsigned char stack[0x0100];
        unsigned char ram[0x0600];
        unsigned char mirrors1[0x1800];
        unsigned char ioRegisters1[0x0008];
        unsigned char mirrors2[0x1FF8];
        unsigned char ioRegisters2[0x0020];
        unsigned char expansionRom[0x1FE0];
        unsigned char sram[0x2000];
        unsigned char prgRomLowerBank[0x4000];
        unsigned char prgRomUpperBank[0x4000];
    };
    unsigned char raw[0x10000];
};

union NESCPURegisters{
    struct {
        unsigned short programCounter;
        unsigned char stackPointer;
        unsigned char accumulator;
        unsigned char indexX;
        unsigned char indexY;
        unsigned char processorStatus;
    };
    struct {
        unsigned short PC;
        unsigned char SP;
        unsigned char A;
        unsigned char X;
        unsigned char Y;
        unsigned char P;
    };
};

class CPU
{
private:
	NESCPURegisters* registers;
    NESCPUMemoryMap* memory;
public:
	//sizes related to hardware (in bytes) :
	const unsigned int prgRomBankSize = 16 * KILOBYTE;
	const unsigned int vRomBankSize = 8 * KILOBYTE;
	const unsigned int prgRamBankSize = 8 * KILOBYTE;
	
    CPU(const Rom* rom);
};

#endif // CPU_H
