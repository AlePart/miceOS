#include "basic_allocator.h"

#define FREE_PAGE 1<<0;

typedef struct
{
  PAGE_SIZE sz;
  uint32_t max_pages;
  uint32_t spare;
}ALLOCATOR_HEADER;
typedef struct
{
  uint32_t* base_dictionary;
  uint32_t* type_mask; 
  ALLOCATOR_LIST* next;
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
    
  allocator_hdr_addr = base_address;
  allocator_hdr_addr.sz=pg_size;
  allocator_hdr_addr.spare = 0;
  uint32_t counter = 0;
  ALLOCATOR_ELEMENT* current_el= (ALLOCATOR_ELEMENT*)(allocator_hdr_addr + sizeof(ALLOCATOR_HEADER));
  do
  {
       current_el->base_dictionary=NULL;
       current_el->type_mask=FREE_PAGE;
       current_el->next=(current_el+sizeof(ALLOCATOR_ELEMENT));
       current_el=current_el->next;
  }while(++counter < allocator_reserved_pages);
  
  current_el->next=NULL; //stopping allocation
}
