#ifndef BASIC_ALLOCATOR_H
#define BASIC_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t Word;

const size_t WORD_SIZE = sizeof(Word);

typedef struct SegmentStruct {
    Word m_size;
    Word m_previous;
    Word m_next;
} Segment;

Segment* segment_previous(const Segment* self);
void segment_set_previous(Segment* self, Segment* previous);
Segment* segment_next(const Segment* self);
void segment_set_next(Segment* self, Segment* next);
size_t segment_size(const Segment* self);
void segment_set_size(Segment* self, size_t size);

void basic_allocator_initialize(void* start, size_t memory_size);
void* basic_allocator_alloc(size_t size);
void basic_allocator_free(void* address);
size_t basic_allocator_num_free_segments();
size_t basic_allocator_num_used_fregments();
Segment* basic_allocator_free_head();
Segment* basic_allocator_used_head();


void segment_list_pop(Segment** head, Segment* segment);
void segment_list_push(Segment** head, Segment* segment);
size_t segment_list_size(Segment* head);

#endif
