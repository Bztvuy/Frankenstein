#include "rom.h"

using namespace Frankenstein;

<<<<<<< HEAD
const iNesHeader IRom::MakeHeader() const {
    iNesHeader header;
    u8 const *const raw = this->GetRaw();
=======
const iNesHeader* StaticRom::GetHeader() const{
    iNesHeader* header = new iNesHeader;
>>>>>>> 72ec4108796e94e0a2c277c4d45982e3a393de20
    for (unsigned int i = 0; i < sizeof(iNesHeader); ++i){
        header.raw[i] = raw[i];
    }
    return header;
}

<<<<<<< HEAD
unsigned int IRom::GetTrainerOffset() const {
    return CheckBit<3>(this->GetHeader().controlByte1) * IRom::TrainerSize;
}
=======
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
>>>>>>> 72ec4108796e94e0a2c277c4d45982e3a393de20
