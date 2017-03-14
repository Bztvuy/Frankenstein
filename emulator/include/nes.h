#ifndef NES_H
#define NES_H

#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "memory.h"
#include "gamepad.h"

namespace Frankenstein {

class Nes
{
public:
    Gamepad pad1;
    Gamepad pad2;
    NesMemory ram;
    Rom rom;
    Cpu cpu;
    Ppu ppu;

    Nes();
    void Step();
};

}

#endif // CPU_H
