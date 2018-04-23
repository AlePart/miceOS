
#include "../allocator/basic_allocator.h"
#define PAGE_SHIFT (12)
#define DIR_SHIFT (22)
#define PAGE_DIR_SIZE (1024)
#define PAGE_TBL_SIZE (1024)
#define PAGE_DIR_SIZE_SHIFT (10)
#define PAGE_SIZE_4K (4096)

typedef uint32_t** PAGE_DIR;
typedef uint32_t** PAGE_TBL;




PAGE_DIR page_allocator_init(size_t memory_size)
{
  basic_allocator_initialize(0x00000000, memory_size);
  return allocate_pages(PAGE_SIZE_4K * PAGE_SIZE_4K); // 16MB kernel reservation
   
  /*mov eax, page_directory
  mov cr3, eax*/


  
   /*mov eax, cr0
   or eax, 0x80000001
   mov cr0, eax*/
  // enable paging by setting CR0

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
    for(uint16_t cnt =0 ; i < PAGE_TBL_SIZE ; i++)
    {
      page_tbl[cnt] = basic_allocator_alloc(PAGE_SIZE_4K) ; // the real allocation
    }
  }
 
  return page_directory;
}

void change_dir_tbl(PAGE_DIR directory)
{
  
  //setup cr3
}




