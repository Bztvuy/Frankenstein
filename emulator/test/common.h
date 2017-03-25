#pragma once

#include <gtest/gtest.h>
#include <memory.h>
#include <cpu.h>
#include <gamepad.h>

struct MemoryTest : testing::Test {
    Frankenstein::Gamepad pad1;
    Frankenstein::Gamepad pad2;
    Frankenstein::NesMemory ram;

    MemoryTest() : ram(pad1, pad2)
    {
    }

    virtual ~MemoryTest()
    {
    }
};

struct CPUTest : MemoryTest {
    Frankenstein::Cpu cpu;

    CPUTest() : cpu ( ram )
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
