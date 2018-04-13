#include "basic_allocator.h"

#define FREE_PAGE 1<<0;

typedef struct
{
  PAGE_SIZE sz;
  uint32_t max_pages;
}ALLOCATOR_HEADER;
typedef struct
{
  void* base_directory;
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
  void* base_directory=allocator_mem_reserve + base_address;
  allocator_hdr_addr = base_address;
  allocator_hdr_addr.sz=pg_size;
  uint32_t counter = 0;
  ALLOCATOR_ELEMENT* current_el= (ALLOCATOR_ELEMENT*)(allocator_hdr_addr + sizeof(ALLOCATOR_HEADER));
  do
  {
       current_el->base_directory=base_directory;
       current_el->type_mask=FREE_PAGE;
       base_directory +=(1<<allocator_hdr_addr.sz);
 
  }while(counter++ < allocator_reserved_pages);
  return true;
}

ALLOCATOR_ELEMENT* allocate_page()
{
  ALLOCATOR_ELEMENT* current_el= ((void*)allocator_hdr_addr)+ sizeof(ALLOCATOR_HEADER);
  while( 0x00000000 == current_el->type_mask & FREE_PAGE)
  {
    current_el++; //next element;
  }
  current_el->type_mask ^= (uint32_t)FREE_PAGE; //set flag
  phys_addr
  return current_el;
}

void free_page(void* address)
{
  void* directory = (void*)(uint32_t address & (1<<allocator_hdr_addr->sz)); 
  ALLOCATOR_ELEMENT* current_el= ((void*)allocator_hdr_addr)+ sizeof(ALLOCATOR_HEADER);
  while( directory != current_el->base_directory )
  {
    current_el++;
  }
  current_el->type_mask = FREE_PAGE;
  current_el->prev=NULL;
  current_el->next=NULL;
}

void* allocate_area(size_t size)
{
  ALLOCATOR_ELEMENT* prev_el=NULL;
  void* elem_to_ret = NULL;
  uint32_t pages= size/(1<<allocator_hdr_addr->sz); // how many pages?;
  if( size%(1<<allocator_hdr_addr->sz)) //residual page
  {
    pages++;
  }

  do
  {
    ALLOCATOR_ELEMENT* current_el=allocate_page();
    if(NULL != prev_el)
    {
      elem_to_ret = current_el->base_directory;
      prev_el->next=current_el;
    }
    current_el->prev = prev_el;
    prev_el = current_el;
    pages--;
  }while(pages!=0)
  return elem_to_ret;
}

