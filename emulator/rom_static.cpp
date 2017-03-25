#include "rom_static.h"

using namespace Frankenstein;

StaticRom::StaticRom() {
    header = MakeHeader();
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
