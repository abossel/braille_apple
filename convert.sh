# convert the video to individual frames
ffmpeg -i badapple.webm -vf fps=10 frame_%04d.png

# resize and convert the frames and convert to black and white
for i in frame_*.png
do
  if [ -f "$i" ]
  then
    echo "Resizing $i"
    convert "$i" -resize 120x90 -monochrome -depth 1 "resize_$i"
  fi
done

# generate the c file
cat << EOF >> badapple.h
#ifndef BADAPPLE_H
#define BADAPPLE_H

#include <stddef.h>

char *badapple[] =
{
EOF

for i in resize_frame_*.png
do
  if [ -f "$i" ]
  then
    echo "Adding $i to badapple.h"
    python3 pngtostr.py "$i" >> badapple.h
  fi
done

cat << EOF >> badapple.h
    NULL
};

#endif
EOF
