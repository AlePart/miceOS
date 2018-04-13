#ifndef BASIC_ALLOCATOR_H
#define BASIC_ALLOCATOR_H
# include <stdint.h>

#define FREE_PAGE (1<<0)
#define KERNEL_PAGE (1<<1)
#define USER_PAGE (1<<2)

typedef enum {
  OWNER_KERNEL,
  OWNER_USER
}PAGE_OWNER;

typedef enum {
  PAGE_4K =12,
  PAGE_2k =11,
  PAGE_1K =10
}PAGE_SIZE;

bool basic_allocator_init(uint32_t* base_address, uint32_t mem_size, PAGE_SIZE pg_size);
void* allocate_area(size_t size, PAGE_OWNER owner); //retuns the first base_dictionary
void* append_area(void* address, size_t size, PAGE_OWNER owner); //returns the new area appended to the given address NULL if error
void free_area(void* address);
#endif
