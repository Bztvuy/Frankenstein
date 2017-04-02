#pragma once

#include "util.h"
#include "rom.h"

namespace Frankenstein {

class Mapper{
public:
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
    
    Mapper1();
    ~Mapper1() override;
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