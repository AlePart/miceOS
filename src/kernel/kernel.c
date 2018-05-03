#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "memory/allocator/basic_allocator.h"
#include "memory/mmu/paging_management.h"
#include "memory/GDT/GDT.h"

extern void kmemcpy(void*,void*,size_t);
extern void kmemset(void*, uint8_t, size_t);
extern uint32_t kmemcmp(void*, void*, size_t);

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) 
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}


char * itoa( int value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    low = ptr;
    do
    {
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    *ptr-- = '\0';
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}
void memcpy_i386(void* dest, void* src, size_t size)
{
    __asm__ volatile ( "cld\n\t" "rep\n\t" "movsb" : : "S" (src), "D" (dest), "c" (size)  );
}
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;

/* Note the use of the volatile keyword to prevent the compiler from eliminating dead stores. */
volatile uint16_t* terminal_buffer;

inline void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) 
{
    terminal_color = color;
}


void terminal_scroll() 
{
    memcpy_i386((void*)terminal_buffer,(void*)(terminal_buffer + VGA_WIDTH), VGA_WIDTH*(VGA_HEIGHT-1) );
    terminal_row--;
} 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) 
{

    if('\n'==c)
    {
        terminal_newline();
    }
    else
    {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row--;
        }
    }


}

void terminal_write(const char* data, size_t size) 
{
    for (size_t i = 0; i < size; i++)
    {
        if(terminal_row == VGA_HEIGHT)
            terminal_scroll();
        terminal_putchar(data[i]);

    }
}

inline void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}

void terminal_change_color(enum vga_color fg, enum vga_color bg) 
{

    terminal_color = vga_entry_color(fg, bg);

}
void terminal_newline()
{
    terminal_row++;
    terminal_column=0;
}


static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}
static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

char getScancode()
{
    while (!(inb(0x64) & 1));
    return inb(0x60);
}
char getchar() {

    return getScancode(); // must be pasrsed with scancode
}


void kernel_main(/*multiboot_info_t* mbd, unsigned int magic*/)
{
    __asm__("nop");
    uint8_t test[5];
    uint8_t test2[5];
    uint32_t cmp = 0;
    kmemset(&test,0xFF,sizeof(test));

    cmp = kmemcmp(test2,test,sizeof(test));
    kmemcpy(test2,test,sizeof(test));
    cmp = kmemcmp(test,test2,sizeof(test));
    test2[3]=5;
    cmp = kmemcmp(test,test2,sizeof(test));

    GDT_init(1024*1024*1024 * 4);
    GDT_update();
    basic_allocator_initialize(1024*1024*1,1024*1024*1024 *3);
    uint32_t* add1 = (uint32_t*)basic_allocator_alloc(1024*4);
    uint32_t* add2 = (uint32_t*)basic_allocator_alloc(1024*4);
    uint32_t* add3 = (uint32_t*)basic_allocator_alloc(1024*4);
    uint32_t* add4 = (uint32_t*)basic_allocator_alloc(1024*4);
    uint32_t* add5 = (uint32_t*)basic_allocator_alloc(1024*4);
    uint32_t* add6 = (uint32_t*)basic_allocator_alloc(1024*4);
    basic_allocator_free(add1);
    basic_allocator_free(add2);
    basic_allocator_free(add3);
    basic_allocator_free(add4);
    basic_allocator_free(add5);
    basic_allocator_free(add6);
    /*
    PAGE_DIR dir = page_allocator_init(0x1000000);
    PAGE_DIR d = allocate_pages(1024*1024*30);*/
    /* Initialize terminal interface */
    terminal_initialize();
    terminal_writestring("Hello, kernel World!\n");



}
