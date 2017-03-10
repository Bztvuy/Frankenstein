#include "nes.h"

using namespace Frankenstein;

Nes::Nes() : ram(), rom(), cpu(ram, rom), ppu(ram, rom, cpu){
}

void Nes::Step(){
    cpu.Step();
    for(u16 i = 0; i < (cpu.cycles * 3); ++i)
        ppu.Step();
    cpu.cycles = 0;
}
