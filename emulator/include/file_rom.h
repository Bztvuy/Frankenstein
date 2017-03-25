#pragma once

#include "rom.h"
#include "util.h"

#include <string>
#include <vector>

namespace Frankenstein {
    
    class FileRom : public Rom {
    public:
        FileRom(std::string filename);

        const iNesHeader* GetHeader() const override;
        const char* GetRaw() const override;
        unsigned int GetLength() const override;
        unsigned int GetTrainerOffset() const override;
        ~FileRom() override;
    private:
        std::vector<char> romData;
    };

}