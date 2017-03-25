#pragma once

#include "rom.h"

namespace Frankenstein {

class StaticRom : public IRom {
public:
    virtual const u8* GetRaw() const override;
    virtual unsigned int GetLength() const override;
    virtual const iNesHeader GetHeader() const override;

    virtual ~StaticRom();
    StaticRom();

private:
    iNesHeader header;
    static const unsigned int length;
    static const u8 raw[];
};

}
