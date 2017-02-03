#ifndef UTIL_H
#define UTIL_H

#define KILOBYTE 1024
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define SET_BIT(var,pos) ((var) |= (1 << (pos)))
#define CLEAR_BIT(var,pos) ((var) &= ~(1 << (pos)))
#define ASSIGN_BIT(var,pos,val) ((var) ^= (-(val) ^ (var)) & (1 << (pos)))
#endif // UTIL_H
