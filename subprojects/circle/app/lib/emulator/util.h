#ifndef UTIL_H
#define UTIL_H

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef signed char		s8;
typedef signed short		s16;
typedef signed int		s32;
typedef signed long long	s64;

#define KILOBYTE 1024
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define SET_BIT(var,pos) ((var) |= (1 << (pos)))
#define CLEAR_BIT(var,pos) ((var) &= ~(1 << (pos)))
#define ASSIGN_BIT(var,pos,val) ((var) ^= (-(val) ^ (var)) & (1 << (pos)))

//sizes related to hardware (in bytes) :
const u32 PRGROM_BANK_SIZE = 16 * KILOBYTE;
const u32 VROM_BANK_SIZE = 8 * KILOBYTE;
const u32 PRGRAM_BANK_SIZE = 8 * KILOBYTE;
const u8 TILE_SIZE = 16;
const u8 BLOCK_SIZE = 64;
#endif // UTIL_H
