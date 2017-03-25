#include "rom_file.h"

#include <cstdio>

using namespace Frankenstein;

const u8 * FileRom::GetRaw() const {
    return this->raw.data();
}

unsigned int FileRom::GetLength() const {
    return this->raw.size();
}

const iNesHeader FileRom::GetHeader() const {
    return this->header;
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
}
