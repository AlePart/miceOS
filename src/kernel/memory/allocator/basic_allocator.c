#include "basic_allocator.h"

static void* memory_start;
static Segment* free_segment_list;
static Segment* used_segment_list;

void basic_allocator_initialize(void *start, size_t memory_bytes_size)
{
    memory_start = start;
    free_segment_list = (Segment*) memory_start;
    used_segment_list = NULL;

    segment_set_size(free_segment_list, memory_bytes_size - 3 * WORD_SIZE);
    segment_set_previous(free_segment_list, NULL);
    segment_set_next(free_segment_list, NULL);
}

void *basic_allocator_alloc(size_t bytes_to_alloc)
{
    bytes_to_alloc = ((bytes_to_alloc / WORD_SIZE) + 1) * WORD_SIZE;

    Segment* current_segment = free_segment_list;

    while (current_segment) {
        if (segment_size(current_segment) < bytes_to_alloc) {
            current_segment = segment_next(current_segment);
            continue;
        }

        if (segment_size(current_segment) > bytes_to_alloc) {
            Segment* remainder = current_segment + 3 + (bytes_to_alloc / WORD_SIZE);
            segment_set_size(remainder, segment_size(current_segment) - bytes_to_alloc - 3 * WORD_SIZE);
            segment_set_previous(remainder, current_segment);
            segment_set_next(remainder, segment_next(current_segment));

            segment_set_size(current_segment, bytes_to_alloc);
            segment_set_next(current_segment, remainder);
        }

        segment_list_pop(&free_segment_list, current_segment);
        segment_list_push(&used_segment_list, current_segment);

        return current_segment + 3;
    }

    return NULL;
}

void basic_allocator_free(void *address)
{
    Segment* segment = used_segment_list;
    while (segment) {
        if (segment + 3 == address) {
            segment_list_pop(&used_segment_list, segment);
            segment_list_push(&free_segment_list, segment);
            return;
        } else {
            segment = segment_next(segment);
        }
    }
}

size_t basic_allocator_num_free_segments()
{
    return segment_list_size(free_segment_list);
}

size_t basic_allocator_num_used_fregments()
{
    return segment_list_size(used_segment_list);
}

Segment *basic_allocator_free_head()
{
    return free_segment_list;
}

Segment *basic_allocator_used_head()
{
    return used_segment_list;
}

Segment *segment_previous(const Segment *self)
{
    return (Segment*)self->m_previous;
}

void segment_set_previous(Segment *self, Segment *previous)
{
    self->m_previous = (Word)previous;
}

Segment *segment_next(const Segment *self)
{
    return (Segment*)self->m_next;
}

void segment_set_next(Segment *self, Segment *next)
{
    self->m_next = (Word)next;
}

size_t segment_size(const Segment *self)
{
    return self->m_size;
}

void segment_set_size(Segment *self, size_t size)
{
    self->m_size = size;
}

void segment_list_pop(Segment **head, Segment *segment)
{
    if (segment_previous(segment)) {
        segment_set_next(segment_previous(segment), segment_next(segment));
    }
    else if (segment_next(segment)) {
        segment_set_previous(segment_next(segment), NULL);
        *head = segment_next(segment);
    } else {
        *head = NULL;
    }

    segment_set_previous(segment, NULL);
    segment_set_next(segment, NULL);
}

void segment_list_push(Segment **head, Segment *segment)
{
    if (*head) {
        segment_set_next(*head, segment);
        segment_set_previous(segment, *head);
        segment_set_next(segment, NULL);
    } else {
        *head = segment;
        segment_set_previous(segment, NULL);
        segment_set_next(segment, NULL);
    }
}

size_t segment_list_size(Segment *head)
{
    size_t result = 0;
    Segment* segment = head;
    while (segment) {
        ++result;
        segment = segment_next(segment);
    }
    return result;
}
