#include "rom.h"

using namespace Frankenstein;

const iNesHeader IRom::MakeHeader() const {
    iNesHeader header;
    u8 const *const raw = this->GetRaw();
    for (unsigned int i = 0; i < sizeof(iNesHeader); ++i){
        header.raw[i] = raw[i];
    }
    return header;
}

unsigned int IRom::GetTrainerOffset() const {
    return CheckBit<3>(this->GetHeader().controlByte1) * IRom::TrainerSize;
}
