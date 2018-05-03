#include "static_allocator.h"

#include "../../libc/libc.h"

#ifndef STATIC_ALLOC_DICTIONARY_INITAL_PAGES
#define STATIC_ALLOC_DICTIONARY_INITAL_PAGES (16)
#endif

#define SPACE_ELEMENT_FREE (1<<0)
#define SPACE_ELEMENTS_ARRAY (1<<1)
#define SPACE_DICTIONARY_LIST (1<<1)
#define SPACE_ELEMENT_ALLOCATED (1<<2)


#define STATIC_DICTIONARY_ELEMENTS  ((4096 / sizeof(STATIC_ALLOC_DICTIONARY_ELEMENT)) * STATIC_ALLOC_DICTIONARY_INITAL_PAGES)

typedef struct
{
    uint32_t* addr_start;
    uint32_t* addr_end;
    uint16_t type;

}STATIC_ALLOC_DICTIONARY_ELEMENT;
typedef struct
{
    struct DICTIONARY_LIST* next;
    struct DICTIONARY_LIST* prev;
}DICTIONARY_LIST;

static size_t STATIC_DICTIONARY_ELEMENT_FOR_PAGE = (4096 / sizeof(STATIC_ALLOC_DICTIONARY_ELEMENT));

static STATIC_ALLOC_DICTIONARY_ELEMENT* base_dictionary;


static STATIC_ALLOC_DICTIONARY_ELEMENT*  last_element;

void static_allocator_init(uint32_t base_address, size_t mem_size)
{
    base_dictionary = (uint32_t*)(base_address & 0xFFFFF000);
    kmemset(base_dictionary, 0x00, sizeof(base_dictionary));
    base_dictionary[0].addr_start = base_dictionary;
    base_dictionary[0].addr_end = base_dictionary + STATIC_DICTIONARY_ELEMENTS;
    base_dictionary[0].type |= SPACE_ELEMENTS_ARRAY;
    last_element = &base_dictionary[STATIC_DICTIONARY_ELEMENTS - 1];


}

ALLOCATION_RESULT static_allocator_alloc(size_t size)
{
    return ALLOCATION_OK;
}
