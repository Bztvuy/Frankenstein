#pragma once

#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "memory_nes.h"
#include "gamepad.h"

namespace Frankenstein {

class Nes
{
public:
    Gamepad pad1;
    Gamepad pad2;
    NesMemory ram;
    const IRom &rom;
    Cpu cpu;
    Ppu ppu;

    explicit Nes(IRom &rom);
    void Step();
};

}
