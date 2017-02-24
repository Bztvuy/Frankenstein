#pragma once

#include <memory.h>
#include <cpu.h>

struct MemoryTest : testing::Test
{
    Frankenstein::Memory ram;
    
    MemoryTest()
    {
    }
    
    virtual ~MemoryTest()
    {
    }
};

struct CPUTest : MemoryTest
{
    Frankenstein::Cpu cpu;
    
    CPUTest() : cpu(ram)
    {
    }
    
    virtual ~CPUTest()
    {
    }
};
