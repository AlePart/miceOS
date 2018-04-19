#ifndef GDT_H
#define GDT_H
#include "GDT_types.h"

 
void GDT_create_descriptor(uint32_t base, uint32_t limit, uint16_t flag, uint64_t & descriptor);
void GDT_init(uint32_t kernel_reservation);
void GDT_append_descriptor(uint32_t base, uint32_t limit, uint16_t flag);
void GDT_remove_descriptor(uint32_t base, uint32_t limit, uint16_t flag);



#endif
