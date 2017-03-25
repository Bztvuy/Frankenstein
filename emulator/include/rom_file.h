#pragma once

#include "rom.h"

#include <string>
#include <vector>

namespace Frankenstein {

class FileRom : public IRom {
public:
    virtual const u8* GetRaw() const override;
    virtual unsigned int GetLength() const override;
    virtual const iNesHeader GetHeader() const override;

    virtual ~FileRom();
    FileRom();
    FileRom(std::string file);
    void load(std::string file);

private:
    std::vector<u8> raw;
    int raw_size;
    std::string filename;
    iNesHeader header;
};

}
