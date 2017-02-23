#include "nes.h"

Nes::Nes(){
    rom = new Rom;
    cpu = new Cpu(rom);
    ppu = new Ppu(rom, cpu);
}
