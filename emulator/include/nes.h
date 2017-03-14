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
    NesMemory ram;
    Rom rom;
    Cpu cpu;
    Ppu ppu;
    Gamepad controller1;
    Gamepad controller2;

    Nes();
    void Step();
};

}

#endif // CPU_H
