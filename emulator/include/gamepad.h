#pragma once

#include "util.h"

namespace Frankenstein {

class Gamepad {
public:
    enum ButtonIndex : u8 {
        A = 0,
        B,
        Select,
        Start,
        Up,
        Down,
        Left,
        Right,
    };
    
    u8 index;
    u8 strobe;
    bool buttons[8];

    u8 Read();
    void Write(u8 value);
    
    Gamepad();
};
}
