
#include "../allocator/basic_allocator.h"
#define PAGE_SHIFT (12)
#define DIR_SHIFT (22)
#define PAGE_DIR_SIZE (1024)
#define PAGE_TBL_SIZE (1024)
#define PAGE_DIR_SIZE_SHIFT (10)
#define PAGE_SIZE_4K (4096)
#define PAGE_DESCRIPTOR_ENTRY_FLAG_MASK (0x00000FFF)
#define PAGE_PRESENT (1<<0)
#define PAGE_RW (1<<1)
#define PAGE_USER (1<<2)
typedef uint32_t** PAGE_DIR;
typedef uint32_t** PAGE_TBL;
register uint32_t CR1 asm ("cr1");
register uint32_t CR3 asm ("cr3");




inline PAGE_DIR page_allocator_init(size_t memory_size)
{
  basic_allocator_initialize(0x00000000, memory_size);
  PAGE_DIR ker_dir_addr=allocate_pages(PAGE_SIZE_4K * PAGE_SIZE_4K); // 16MB kernel reservation

  change_dir_tbl(ker_dir_addr);
  CR1 |= 0x80000001;

  return ker_dir_addr;
}

void free_pages(PAGE_DIR directory)
{
  for(uint32_t i=0; i< PAGE_DIR_SIZE; i++)
  {
    PAGE_TBL pg = directory[i];
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
  uint32_t need_dir = 1 + (size >> DIR_SHIFT);
  if(0 != size & MASK_PAGE_4K )
  {
    need_pages++;
  }
   
  PAGE_DIR page_directory =(PAGE_DIR)basic_allocator_alloc(PAGE_SIZE_4K); //allocate 4k for directory
  //TODO MEMSET instead for
  for(uint32_t i=0; i< PAGE_SIZE_4K; i++)
  {
    (uint8_t)(page_directory[i])=0;
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

  for(int i =0 ; i < need_dir; need_dir++)
  {
    page_directory[i] = (uint32_t*)(((void*)page_tbl) +  (i << PAGE_SIZE_4K)); // dir_entry[i] is the base pg tbl plus shifted index
    page_directory[i] &= (~PAGE_DESCRIPTOR_ENTRY_FLAG_MASK);
    page_directory[i] |= PAGE_PRESENT | PAGE_RW | PAGE_USER;
    

    for(uint16_t cnt =0 ; i < PAGE_TBL_SIZE ; i++)
    {
      page_tbl[cnt] = basic_allocator_alloc(PAGE_SIZE_4K) ; // the real allocation
      page_tbl[cnt] &= (~PAGE_DESCRIPTOR_ENTRY_FLAG_MASK);
      page_tbl[cnt] |= PAGE_PRESENT | PAGE_RW | PAGE_USER;
    }
  }
 
  return page_directory;
}

inline void change_dir_tbl(PAGE_DIR directory)
{
  CR3 = directory;
}




