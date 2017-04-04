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

class Rom {
public:
    //sizes related to rom file format (in bytes) :
    static constexpr u32 HeaderSize = 16;
    static constexpr u32 TrainerSize = 512;

    u32 GetTrainerOffset() const;
    const iNesHeader GetHeader() const;
    const u8* const GetRaw() const;
    u32 GetLength() const;
    u8* GetPRG() const;
    u8* GetCHR() const;
    u8* GetSRAM() const;

    Rom(const u8* const data, u64 size);
    
private:
    const u8* const raw;
    const u64 length;

    iNesHeader header;
    u8* PRG;
    u8* CHR;
    u8* SRAM;

    iNesHeader MakeHeader() const;
    u8* MakePRG() const;
    u8* MakeCHR() const;
    u8* MakeSRAM() const;
};
}
