#pragma once

#include "rom.h"

namespace Frankenstein {

class StaticRom : public IRom {
public:
    virtual const u8* GetRaw() const override;
    virtual unsigned int GetLength() const override;
    virtual const iNesHeader GetHeader() const override;
    virtual u8* GetPRG() const override;
    virtual u8* GetCHR() const override;
    virtual u8* GetSRAM() const override;

    virtual ~StaticRom();
    StaticRom();

private:
    iNesHeader header;
    static const unsigned int length;
    static const u8 raw[];
    u8* PRG;
    u8* CHR;
    u8* SRAM;
};

}
