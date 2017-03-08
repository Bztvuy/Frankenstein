#include "rom.h"

using namespace Frankenstein;

const iNesHeader* StaticRom::GetHeader() const{
    iNesHeader* header = new iNesHeader;
    for (unsigned int i = 0; i < sizeof(iNesHeader); ++i){
        header->raw[i] = this->GetRaw()[i];
    }
    return header;
}

const char* StaticRom::GetRaw() const{
    return raw;
}

unsigned int StaticRom::GetLength() const {
    return length;
}

unsigned int StaticRom::GetTrainerOffset() const{
	return CheckBit<3>(this->GetHeader()->controlByte1) * Rom::TrainerSize;
}

StaticRom::~StaticRom() {}
// ...
