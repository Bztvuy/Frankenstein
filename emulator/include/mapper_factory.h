#pragma once

#include "util.h"
#include "mapper.h"

namespace Frankenstein {

class MapperFactory{
public:
    static Mapper* MakeMapper(u8 mapper, Rom& rom);
};

}
