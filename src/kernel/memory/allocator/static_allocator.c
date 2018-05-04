#include "static_allocator.h"

#include "../../libc/libc.h"

#ifndef STATIC_ALLOC_DICTIONARY_INITAL_PAGES
#define STATIC_ALLOC_DICTIONARY_INITAL_PAGES (16)
#endif


#define STATIC_DICTIONARY_ELEMENTS  (1024*1024)

typedef struct
{
    uint8_t* addr_start;
    uint8_t* addr_end;

}STATIC_ALLOC_DICTIONARY_ELEMENT;



static STATIC_ALLOC_DICTIONARY_ELEMENT* base_dictionary;
static const STATIC_ALLOC_DICTIONARY_ELEMENT* last_element;


void static_allocator_init(uint32_t* base_address)
{
    base_dictionary = (STATIC_ALLOC_DICTIONARY_ELEMENT*)base_address;
    kmemset((void*)base_dictionary,0x00, STATIC_DICTIONARY_ELEMENTS*sizeof(STATIC_ALLOC_DICTIONARY_ELEMENT));
    base_dictionary[0].addr_start=(uint8_t*)base_address;
    base_dictionary[0].addr_end= (uint8_t*)(((uint32_t)base_address) + STATIC_DICTIONARY_ELEMENTS*sizeof(STATIC_ALLOC_DICTIONARY_ELEMENT)) -1;
    last_element = &base_dictionary[ (STATIC_DICTIONARY_ELEMENTS*sizeof(STATIC_ALLOC_DICTIONARY_ELEMENT)) - 1];
}

STATIC_ALLOC_DICTIONARY_ELEMENT* search_allocation_present(uint8_t* addr, size_t size) // find if an address is just allocated
{
    STATIC_ALLOC_DICTIONARY_ELEMENT* el = &base_dictionary[0];
    uint8_t* addr_end = addr+size;
    while( el->addr_start != NULL )
    {
        if( ( (el->addr_start < addr && el->addr_end < addr) ||
              (el->addr_start > addr && el->addr_end > addr)) &&
                ((  (el->addr_start < addr_end && el->addr_end < addr_end) ||
                    (el->addr_start > addr_end && el->addr_end > addr_end))))
        {
            el++; // element not found
        }
        else
        {
            return el; //element found
        }
    }


    return NULL;

}

STATIC_ALLOC_DICTIONARY_ELEMENT* search_allocation_space(size_t size)  // search space
{
    STATIC_ALLOC_DICTIONARY_ELEMENT* el = &base_dictionary[0];
    uint8_t* suggested_addr = NULL;
    while(el->addr_start != NULL && el != last_element)
    {
        if(NULL == suggested_addr)
        {

            suggested_addr = el->addr_end+1;
            if(NULL !=search_allocation_present(suggested_addr, size) )
            {
                suggested_addr = NULL;
            }
        }
        el++;
    }
    if(NULL == suggested_addr) // to inspect last element if necessary
    {
        STATIC_ALLOC_DICTIONARY_ELEMENT* prev_el = (el - 1);
        suggested_addr = prev_el->addr_end+1;
        if(NULL !=search_allocation_present(suggested_addr, size) )
        {
            suggested_addr = NULL;
        }
    }

    if(NULL == suggested_addr)
    {
        el = NULL;

    }
    else
    {
        el->addr_start= suggested_addr;
        el->addr_end= el->addr_start+size;
    }
    return el;
}

ALLOCATION_RESULT static_allocator_alloc(size_t size, void** start_addr)
{
    STATIC_ALLOC_DICTIONARY_ELEMENT* el = search_allocation_space(size);
    if(el == 0)
    {
        return ALLOCATION_UNABLE_TO_ALLOC;
    }
    *start_addr = el->addr_start;

    return ALLOCATION_OK;
}
