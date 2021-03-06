

#include "GDT.h"

uint64_t descriptors[MAX_DESCRIPTORS]; //64k descriptors size
extern void reload_data(void);
static struct gdtstr gdt;

void GDT_create_descriptor(uint32_t base, uint32_t limit, uint16_t flag, uint64_t *descriptor)
{
    // Create the high 32 bit segment
    *descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    *descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    *descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    *descriptor |=  base        & 0xFF000000;         // set base bits 31:24

    // Shift by 32 to allow for low part of segment
    *descriptor <<= 32;

    // Create the low 32 bit segment
    *descriptor |= base  << 16;                       // set base bits 15:0
    *descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0
    gdt.size++;

}

uint32_t GDT_search_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t element;
    GDT_create_descriptor(base,limit,flag,element);
    for(uint32_t i =0; i<=gdt.size; ++i)
    {
        if(element == descriptors[i])
        {
            return i;
        }
    }
    return 0;
}

void GDT_init(size_t size)
{  
    gdt.size =0;
    gdt.address=0;
    //allocate GDT
    gdt.address = (uint32_t) descriptors;
    GDT_create_descriptor(0, 0, 0,&descriptors[0]);
    GDT_create_descriptor(0, size, (GDT_CODE_PL0),&descriptors[1]);
    GDT_create_descriptor(0, size, (GDT_DATA_PL0),&descriptors[2]);
    GDT_create_descriptor(0, size, (GDT_CODE_PL3),&descriptors[3]);
    GDT_create_descriptor(0, size, (GDT_DATA_PL3),&descriptors[4]);
    gdt.size--; // size must be element number -1
}


void GDT_append_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    gdt.size++;
    GDT_create_descriptor(base, limit, flag ,descriptors[gdt.size]);

}

void GDT_remove_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint32_t elem_pos = GDT_search_descriptor(base,limit,flag);

    for(uint32_t i = elem_pos ; i<gdt.size; i++) // from found element position to element size - 1
    {
        descriptors[i]=descriptors[i+1];
    }
    //clean last element
    descriptors[gdt.size] = 0;
    gdt.size--;

}


void GDT_update()
{
    asm ("movl %0, %%eax"
    :
    :"r"(gdt.address)     /* x is input operand */
            :"%eax");   /* %eax is clobbered register */
    reload_data();
}




