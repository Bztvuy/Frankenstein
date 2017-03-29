#include "common.h"

using namespace Frankenstein;

TEST_F(CPUTest, DefaultState)
{
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_EQ(0, nes.cpu.registers.X);
    EXPECT_EQ(0, nes.cpu.registers.Y);
    EXPECT_EQ(0b00100100, nes.cpu.registers.P);
    EXPECT_EQ((nes.ram[0xFFFC] | nes.ram[0xFFFD] << 8), nes.cpu.registers.PC);
    EXPECT_EQ(0xFF, nes.cpu.registers.SP);
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

///////////////////////////////////////////////////////////////////////////////
// SBC Tests
///////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, SBC_ZeroAndZero)
{
    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 0;
    u8 value = 0;
    nes.cpu.SBC(value);
    EXPECT_EQ(0,     nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, SBC_ZeroAndOne)
{
    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 0;
    u8 value = 1;
    nes.cpu.SBC(value);
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, SBC_WithCarry)
{
    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 1;

    u8 value = 0;
    nes.cpu.SBC(value);
     EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
}

TEST_F(CPUTest, SBC_Overflow)
{
    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 0x80;

    u8 value = 1;
    nes.cpu.SBC(value);
     EXPECT_EQ(0x7F, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::V>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
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

TEST_F(CPUTest, ROL_ACC_WithCarry)
{
    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 0xFE;
    nes.cpu.ROL_ACC();
    
    EXPECT_EQ(0xFD, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ROL_ACC_WithoutCarry)
{
    nes.cpu.Set<Cpu::Flags::C>(false);
    nes.cpu.registers.A = 0xFE;
    nes.cpu.ROL_ACC();
    
    EXPECT_EQ(0xFC, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ROL_ACC_WithCarryNegative)
{
    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 0x7F;
    nes.cpu.ROL_ACC();
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ROL_ACC_WithCarryPositive)
{
    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 0xBF;
    nes.cpu.ROL_ACC();
    
    EXPECT_EQ(0x7F, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ROL_ACC_Zero)
{
    nes.cpu.Set<Cpu::Flags::C>(false);
    nes.cpu.registers.A = 0x80;
    nes.cpu.ROL_ACC();
    
    EXPECT_EQ(0x0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ROR_ACC_WithCarry)
{
    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 0xFE;
    nes.cpu.ROR_ACC();
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ROR_ACC_WithoutCarry)
{
    nes.cpu.Set<Cpu::Flags::C>(false);
    nes.cpu.registers.A = 0xFE;
    nes.cpu.ROR_ACC();
    
    EXPECT_EQ(0x7F, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ROR_ACC_WithCarryNegative)
{
    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 0x7F;
    nes.cpu.ROR_ACC();
    
    EXPECT_EQ(0xBF, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ROR_ACC_WithCarryPositive)
{
    nes.cpu.Set<Cpu::Flags::C>(true);
    nes.cpu.registers.A = 0xBF;
    nes.cpu.ROR_ACC();
    
    EXPECT_EQ(0xDF, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ROR_ACC_Zero)
{
    nes.cpu.Set<Cpu::Flags::C>(false);
    nes.cpu.registers.A = 0x01;
    nes.cpu.ROR_ACC();
    
    EXPECT_EQ(0x0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TAY_Zero)
{
    nes.cpu.registers.A = 0;
    nes.cpu.TAY();
    
    EXPECT_EQ(0, nes.cpu.registers.Y);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TAY_One)
{
    nes.cpu.registers.A = 0x01;
    nes.cpu.TAY();
    
    EXPECT_EQ(0x01, nes.cpu.registers.Y);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TAY_Negative)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.TAY();
    
    EXPECT_EQ(0xFF, nes.cpu.registers.Y);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TYA_Zero)
{
    nes.cpu.registers.Y = 0;
    nes.cpu.TYA();
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TYA_One)
{
    nes.cpu.registers.Y = 0x01;
    nes.cpu.TYA();
    
    EXPECT_EQ(0x01, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TYA_Negative)
{
    nes.cpu.registers.Y = 0xFF;
    nes.cpu.TYA();
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TAX_Zero)
{
    nes.cpu.registers.A = 0;
    nes.cpu.TAX();
    
    EXPECT_EQ(0, nes.cpu.registers.X);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TAX_One)
{
    nes.cpu.registers.A = 0x01;
    nes.cpu.TAX();
    
    EXPECT_EQ(0x01, nes.cpu.registers.X);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TAX_Negative)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.TAX();
    
    EXPECT_EQ(0xFF, nes.cpu.registers.X);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TXA_Zero)
{
    nes.cpu.registers.X = 0;
    nes.cpu.TXA();
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TXA_One)
{
    nes.cpu.registers.X = 0x01;
    nes.cpu.TXA();
    
    EXPECT_EQ(0x01, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, TXA_Negative)
{
    nes.cpu.registers.X = 0xFF;
    nes.cpu.TXA();
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ASL_ACC_WithCarry)
{
    nes.cpu.registers.A = 0xFE;
    nes.cpu.ASL_ACC();
    
    EXPECT_EQ(0xFC, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ASL_ACC_WithCarryNegative)
{
    nes.cpu.registers.A = 0x7F;
    nes.cpu.ASL_ACC();
    
    EXPECT_EQ(0xFE, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ASL_ACC_WithCarryPositive)
{
    nes.cpu.registers.A = 0xBF;
    nes.cpu.ASL_ACC();
    
    EXPECT_EQ(0x7E, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ASL_ACC_Zero)
{
    nes.cpu.registers.A = 0x80;
    nes.cpu.ASL_ACC();
    
    EXPECT_EQ(0x0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, LSR_ACC_WithCarry)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.LSR_ACC();
    
    EXPECT_EQ(0x7F, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, LSR_ACC_WithoutCarry)
{
    nes.cpu.registers.A = 0xFE;
    nes.cpu.LSR_ACC();
    
    EXPECT_EQ(0x7F, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, LSR_ACC_Zero)
{
    nes.cpu.registers.A = 0x01;
    nes.cpu.LSR_ACC();
    
    EXPECT_EQ(0x0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ORA_Zero)
{
    nes.cpu.registers.A = 0;
    nes.cpu.ORA(0);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ORA_One)
{
    nes.cpu.registers.A = 1;
    nes.cpu.ORA(1);
    
    EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ORA_Zero_One)
{
    nes.cpu.registers.A = 0;
    nes.cpu.ORA(1);
    
    EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ORA_One_Zero)
{
    nes.cpu.registers.A = 1;
    nes.cpu.ORA(0);
    
    EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ORA_MinusOne_Zero)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.ORA(0);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ORA_Zero_MinusOne)
{
    nes.cpu.registers.A = 0;
    nes.cpu.ORA(0xFF);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, ORA_MinusOne_MinusOne)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.ORA(0xFF);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_Zero)
{
    nes.cpu.registers.A = 0;
    nes.cpu.AND(0);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_One)
{
    nes.cpu.registers.A = 1;
    nes.cpu.AND(1);
    
    EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_Zero_One)
{
    nes.cpu.registers.A = 0;
    nes.cpu.AND(1);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_One_Zero)
{
    nes.cpu.registers.A = 1;
    nes.cpu.AND(0);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_MinusOne_Zero)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.AND(0);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_Zero_MinusOne)
{
    nes.cpu.registers.A = 0;
    nes.cpu.AND(0xFF);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_MinusOne_MinusOne)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.AND(0xFF);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, EOR_Zero)
{
    nes.cpu.registers.A = 0;
    nes.cpu.EOR(0);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, EOR_One)
{
    nes.cpu.registers.A = 1;
    nes.cpu.EOR(1);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, EOR_Zero_One)
{
    nes.cpu.registers.A = 0;
    nes.cpu.EOR(1);
    
    EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, EOR_One_Zero)
{
    nes.cpu.registers.A = 1;
    nes.cpu.EOR(0);
    
    EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, EOR_MinusOne_Zero)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.EOR(0);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, EOR_Zero_MinusOne)
{
    nes.cpu.registers.A = 0;
    nes.cpu.EOR(0xFF);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, EOR_MinusOne_MinusOne)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.EOR(0xFF);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CMP_Zero)
{
    nes.cpu.registers.A = 0;
    nes.cpu.CMP(0);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CMP_One)
{
    nes.cpu.registers.A = 1;
    nes.cpu.CMP(1);
    
    EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CMP_Zero_One)
{
    nes.cpu.registers.A = 0;
    nes.cpu.CMP(1);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CMP_One_Zero)
{
    nes.cpu.registers.A = 1;
    nes.cpu.CMP(0);
    
    EXPECT_EQ(1, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CMP_MinusOne_Zero)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.CMP(0);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CMP_Zero_MinusOne)
{
    nes.cpu.registers.A = 0;
    nes.cpu.CMP(0xFF);
    
    EXPECT_EQ(0, nes.cpu.registers.A);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CMP_MinusOne_MinusOne)
{
    nes.cpu.registers.A = 0xFF;
    nes.cpu.CMP(0xFF);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.A);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPX_Zero)
{
    nes.cpu.registers.X = 0;
    nes.cpu.CPX(0);
    
    EXPECT_EQ(0, nes.cpu.registers.X);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPX_One)
{
    nes.cpu.registers.X = 1;
    nes.cpu.CPX(1);
    
    EXPECT_EQ(1, nes.cpu.registers.X);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPX_Zero_One)
{
    nes.cpu.registers.X = 0;
    nes.cpu.CPX(1);
    
    EXPECT_EQ(0, nes.cpu.registers.X);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPX_One_Zero)
{
    nes.cpu.registers.X = 1;
    nes.cpu.CPX(0);
    
    EXPECT_EQ(1, nes.cpu.registers.X);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPX_MinusOne_Zero)
{
    nes.cpu.registers.X = 0xFF;
    nes.cpu.CPX(0);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.X);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPX_Zero_MinusOne)
{
    nes.cpu.registers.X = 0;
    nes.cpu.CPX(0xFF);
    
    EXPECT_EQ(0, nes.cpu.registers.X);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPX_MinusOne_MinusOne)
{
    nes.cpu.registers.X = 0xFF;
    nes.cpu.CPX(0xFF);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.X);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPY_Zero)
{
    nes.cpu.registers.Y = 0;
    nes.cpu.CPY(0);
    
    EXPECT_EQ(0, nes.cpu.registers.Y);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPY_One)
{
    nes.cpu.registers.Y = 1;
    nes.cpu.CPY(1);
    
    EXPECT_EQ(1, nes.cpu.registers.Y);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPY_Zero_One)
{
    nes.cpu.registers.Y = 0;
    nes.cpu.CPY(1);
    
    EXPECT_EQ(0, nes.cpu.registers.Y);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPY_One_Zero)
{
    nes.cpu.registers.Y = 1;
    nes.cpu.CPY(0);
    
    EXPECT_EQ(1, nes.cpu.registers.Y);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPY_MinusOne_Zero)
{
    nes.cpu.registers.Y = 0xFF;
    nes.cpu.CPY(0);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.Y);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPY_Zero_MinusOne)
{
    nes.cpu.registers.Y = 0;
    nes.cpu.CPY(0xFF);
    
    EXPECT_EQ(0, nes.cpu.registers.Y);
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, CPY_MinusOne_MinusOne)
{
    nes.cpu.registers.Y = 0xFF;
    nes.cpu.CPY(0xFF);
    
    EXPECT_EQ(0xFF, nes.cpu.registers.Y);
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::C>());
    EXPECT_TRUE(nes.cpu.Get<Cpu::Flags::Z>());
    EXPECT_FALSE(nes.cpu.Get<Cpu::Flags::S>());
}