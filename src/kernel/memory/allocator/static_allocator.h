#ifndef STATIC_ALLOCATOR_H
#define STATIC_ALLOCATOR_H
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    ALLOCATION_OK,
    ALLOCATION_DICTIONARY_PAGE_REQUEST,
    ALLOCATION_UNABLE_TO_ALLOC
}ALLOCATION_RESULT;

void static_allocator_init(uint32_t *base_address);
ALLOCATION_RESULT static_allocator_alloc(size_t size, void **start_addr);






#endif // STATIC_ALLOCATOR_H
