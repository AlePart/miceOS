
#include "paging_management.h"
#include "../allocator/static_allocator.h"
#include "../../libc/libc.h"
#define DIRECTORY_SHIFT (10)




void page_allocator_init(size_t high_mem_size)
{
    static_allocator_init(high_mem_size & 0xFFFFF000);

    change_dir_tbl(high_mem_size & 0xFFFFF00);
    uint32_t val = 0x80000001;
    asm("mov %0, %%cr0":"=r"(val));
}

void page_allocator_free_pages(PAGE_DIR directory)
{
}


void page_allocator_allocate_page(void* virt_addr, uint8_t** directory)
{

    volatile uint8_t* pg_tbl_addr[1];
    volatile uint8_t* pg_addr[1];
    volatile uint8_t* dir_addr[1];
    if (ALLOCATION_OK != static_allocator_alloc(4096,pg_addr))
    {
        return NULL;
    }

    if(ALLOCATION_OK != static_allocator_alloc(4096, dir_addr))
    {
        static_allocator_free(pg_addr, 1);
        return NULL;
    }
    if( ALLOCATION_OK != static_allocator_alloc(4096, pg_tbl_addr))
    {
        static_allocator_free(pg_addr, 1);
        static_allocator_free(dir_addr,1);
        return NULL;
    }
   dir_addr[((uint32_t)virt_addr & 0xFFC00000) >> (PAGE_SHIFT + DIRECTORY_SHIFT)] = pg_tbl_addr[0];
   pg_tbl_addr[((uint32_t)virt_addr & 0xFFC00000) >> (PAGE_SHIFT + DIRECTORY_SHIFT)] = dir_addr[0];
   *directory = dir_addr[0];
   /* ((*directory)[((uint32_t)virt_addr & 0xFFC00000) >> (PAGE_SHIFT + DIRECTORY_SHIFT)]) = pgTbl[1];
    pgTbl[((uint32_t)virt_addr & 0x003FF000) >> PAGE_SHIFT] = (pg_addr[0]);*/
   /* pg_tbl[0][((uint32_t)virt_addr & 0x003FF000) >> PAGE_SHIFT]= (uint8_t*)pg_addr[0];
    (directory)[0][((uint32_t)virt_addr & 0xFFC00000) >> (PAGE_SHIFT + DIRECTORY_SHIFT)] = (uint8_t*)pg_tbl[0];*/

}

PAGE_APPEND_RESULT page_allocator_append_page(PAGE_DIR dir ,void *virt_addr)
{
    volatile uint8_t* pg_addr[1];
    volatile uint8_t* pgTbl = dir[((uint32_t)virt_addr & 0xFFC00000) >> (PAGE_SHIFT + DIRECTORY_SHIFT)];
    if( pgTbl != NULL)
    {

        if(pgTbl[((uint32_t)virt_addr & 0x003FF000) >> PAGE_SHIFT] != NULL)
        {
            return APPEND_FAIL_ADDRESS_IN_USE;
        }
        else
        {
            if (ALLOCATION_OK != static_allocator_alloc(4096,pg_addr))
            {
                return APPEND_FAIL;
            }
            else
            {
                pgTbl[((uint32_t)virt_addr & 0x003FF000) >> PAGE_SHIFT] = (pg_addr[0]);
            }

        }
    }
    else
    {

        if( ALLOCATION_OK != static_allocator_alloc(4096, pgTbl))
        {
            return APPEND_FAIL;
        }
        else
        {
            if (ALLOCATION_OK != static_allocator_alloc(4096,pg_addr))
            {
                static_allocator_free(pgTbl,4096);
                return APPEND_FAIL;
            }
            else
            {
                pgTbl[((uint32_t)virt_addr & 0x003FF000) >> PAGE_SHIFT] = (pg_addr[0]);
            }
        }
    }
}
static inline void change_dir_tbl(PAGE_DIR directory)
{
    asm("mov %0, %%cr3" : : "r"(directory) );
}




