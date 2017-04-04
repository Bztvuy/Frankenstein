#include "rom_loader.h"

#include "util.h"
#include "rom.h"
#include <cstdio>

using namespace Frankenstein;

Frankenstein::Rom RomLoader::GetRom(std::string file) {
    FILE *f = fopen(file.c_str(), "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);

    auto data = new u8[fsize];

    fread(data, fsize, 1, f);
    fclose(f);

    return Rom(data, fsize);
}
