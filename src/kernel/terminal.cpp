#include "terminal.h"

#include <stdint.h>

namespace
{
    size_t terminal_row;
    size_t terminal_column;
    uint8_t terminal_color;
    uint16_t* terminal_buffer;

    inline uint8_t vga_entry_color(Terminal::Color fg, Terminal::Color bg)
    {
        return fg | bg << 4;
    }

    inline uint16_t vga_entry(unsigned char uc, uint8_t color)
    {
        return (uint16_t) uc | (uint16_t) color << 8;
    }
}

void Terminal::initialize()
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

void Terminal::newline()
{
    terminal_row++;
    terminal_column=0;
}

void Terminal::setcolor(uint8_t color)
{
    terminal_color = color;
}

void Terminal::scroll()
{
    memcpy_i386((void*)terminal_buffer,(void*)(terminal_buffer + VGA_WIDTH), VGA_WIDTH*(VGA_HEIGHT-1) );
    terminal_row--;
}

void Terminal::putentry_at(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void Terminal::putchar(char c)
{
    if('\n' == c) {
        newline();
    }
    else {
        putentry_at(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row--;
        }
    }
}

void Terminal::write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        if(terminal_row == VGA_HEIGHT)
            scroll();
        putchar(data[i]);
    }
}

void Terminal::write(const char *data)
{
    write(data, strlen(data));
}

void Terminal::change_color(Color fg, Color bg)
{
    terminal_color = vga_entry_color(fg, bg);
}
