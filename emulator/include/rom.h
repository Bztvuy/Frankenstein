#pragma once

#include "util.h"

namespace Frankenstein {

union iNesHeader {
    struct {
        u8 magicWord[4];
        u8 prgRomBanks;
        u8 vRomBanks;
        u8 controlByte1;
        u8 controlByte2;
        u8 prgRamSize;
        u8 flags9;
        u8 flags10;
        u8 zeros[5];
    };
    u8 raw[16];
};

class IRom {
public:
    //sizes related to rom file format (in bytes) :
    static constexpr u32 HeaderSize = 16;
    static constexpr u32 TrainerSize = 512;

    u32 GetTrainerOffset() const;

    virtual const iNesHeader GetHeader() const = 0;
    virtual const u8* GetRaw() const = 0;
    virtual u32 GetLength() const = 0;
    virtual u8* GetPRG() const = 0;
    virtual u8* GetCHR() const = 0;
    virtual u8* GetSRAM() const = 0;
    virtual ~IRom();

protected:
    iNesHeader MakeHeader() const;
    u8* MakePRG() const;
    u8* MakeCHR() const;
    u8* MakeSRAM() const;
};

}
