#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "badapple.h"

#define DISPLAY_WIDTH 120
#define DISPLAY_HEIGHT 90

/*
 * convert unicode code point to utf-8 string
 */
void code_point_to_utf8(char *dst, int code)
{
    if (code >= 0x0000 && code <= 0x007f)
    {
        dst[0] = code & 0x7f;
        dst[1] = '\0';
    }
    if (code >= 0x0080 && code <= 0x07ff)
    {
        dst[0] = (code >> 6) & 0x1f;
        dst[0] |= 0xc0;
        dst[1] = code & 0x3f;
        dst[1] |= 0x80;
        dst[2] = '\0';
    }
    if (code >= 0x0800 && code <= 0xffff)
    {
        dst[0] = (code >> 12) & 0x0f;
        dst[0] |= 0xe0;
        dst[1] = (code >> 6) & 0x3f;
        dst[1] |= 0x80;
        dst[2] = code & 0x3f;
        dst[2] |= 0x80;
        dst[3] = '\0';
    }
    if (code >= 0x010000 && code <= 0x10ffff)
    {
        dst[0] = (code >> 18) & 0x07;
        dst[0] |= 0xf0;
        dst[1] = (code >> 12) & 0x3f;
        dst[1] |= 0x80;
        dst[2] = (code >> 6) & 0x3f;
        dst[2] |= 0x80;
        dst[3] = code & 0x3f;
        dst[3] |= 0x80;
        dst[4] = '\0';
    }
}

/*
 * free a null-terminated array
 */
char **free_nta(char **nta)
{
    int i;

    if (nta != NULL)
    {
        i = 0;
        while (nta[i] != NULL)
        {
            free(nta[i]);
            i++;
        }
        free(nta);
    }

    return NULL;
}

/*
 * generate a null-terminated array of the braille utf-8 characters
 */
char **generate_braille_utf8()
{
    char **braille;
    int i;

    // allocate the nta
    braille = malloc(sizeof(char *) * 257);
    if (braille == NULL)
        return NULL;
    for (i = 0; i < 256; i++)
    {
        braille[i] = malloc(5);
        if (braille[i] == NULL)
            return free_nta(braille);
        braille[i][0] = '\0';
    }
    braille[i] = NULL;

    // generate braille utf-8 characters U+2800 to U+28FF
    for (i = 0; i < 256; i++)
        code_point_to_utf8(braille[i], 0x2800 | i);

    return braille;
}

/*
 * set every pixel in the display buffer to black
 */
void clear_display(char *display)
{
    // the display stores 
    memset(display, 0, DISPLAY_WIDTH * DISPLAY_HEIGHT);
}

/*
 * copy from the movie frame to the display buffer
 */
void copy_frame_to_display(char *display, char *frame)
{
    int i;

    // copy each bit from the frame to 1 byte on the display
    // each byte in the frame data stores 8 pixels
    for (i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++)
        display[i] = (frame[i / 8] >> (7 - (i % 8))) & 0x1;
}

/*
 * get a pixel from the display buffer or 0 if out of range
 */
char get_pixel(char *display, int x, int y)
{
    if (x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT)
        return display[x + y * DISPLAY_WIDTH];
    return 0;
}

/*
 * draw the display buffer on the console
 */
void draw_display(char *display, char **braille)
{
    // buffer needs approx W x H x 3 + H + 10 bytes
    char buffer[32768];
    char *back;
    int size;
    int x, y, p;

    // back points to the end of the buffer
    // size is the number of bytes in the buffer
    back = buffer;
    size = 0;

    // clear the screen and move cursor to home
    memcpy(back, "\e[2J\e[H", 7);
    back += 7;
    size += 7;

    for (y = 0; y < DISPLAY_HEIGHT; y += 4)
    {
        for (x = 0; x < DISPLAY_WIDTH; x += 2)
        {
            // dots on braille chars map to bits in the unicode code point
            p = 0;
            if (get_pixel(display, (x + 0), (y + 0))) p |= 0x01;
            if (get_pixel(display, (x + 0), (y + 1))) p |= 0x02;
            if (get_pixel(display, (x + 0), (y + 2))) p |= 0x04;
            if (get_pixel(display, (x + 0), (y + 3))) p |= 0x40;
            if (get_pixel(display, (x + 1), (y + 0))) p |= 0x08;
            if (get_pixel(display, (x + 1), (y + 1))) p |= 0x10;
            if (get_pixel(display, (x + 1), (y + 2))) p |= 0x20;
            if (get_pixel(display, (x + 1), (y + 3))) p |= 0x80;
            // all braille utf-8 chars are 3 bytes
            memcpy(back, braille[p], 3);
            back += 3;
            size += 3;
        }
        *back = '\n';
        back += 1;
        size += 1;
    }

    write(STDOUT_FILENO, buffer, size);
}

int main()
{
    char display[DISPLAY_WIDTH * DISPLAY_HEIGHT];
    char **braille;
    int i;

    braille = generate_braille_utf8();
    if (braille == NULL)
        return 1;

    i = 0;
    clear_display(display);
    while (badapple[i] != NULL)
    {
        copy_frame_to_display(display, badapple[i]);
        draw_display(display, braille);
        // movie is 10 fps so sleep for 1/10 second
        usleep(100000);
        i++;
    }

    free_nta(braille);

    return 0;
}
