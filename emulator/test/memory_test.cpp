#include "common.h"

using namespace Frankenstein;

////////////////////////////////////////////////////////////////////////////////
// Adressing Modes Tests
////////////////////////////////////////////////////////////////////////////////
TEST_F(MemoryTest, Addressing_ZeroPage)
{
    auto addr = NesMemory::ZeroPage(0x00);
    EXPECT_EQ((u16)0x0000, addr);

    addr = NesMemory::ZeroPage(0x10);
    EXPECT_EQ((u16)0x0010, addr);

    addr = NesMemory::ZeroPage(0xFF);
    EXPECT_EQ((u16)0x00FF, addr);
}

TEST_F(MemoryTest, Addressing_ZeroPageIndexed)
{
    auto addr = NesMemory::ZeroPageIndexed(0x00, 0x01);
    EXPECT_EQ((u16)0x0001, addr);

    addr = NesMemory::ZeroPageIndexed(0x0F, 0x01);
    EXPECT_EQ((u16)0x0010, addr);

    addr = NesMemory::ZeroPageIndexed(0x00, 0x01);
    EXPECT_EQ((u16)0x0001, addr);
}

TEST_F(MemoryTest, Addressing_Absolute)
{
    auto addr = NesMemory::Absolute(0x00, 0x01);
    EXPECT_EQ((u16)0x0100, addr);

    addr = NesMemory::Absolute(0x0F, 0x01);
    EXPECT_EQ((u16)0x010F, addr);

    addr = NesMemory::Absolute(0xFF, 0xFF);
    EXPECT_EQ((u16)0xFFFF, addr);
}

TEST_F(MemoryTest, Addressing_Indexed)
{
    auto addr = NesMemory::Indexed(0x00, 0x01, 0x01);
    EXPECT_EQ((u16)0x0101, addr);

    addr = NesMemory::Indexed(0xFF, 0x01, 0x01);
    EXPECT_EQ((u16)0x0200, addr);

    addr = NesMemory::Indexed(0xFF, 0xFF, 0x01);
    EXPECT_EQ((u16)0x0000, addr);
}

TEST_F(MemoryTest, Addressing_Indirect)
{
    ram[0x0000] = 0x10;
    ram[0x0001] = 0x01;
    ram[0x0002] = 0x02;
    ram[0x00FF] = 0xFF;

    auto addr = ram.Indirect(0x01, 0x00);
    EXPECT_EQ((u16)0x0201, addr);

    addr = ram.Indirect(0xFF, 0x00);
    EXPECT_EQ((u16)0x10FF, addr);
}

TEST_F(MemoryTest, Addressing_PostIndexedIndirect)
{
    ram[0x0000] = 0x01;
    ram[0x0001] = 0x02;
    ram[0x00FF] = 0xFF;

    auto addr = ram.PostIndexedIndirect(0x00, 0x00);
    EXPECT_EQ((u16)0x0201, addr);

    addr = ram.PostIndexedIndirect(0x00, 0x01);
    EXPECT_EQ((u16)0x0202, addr);

    addr = ram.PostIndexedIndirect(0xFF, 0x00);
    EXPECT_EQ((u16)0x01FF, addr);

    addr = ram.PostIndexedIndirect(0xFF, 0x01);
    EXPECT_EQ((u16)0x0200, addr);
}

TEST_F(MemoryTest, Addressing_PreIndexedIndirect)
{
    ram[0x0000] = 0x01;
    ram[0x0001] = 0x02;
    ram[0x0002] = 0x03;
    ram[0x00FF] = 0xFF;

    auto addr = ram.PreIndexedIndirect(0x00, 0x00);
    EXPECT_EQ((u16)0x0201, addr);

    addr = ram.PreIndexedIndirect(0x00, 0x01);
    EXPECT_EQ((u16)0x0302, addr);

    addr = ram.PreIndexedIndirect(0xFF, 0x00);
    EXPECT_EQ((u16)0x01FF, addr);

    addr = ram.PreIndexedIndirect(0xFF, 0x01);
    EXPECT_EQ((u16)0x0201, addr);
}
