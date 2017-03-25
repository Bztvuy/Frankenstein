#include "common.h"

#include "rom_file.h"

using namespace Frankenstein;

TEST_F(RomTest, CanLoadBasicRom)
{
    FileRom rom("./roms/01-basics.nes");
    ASSERT_EQ((int)0, (int)rom.GetTrainerOffset());
}
