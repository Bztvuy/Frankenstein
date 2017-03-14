#include "nes.h"

using namespace Frankenstein;

Nes::Nes() : ram(), rom(), cpu(ram, rom), ppu(ram, rom, cpu), controller1(ram->Get(0x4016), ram->Get(0x4016)), controller2(ram->Get(0x4017), ram->Get(0x4016)){
    ram->SetGamepad1(controller1);
    ram->SetGamepad2(controller2);
}

void Nes::Step(){
    cpu.Step();
    for(u16 i = 0; i < (cpu.cycles * 3); ++i)
        ppu.Step();
    cpu.cycles = 0;
}
