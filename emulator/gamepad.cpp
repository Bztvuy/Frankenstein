#include "gamepad.h"

Gamepad::Gamepad(u8& readAddress, u8& writeAddress) : readButtons(readAddress), writeButtons(writeAddress){
    index = 1;
}

u8 Gamepad::Read(){
    u8 value = index < 9 && CheckBit<index>readButtons;
    ++index;
    if (CheckBit<1>(writeButtons)) 
        index = 1;
    return value;
}

void Gamepad::Write(){
    if (CheckBit<1>(writeButtons)) 
        index = 1;
}