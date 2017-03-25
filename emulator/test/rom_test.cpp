#include "common.h"

using namespace Frankenstein;

TEST_F(RomTest, CanLoadBasicRom)
{
    FileRom rom("./roms/01-basics.nes");
    ASSERT_EQ(0, rom.GetTrainerOffset());
}
