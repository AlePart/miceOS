#include "basic_allocator.h"

#define FREE_PAGE 1<<0;

typedef struct
{
  PAGE_SIZE sz;
  uint32_t max_pages;
}ALLOCATOR_HEADER;
typedef struct
{
  void* base_dictionary;
  uint32_t type_mask; 
  ALLOCATOR_ELEMENT* prev;
  ALLOCATOR_ELEMENT* next;
}ALLOCATOR_ELEMENT;

typedef enum {
  PAGE_4K =12,
  PAGE_2k =11,
  PAGE_1K =10
}PAGE_SIZE;

ALLOCATOR_HEADER* allocator_hdr_addr;

bool basic_allocator_init(uint32_t* base_address, uint32_t mem_size, PAGE_SIZE pg_size)
{
  uint32_t availble_pages= mem_size>>(uint32_t)pg_size;
  uint32_t allocator_element_needed= (mem_size >>(uint32_t)pg_size);
  uint32_t allocator_mem_reserve= allocator_element_needed*sizeof(ALLOCATOR_ELEMENT) + sizeof(ALLOCATOR_HEADER);
  void* base_dictionary=allocator_mem_reserve + base_address;
  allocator_hdr_addr = base_address;
  allocator_hdr_addr.sz=pg_size;
  uint32_t counter = 0;
  ALLOCATOR_ELEMENT* current_el= (ALLOCATOR_ELEMENT*)(allocator_hdr_addr + sizeof(ALLOCATOR_HEADER));
  do
  {
       current_el->base_dictionary=base_dictionary;
       current_el->type_mask=FREE_PAGE;
       base_dictionary +=(1<<allocator_hdr_addr.sz);
 
  }while(counter++ < allocator_reserved_pages);
  return true;
}

void* allocate_page()
{
  ALLOCATOR_ELEMENT* current_el= ((void*)allocator_hdr_addr)+ sizeof(ALLOCATOR_HEADER);
  while( 0x00000000 == current_el->type_mask & FREE_PAGE)
  {
    current_el++; //next element;
  }
  current_el->type_mask ^= (uint32_t)FREE_PAGE; //set flag
  return current_el->base_dictionary;
}

