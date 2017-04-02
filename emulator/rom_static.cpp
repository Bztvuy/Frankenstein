#include "rom_static.h"

using namespace Frankenstein;

StaticRom::StaticRom() {
    header = MakeHeader();
    PRG = MakePRG();
    CHR = MakeCHR();
    SRAM = MakeSRAM();
}

StaticRom::~StaticRom() {
}

const u8* StaticRom::GetRaw() const{
    return raw;
}

unsigned int StaticRom::GetLength() const{
    return length;
}

const iNesHeader StaticRom::GetHeader() const {
    return this->header;
}

u8* StaticRom::GetCHR() const {
    return this->CHR;
}

u8* StaticRom::GetPRG() const {
    return this->PRG;
}

u8* StaticRom::GetSRAM() const {
    return this->SRAM;
}
