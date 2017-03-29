#include <fstream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <bitset>

#include "nes.h"
#include "cpu.h"
#include "memory.h"
#include "rom_file.h"

int main(int argc, char* argv[])
{
    std::string file(argv[1]);
    Frankenstein::FileRom rom(file);
    Frankenstein::Nes nes(rom);

    std::ofstream out("debug2.txt", std::ios::out | std::ios::binary);
    out << "EX.TIME|PC  |SVABDIZC|A |X |Y |Instruction| Hex data" << std::endl;

    bool isTestDone = false;

    while (!isTestDone)
    {
        std::stringstream instString;
        auto op = nes.cpu.OpCode();
        auto instr = nes.cpu.instructions[op];
        instString << std::setfill('0') << std::setw(4) << std::hex
                   << (unsigned int)nes.cpu.registers.PC << "|";
        instString << std::bitset<8>(nes.cpu.registers.P) << "|";
        instString << std::setfill('0') << std::setw(2) << std::hex
                   << (unsigned int)nes.cpu.registers.A << "|";
        instString << std::setfill('0') << std::setw(2) << std::hex
                   << (unsigned int)nes.cpu.registers.X << "|";
        instString << std::setfill('0') << std::setw(2) << std::hex
                   << (unsigned int)nes.cpu.registers.Y << "|";
        instString << std::setfill(' ') << std::setw(11)
                   << instr.name << "| ";

        int max = instr.size == 0 ? 3 : instr.size;
        for(int i = 0; i < max; i++) {
            instString << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)nes.cpu.Operand(i);
            instString << " ";
        }

        auto begin = std::chrono::high_resolution_clock::now();
        nes.Step();
        auto end = std::chrono::high_resolution_clock::now();

        out << std::setfill(' ') << std::setw(5) << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << "ns|";
        out << instString.str();
        out << std::endl;


        // The test status is written to $6000. $80 means the test is running, $81
        // means the test needs the reset button pressed, but delayed by at least
        // 100 msec from now. $00-$7F means the test has completed and given that
        // result code.
        // To allow an emulator to know when one of these tests is running and the
        // data at $6000+ is valid, as opposed to some other NES program, $DE $B0
        // $G1 is written to $6001-$6003.$DE $B0 $G1 is written to $6001-$6003.
        if(nes.ram[0x6000] <  0x80 &&
           nes.ram[0x6001] == 0xDE &&
           nes.ram[0x6002] == 0xB0 &&
           nes.ram[0x6003] == 0x61) {

            out << "Test Done";
            out << "\nStatus: " << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)(char)nes.ram[0x6000];
            out << "\n";

            int i = 0;
            char c;
            do {
                c = nes.ram[0x6004+i];
                out << c;
                i++;
            }
            while(c != '\0');

            isTestDone = true;
        }
    }

    return 0;
}
