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

    class Rom {
    public:
        //sizes related to rom file format (in bytes) :
        static const unsigned int HeaderSize = 16;
        static const unsigned int TrainerSize = 512;
        
        virtual const iNesHeader* GetHeader() const = 0;
        virtual const char* GetRaw() const = 0;
        virtual unsigned int GetLength() const = 0;
        virtual unsigned int GetTrainerOffset() const = 0;
        virtual ~Rom() {}
    };
    
    class StaticRom : public Rom
    {
    public:
        const iNesHeader* GetHeader() const override;
        const char* GetRaw() const override;
        unsigned int GetLength() const override;
        unsigned int GetTrainerOffset() const override;

        ~StaticRom() override;
    private:
        static const unsigned int length;
        static const char raw[40976];
    };
}
#endif
