#include "nes.h"

using namespace Frankenstein;

Nes::Nes() : ram(), rom(), cpu(ram, rom), ppu(ram, rom, cpu){
}
