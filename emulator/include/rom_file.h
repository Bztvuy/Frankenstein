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
    virtual u8* GetPRG() const override;
    virtual u8* GetCHR() const override;
    virtual u8* GetSRAM() const override;

    virtual ~FileRom();
    FileRom();
    FileRom(std::string file);
    void load(std::string file);

private:
    std::vector<u8> raw;
    int raw_size;
    std::string filename;
    iNesHeader header;
    u8* PRG;
    u8* CHR;
    u8* SRAM;
};

}
