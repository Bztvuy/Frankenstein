/* 
 * File:   Addressing.cpp
 * Author: dinf0014_admin
 * 
 * Created on 1 février 2017, 12:34
 */

#include "cpu.h"


inline u16 Cpu::FromValues(const u8 low) {
    u16 res = 0 | low;
    return res;
}

inline u16 Cpu::FromValues(const u8 low, const u8 high) {
    u16 res = high;
    res <<= 8;
    res |= low;
    return res;
}

// Litteral value 
u8 Cpu::Immediate(const u8 value) const {
    return FromValues(value);
}

// Memory address [00][low]
u16 Cpu::ZeroPage(const u8 low) const {
    return FromValues(low);
}

// Memory address [high][low]
u16 Cpu::Absolute(const u8 low, const u8 high) const {
    return FromValues(low, high);
}

// Memory address [high][low] + register
u16 Cpu::Indexed(const u8 low, const u8 high, const u8 const& reg) const {
    return FromValues(low, high) + reg;
}

// Memory address [00][low] + register
u16 Cpu::ZeroPageIndexed(const u8 low, const u8 const& reg)  const {
    return FromValues(low) + reg;
}

// Memory address [??][??] at memory [high][low]
u16 Cpu::Indirect(const u8 low, const u8 high) const {
    auto memAddrAddr = FromValues(low, high);
    auto valLow = this->memory->raw[memAddrAddr];
    auto valHigh = this->memory->raw[memAddrAddr+1];
    return FromValues(valLow, valHigh);
}

// Memory address [??][??] at memory [00][low + reg]
u16 Cpu::PreIndexedIndirect(const u8 low, const u8 const& reg) const {
    return Indirect((low + reg) % (0xFF+1), 0);
}

// Memory address [??][??] + reg at memory [00][low]
u16 Cpu::PostIndexedIndirect(const u8 low, const u8 const& reg) const {
    return Indirect(low, 0) + reg;
}