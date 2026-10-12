#!/usr/bin/env python3
"""w21b: print ECOFF .mdebug local symbols (name, st, sc, value) of an IDO object built with -g3/-g2.
    python3 mdlocals.py OBJ      (diagnostic only: shows which named locals got which frame offsets)"""
import struct, sys
d = open(sys.argv[1], 'rb').read()
# ELF32 big-endian section headers
shoff, = struct.unpack('>I', d[0x20:0x24]); shentsize, shnum, shstrndx = struct.unpack('>HHH', d[0x2E:0x34])
secs = [struct.unpack('>IIIIIIIIII', d[shoff + i * shentsize: shoff + (i + 1) * shentsize]) for i in range(shnum)]
strtab = secs[shstrndx]
name = lambda o: d[strtab[4] + o: d.index(b'\0', strtab[4] + o)].decode()
md = [s for s in secs if name(s[0]) == '.mdebug'][0]
h = struct.unpack('>HH' + 'I' * 23, d[md[4]: md[4] + 96])
isymMax, cbSymOffset, issMax, cbSsOffset = h[9], h[10], h[15], h[16]
ST = {1: 'Global', 2: 'Static', 3: 'Param', 4: 'Local', 5: 'Label', 6: 'Proc', 7: 'Block', 8: 'End', 14: 'StaticProc'}
for i in range(isymMax):
    iss, value, bits = struct.unpack('>iiI', d[cbSymOffset + 12 * i: cbSymOffset + 12 * i + 12])
    st, sc = bits >> 26, (bits >> 21) & 31
    nm = d[cbSsOffset + iss: d.index(b'\0', cbSsOffset + iss)].decode() if iss >= 0 else ''
    print(f"{ST.get(st, st):10} sc={sc:2} value={value:6} {nm}")
