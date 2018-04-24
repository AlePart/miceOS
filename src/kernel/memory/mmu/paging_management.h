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
typedef uint32_t** PAGE_DIR;
typedef uint32_t** PAGE_TBL;

PAGE_DIR page_allocator_init(size_t high_mem_size);

void free_pages(PAGE_DIR directory);

PAGE_DIR allocate_pages(size_t size);

static inline void change_dir_tbl(PAGE_DIR directory);

#endif // PAGING_MANAGEMENT_H
