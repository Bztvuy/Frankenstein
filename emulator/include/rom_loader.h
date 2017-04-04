#pragma once

#include "rom.h"
#include <string>

namespace Frankenstein {

struct RomLoader {
    static Rom GetRom(std::string file);
};

}
