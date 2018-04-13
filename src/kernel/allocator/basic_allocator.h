#ifndef BASIC_ALLOCATOR_H
#define BASIC_ALLOCATOR_H
# include <stdint.h>

typedef enum {
  PAGE_4K =12,
  PAGE_2k =11,
  PAGE_1K =10
}PAGE_SIZE;

bool basic_allocator_init(uint32_t* base_address, uint32_t mem_size, PAGE_SIZE pg_size);
void* allocate_area(size_t size); //retuns the first base_dictionary
void* append_area(void* address, size_t size); //returns the new area appended to the given address NULL if error
void free_area(void* address);
#endif
