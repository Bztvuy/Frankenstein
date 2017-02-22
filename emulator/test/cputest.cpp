#include "catch.hpp"
#include <cpu.h>

TEST_CASE( "ADC", "[6502 instruction set]" ) {
    Cpu* cpu = new Cpu;
    
    SECTION( "without addressing modes" ){
        SECTION( "basic test" ){
            cpu->registers.A = 1;
	    auto value = u8(1);
            cpu->ADC(value);
            REQUIRE(cpu->registers.A == 2);
            REQUIRE(cpu->GetFlag(cpu->Z) == 0);
            REQUIRE(cpu->GetFlag(cpu->S) == 0);
            REQUIRE(cpu->GetFlag(cpu->V) == 0);
            REQUIRE(cpu->GetFlag(cpu->C) == 0);
        }
    }
}