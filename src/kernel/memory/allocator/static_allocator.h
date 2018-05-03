#ifndef STATIC_ALLOCATOR_H
#define STATIC_ALLOCATOR_H
#include <stddef.h>
#include <stdint.h>



void static_allocator_init(uint32_t base_address, size_t mem_size);
void static_allocator_alloc(size_t size);






#endif // STATIC_ALLOCATOR_H
