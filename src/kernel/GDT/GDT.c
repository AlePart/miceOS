#incldue "GDT.h"

uint64_t descriptors[MAX_DESCRIPTORS]; 

void GDT_create_descriptor(uint32_t base, uint32_t limit, uint16_t flag, uint64_t & descriptor)
{
    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24
 
    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;
 
    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0
    gdt.size++;
 
}

uint64_t* GDT_search_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t element;
    GDT_create_descriptor(base,limit,flag,element);
    for(int i =0; i<=gdt.size; ++i)
    {
      if(element == descriptors[i])
      {
        return &descriptors[i];
      }
    } 
    return NULL;
}


void GDT_init(uint32_t kernel_reservation)
{
  gdt.size =0;
  gdt.address=0;
  //allocate GDT
  gdt.address = gdt.descriptors;
  create_descriptor(0, 0, 0,descriptors[0]);
  create_descriptor(0, 0x0001000000, (GDT_CODE_PL0),ker_code,descriptors[1]);
  create_descriptor(0, 0x0001000000, (GDT_DATA_PL0),descriptors[2]);
  gdt.size--; // size must be element number -1
}





void GDT_append_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
  gdt.size++;
  create_descriptor(base, limit, flag ,descriptors[gdt.size]);
  
}

void GDT_remove_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
  uint64_t* elem = GDT_search_descriptor(base,limit,flag)
  if(NULL != elem)
  {
    
  }
}
