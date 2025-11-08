#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "badapple.h"
#include "sparkle.h"

#define DISPLAY_WIDTH 120
#define DISPLAY_HEIGHT 90

/*
 * copy from the movie frame to the display buffer
 */
void copy_frame_to_display(t_sparkle *s, char *frame)
{
    int x, y, i;
    char byte, colour;

    byte = 0;
    colour = 0;
    i = 0;
    for (y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (x = 0; x < DISPLAY_WIDTH; x++)
        {
            // compressed data use the first bit to indicate the colour
            // the remaining 7 bits show the length of that colour
            if (byte == 0)
            {
                if (frame[i] == '\0')
                    return;
                byte = frame[i];
                colour = (byte & 0x80) >> 7;
                byte &= 0x7f;
                i++;
            }
            byte--;
            sparkle_set_pixel(s, x, y, colour);
        }
    }
}

int main()
{
    t_sparkle *s;
    int i;

    s = sparkle_create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if (s == NULL)
        return 1;
    sparkle_clear(s);

    i = 0;
    while (badapple[i] != NULL)
    {
        copy_frame_to_display(s, badapple[i]);
        sparkle_draw(s);
        // movie is 10 fps so sleep for 1/10 second
        usleep(100000);
        i++;
    }

    sparkle_free(s);

    return 0;
}
