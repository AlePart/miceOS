#include "basic_allocator.h"

namespace {

void* memory_start;
Segment* free_segment_list;
Segment* used_segment_list;

}

void SegmentList::pop(Segment *&head, Segment *segment)
{
    if (segment->previous()) {
        segment->previous()->set_next(segment->next());
    }
    else if (segment->next()) {
        segment->next()->set_previous(nullptr);
        head = segment->next();
    } else {
        head = nullptr;
    }

    segment->set_previous(nullptr);
    segment->set_previous(nullptr);
}

void SegmentList::push(Segment *&head, Segment *segment)
{
    if (head) {
        head->set_next(segment);
        segment->set_previous(head);
        segment->set_next(nullptr);
    } else {
        head = segment;
        segment->set_previous(nullptr);
        segment->set_next(nullptr);
    }
}

size_t SegmentList::size(Segment* head)
{
    size_t result = 0;
    Segment* segment = head;
    while (segment) {
        ++result;
        segment = segment->next();
    }
    return result;
}

void BasicAllocator::initialize(void* start, size_t num_bytes) {
    ::memory_start = start;

    ::free_segment_list = reinterpret_cast<Segment*>(::memory_start);
    ::used_segment_list = nullptr;

    ::free_segment_list->set_size(num_bytes - 3 * WORD_SIZE); // Size
    ::free_segment_list->set_previous(nullptr); // Previous
    ::free_segment_list->set_next(nullptr); // Next
}

void* BasicAllocator::alloc(size_t bytes_to_alloc) {
    bytes_to_alloc = ((bytes_to_alloc / WORD_SIZE) + 1) * WORD_SIZE;

    Segment* current_segment = free_segment_list;

    while (current_segment) {
        if (current_segment->size() < bytes_to_alloc) {
            current_segment = current_segment->next();
            continue;
        }

        if (current_segment->size() > bytes_to_alloc) {
            Segment* remainder = current_segment + 3 + (bytes_to_alloc / WORD_SIZE);
            remainder->set_size(current_segment->size() - bytes_to_alloc - 3 * WORD_SIZE);
            remainder->set_previous(current_segment);
            remainder->set_next(current_segment->next());

            current_segment->set_size(bytes_to_alloc);
            current_segment->set_next(remainder);
        }

        SegmentList::pop(::free_segment_list, current_segment);
        SegmentList::push(::used_segment_list, current_segment);

        return current_segment + 3;
    }

    return nullptr;
}

void BasicAllocator::free(void* address) {
    Segment* segment = ::used_segment_list;
    while (segment) {
        if (segment + 3 == address) {
            SegmentList::pop(::used_segment_list, segment);
            SegmentList::push(::free_segment_list, segment);
            return;
        } else {
            segment = segment->next();
        }
    }
}

size_t BasicAllocator::num_free_segments()
{
    return SegmentList::size(::free_segment_list);
}

size_t BasicAllocator::num_used_segments()
{
    return SegmentList::size(::used_segment_list);
}

Segment *BasicAllocator::free_head()
{
    return ::free_segment_list;
}

Segment *BasicAllocator::used_head()
{
    return ::used_segment_list;
}

Segment *Segment::previous() const {
    return reinterpret_cast<Segment*>(m_previous);
}

void Segment::set_previous(Segment *previous) {
    m_previous = reinterpret_cast<Word>(previous);
}

Segment *Segment::next() const {
    return reinterpret_cast<Segment*>(m_next);
}

void Segment::set_next(Segment *next) {
    m_next = reinterpret_cast<Word>(next);
}

size_t Segment::size() const {
    return m_size;
}

void Segment::set_size(size_t size) {
    m_size = size;
}
