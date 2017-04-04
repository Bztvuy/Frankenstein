#pragma once

#include "util.h"
#include "rom.h"

namespace Frankenstein {

class Mapper{
public:
    enum MirrorMode {
	MirrorHorizontal = 0,
	MirrorVertical   = 1,
	MirrorSingle0    = 2,
	MirrorSingle1    = 3,
	MirrorFour       = 4
    };
    
    virtual u8 Read(u16 address) = 0;
    virtual void Write(u16 address, u8 value) = 0;
    virtual void Step() = 0;
    
    virtual ~Mapper() = 0;
};

class Mapper1 : public Mapper{
public:
    virtual u8 Read(u16 address) override;
    virtual void Write(u16 address, u8 value) override;
    virtual void Step() override;
    
    void loadRegister(u16 address, u8 value);
    void writeRegister(u16 address, u8 value);
    void writeControl(u8 value);
    void writeCHRBank0(u8 value);
    void writeCHRBank1(u8 value);
    void writePRGBank(u8 value);
    s32 prgBankOffset(s32 index);
    s32 chrBankOffset(s32 index);
    void updateOffsets();

    MirrorMode mirrorMode;
    
    Mapper1(IRom& pRom);
    ~Mapper1() override;
    
private:
    IRom& rom;
    u8 shiftRegister;
    u8 control;     
    u8 prgMode;
    u8 chrMode;
    u8 prgBank;
    u8 chrBank0;
    u8 chrBank1;
    u32 prgOffsets[2];
    u32 chrOffsets[2];
};

class Mapper2 : public Mapper{
public:
    virtual u8 Read(u16 address) override;
    virtual void Write(u16 address, u8 value) override;
    virtual void Step() override;
    
    Mapper2(IRom& pRom);
    ~Mapper2() override;
    
private:
    IRom& rom;
    u8 prgBanks;
    u8 prgBank1;
    u8 prgBank2;
};

class Mapper3 : public Mapper{
public:
    virtual u8 Read(u16 address) override;
    virtual void Write(u16 address, u8 value) override;
    virtual void Step() override;
    
    Mapper3();
    ~Mapper3() override;
};

class Mapper4 : public Mapper{
public:
    virtual u8 Read(u16 address) override;
    virtual void Write(u16 address, u8 value) override;
    virtual void Step() override;
    
    Mapper4();
    ~Mapper4() override;
};

class Mapper7 : public Mapper{
public:
    virtual u8 Read(u16 address) override;
    virtual void Write(u16 address, u8 value) override;
    virtual void Step() override;
    
    Mapper7();
    ~Mapper7() override;
};



}