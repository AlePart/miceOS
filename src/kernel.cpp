/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#include "kernel/allocator/basic_allocator.h"
#include "kernel/terminal.h"
#include "kernel/string.h"
#include "kernel/memory.h"

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

extern "C" {

void kernel_main(void) 
{
    Terminal::initialize();

    /* Initialize terminal interface */
    Terminal::write("Hello, kernel World!");
    Terminal::newline();
    Terminal::write("Wow a new Line\n");
    Terminal::write("Wow a new Line without call terminal_newline() function\n");
    Terminal::write("LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LONG LINE\n");
    for (int i =0; i< 20; i++)

    {
        char n[3];
        itoa(i,n,10);
        Terminal::write(n);
        Terminal::newline();
    }
    Terminal::change_color(Terminal::VGA_COLOR_RED, Terminal::VGA_COLOR_WHITE);
    char c = getchar();
    Terminal::write(&c);
    Terminal::newline();
    Terminal::write("last line");

}

}
