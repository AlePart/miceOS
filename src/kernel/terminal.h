#ifndef H
#define H

#include <stddef.h>
#include <stdint.h>

#include "memory.h"
#include "string.h"

/* Hardware text mode color constants. */

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

class Terminal
{
public:
    enum Color {
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

    static void initialize(void);
    static void newline();
    static void setcolor(uint8_t color);
    static void scroll();
    static void putentry_at(char c, uint8_t color, size_t x, size_t y);
    static void putchar(char c);
    static void write(const char* data, size_t size);
    static void write(const char* data);
    static void change_color(Color fg, Color bg);
};


#endif
