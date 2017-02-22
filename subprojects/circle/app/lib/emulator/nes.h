#ifndef NES_H
#define NES_H

#include "rom.h"
#include "cpu.h"
#include "ppu.h"
class Nes
{
private:
    Rom* rom;
    Cpu* cpu;
    Ppu* ppu;
public:
    Nes();
};

#endif // CPU_H
