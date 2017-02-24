#pragma once

#include "util.h"

namespace Frankenstein {

    class Memory {
    private:
        u8 raw[0x10000];
        
    public:
        
        static constexpr u16 ADDR_ZERO_PAGE      = 0x0000;
        static constexpr u16 ADDR_STACK          = 0x0100;
        static constexpr u16 ADDR_RAM            = 0x0200;
        static constexpr u16 ADDR_MIRROIR_1      = 0x0800;
        static constexpr u16 ADDR_IO_REGISTERS_1 = 0x2000;
        static constexpr u16 ADDR_MIRROIR_2      = 0x2008;
        static constexpr u16 ADDR_IO_REGISTERS_2 = 0x4000;
        static constexpr u16 ADDR_EXPANSION_ROM  = 0x4020;
        static constexpr u16 ADDR_SRAM           = 0x6000;
        static constexpr u16 ADDR_PRG_ROM_LOWER_BANK = 0x8000;
        static constexpr u16 ADDR_PRG_ROM_UPPER_BANK = 0xC000;
        
        enum class Addressing {
            Absolute,
            ZeroPage,
            Indexed,
            ZeroPageIndexed,
            Indirect,
            PreIndexedIndirect,
            PostIndexedIndirect,
        };
        
        u8& operator[] (const u16 addr);
        
        u8& get(const u16 addr);
        
        template <Memory::Addressing N>
        u8& get(const u8);
        
        template <Memory::Addressing N>
        u8& get(const u8, const u8);
        
        template <Memory::Addressing N>
        u8& get(const u8, const u8, const u8);
               
        /**
         * Detects if two addresses are on the same page
         * @param startAddress
         * @param endAddress
         * @return if a page is crossed
         */
        static bool IsPageCrossed(u16 startAddress, u16 endAddress);    
        
        /**
         * Build an address using only the least significative byte. The first half of
         * the address is assumed to be zero.
         *
         * @param low the least significant byte of the address
         * @return the two bytes address
         */
        static u16 FromValues(const u8 low);

        /**
         * Build an address using the most and the least significative bytes. Useful
         * because the memory is stored as little endian.
         *
         * @param low  the least significant byte of the address
         * @param high the most significant byte of the address
         * @return the two bytes address
         */
        static u16 FromValues(const u8 low, const u8 high);

        /**
         * Build the address [00][low].
         * @param low the least significant byte of the address
         * @return the builded address
         */
        static u16 ZeroPage(const u8 low);

        /**
         * Build the address [high][low].
         * @param low  the least significant byte of the address
         * @param high the most significant byte of the address
         * @return the builded address
         */
        static u16 Absolute(const u8 low, const u8 high);

        /**
         * Compute the complete address [high][low] + index
         * @param low   the least significant byte of the address
         * @param high  the most significant byte of the address
         * @param index a value to add to the constructed address
         * @return the computed address
         */
        static u16 Indexed(const u8 low, const u8 high, const u8 reg);

        /**
         * Compute the ZeroPage address [00][low] + index.
         * @param low   the least significant byte of the address
         * @param index a value to add to the constructed address
         * @return the computed address
         */
        static u16 ZeroPageIndexed(const u8 low, const u8 reg);

        /**
         * Fetch an address at memory [high][low].
         * @param  low  the least significant byte
         * @param  high the most significant byte
         * @return the stored address
         */
        u16 Indirect(const u8 low, const u8 high);

        /**
         * Fetch an address at memory [00][low + index]. If [low + index] overflow, only
         * the lowest byte is kept.
         *
         * @param  low   ZeroPage address of the memory containing the address
         * @param  index a value to add to low to get the actual address
         * @return the pre-indexed address
         */
        u16 PreIndexedIndirect(const u8 low, const u8 index);

        /**
         * Fetch an address at memory [00][low] then increase the address by index.
         * @param  low   ZeroPage address of the memory containing the address
         * @param  index an index to add to the loaded address. Usually a register [X|Y]
         * @return the post-indexed address
         */
        u16 PostIndexedIndirect(const u8 low, const u8 index);

        //u16 Implied();
        //u16 Accumulator();
        //u16 Relative();

    };
}
