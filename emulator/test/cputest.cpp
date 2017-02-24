#include <gtest/gtest.h>
#include <cpu.h>

struct CPUTest : testing::Test
{
  Cpu* cpu;
  CPUTest()
  {
    cpu = new Cpu;
  }
  virtual ~CPUTest()
  {
    delete cpu;
  }
};

TEST_F(CPUTest, DefaultState) {
    EXPECT_EQ(0, cpu->registers.A);
    EXPECT_EQ(0, cpu->registers.X);
    EXPECT_EQ(0, cpu->registers.Y);
    EXPECT_EQ(0b00100000, cpu->registers.P);
    EXPECT_EQ(0, cpu->registers.PC);
    EXPECT_EQ(0xFF, cpu->registers.SP);
    EXPECT_EQ(0, cpu->Get<Cpu::Flags::Z>());
    EXPECT_EQ(0, cpu->Get<Cpu::Flags::S>());
    EXPECT_EQ(0, cpu->Get<Cpu::Flags::V>());
    EXPECT_EQ(0, cpu->Get<Cpu::Flags::C>());
}

////////////////////////////////////////////////////////////////////////////////
// Adressing Modes Tests
////////////////////////////////////////////////////////////////////////////////
TEST_F(CPUTest, Addressing_ZeroPage) {
    auto addr = cpu->ZeroPage(0x00);
    EXPECT_EQ((u16)0x0000, addr);
    
    addr = cpu->ZeroPage(0x10);
    EXPECT_EQ((u16)0x0010, addr);
    
    addr = cpu->ZeroPage(0xFF);
    EXPECT_EQ((u16)0x00FF, addr);    
}

TEST_F(CPUTest, Addressing_ZeroPageIndexed) {
    auto addr = cpu->ZeroPageIndexed(0x00, 0x01);
    EXPECT_EQ((u16)0x0001, addr);
    
    addr = cpu->ZeroPageIndexed(0x0F, 0x01);
    EXPECT_EQ((u16)0x0010, addr);
    
    addr = cpu->ZeroPageIndexed(0x00, 0x01);
    EXPECT_EQ((u16)0x0001, addr);
}

TEST_F(CPUTest, Addressing_Absolute) {
    auto addr = cpu->Absolute(0x00, 0x01);
    EXPECT_EQ((u16)0x0100, addr);
    
    addr = cpu->Absolute(0x0F, 0x01);
    EXPECT_EQ((u16)0x010F, addr);
    
    addr = cpu->Absolute(0xFF, 0xFF);
    EXPECT_EQ((u16)0xFFFF, addr);
}

TEST_F(CPUTest, Addressing_Indexed) {
    auto addr = cpu->Indexed(0x00, 0x01, 0x01);
    EXPECT_EQ((u16)0x0101, addr);
    
    addr = cpu->Indexed(0xFF, 0x01, 0x01);
    EXPECT_EQ((u16)0x0200, addr);
    
    addr = cpu->Indexed(0xFF, 0xFF, 0x01);
    EXPECT_EQ((u16)0x0000, addr);
}

TEST_F(CPUTest, Addressing_Indirect) {
    cpu->memory.raw[0x0000] = 0x10;
    cpu->memory.raw[0x0001] = 0x01;
    cpu->memory.raw[0x0002] = 0x02;
    cpu->memory.raw[0x00FF] = 0xFF;
    
    auto addr = cpu->Indirect(0x01, 0x00);
    EXPECT_EQ((u16)0x0201, addr);
    
    addr = cpu->Indirect(0xFF, 0x00);
    EXPECT_EQ((u16)0x10FF, addr);
}

TEST_F(CPUTest, Addressing_PostIndexedIndirect) {  
    cpu->memory.raw[0x0000] = 0x01;  
    cpu->memory.raw[0x0001] = 0x02;
    cpu->memory.raw[0x00FF] = 0xFF;
    
    auto addr = cpu->PostIndexedIndirect(0x00, 0x00);
    EXPECT_EQ((u16)0x0201, addr);
    
    addr = cpu->PostIndexedIndirect(0x00, 0x01);
    EXPECT_EQ((u16)0x0202, addr);
    
    addr = cpu->PostIndexedIndirect(0xFF, 0x00);
    EXPECT_EQ((u16)0x01FF, addr);
    
    addr = cpu->PostIndexedIndirect(0xFF, 0x01);
    EXPECT_EQ((u16)0x0200, addr);
}

TEST_F(CPUTest, Addressing_PreIndexedIndirect) {
    cpu->memory.raw[0x0000] = 0x01;  
    cpu->memory.raw[0x0001] = 0x02;  
    cpu->memory.raw[0x0002] = 0x03;
    cpu->memory.raw[0x00FF] = 0xFF;
    
    auto addr = cpu->PreIndexedIndirect(0x00, 0x00);
    EXPECT_EQ((u16)0x0201, addr);
    
    addr = cpu->PreIndexedIndirect(0x00, 0x01);
    EXPECT_EQ((u16)0x0302, addr);
    
    addr = cpu->PreIndexedIndirect(0xFF, 0x00);
    EXPECT_EQ((u16)0x01FF, addr);
    
    addr = cpu->PreIndexedIndirect(0xFF, 0x01);
    EXPECT_EQ((u16)0x0201, addr);
}

////////////////////////////////////////////////////////////////////////////////
// AND Tests
////////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, AND_ZeroAndOne) {
    cpu->registers.A = 0;
    u8 value = 1;
    cpu->AND(value);
    EXPECT_EQ(0, cpu->registers.A);
    EXPECT_EQ(true,  cpu->Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu->Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_OneAndZero) {
    cpu->registers.A = 1;
    u8 value = 0;
    cpu->AND(value);
    EXPECT_EQ(0, cpu->registers.A);
    EXPECT_EQ(true,  cpu->Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu->Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_OneAndOne) {
    cpu->registers.A = 1;
    u8 value = 1;
    cpu->AND(value);    
    EXPECT_EQ(1, cpu->registers.A);
    EXPECT_EQ(false, cpu->Get<Cpu::Flags::Z>());
    EXPECT_EQ(false, cpu->Get<Cpu::Flags::S>());
}

TEST_F(CPUTest, AND_NegativeResult) {
    cpu->registers.A = 0xFF;
    u8 value = 0xF0;
    cpu->AND(value);    
    EXPECT_EQ(value, cpu->registers.A);
    EXPECT_EQ(false, cpu->Get<Cpu::Flags::Z>());
    EXPECT_EQ(true,  cpu->Get<Cpu::Flags::S>());
}

////////////////////////////////////////////////////////////////////////////////
// ADC Tests
////////////////////////////////////////////////////////////////////////////////

TEST_F(CPUTest, ADC)
{
    cpu->registers.A = 1;
    u8 value = 1;
    cpu->ADC(value);
    EXPECT_EQ(2, cpu->registers.A);
    EXPECT_EQ(0, cpu->Get<Cpu::Flags::Z>());
    EXPECT_EQ(0, cpu->Get<Cpu::Flags::S>());
    EXPECT_EQ(0, cpu->Get<Cpu::Flags::V>());
    EXPECT_EQ(0, cpu->Get<Cpu::Flags::C>());
}

////////////////////////////////////////////////////////////////////////////////
// ASL Tests
////////////////////////////////////////////////////////////////////////////////