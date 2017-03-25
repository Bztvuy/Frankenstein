#include "nes.h"

using namespace Frankenstein;

Nes::Nes(IRom &pRom) : pad1(), pad2(), ram(pad1, pad2), rom(pRom), cpu(ram, rom), ppu(ram, rom, cpu){}

void Nes::Step(){
    cpu.Step();
    for(u16 i = 0; i < (cpu.cycles * 3); ++i) {
        ppu.Step();
    }
}
