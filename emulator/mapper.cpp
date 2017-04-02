#include "mapper.h"

using namespace Frankenstein;

/**********************************************/
/***************** MAPPER 1 *******************/
/**********************************************/

u8 Mapper1::Read(u16 address){
    return 0;
}

void Mapper1::Write(u16 address, u8 value) {

}

void Mapper1::Step() {

}

Mapper1::~Mapper1() {}

/**********************************************/
/***************** MAPPER 2 *******************/
/**********************************************/

u8 Mapper2::Read(u16 address){
    u32 index;
    if (address < 0x2000) {
	return rom.GetCHR()[address];
    } else if (address >= 0xC000) {
	index = prgBank2*0x4000 + u32(address-0xC000);
	return rom.GetPRG()[index];
    } else if (address >= 0x8000) {
	index = prgBank1*0x4000 + u32(address-0x8000);
	return rom.GetPRG()[index];
    } else if (address >= 0x6000) {
	index = u32(address) - 0x6000;
	return rom.GetSRAM()[index];
    } else {
	return 0;
    }
}

void Mapper2::Write(u16 address, u8 value) {
    u32 index;
    if (address < 0x2000) {
	rom.GetCHR()[address] = value;
    } else if (address >= 0x8000) {
	prgBank1 = u32(value) % prgBanks;
    } else if (address >= 0x6000) {
	index = u32(address) - 0x6000;
	rom.GetSRAM()[index] = value;;
    }
}

void Mapper2::Step() {
    //nothing to do
}

Mapper2::Mapper2(IRom& pRom) : rom(pRom){
    prgBanks = rom.GetHeader().prgRomBanks;
    prgBank1 = 0;
    prgBank2 = prgBanks - 1;
}

Mapper2::~Mapper2() {}

/**********************************************/
/***************** MAPPER 3 *******************/
/**********************************************/

u8 Mapper3::Read(u16 address){
    return 0;
}

void Mapper3::Write(u16 address, u8 value) {

}

void Mapper3::Step() {

}

Mapper3::~Mapper3() {}

/**********************************************/
/***************** MAPPER 4 *******************/
/**********************************************/

u8 Mapper4::Read(u16 address){
    return 0;
}

void Mapper4::Write(u16 address, u8 value) {

}

void Mapper4::Step() {

}

Mapper4::~Mapper4() {}

/**********************************************/
/***************** MAPPER 7 *******************/
/**********************************************/

u8 Mapper7::Read(u16 address){
    return 0;
}

void Mapper7::Write(u16 address, u8 value) {

}

void Mapper7::Step() {

}

Mapper7::~Mapper7() {}
