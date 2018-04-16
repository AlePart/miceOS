#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

inline void memcpy_i386(void* dest, void* src, size_t size)
{
    __asm__ volatile ( "cld\n\t" "rep\n\t" "movsb" : : "S" (src), "D" (dest), "c" (size)  );
}


#endif
