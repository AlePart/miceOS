#ifndef STATIC_ALLOCATOR_H
#define STATIC_ALLOCATOR_H
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    ALLOCATION_OK,
    ALLOCATION_DICTIONARY_PAGE_REQUEST,
    ALLOCATION_UNABLE_TO_ALLOC,
    ALLOCATION_FREE_OK,
    ALLOCATION_UNABLE_TO_FREE
}ALLOCATOR_RESULT;

void static_allocator_init(uint32_t *base_address);
ALLOCATOR_RESULT static_allocator_alloc(size_t size, uint8_t *start_addr[]);
ALLOCATOR_RESULT static_allocator_free(uint8_t *addr[], size_t pages_num);







#endif // STATIC_ALLOCATOR_H
