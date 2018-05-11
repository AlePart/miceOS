#ifndef PAGING_MANAGEMENT_H
#define PAGING_MANAGEMENT_H

#include <stdint.h>
#include <stddef.h>

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
#define MASK_PAGE_4K (0x00000FFF)
typedef uint8_t** PAGE_DIR;
typedef uint8_t** PAGE_TBL;

typedef enum
{
    APPEND_OK,
    APPEND_FAIL_ADDRESS_IN_USE,
    APPEND_FAIL
}PAGE_APPEND_RESULT;
void page_allocator_init(size_t high_mem_size);

void  page_allocator_free_pages(PAGE_DIR directory);

void page_allocator_allocate_page(void* virt_addr, uint8_t **directory);

PAGE_APPEND_RESULT page_allocator_append_page(PAGE_DIR dir ,void *virt_addr);

static inline void change_dir_tbl(PAGE_DIR directory);

#endif // PAGING_MANAGEMENT_H
