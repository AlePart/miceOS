
#define PAGE_SHIFT (12)
#define DIR_SHIFT (22)
#define PAGE_DIR_SIZE (1024)
#define PAGE_DIR_SIZE_SHIFT (10)

typedef uint32_t** PAGE_DIR;
;



void page_allocator_init(uint32_t* page_dir, uint32_t* page_table)
{

  //memsetting page_descriptors

  /*mov eax, page_directory
  mov cr3, eax*/


 
   /*mov eax, cr0
   or eax, 0x80000001
   mov cr0, eax*/
  // enable paging by setting CR0

}

PAGE_DIR allocate_pages(size_t size)
{
  uint32_t need_pages = size >> PAGE_SHIFT;
  

  if(0 != size & MASK_PAGE_4K )
  {
    need_pages++;
  }
  //page for this entry
 
  // TODO allocator for 4k dir
  uint32_t* page_directory[1+(size >> DIR_SHIFT)];
  //TODO ALLOCATOR FOR page table 4k
  // TODO MANAGE MORE 1024 pages
  uint32_t* page_directory[need_pages];
  
  for(need_pages; need_pages > 0; need_pages--)
  {
    
    uint32_t* allocation;
    page_directory[need_pages >> PAGE_DIR_SIZE_SHIFT ] = (uint32_t*)(allocation << PAGE_SHIFT);
  } 
  page_directory[0] = page_directory;
  return (PAGE_DIR)page_directory;
}

void change_dir_tbl()
{

  //setup cr3
}




