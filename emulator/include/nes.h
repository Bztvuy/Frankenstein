#ifndef NES_H
#define NES_H

#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "memory.h"

namespace Frankenstein {
    
class Nes
{
private:
    Memory ram;
    Rom rom;
    Cpu cpu;
    Ppu ppu;
public:
    Nes();
};

}

#endif // CPU_H
