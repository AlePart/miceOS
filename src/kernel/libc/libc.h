#ifndef LIBC_H
#define LIBC_H

extern void kmemcpy(void*,void*,size_t);
extern void kmemset(void*, uint8_t, size_t);
extern uint32_t kmemcmp(void*, void*, size_t);

#endif // LIBC_H
