#include "gamepad.h"

namespace Frankenstein {

Gamepad::Gamepad(){
    index = 1;
}

u8 Gamepad::Read(u8 read, u8 write){
    u8 value = index < 9 && CheckBit<>(read, index);
    ++index;
    if (CheckBit<1>(write)) 
        index = 1;
    return value;
}

void Gamepad::Write(u8 write){
    if (CheckBit<1>(write)) 
        index = 1;
}

}
