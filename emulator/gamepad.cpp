#include "gamepad.h"

namespace Frankenstein {

u8 Gamepad::Read(){
    u8 value = 0;
    if (index < 8 && buttons[index]) {
	    value = 1;
    }
    index++;
    if ((strobe&1) == 1) {
	    index = 0;
    }
    return value;
}

void Gamepad::Write(u8 value){
    strobe = value;
	if ((strobe&1) == 1) {
		index = 0;
	}
}

}
