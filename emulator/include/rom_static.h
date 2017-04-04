
#pragma once

#include "util.h"
#include "rom.h"

namespace Frankenstein {

class StaticRom : public IRom {
public:
    virtual const u8 * GetRaw() const override;
    virtual u32 GetLength() const override;
    virtual const iNesHeader GetHeader() const override;
    virtual u8* GetPRG() const override;
    virtual u8* GetCHR() const override;
    virtual u8* GetSRAM() const override;

    virtual ~StaticRom();
    explicit StaticRom();

private:
    iNesHeader header;
    static const u32 length;
    static const u8 raw[];
    u8* PRG;
    u8* CHR;
    u8* SRAM;
};

}
