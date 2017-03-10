#include "rom.h"

using namespace Frankenstein;

const iNesHeader* Rom::GetHeader() const{
    iNesHeader* header = new iNesHeader;
    for (unsigned int i = 0; i < sizeof(iNesHeader); ++i){
        header->raw[i] = this->GetRaw()[i];
    }
    return header;
}

const u8* Rom::GetRaw() const{
    return raw;
}

unsigned int Rom::GetLength() const{
    return length;
}

unsigned int Rom::GetTrainerOffset() const{
	return CheckBit<3>(this->GetHeader()->controlByte1) * trainerSize;
}
// ...
