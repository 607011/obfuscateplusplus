#!/usr/bin/env python3

import os
import sys

def lcg(x, a, c, m):
    while True:
        yield x
        x = ((a * x + c) & 0xffffffff) % m

def convert(filename, key):
    size = os.path.getsize(filename)
    r = lcg(key, 48271, 0, 2147483647)
    var_name = os.path.basename(filename).replace(".", "_")
    result = f'constexpr size_t {var_name}_size = {size};\n'
    result += f'const unsigned char {var_name}[{var_name}_size] = {{\n    '
    i = 0
    for byte in open(filename, 'rb').read():
        mask = next(r) & 0xff
        result += f'0x{(byte ^ mask):02x}'
        i += 1
        if i < size:
            result += ', '
        if i % 16 == 0:
            result += '\n    '
    assert(i == size)
    result += '\n};'
    print(result, file=open(f'{filename}.h', 'w+'))

def main(filename, obfuscation_key):
    convert(filename, int(obfuscation_key, base=16))

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('''Usage: 
            file2h.py input_file key
        ''')
    main(sys.argv[1], sys.argv[2])
