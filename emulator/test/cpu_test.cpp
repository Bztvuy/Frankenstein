#include "common.h"

using namespace Frankenstein;

TEST_F(CPUTest, DefaultState)
{
    EXPECT_EQ(0, cpu.registers.A);
    EXPECT_EQ(0, cpu.registers.X);
    EXPECT_EQ(0, cpu.registers.Y);
    EXPECT_EQ(0b00100000, cpu.registers.P);
    EXPECT_EQ(0, cpu.registers.PC);
    EXPECT_EQ(0xFF, cpu.registers.SP);
    EXPECT_EQ(0, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(0, cpu.Get<Cpu::Flags::S>());
    EXPECT_EQ(0, cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(0, cpu.Get<Cpu::Flags::C>());
}

///////////////////////////////////////////////////////////////////////////////
// AND Tests
///////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, AND_ZeroAndOne)
{
    cpu.registers.A = 0;
    u8 value = 1;
    cpu.AND(value);
    EXPECT_EQ(0,     cpu.registers.A);
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_OneAndZero)
{
    cpu.registers.A = 1;
    u8 value = 0;
    cpu.AND(value);
    EXPECT_EQ(0,     cpu.registers.A);
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_OneAndOne)
{
    cpu.registers.A = 1;
    u8 value = 1;
    cpu.AND(value);
    EXPECT_EQ(1,     cpu.registers.A);
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_NegativeResult)
{
    cpu.registers.A = 0xFF;
    u8 value = 0xF0;
    cpu.AND(value);
    EXPECT_EQ(value, cpu.registers.A);
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::S>());
}

///////////////////////////////////////////////////////////////////////////////
// ASL Tests
///////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, ASL_WithCarry)
{
    u8 value = 0xFF;
    cpu.ASL(value);
    EXPECT_EQ(0xFE, value);
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::C>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ASL_WithoutCarry) 
{
    u8 value = 0x0F;
    cpu.ASL(value);
    EXPECT_EQ(0x1E, value);
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::C>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::S>());
}

///////////////////////////////////////////////////////////////////////////////
// BIT Tests
///////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, BIT_ZeroResultAndV)
{
    cpu.registers.A = 0x00;
    u8 value = 0x7F;
    cpu.BIT(value);
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, BIT_NonZeroResultAndS)
{
    cpu.registers.A = 0xF0;
    u8 value = 0xB0;
    cpu.BIT(value);
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(false,  cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::S>());
}


///////////////////////////////////////////////////////////////////////////////
// ADC Tests
///////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, ADC_ZeroAndZero)
{
    cpu.registers.A = 0;
    u8 value = 0;
    cpu.ADC(value);
    EXPECT_EQ(0, cpu.registers.A);
    EXPECT_EQ(true, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::S>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, ADC_ZeroAndOne)
{
    cpu.registers.A = 0;
    u8 value = 1;
    cpu.ADC(value);
    EXPECT_EQ(1, cpu.registers.A);
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::S>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, ADC_WithCarry)
{
    cpu.registers.A = 0;
    cpu.Set<Cpu::Flags::C>(true);

    u8 value = 1;
    cpu.ADC(value);
    EXPECT_EQ(2, cpu.registers.A);
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::S>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, ADC_SetCarry)
{
    cpu.registers.A = 0xFF;

    u8 value = 1;
    cpu.ADC(value);
    EXPECT_EQ(0, cpu.registers.A);
    EXPECT_EQ(true, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::S>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(true, cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, ADC_Overflow)
{
    cpu.registers.A = 0x80;                                 // -128
    u8 value = 0xFF;                                        // -1
    cpu.ADC(value);
    EXPECT_EQ(0x7F,  cpu.registers.A);                      // 127
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::S>());
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::C>());

    cpu.registers.A = 0x7F;                                 // 127
    value = 0x01;                                           // 1
    cpu.ADC(value);                                         // + carry
    EXPECT_EQ(0x81,  cpu.registers.A);                      // -127
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::S>());
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::C>());

    cpu.Set<Cpu::Flags::C>(true);
    cpu.registers.A = 0x7E;                                 // 126
    value = 0x01;                                           // 1
    cpu.ADC(value);                                         // + carry
    EXPECT_EQ(0x80,  cpu.registers.A);                      // -128
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::S>());
    EXPECT_EQ(true,  cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(false, cpu.Get<Cpu::Flags::C>());
}
