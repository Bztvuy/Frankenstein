#pragma once

#include "util.h"
#include "mapper.h"

namespace Frankenstein {

class MapperFactory{
public:
    Mapper* MakeMapper(u8 mapper, IRom& rom);
};

}