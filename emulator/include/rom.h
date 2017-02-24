//
// rom.h
//
#ifndef _rom_h
#define _rom_h

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

    class Rom
    {
    public:
        const iNesHeader* GetHeader() const;
        const char* GetRaw() const;
        unsigned int GetLength() const;
        unsigned int GetTrainerOffset() const;

        //sizes related to rom file format (in bytes) :
        const unsigned int headerSize = 16;
        const unsigned int trainerSize = 512;

    private:    
        static const unsigned int length;
        static const char raw[262160];
    };

}
#endif
