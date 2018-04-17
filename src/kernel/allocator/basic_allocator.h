#ifndef BASIC_ALLOCATOR_H
#define BASIC_ALLOCATOR_H
# include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define FREE_PAGE (1<<0)
#define KERNEL_PAGE (1<<1)
#define USER_PAGE (1<<2)
#define STACK_PAGE (1<<3)
#define HEAP_PAGE (1<<4)

typedef enum {
  OWNER_KERNEL,
  OWNER_USER
}PAGE_OWNER;
typedef enum
{
  PAGE_STACK,
  PAGE_DATA,
  PAGE_EXECUTABLE,
  PAGE_HEAP
}PAGE_TYPE;
typedef enum {
  PAGE_4K =12,
  PAGE_2k =11,
  PAGE_1K =10
}PAGE_SIZE;

typedef struct
{
  void* base_directory;
  uint32_t type_mask; 
  void* prev;
  void* next;
}ALLOCATOR_ELEMENT;

bool basic_allocator_init(uint32_t mem_size, PAGE_SIZE pg_size);
void* allocate_area(size_t size, PAGE_OWNER owner); //retuns the first base_dictionary
void* append_area(void* address, size_t size, PAGE_OWNER owner); //returns the new area appended to the given address NULL if error
void free_area(void* address);
ALLOCATOR_ELEMENT* search_page(void* address); //search an allocator element by given address useful for mmu
#endif
