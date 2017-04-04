#pragma once

#include "rom.h"
#include "util.h"

#include <string>
#include <vector>

namespace Frankenstein {

class FileRom : public IRom {
public:
    virtual const u8 * GetRaw() const override;
    virtual u32 GetLength() const override;
    virtual const iNesHeader GetHeader() const override;
    virtual u8* GetPRG() const override;
    virtual u8* GetCHR() const override;
    virtual u8* GetSRAM() const override;

    virtual ~FileRom();
    explicit FileRom();
    explicit FileRom(std::string file);
    void load(std::string file);

private:
    std::vector<u8> raw;
    u32 raw_size;
    std::string filename;
    iNesHeader header;
    u8* PRG;
    u8* CHR;
    u8* SRAM;
};

}
