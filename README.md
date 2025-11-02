# Braille Apple

Play the "Bad Apple" video on the console with braille characters. Compile `badapple.c` and run to display the animation. Press ctrl-c to exit.

Braille characters have 2x4 dots so an 80x24 console can display a 160x96 monochrome image. Changing the terminal colours would allow for some colour images. But all 8 pixels in one braille character would be the same colour.

The video was converted to 10fps and inidividual frames extracted with FFmpeg, resized with ImageMagick then converted to C strings with `pngtostr.py`. Each frame of the animation is stored as a string with each byte storing 8 pixels.

It should be easy to convert other videos for this. The video would need to be 4:3 aspect ratio as the convertion script will resize the video to 120x90. The "Bad Apple" video works well because it is already black and white.
