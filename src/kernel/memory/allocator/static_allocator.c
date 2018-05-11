#include "static_allocator.h"

#include "../../libc/libc.h"

#define TOTAL_PAGES ( 1024 * 1024 )
#define PAGE_SHIFT (12)

#define PAGE_ALLOCATED ( 1 << 0 )
#define PAGE_RESERVED (1 << 1 )
typedef struct
{
    uint8_t* page_phy_address;
    uint32_t mask;
}ALLOCATOR_ALLOCATED_PAGES;

ALLOCATOR_ALLOCATED_PAGES* pages;
uint32_t page_offset;
void static_allocator_init(uint32_t* base_address)
{
    pages = (ALLOCATOR_ALLOCATED_PAGES*)(((uint32_t)base_address) & 0xFFFFF000);
    page_offset = (uint32_t)(((uint32_t)base_address & 0xFFFFF000)>> PAGE_SHIFT);
    kmemset(pages,0x00,sizeof(ALLOCATOR_ALLOCATED_PAGES)*TOTAL_PAGES);
    for(uint32_t i = 0; i< TOTAL_PAGES; i++)
    {
        pages[i].page_phy_address= (uint8_t*)(((uint32_t)base_address & 0xFFFFF000) + (i << PAGE_SHIFT));
        if(i < ((sizeof(ALLOCATOR_ALLOCATED_PAGES)*TOTAL_PAGES) >> PAGE_SHIFT))
        {
            pages[i].mask |= PAGE_ALLOCATED;
        }
    }

}

ALLOCATOR_ALLOCATED_PAGES* static_allocator_find_pages(uint8_t* addr)
{

}

ALLOCATOR_RESULT static_allocator_alloc(size_t size, uint8_t* start_addr[])
{

    uint32_t need_pages = size >> PAGE_SHIFT;
    uint32_t cnt = 0;
    for(uint32_t i =0 ; i<TOTAL_PAGES; i++)
    {
        if( 0 == (pages[i].mask & PAGE_ALLOCATED))
        {
            start_addr[cnt++]=(void*)pages[i].page_phy_address;
            pages[i].mask |= PAGE_RESERVED;
            need_pages--;
        }
        if(0 == need_pages)
        {
            break;
        }
    }
    if(0 != need_pages)
    {
        for(uint32_t i=0; i< (size >> PAGE_SHIFT) - need_pages; i++)
        {
            (pages[(((uint32_t)(start_addr[i])) >> PAGE_SHIFT) - page_offset]).mask &= ~PAGE_RESERVED;
        }
        return ALLOCATION_UNABLE_TO_ALLOC;
    }
    else
    {
        for(uint32_t i=0; i< (size >> PAGE_SHIFT) - need_pages; i++)
        {
            (pages[(((uint32_t)(start_addr[i])) >> PAGE_SHIFT) - page_offset]).mask &= ~PAGE_RESERVED;
            (pages[(((uint32_t)(start_addr[i])) >> PAGE_SHIFT) - page_offset]).mask |= PAGE_ALLOCATED;
        }
    }
    return ALLOCATION_OK;
}


ALLOCATOR_RESULT static_allocator_free(uint8_t* addr[], size_t pages_num)
{
    ALLOCATOR_ALLOCATED_PAGES* current[pages_num];
    for(uint32_t i=0; i< pages_num; i++)
    {
        ((ALLOCATOR_ALLOCATED_PAGES*)(&(pages[(((uint32_t)addr[i]) >> PAGE_SHIFT) - page_offset])))->mask &= ~PAGE_ALLOCATED;

        addr[i]=NULL;
    }
    return ALLOCATION_OK;

}
