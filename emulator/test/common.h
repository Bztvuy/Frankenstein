#pragma once

#include <gtest/gtest.h>
#include <nes.h>
#include <rom_static.h>

struct MemoryTest : testing::Test {
    Frankenstein::StaticRom rom;
    Frankenstein::Nes nes;

    MemoryTest() : rom(), nes(rom)
    {
    }

    virtual ~MemoryTest()
    {
    }
};

struct CPUTest : MemoryTest {
    Frankenstein::StaticRom rom;
    Frankenstein::Nes nes = Frankenstein::Nes(rom);

    CPUTest() : rom(), nes(rom)
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
