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

TEST_F(CPUTest, ADC)
{
    cpu->registers.A = 1;
    u8 value = 1;
    cpu->ADC(value);
    EXPECT_EQ(2, cpu->registers.A);
    EXPECT_EQ(0, cpu->GetFlag(cpu->Z));
    EXPECT_EQ(0, cpu->GetFlag(cpu->S));
    EXPECT_EQ(0, cpu->GetFlag(cpu->V));
    EXPECT_EQ(0, cpu->GetFlag(cpu->C));
}