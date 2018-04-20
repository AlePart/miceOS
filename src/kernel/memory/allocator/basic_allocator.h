#ifndef BASIC_ALLOCATOR_H
#define BASIC_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

typedef struct SegmentStruct {
    size_t size;
    struct SegmentStruct* previous;
    struct SegmentStruct* next;
} Segment;

void basic_allocator_initialize(void* start, size_t memory_size);
void* basic_allocator_alloc(size_t size);
void basic_allocator_free(void* address);
size_t basic_allocator_num_free_segments();
size_t basic_allocator_num_used_fregments();
Segment* basic_allocator_free_head();
Segment* basic_allocator_used_head();

#endif
