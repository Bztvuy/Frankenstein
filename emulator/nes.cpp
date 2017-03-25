#include "nes.h"

using namespace Frankenstein;

<<<<<<< HEAD
Nes::Nes(IRom &pRom) : pad1(), pad2(), ram(pad1, pad2), rom(pRom), cpu(ram, rom), ppu(ram, rom, cpu){}
=======
Nes::Nes(Rom& romToLoad) : ram(), rom(romToLoad), cpu(ram, rom), ppu(ram, rom, cpu){
}
>>>>>>> 72ec4108796e94e0a2c277c4d45982e3a393de20

void Nes::Step(){
    cpu.Step();
    for(u16 i = 0; i < (cpu.cycles * 3); ++i) {
        ppu.Step();
    }
}
