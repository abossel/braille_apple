#include <stdio.h>
#include "badapple_uncompressed.h"

#define DISPLAY_WIDTH 120
#define DISPLAY_HEIGHT 90

void compress_frame(char *frame)
{
    int x, y, i;
    int line, byte, count, previous, colour;

    i = 0;
    line = 0;
    count = 0;
    previous = -1;
    printf("    \"");
    for (y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (x = 0; x < DISPLAY_WIDTH; x++)
        {
            // each byte in the frame data stores 8 pixels
            colour = (frame[i / 8] >> (7 - (i % 8))) & 0x1;

            if (colour != previous)
            {
                if (count != 0)
                {
                    byte |= count;
                    printf("\\x%02x", byte);
                    line++;
                }
                previous = colour;
                byte = 0;
                if (colour)
                    byte |= 0x80;
                count = 1;
            }
            else
            {
                if (count == 127)
                {
                    byte |= count;
                    printf("\\x%02x", byte);
                    line++;
                    byte &= 0x80;
                    count = 0;
                }
                count++;
            }

            if (line == 18)
            {
                printf("\"\n    \"");
                line = 0;
            }

            i++;
        }
    }
    if (count != 0)
    {
        byte |= count;
        printf("\\x%02x", byte);
    }
    printf("\",\n\n");
}

int main()
{
    int i;

    printf("#ifndef BADAPPLE_H\n"
           "#define BADAPPLE_H\n"
           "\n"
           "#include <stddef.h>\n"
           "\n"
           "char *badapple[] =\n"
           "{\n");

    i = 0;
    while (badapple[i] != NULL)
    {
        compress_frame(badapple[i]);
        i++;
    }

    printf("    NULL\n"
           "};\n"
           "\n"
           "#endif\n");

    return 0;
}
