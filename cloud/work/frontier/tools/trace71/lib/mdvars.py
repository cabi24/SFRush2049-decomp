#!/usr/bin/env python3
"""mdvars.py OBJ FN -- the declared locals and parameters of FN from an IDO `-g3` object's ECOFF .mdebug, one
`OFFSET NAME KIND` line each (KIND Local/Param). For IDO 7.1 the mdebug value of a local is its cfe virtual-frame
offset, the same number the trace71 colouring trace prints as `var@OFF` (w22c; after w21b's mdlocals.py), so
sum71.sh can name every variable web. Stdlib only."""
import struct
import sys

ST = {1: 'Global', 2: 'Static', 3: 'Param', 4: 'Local', 5: 'Label', 6: 'Proc', 7: 'Block', 8: 'End', 14: 'StaticProc'}


def symbols(data):
    """Yield (st_name, sc, value, name) for every local symbol in the .mdebug of a big-endian ELF32 object."""
    shoff, = struct.unpack('>I', data[0x20:0x24])
    shentsize, shnum, shstrndx = struct.unpack('>HHH', data[0x2E:0x34])
    secs = [struct.unpack('>IIIIIIIIII', data[shoff + i * shentsize: shoff + (i + 1) * shentsize]) for i in range(shnum)]
    strtab = secs[shstrndx]

    def secname(off):
        start = strtab[4] + off
        return data[start: data.index(b'\0', start)].decode()

    md = [s for s in secs if secname(s[0]) == '.mdebug'][0]
    hdr = struct.unpack('>HH' + 'I' * 23, data[md[4]: md[4] + 96])
    isym_max, cb_sym, cb_ss = hdr[9], hdr[10], hdr[16]
    for i in range(isym_max):
        iss, value, bits = struct.unpack('>iiI', data[cb_sym + 12 * i: cb_sym + 12 * i + 12])
        st, sc = bits >> 26, (bits >> 21) & 31
        name = data[cb_ss + iss: data.index(b'\0', cb_ss + iss)].decode() if iss >= 0 else ''
        yield ST.get(st, str(st)), sc, value, name


def proc_vars(syms, fn):
    """The Local/Param symbols between FN's Proc symbol and its matching End."""
    out, depth, inside = [], 0, False
    for st, _sc, value, name in syms:
        if not inside:
            if st in ('Proc', 'StaticProc') and name == fn:
                inside, depth = True, 1
            continue
        if st in ('Proc', 'StaticProc', 'Block'):
            depth += 1
        elif st == 'End':
            depth -= 1
            if depth == 0:
                break
        elif st in ('Local', 'Param'):
            out.append((value, name, st))
    return out


def main():
    data = open(sys.argv[1], 'rb').read()
    for value, name, st in proc_vars(symbols(data), sys.argv[2]):
        print(value, name, st)


if __name__ == '__main__':
    main()
