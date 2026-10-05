#!/usr/bin/env python3
"""rw.py ADDR [N]: print N retail words (and as float) at image address ADDR from the tracked own-data artefact."""
import sys, struct
sys.path.insert(0, 'tools/cloud')
import owndata
img = owndata.ImageData.from_artifact('asm/us/blob_data')
a = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 1
for base, data in img.runs:
    if base <= a < base + len(data):
        for i in range(n):
            w = data[a - base + 4 * i: a - base + 4 * i + 4]
            print('%08X: %s  %r' % (a + 4 * i, w.hex(), struct.unpack('>f', w)[0]))
        break
else:
    print('not in artefact')
