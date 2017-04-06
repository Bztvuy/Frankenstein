#include "gamepad.h"

namespace Frankenstein {

Gamepad::Gamepad()
    : index(0)
    , strobe(0)
    , buttons{false}
{
}

u8 Gamepad::Read()
{
    u8 value = buttons[index];
    index++;
    if (index >= 8) {
        index = 0;
    }
    if ((strobe & 1) == 1) {
        index = 0;
    }
    return value;
}

void Gamepad::Write(u8 value)
{
    strobe = value;
    if ((strobe & 1) == 1) {
        index = 0;
    }
}
}
