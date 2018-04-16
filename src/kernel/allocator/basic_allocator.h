#ifndef BASIC_ALLOCATOR_H
#define BASIC_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

using Word = uint64_t;
const size_t WORD_SIZE = sizeof(Word);

class Segment;

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

class Segment {
public:
    Segment* previous() const;
    void set_previous(Segment* previous);

    Segment* next() const;
    void set_next(Segment* next);

    size_t size() const;
    void set_size(size_t size);

private:
    Word m_size;
    Word m_previous;
    Word m_next;
} __attribute__((packed));

class SegmentList {
public:
    SegmentList() = delete;

    static void pop(Segment*& head, Segment* segment);
    static void push(Segment*& head, Segment* Segment);
    static size_t size(Segment* head);
};

static_assert(sizeof(Segment) == 3 * WORD_SIZE);

#endif
