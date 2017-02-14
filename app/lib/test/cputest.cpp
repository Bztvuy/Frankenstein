#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <emulator/cpu.h>


TEST_CASE( "ADC", "[6502 instruction set]" ) {
    Cpu* cpu = new Cpu;
    
    SECTION( "without addressing modes" ){
        SECTION( "basic test" ){
            cpu->registers->A = 1;
            cpu->ADC(1);
            REQUIRE(cpu->registers->A == 2);
            REQUIRE(cpu->GetFlag(Z) == 0);
            REQUIRE(cpu->GetFlag(S) == 0);
            REQUIRE(cpu->GetFlag(V) == 0);
            REQUIRE(cpu->GetFlag(C) == 0);
        }
    }
}