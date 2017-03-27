#pragma once

#include "util.h"

namespace Frankenstein {

class Gamepad{
public:
    u8 index;
    
    Gamepad();
    
    u8 Read(u8 read, u8 write);
    void Write(u8 write);
};

}


