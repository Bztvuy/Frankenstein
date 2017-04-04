#include "mapper_factory.h"

using namespace Frankenstein;

Mapper* MapperFactory::MakeMapper(u8 mapper, IRom& rom){
    switch (mapper) {
	case 0:
		return new Mapper2(rom);
	case 1:
		return new Mapper1(rom);
	case 2:
		return new Mapper2(rom);
	case 3:
		return new Mapper3();
	case 4:
		return new Mapper4();
	case 7:
		return new Mapper7();
	}
    return nullptr;
}
