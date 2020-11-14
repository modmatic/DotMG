import os
import sys

# Works as of Pillow 7.2.0
# Run: pip install Pillow to use PIL
from PIL import Image


def to_4_bits(n):
    return int((n * 0xF) / 0xFF)

def to_4444_rgba(p):
    r = to_4_bits(p[0])
    g = to_4_bits(p[1])
    b = to_4_bits(p[2])
    a = 0xFF
    if len(p) >= 4:
        a = to_4_bits(p[3])
    return (r << 12) | (g << 8) | (b << 4) | a

def chunk(lst, n):
    for i in range(0, len(lst), n):
        yield lst[i:i+n]

def format_data_string(data, width):
    data_str = list(map(lambda d: '0x%04X' % d, data))
    rows = chunk(data_str, width)
    rows_str = list(map(lambda r: '  ' + ', '.join(r), rows))
    return ',\n'.join(rows_str)

if len(sys.argv) < 3:
    print('input and output paths required')
    exit(1)

path = sys.argv[1]
out = sys.argv[2]

name, ext = os.path.splitext(os.path.basename(out))

if ext != '.mg' and ext != '.h':
    print("output path extension must be '.mg' or '.h'")
    exit(1)

img = Image.open(path)
data = list(img.getdata())
data = list(map(to_4444_rgba, data))

if ext == '.mg':
    with open(out, 'wb') as fh:
        fh.write(bytearray([img.width, img.height] + data))
else: # .cpp
     with open(out, 'wt') as fh:
        fh.write('#ifndef '+ name.upper() + '_H\n')
        fh.write('#define '+ name.upper() + '_H\n\n')
        fh.write('const uint16_t ' + name + 'Width = ' + str(img.width) + ';\n')
        fh.write('const uint16_t ' + name + 'Height = ' + str(img.height) + ';\n')
        fh.write('const uint16_t ' + name + 'Data[] = {\n' + format_data_string(data, img.width) + '\n};\n')
        fh.write('const Color *' + name + ' = (Color *)' + name + 'Data;\n')
        fh.write('\n#endif // '+ name.upper() + '_H\n')
