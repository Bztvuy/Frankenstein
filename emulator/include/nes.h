#ifndef NES_H
#define NES_H

#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "memory.h"

namespace Frankenstein {
    
class Nes
{
public:
    Memory ram;
    Rom rom;
    Cpu cpu;
    Ppu ppu;

    Nes();
    void Step();
};

}

#endif // CPU_H
