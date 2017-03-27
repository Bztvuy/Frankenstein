#include "common.h"

using namespace Frankenstein;

TEST_F(CPUTest, DefaultState)
{
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_EQ(0, nes.cpu.registers.X);
    EXPECT_EQ(0, nes.cpu.registers.Y);
    EXPECT_EQ(0b00100100, nes.cpu.registers.P);
    EXPECT_EQ(0, nes.cpu.registers.PC);
    EXPECT_EQ(0xFD, nes.cpu.registers.SP);
    EXPECT_EQ(0, nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_EQ(0, nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_EQ(0, nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_EQ(0, nes.cpu.Get<Cpu::Flags::C>());
}

///////////////////////////////////////////////////////////////////////////////
// AND Tests
///////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, AND_ZeroAndOne)
{
    nes.cpu.registers.A = 0;
    u8 value = 1;
    nes.cpu.AND(value);
    EXPECT_EQ(0,     nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_OneAndZero)
{
    nes.cpu.registers.A = 1;
    u8 value = 0;
    nes.cpu.AND(value);
    EXPECT_EQ(0,nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_OneAndOne)
{
    nes.cpu.registers.A = 1;
    u8 value = 1;
    nes.cpu.AND(value);
    EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_NegativeResult)
{
    nes.cpu.registers.A = 0xFF;
    u8 value = 0xF0;
    nes.cpu.AND(value);
    EXPECT_EQ(value,nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

///////////////////////////////////////////////////////////////////////////////
// ASL Tests
///////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, ASL_WithCarry)
{
    u8 value = 0xFF;
    nes.cpu.ASL(value);
    EXPECT_EQ(0xFE, value);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ASL_WithoutCarry) 
{
    u8 value = 0x0F;
    nes.cpu.ASL(value);
    EXPECT_EQ(0x1E, value);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

///////////////////////////////////////////////////////////////////////////////
// BIT Tests
///////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, BIT_ZeroResultAndV)
{
    nes.cpu.registers.A = 0x00;
    u8 value = 0x7F;
    nes.cpu.BIT(value);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, BIT_NonZeroResultAndS)
{
    nes.cpu.registers.A = 0xF0;
    u8 value = 0xB0;
    nes.cpu.BIT(value);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}


///////////////////////////////////////////////////////////////////////////////
// ADC Tests
///////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, ADC_ZeroAndZero)
{
    nes.cpu.registers.A = 0;
    u8 value = 0;
    nes.cpu.ADC(value);
    EXPECT_EQ(0,     nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, ADC_ZeroAndOne)
{
    nes.cpu.registers.A = 0;
    u8 value = 1;
    nes.cpu.ADC(value);
    EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, ADC_WithCarry)
{
    nes.cpu.registers.A = 0;
    nes.cpu.Set<Cpu::Flags::C>(true);

    u8 value = 1;
    nes.cpu.ADC(value);
//     EXPECT_EQ(2, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, ADC_SetCarry)
{
    nes.cpu.registers.A = 0xFF;

    u8 value = 1;
    nes.cpu.ADC(value);
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, ADC_Overflow)
{
    nes.cpu.registers.A = 0x80;                                 // -128
    u8 value = 0xFF;                                        // -1
    nes.cpu.ADC(value);
    EXPECT_EQ(0x7F,  nes.cpu.registers.A);                      // 127
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());

    nes.cpu.registers.A = 0x7F;                                 // 127
    value = 0x01;                                           // 1
    nes.cpu.ADC(value);                                         // + carry
    EXPECT_EQ(0x81,  nes.cpu.registers.A);                      // -127
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());

    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 0x7E;                                 // 126
    value = 0x01;                                           // 1
    nes.cpu.ADC(value);                                         // + carry
    EXPECT_EQ(0x80,  nes.cpu.registers.A);                      // -128
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, DEC_ABS)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.registers.PC = 0x8000;
    nes.ram[0x8001] = 10;
    nes.ram[0x8002] = 10;
    nes.ram[nes.ram.Absolute(10, 10)] = 32;

    nes.cpu.DEC_ABS();
    
    EXPECT_EQ(31, nes.ram[nes.ram.Absolute(10, 10)]);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, Stack_Ops)
{
    nes.cpu.PushOnStack(0x10);
    nes.cpu.PushOnStack(0x20);

    u8 first = nes.cpu.PopFromStack();
    u8 second = nes.cpu.PopFromStack();
    
    EXPECT_EQ(0x20, first);
    EXPECT_EQ(0x10, second);
}
