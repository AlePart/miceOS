#ifndef BASIC_ALLOCATOR_H
#define BASIC_ALLOCATOR_H
# include <stdint.h>

bool basic_allocator_init(uint32_t base_address);
void* allocate_page();

#endif
