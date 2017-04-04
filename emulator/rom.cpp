#include "rom.h"

using namespace Frankenstein;

const iNesHeader IRom::MakeHeader() const {
    iNesHeader header;
    u8 const *const raw = this->GetRaw();
    for (u32 i = 0; i < sizeof(iNesHeader); ++i){
        header.raw[i] = raw[i];
    }
    return header;
}

u32 IRom::GetTrainerOffset() const {
    return CheckBit<3>(this->GetHeader().controlByte1) * IRom::TrainerSize;
}
