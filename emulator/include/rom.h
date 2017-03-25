#pragma once

#include "util.h"

namespace Frankenstein {

union iNesHeader {
    struct {
        char magicWord[4];
        char prgRomBanks;
        char vRomBanks;
        char controlByte1;
        char controlByte2;
        char prgRamSize;
        char flags9;
        char flags10;
        char zeros[5];
    };
    char raw[16];
};

class IRom {
public:
    //sizes related to rom file format (in bytes) :
    static constexpr int HeaderSize = 16;
    static constexpr int TrainerSize = 512;

    unsigned int GetTrainerOffset() const;

    virtual const iNesHeader GetHeader() const = 0;
    virtual const u8* GetRaw() const = 0;
    virtual unsigned int GetLength() const = 0;
    virtual ~IRom(){}

protected:
    const iNesHeader MakeHeader() const;
};

}
