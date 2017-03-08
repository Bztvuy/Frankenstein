#include "common.h"

using namespace Frankenstein;

TEST_F(RomTest, CanLoadBasicRom)
{
    FileRom rom("./rom_singles/01-basics.nes");
    ASSERT_EQ(0, rom.GetTrainerOffset());
}

TEST_F(RomTest, CanCopyToCPU)
{
    FileRom rom("./rom_singles/01-basics.nes");
    cpu.LoadRom(rom);
    ASSERT_EQ()
}