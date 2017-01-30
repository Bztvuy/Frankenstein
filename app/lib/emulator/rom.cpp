//
// rom.cpp
//
#include "rom.h"

const iNesHeader* Rom::GetHeader() const{
    iNesHeader* header = new iNesHeader;
    for (unsigned int i = 0; i < sizeof(iNesHeader); ++i){
        header->raw[i] = this->GetRaw()[i];
    }
    return header;
}

const char* Rom::GetRaw() const{
    return raw;
}

const unsigned int Rom::GetLength() const{
    return length;
}

const unsigned int Rom::GetTrainerOffset() const{
	return CHECK_BIT(this->GetHeader()->controlByte1, 2) * trainerSize;
}
// ...
