//
// rom.cpp
//
#include <template/rom.h>

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

// ...
