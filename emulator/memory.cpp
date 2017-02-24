#include "memory.h"

using namespace Frankenstein;

u8& Memory::operator[] (const u16 addr) {
    return raw[addr];
}
        
u8& Memory::get(const u16 addr) {
    return this->operator [](addr);
}

bool Memory::IsPageCrossed(u16 startAddress, u16 endAddress){
    return (startAddress & 0xFF00) != (endAddress & 0xFF00);
}
        
u16 Memory::FromValues(const u8 low) {
    u16 res = 0 | low;
    return res;
}

u16 Memory::FromValues(const u8 low, const u8 high) {
    u16 res = high;
    res <<= 8;
    res |= low;
    return res;
}

u16 Memory::ZeroPage(const u8 low) {
    return FromValues(low);
}

u16 Memory::Absolute(const u8 low, const u8 high) {
    return FromValues(low, high);
}

u16 Memory::Indexed(const u8 low, const u8 high, const u8 index) {
    return FromValues(low, high) + index;
}

u16 Memory::ZeroPageIndexed(const u8 low, const u8 index) {
    return FromValues(low + index);
}

u16 Memory::Indirect(const u8 low, const u8 high) {
    auto valLow = raw[FromValues(low, high)];
    auto valHigh = raw[FromValues((low+1)%0x100, high)];
    return FromValues(valLow, valHigh);
}

u16 Memory::PreIndexedIndirect(const u8 low, const u8 index) {
    return Indirect((low + index) % (0x100), 0);
}

u16 Memory::PostIndexedIndirect(const u8 low, const u8 index) {
    return Indirect(low, 0) + index;
}

namespace Frankenstein {
    template <Memory::Addressing N>
    u8& Memory::get(const u8) {
        static_assert(sizeof(N) >= 0, "failure to specialise template: please use a valid Addressing mode");
        return 0xFF;
    }

    template <Memory::Addressing N>
    u8& Memory::get(const u8, const u8) {
        static_assert(sizeof(N) >= 0, "failure to specialise template: please use a valid Addressing mode");
        return 0xFF;
    }

    template <Memory::Addressing N>
    u8& Memory::get(const u8, const u8, const u8) {
        static_assert(sizeof(N) >= 0, "failure to specialise template: please use a valid Addressing mode");
        return 0xFF;
    }

    template <>
    u8& Memory::get<Memory::Addressing::Absolute>(const u8 low, const u8 high) {
        return raw[Absolute(low, high)];
    }

    template <>
    u8& Memory::get<Memory::Addressing::ZeroPage>(const u8 low) {
        return raw[ZeroPage(low)];
    }

    template <>
    u8& Memory::get<Memory::Addressing::Indexed>(const u8 low, const u8 high, const u8 index) {
        return raw[Indexed(low, high, index)];
    }

    template <>
    u8& Memory::get<Memory::Addressing::ZeroPageIndexed>(const u8 low, const u8 index) {
        return raw[ZeroPageIndexed(low, index)];
    }

    template <>
    u8& Memory::get<Memory::Addressing::Indirect>(const u8 low, const u8 high) {
        return raw[Indirect(low, high)];
    }

    template <>
    u8& Memory::get<Memory::Addressing::PreIndexedIndirect>(const u8 low, const u8 index) {
        return raw[PreIndexedIndirect(low, index)];
    }

    template <>
    u8& Memory::get<Memory::Addressing::PostIndexedIndirect>(const u8 low, const u8 index) {
        return raw[PostIndexedIndirect(low, index)];
    }       
}