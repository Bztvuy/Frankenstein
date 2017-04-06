#pragma once

#include "util.h"

namespace Frankenstein {

class Gamepad {
public:
    u8 index;
    u8 strobe;
    bool buttons[8];

    u8 Read();
    void Write(u8 value);
};
}
