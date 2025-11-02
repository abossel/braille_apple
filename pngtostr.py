from PIL import Image
import sys

if len(sys.argv) != 2:
    print('Missing file argument', file=sys.stderr)
    exit(1)

try:
    img = Image.open(sys.argv[1], 'r')
except FileNotFoundError:
    print(f'Cannot open file: {sys.argv[1]}', file=sys.stderr)
    exit(1)

byte = 0
shift = 0
newline = 0

for y in range(img.height):
    for x in range(img.width):
        if img.getpixel((x, y)) == 255:
            byte |= 1 << (7 - shift)
        if newline == 0:
            print('    "', end='')
        shift += 1
        newline += 1
        if shift == 8:
            print(f'\\x{byte:02x}', end='')
            byte = 0
            shift = 0
        if newline == 120:
            if y == img.height - 1 and x == img.width - 1:
                print('",')
            else:
                print('"')
            newline = 0
print('')
img.close()