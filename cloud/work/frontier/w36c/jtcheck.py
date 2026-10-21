#!/usr/bin/env python3
"""jtcheck.py OBJ NAME RODATA_OFF N RETAIL_TABLE_ADDR: compare a jump table's entries (as offsets from NAME's start)."""
import sys, struct, subprocess, re
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
obj, name, roff, n, raddr = sys.argv[1], sys.argv[2], int(sys.argv[3], 0), int(sys.argv[4]), int(sys.argv[5], 0)
img = open(ROOT / 'build/ovl/a/image.bin', 'rb').read(); BASE = 0x8038A400
import json
fstart_r = int(json.load(open(ROOT / 'asm/us/ovl_a/symbols.json'))['symbols'][name], 16)
ret = [struct.unpack('>I', img[raddr - BASE + 4*i: raddr - BASE + 4*i + 4])[0] - fstart_r for i in range(n)]
# object: relocs in .rel.rodata
out = subprocess.run(['mips-linux-gnu-readelf', '-r', '-s', '-x', '.rodata', obj], capture_output=True, text=True).stdout
syms = {}
for l in out.splitlines():
    m = re.match(r'\s*\d+:\s+([0-9a-f]+)\s+\d+\s+FUNC\s+\w+\s+\w+\s+\d+\s+(\S+)', l)
    if m: syms[m.group(2)] = int(m.group(1), 16)
ro = subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '--only-section=.rodata', obj, '/dev/stdout'], capture_output=True).stdout
words = [struct.unpack('>I', ro[roff + 4*i: roff + 4*i + 4])[0] for i in range(n)]
got = [w - syms[name] for w in words]
print('retail', [hex(x) for x in ret]); print('object', [hex(x) for x in got]); print('EQUAL' if ret == got else 'DIFF')
