#include "memory.h"

namespace Frankenstein {

template<>
Memory<u8, u16, 0x10000>::Memory(Nes& pNes);

template <>
Memory<u8, u16, 0x10000>::Ref Memory<u8, u16, 0x10000>::operator[](const u16 addr);

template <>
u8 Memory<u8, u16, 0x10000>::Read(const u16 address);

template <>
void Memory<u8, u16, 0x10000>::Write(const u16 address, const u8 val);

template <>
void Memory<u8, u16, 0x10000>::Copy(const u8* source, const u16 destination, const unsigned int size);

template <>
bool Memory<u8, u16, 0x10000>::IsPageCrossed(u16 startAddress, u16 endAddress);

template <>
u16 Memory<u8, u16, 0x10000>::FromValues(const u8 low);

template <>
u16 Memory<u8, u16, 0x10000>::FromValues(const u8 low, const u8 high);

template <>
u16 Memory<u8, u16, 0x10000>::ZeroPage(const u8 low);

template <>
u16 Memory<u8, u16, 0x10000>::Absolute(const u8 low, const u8 high);

template <>
u16 Memory<u8, u16, 0x10000>::Indexed(const u8 low, const u8 high, const u8 index);

template <>
u16 Memory<u8, u16, 0x10000>::ZeroPageIndexed(const u8 low, const u8 index);

template <>
u16 Memory<u8, u16, 0x10000>::Indirect(const u8 low, const u8 high);

template <>
u16 Memory<u8, u16, 0x10000>::PreIndexedIndirect(const u8 low, const u8 index);

template <>
u16 Memory<u8, u16, 0x10000>::PostIndexedIndirect(const u8 low, const u8 index);

template <>
template <>
Memory<u8, u16, 0x10000>::Ref Memory<u8, u16, 0x10000>::Get<Addressing::Absolute>(const u8 low, const u8 high);

template <>
template <>
Memory<u8, u16, 0x10000>::Ref Memory<u8, u16, 0x10000>::Get<Addressing::ZeroPage>(const u8 low);

template <>
template <>
Memory<u8, u16, 0x10000>::Ref Memory<u8, u16, 0x10000>::Get<Addressing::Indexed>(const u8 low, const u8 high, const u8 index);

template <>
template <>
Memory<u8, u16, 0x10000>::Ref Memory<u8, u16, 0x10000>::Get<Addressing::ZeroPageIndexed>(const u8 low, const u8 index);

template <>
template <>
Memory<u8, u16, 0x10000>::Ref Memory<u8, u16, 0x10000>::Get<Addressing::Indirect>(const u8 low, const u8 high);

template <>
template <>
Memory<u8, u16, 0x10000>::Ref Memory<u8, u16, 0x10000>::Get<Addressing::PreIndexedIndirect>(const u8 low, const u8 index);

template <>
template <>
Memory<u8, u16, 0x10000>::Ref Memory<u8, u16, 0x10000>::Get<Addressing::PostIndexedIndirect>(const u8 low, const u8 index);

using NesMemory = Memory<u8, u16, 0x10000>;

}
