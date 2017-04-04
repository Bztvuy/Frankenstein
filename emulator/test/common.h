#pragma once

#include <gtest/gtest.h>
#include <nes.h>
#include <rom_static.h>
#include <rom_loader.h>

struct MemoryTest : testing::Test {
    Frankenstein::Rom rom;
    Frankenstein::Nes nes;

    MemoryTest() : rom(Frankenstein::RomLoader::GetRom("roms/01-basics.nes")), nes(rom)
    {
    }

    virtual ~MemoryTest()
    {
    }
};

struct CPUTest : MemoryTest {
    Frankenstein::Rom rom;
    Frankenstein::Nes nes = Frankenstein::Nes(rom);

    CPUTest() : rom(Frankenstein::RomLoader::GetRom("roms/01-basics.nes")), nes(rom)
    {
    }

    virtual ~CPUTest()
    {
    }
};

struct RomTest : CPUTest {

    RomTest() {

    }

    virtual ~RomTest()
    {
    }
};
