#include "rom_file.h"

#include <cstdio>

using namespace Frankenstein;

const u8 * FileRom::GetRaw() const {
    return this->raw.data();
}

u32 FileRom::GetLength() const {
    return this->raw.size();
}

const iNesHeader FileRom::GetHeader() const {
    return this->header;
}

u8* FileRom::GetCHR() const {
    return this->CHR;
}

u8* FileRom::GetPRG() const {
    return this->PRG;
}

u8* FileRom::GetSRAM() const {
    return this->SRAM;
}

FileRom::~FileRom() {
}

FileRom::FileRom() : filename() {
}

FileRom::FileRom(std::string file) {
    load(file);
}

void FileRom::load(std::string file) {
    this->filename = file;

    {
        FILE *f = fopen(file.c_str(), "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  //same as rewind(f);

        this->raw.resize(fsize);

        fread(this->raw.data(), fsize, 1, f);
        fclose(f);
    }

    this->header = MakeHeader();
    this->PRG = MakePRG();
    this->CHR = MakeCHR();
    this->SRAM = MakeSRAM();
}
