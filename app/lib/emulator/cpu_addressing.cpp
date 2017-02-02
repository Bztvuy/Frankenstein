/* 
 * File:   Addressing.cpp
 * Author: Maxime Goyette <maxime.g.goyette@usherbrooke.ca>
 * 
 * Created on 2017-01-01, 12:34
 * 
 * The functions describing the numerous NES addressing mode. 
 */

#include "cpu.h"

/**
 * Build an address using only the least significative byte. The first half of 
 * the address is assumed to be zero.
 * 
 * @param low the least significant byte of the address
 * @return the two bytes address
 */
u16 Cpu::FromValues(const u8 low) {
    u16 res = 0 | low;
    return res;
}

/**
 * Build an address using the most and the least significative bytes. Useful 
 * because the memory is stored as little endian.
 * 
 * @param low  the least significant byte of the address
 * @param high the most significant byte of the address
 * @return the two bytes address
 */
u16 Cpu::FromValues(const u8 low, const u8 high) {
    u16 res = high;
    res <<= 8;
    res |= low;
    return res;
}

/**
 * A literal value.
 * @param value the value to return
 * @return the value
 */
u8 Cpu::Immediate(const u8 value) const {
    return value;
}

/**
 * Build the address [00][low]. 
 * @param low the least significant byte of the address
 * @return the builded address
 */
u16 Cpu::ZeroPage(const u8 low) const {
    return FromValues(low);
}

/**
 * Build the address [high][low].
 * @param low  the least significant byte of the address
 * @param high the most significant byte of the address
 * @return the builded address
 */
u16 Cpu::Absolute(const u8 low, const u8 high) const {
    return FromValues(low, high);
}

/**
 * Compute the complete address [high][low] + index
 * @param low   the least significant byte of the address
 * @param high  the most significant byte of the address
 * @param index a value to add to the constructed address
 * @return the computed address
 */
u16 Cpu::Indexed(const u8 low, const u8 high, const u8 index) const {
    return FromValues(low, high) + index;
}

/**
 * Compute the ZeroPage address [00][low] + index.
 * @param low   the least significant byte of the address
 * @param index a value to add to the constructed address
 * @return the computed address
 */
u16 Cpu::ZeroPageIndexed(const u8 low, const u8 index) const {
    return FromValues(low) + index;
}

/**
 * Fetch an address at memory [high][low].
 * @param  low  the least significant byte 
 * @param  high the most significant byte
 * @return the stored address
 */
u16 Cpu::Indirect(const u8 low, const u8 high) const {
    auto memAddrAddr = FromValues(low, high);
    auto valLow = this->memory->raw[memAddrAddr];
    auto valHigh = this->memory->raw[memAddrAddr+1];
    return FromValues(valLow, valHigh);
}

/**
 * Fetch an address at memory [00][low + index]. If [low + index] overflow, only
 * the lowest byte is kept.
 * 
 * @param  low   ZeroPage address of the memory containing the address
 * @param  index a value to add to low to get the actual address
 * @return the pre-indexed address
 */
u16 Cpu::PreIndexedIndirect(const u8 low, const u8 index) const {
    return Indirect((low + index) % (0xFF+1), 0);
}

/**
 * Fetch an address at memory [00][low] then increase the address by index.
 * @param  low   ZeroPage address of the memory containing the address 
 * @param  index an index to add to the loaded address. Usually a register [X|Y]
 * @return the post-indexed address
 */
u16 Cpu::PostIndexedIndirect(const u8 low, const u8 index) const {
    return Indirect(low, 0) + index;
}