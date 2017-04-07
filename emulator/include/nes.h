#pragma once

#include "rom.h"
#include "cpu.h"
#include "ppu.h"
#include "memory_nes.h"
#include "gamepad.h"

class CScreenDevice;

namespace Frankenstein {

class Nes
{
public:
    Gamepad pad1;
    Gamepad pad2;
    NesMemory ram;
    const Rom &rom;
    Cpu cpu;
    Ppu ppu;
    
    CScreenDevice* screen;
    
    explicit Nes(Rom &rom);
    explicit Nes(Rom &rom, CScreenDevice* pScreen);
    
    void Step();
};

}
