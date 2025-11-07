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

    // copy each bit from the frame to 1 byte on the display
    // each byte in the frame data stores 8 pixels
    i = 0;
    for (y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (x = 0; x < DISPLAY_WIDTH; x++)
        {
            sparkle_set_pixel(s, x, y, (frame[i / 8] >> (7 - (i % 8))) & 0x1);
            i++;
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
