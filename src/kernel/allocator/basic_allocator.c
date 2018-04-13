#include "basic_allocator.h"



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

void set_page_properties(ALLOCATOR_ELEMENT* current_el, PAGE_OWNER owner) //maybe inline is better
{
  if(OWNER_KERNEL ==owner )
  {
    current_el->type_mask |= KERNEL_PAGE;
  }
  else if(OWNER_USER == owner)
  {
    current_el->type_mask |= USER_PAGE
  }
}

ALLOCATOR_ELEMENT* allocate_page(PAGE_OWNER owner)
{
  ALLOCATOR_ELEMENT* current_el= ((void*)allocator_hdr_addr)+ sizeof(ALLOCATOR_HEADER);
  while( 0x00000000 == current_el->type_mask & FREE_PAGE) //page is free?
  {
    current_el++; //next element;
  }
  current_el->type_mask &= ~FREE_PAGE; //reset flag
  set_page_properties(current_el,owner);
  return current_el;
}

ALLOCATOR_ELEMENT* search(void* address) 
{
  void* directory = (void*)(uint32_t address & (1<<allocator_hdr_addr->sz)); 
  ALLOCATOR_ELEMENT* current_el= ((void*)allocator_hdr_addr)+ sizeof(ALLOCATOR_HEADER);

  while( directory != current_el->base_directory ) //search for element with given dir
  {
    current_el++;
  }
}


ALLOCATOR_ELEMENT* allocate_pages(size_t size, PAGE_OWNER owner)
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
    ALLOCATOR_ELEMENT* current_el=allocate_page(owner);
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


ALLOCATOR_ELEMENT* append_pages(void* address, size_t size)
{
  ALLOCATOR_ELEMENT* current_el= search(address);
  if(0x00000000 == (current_el->type_mask & FREE_PAGE) ) // must be non free the page, else ret NULL
  {
    return allocate_pages(size);
  }
  else
  {
    return NULL;
  }
}

void set_page_properties(ALLOCATOR_ELEMENT* el, PAGE_OWNER owner)
{
  
}
void* allocate_area(size_t size, PAGE_OWNER owner)
{
  return allocate_pages(size, owner)->base_directory;
}
void* append_area(void* address, size_t size, PAGE_OWNER owner)
{
  
  return allocate_pages(address, size , owner)->base_directory;
}

void free_area(void* address)
{
  ALLOCATOR_ELEMENT* current_el= search(address);
  while(NULL != current_el->prev) // going to first element
  {
    current_el=current_el->prev;
  }
  ALLOCATOR_ELEMENT* prev_el;
  while(NULL != current_el->next)
  {
    current_el->type_mask = FREE_PAGE; 
    if( NULL!= current_el->prev) //check if prev is not null to null it and save the prev to delete the next pointer
    {
      prev_el = current_el->prev;
      current_el->prev=NULL;
    }
    if( NULL != prev_el) //del next ptr in the end
    {
      prev_el->next=NULL;
    }
  }
  current_el->prev = NULL;
} 

