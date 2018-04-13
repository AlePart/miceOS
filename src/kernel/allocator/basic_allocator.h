#ifndef BASIC_ALLOCATOR_H
#define BASIC_ALLOCATOR_H
# include <stdint.h>

bool basic_allocator_init(uint32_t base_address);
void* allocate_area(size_t size); //retuns the first base_dictionary
void free_page(void* address);

#endif
