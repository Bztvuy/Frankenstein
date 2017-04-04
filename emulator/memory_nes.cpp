#include "dependencies.h"
#include "nes.h"

namespace Frankenstein {
    
template<>
NesMemory::Memory(Nes& pNes) : nes(pNes) {}

template <>
const NesMemory::Ref NesMemory::operator[](const u16 addr)
{
    return Ref(addr, this);
}

template <>
const u8 NesMemory::Read(const u16 address)
{ 
    // $0000-$07FF; With mirrors $0800-$0FFF, $1000-$17FF, $1800-$1FFF; Internal RAM
    if (address < 0x2000) {
        return raw[address & 0x07FF];
    }
    // $2000-$2007; With mirrors $2008-$3FFF; NES PPU registers
    else if (address < 0x4000) {
        return nes.ppu.readRegister(address & 0x2007);
    }
    // $4014; PPU DMA
    else if (address == 0x4014){
	nes.ppu.readRegister(address);
    }
    // $4000-$4017; NES APU and I/O registers
    else if (address < 0x4018) {
        if(address == 0x4016) {
            return nes.pad1.Read(raw[address], raw[address]);
        }
        else if (address == 0x4017) {
            return nes.pad2.Read(raw[address], raw[address-1]);
        }
    }
    // $4018-$401F; APU and I/O functionality that is normally disabled.
    //else if (address < 0x4020) {
    //
    //}
    // $4020-$FFFF; Cartridge space: PRG ROM, PRG RAM, and mapper registers
    //else {
    //    return raw[address];
    //}
    return raw[address];
}

template <>
void NesMemory::Write(const u16 address, const u8 val)
{
    // $0800-$0FFF, $1000-$17FF, $1800-$1FFF are mirrors of $0000-$07FF; Internal RAM
    if (address < 0x2000) {
        raw[address & 0x07FF] = val;
    }
    //$2008-$3FFF are Mirrors of $2000-2007; NES PPU registers
    else if (address < 0x4000) {
        nes.ppu.writeRegister(address & 0x2007, val);
    }
    // $4014; PPU DMA
    else if (address == 0x4014){
	nes.ppu.writeRegister(address, val);
    }
    // $4000-$4017; NES APU and I/O registers
    else if (address < 0x4017) {
        if(address == 0x4016) {
            nes.pad1.Write(raw[address]);
            nes.pad2.Write(raw[address]);
        }
        else if (address == 0x4017) {
            // ignore?
        }
    }
    else {
        raw[address] = val;
    }
}

template <>
void NesMemory::Copy(const u8* source, const u16 destination, const unsigned int size)
{
    memcpy(&raw[destination], source, size);
}

template <>
bool NesMemory::IsPageCrossed(u16 startAddress, u16 endAddress)
{
    return (startAddress & 0xFF00) != (endAddress & 0xFF00);
}

template <>
u16 NesMemory::FromValues(const u8 low)
{
    u16 res = 0 | low;
    return res;
}

template <>
u16 NesMemory::FromValues(const u8 low, const u8 high)
{
    u16 res = high;
    res <<= 8;
    res |= low;
    return res;
}

template <>
u16 NesMemory::ZeroPage(const u8 low)
{
    return FromValues(low);
}

template <>
u16 NesMemory::Absolute(const u8 low, const u8 high)
{
    return FromValues(low, high);
}

template <>
u16 NesMemory::Indexed(const u8 low, const u8 high, const u8 index)
{
    return FromValues(low, high) + index;
}

template <>
u16 NesMemory::ZeroPageIndexed(const u8 low, const u8 index)
{
    return FromValues(low + index);
}

template <>
u16 NesMemory::Indirect(const u8 low, const u8 high)
{
    auto valLow = raw[FromValues(low, high)];
    auto valHigh = raw[FromValues((low + 1) % 0x100, high)];
    return FromValues(valLow, valHigh);
}

template <>
u16 NesMemory::PreIndexedIndirect(const u8 low, const u8 index)
{
    return Indirect((low + index) % (0x100), 0);
}

template <>
u16 NesMemory::PostIndexedIndirect(const u8 low, const u8 index)
{
    return Indirect(low, 0) + index;
}

template <>
template <NesMemory::Addressing N>
const NesMemory::Ref NesMemory::Get(const u8)
{
    static_assert(sizeof(N) >= 0, "failure to specialize template: please use a valid Addressing mode");
    return this->operator[](0);
}

template <>
template <NesMemory::Addressing N>
const NesMemory::Ref NesMemory::Get(const u8, const u8)
{
    static_assert(sizeof(N) >= 0, "failure to specialize template: please use a valid Addressing mode");
    return this->operator[](0);
}

template <>
template <NesMemory::Addressing N>
const NesMemory::Ref NesMemory::Get(const u8, const u8, const u8)
{
    static_assert(sizeof(N) >= 0, "failure to specialize template: please use a valid Addressing mode");
    return this->operator[](0);
}

template <>
template <>
const NesMemory::Ref NesMemory::Get<NesMemory::Addressing::Absolute>(const u8 low, const u8 high)
{
    return this->operator[](Absolute(low, high));
}

template <>
template <>
const NesMemory::Ref NesMemory::Get<NesMemory::Addressing::ZeroPage>(const u8 low)
{
    return this->operator[](ZeroPage(low));
}

template <>
template <>
const NesMemory::Ref NesMemory::Get<NesMemory::Addressing::Indexed>(const u8 low, const u8 high, const u8 index)
{
    return this->operator[](Indexed(low, high, index));
}

template <>
template <>
const NesMemory::Ref NesMemory::Get<NesMemory::Addressing::ZeroPageIndexed>(const u8 low, const u8 index)
{
    return this->operator[](ZeroPageIndexed(low, index));
}

template <>
template <>
const NesMemory::Ref NesMemory::Get<NesMemory::Addressing::Indirect>(const u8 low, const u8 high)
{
    return this->operator[](Indirect(low, high));
}

template <>
template <>
const NesMemory::Ref NesMemory::Get<NesMemory::Addressing::PreIndexedIndirect>(const u8 low, const u8 index)
{
    return this->operator[](NesMemory::PreIndexedIndirect(low, index));
}

template <>
template <>
const NesMemory::Ref NesMemory::Get<NesMemory::Addressing::PostIndexedIndirect>(const u8 low, const u8 index)
{
    return this->operator[](PostIndexedIndirect(low, index));
}
}
