#include "rom_file.h"

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
        std::fstream ifs(file, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type pos = ifs.tellg();

        this->raw.resize(pos);

        ifs.seekg(0, std::ios::beg);
        ifs.read((char*)this->raw.data(), pos);
    }

    this->header = MakeHeader();
}
