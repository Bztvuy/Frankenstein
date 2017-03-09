#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "util.h"

class Gamepad{
public:
    u8& readButtons;
    u8& writeButtons;
    u8 index;
    
    Gamepad(u8& readAddress, u8& writeAddress);
    
    u8 Read();
    void Write();
};

#endif /* GAMEPAD_H */

