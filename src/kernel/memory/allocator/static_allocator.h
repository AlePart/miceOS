#ifndef STATIC_ALLOCATOR_H
#define STATIC_ALLOCATOR_H
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint32_t* addr_start;
    uint32_t* addr_end;
    struct STATIC_ALLOC_DICTIONARY* next;
}STATIC_ALLOC_DICTIONARY;

static STATIC_ALLOC_DICTIONARY* _base = NULL;
static uint32_t* lastAddress;
static uint32_t* startAddress;


void static_allocator_init(uint32_t base_address, size_t mem_size)
{
    lastAddress= (uint32_t*) (((uint8_t*)((base_address& 0xFFFFF000 )) + mem_size));
    startAddress = (uint32_t*)(base_address& 0xFFFFF000 );
}

void static_allocator_alloc(size_t size)
{

    if(NULL == _base)
    {
        _base = startAddress;
    }


}






#endif // STATIC_ALLOCATOR_H
