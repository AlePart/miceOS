#ifndef BASIC_ALLOCATOR_H
#define BASIC_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

struct Segment {
    size_t size;
    Segment* previous;
    Segment* next;
};

class BasicAllocator
{
public:
    static void initialize(void* start, size_t memory_size);

    static void* alloc(size_t size);
    static void free(void* address);

    static size_t num_free_segments();
    static size_t num_used_segments();

    static Segment* free_head();
    static Segment* used_head();
};

#endif
