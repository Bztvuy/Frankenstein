#include "nes.h"

using namespace Frankenstein;

Nes::Nes(Rom &pRom) : pad1(), pad2(), ram(*this), rom(pRom), cpu(*this), ppu(*this){}

void Nes::Step(){
    cpu.Step();
    for(u16 i = 0; i < (cpu.cycles * 3); ++i) {
        ppu.Step();
    }
}
