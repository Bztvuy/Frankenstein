#ifndef UTIL_H
#define UTIL_H

#define KILOBYTE 1024
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define SET_BIT(var,pos) ((var) |= (1 << (pos)))
#define CLEAR_BIT(var,pos) ((var) &= ~(1 << (pos)))
#define ASSIGN_BIT(var,pos,val) ((var) ^= (-(val) ^ (var)) & (1 << (pos)))

//sizes related to hardware (in bytes) :
const u16 PAGE_SIZE = 256;
const u32 PRGROM_BANK_SIZE = 16 * KILOBYTE;
const u32 VROM_BANK_SIZE = 8 * KILOBYTE;
const u32 PRGRAM_BANK_SIZE = 8 * KILOBYTE;
#endif // UTIL_H
