
#include "paging_management.h"
#include "../allocator/basic_allocator.h"





PAGE_DIR page_allocator_init(size_t high_mem_size)
{
  basic_allocator_initialize(0x00000000, high_mem_size);
  PAGE_DIR ker_dir_addr=allocate_pages(PAGE_SIZE_4K * PAGE_SIZE_4K); // 16MB kernel reservation

  change_dir_tbl(ker_dir_addr);
  uint32_t val = 0x80000001;
  asm("mov %0, %%cr0":"=r"(val));
  return ker_dir_addr;
}

void free_pages(PAGE_DIR directory)
{
  for(uint32_t i=0; i< PAGE_DIR_SIZE; i++)
  {
    PAGE_TBL pg = (PAGE_TBL)directory[i];
    if(pg != 0)
    {
      for(uint32_t j=0; i< PAGE_TBL_SIZE; j++)
      {
        if(pg[j] != 0)
        {
          basic_allocator_free((void*)pg[j]);
        }
        pg[j] = 0;
      }
    }
    basic_allocator_free((void*)directory[i]);
    directory[i] = 0;
  }
   basic_allocator_free((void*)directory);
}

PAGE_DIR allocate_pages(size_t size)
{
  uint32_t need_pages = size >> PAGE_SHIFT;
  uint32_t need_dir = (1 + (size >> DIR_SHIFT));
  if(0 != (size & MASK_PAGE_4K) )
  {
    need_pages++;
  }
   
  PAGE_DIR page_directory =(PAGE_DIR)basic_allocator_alloc(PAGE_SIZE_4K); //allocate 4k for directory
  //TODO MEMSET instead for
  for(uint32_t i=0; i< PAGE_SIZE_4K; i++)
  {
    ((uint8_t**)(page_directory))[i]=0;
  }

  PAGE_TBL page_tbl;
  if(0 == need_pages % PAGE_TBL_SIZE) // allocate 4k aligned space
  {
    page_tbl = (PAGE_TBL)basic_allocator_alloc(PAGE_SIZE_4K * (need_pages / PAGE_TBL_SIZE ));
  }
  else
  {
    page_tbl = (PAGE_TBL)basic_allocator_alloc(PAGE_SIZE_4K * (need_pages / PAGE_TBL_SIZE ) + 1);
  }

  for(uint32_t i =0 ; i < need_dir; need_dir++)
  {
    uint32_t entry = (uint32_t)((page_directory)[i]);
    entry = (uint32_t)(((uint8_t*)page_tbl) +  (i << PAGE_SIZE_4K)); // dir_entry[i] is the base pg tbl plus shifted index
    entry &= (~PAGE_DESCRIPTOR_ENTRY_FLAG_MASK);
    entry |= PAGE_PRESENT | PAGE_RW | PAGE_USER;
    (page_directory)[i] = (uint32_t*)entry;
    

    for(uint16_t cnt =0 ; i < PAGE_TBL_SIZE ; cnt++)
    {
      uint32_t entry = (uint32_t)((page_tbl)[cnt]);
      entry = (uint32_t)basic_allocator_alloc(PAGE_SIZE_4K) ; // the real allocation
      entry &= (~PAGE_DESCRIPTOR_ENTRY_FLAG_MASK);
      entry |= PAGE_PRESENT | PAGE_RW | PAGE_USER;
      (page_tbl)[i] = (uint32_t*)entry;
    }
  }
 
  return page_directory;
}

static inline void change_dir_tbl(PAGE_DIR directory)
{
  asm("mov %0, %%cr3" : : "r"(directory) );
}




