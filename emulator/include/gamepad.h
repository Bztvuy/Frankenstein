#ifndef GAMEPAD_H
#define GAMEPAD_H

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

#endif /* GAMEPAD_H */

