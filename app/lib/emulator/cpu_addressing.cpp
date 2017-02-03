/* 
 * File:   Addressing.cpp
 * Author: Maxime Goyette <maxime.g.goyette@usherbrooke.ca>
 * 
 * Created on 2017-01-01, 12:34
 * 
 * The functions describing the numerous NES addressing mode. 
 */

#include "cpu.h"

u16 Cpu::FromValues(const u8 low) const {
    u16 res = 0 | low;
    return res;
}

u16 Cpu::FromValues(const u8 low, const u8 high) const {
    u16 res = high;
    res <<= 8;
    res |= low;
    return res;
}

u8 Cpu::Immediate(const u8 value) const {
    return value;
}

u16 Cpu::ZeroPage(const u8 low) const {
    return FromValues(low);
}

u16 Cpu::Absolute(const u8 low, const u8 high) const {
    return FromValues(low, high);
}

u16 Cpu::Indexed(const u8 low, const u8 high, const u8 index) const {
    return FromValues(low, high) + index;
}

u16 Cpu::ZeroPageIndexed(const u8 low, const u8 index) const {
    return FromValues(low) + index;
}

u16 Cpu::Indirect(const u8 low, const u8 high) const {
    auto memAddrAddr = FromValues(low, high);
    auto valLow = this->memory->raw[memAddrAddr];
    auto valHigh = this->memory->raw[memAddrAddr+1];
    return FromValues(valLow, valHigh);
}

u16 Cpu::PreIndexedIndirect(const u8 low, const u8 index) const {
    return Indirect((low + index) % (0xFF+1), 0);
}

u16 Cpu::PostIndexedIndirect(const u8 low, const u8 index) const {
    return Indirect(low, 0) + index;
}