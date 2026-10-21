"""w36y: content channel for initialised GC data objects (rush_d.elf .data/.rodata/.sdata/.sdata2).

Read-only; standard library only.

1. `check(gc_sym_start, gc_size, scope, n64_addr)`: compares the GC object's initial bytes with the N64 bytes at the
   candidate address word by word. Words the GC relocates (pointers) are compared through their targets: two
   pointers to NUL-terminated strings with the same text count as a match, a NULL pointer must be NULL on the N64.
2. `search(...)`: finds N64 addresses whose bytes equal the GC object's non-pointer words (pointer words are
   wildcards, but a GC string pointer must point to the same string text on the N64). Used to locate GC tables
   (positions, colours, string tables) in the N64 images independently of the code alignment.

N64 memory: game image (build/game_code.bin at 0x80086A50), runtime images A/B (build/ovl/{a,b}/image.bin at
0x8038A400, per image), static segment (baserom.us.z64 0x1000.. at 0x80000400).
"""
import os
import re
import struct

REPO = '/home/cburnes/projects/rush2049-decomp'
ELF = os.path.join(REPO, 'rush_d.elf')
INIT_SECTIONS = ('.rodata', '.data', '.sdata', '.sdata2')


class GCElf(object):
    def __init__(self):
        d = open(ELF, 'rb').read()
        self.d = d
        shoff, = struct.unpack_from('>I', d, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from('>HHH', d, 0x2E)
        secs = []
        for i in range(shnum):
            secs.append(struct.unpack_from('>IIIIIIIIII', d, shoff + i * shentsize))
        strtab_off = secs[shstrndx][4]

        def nm(o):
            e = d.index(b'\0', strtab_off + o)
            return d[strtab_off + o:e].decode()
        self.secs = {}
        for s in secs:
            self.secs[nm(s[0])] = s
        self.mem = []  # (start, end, file offset)
        for n in INIT_SECTIONS:
            s = self.secs[n]
            self.mem.append((s[3], s[3] + s[5], s[4], n))
        # symbols
        st = self.secs['.symtab']
        strs = secs[st[6]]
        self.symval = []
        for i in range(st[5] // 16):
            name_o, value, size, info, other, shndx = struct.unpack_from('>IIIBBH', d, st[4] + i * 16)
            self.symval.append(value)
        # relocations of initialised data: address -> target
        self.ptr = {}
        for n in INIT_SECTIONS:
            r = self.secs.get('.rela' + n)
            if not r:
                continue
            for i in range(r[5] // 12):
                off, info, add = struct.unpack_from('>IIi', d, r[4] + i * 12)
                sym, typ = info >> 8, info & 0xFF
                if typ == 1:  # R_PPC_ADDR32
                    self.ptr[off] = (self.symval[sym] + add) & 0xFFFFFFFF

    def read(self, a, n):
        for s, e, o, nm in self.mem:
            if s <= a and a + n <= e:
                return self.d[o + a - s:o + a - s + n]
        return None

    def section_of(self, a):
        for s, e, o, nm in self.mem:
            if s <= a < e:
                return nm
        return None

    def cstr(self, a, limit=64):
        b = self.read(a, 1)
        if b is None:
            return None
        out = bytearray()
        while len(out) < limit:
            c = self.read(a + len(out), 1)
            if c is None or c == b'\0':
                break
            out += c
        return bytes(out) if _printable(out) else None


def _printable(b):
    return len(b) >= 2 and all(32 <= c < 127 for c in b)


class N64Mem(object):
    def __init__(self):
        self.regions = {}
        g = open(os.path.join(REPO, 'build', 'game_code.bin'), 'rb').read()
        self.regions['game'] = [(0x80086A50, g)]
        rom = open(os.path.join(REPO, 'baserom.us.z64'), 'rb').read()
        self.static = (0x80000400, rom[0x1000:0x2F4E0])
        for t in 'ab':
            img = open(os.path.join(REPO, 'build', 'ovl', t, 'image.bin'), 'rb').read()
            self.regions['ovl_' + t] = [(0x8038A400, img)]

    def blobs(self, scope):
        out = [self.static] + self.regions['game']
        if scope in ('ovl_a', 'ovl_b'):
            out = self.regions[scope] + out
        return out

    def read(self, scope, a, n):
        for base, b in self.blobs(scope):
            if base <= a and a + n <= base + len(b):
                return b[a - base:a - base + n]
        return None

    def cstr(self, scope, a, limit=64):
        out = bytearray()
        while len(out) < limit:
            c = self.read(scope, a + len(out), 1)
            if c is None or c == b'\0':
                break
            out += c
        return bytes(out) if _printable(out) else None


_E = None
_N = None


def _init():
    global _E, _N
    if _E is None:
        _E = GCElf()
        _N = N64Mem()
    return _E, _N


def check(gstart, gsize, scope, naddr, limit=512):
    """(matched words, compared words, details) for the GC object vs the N64 bytes at naddr."""
    E, N = _init()
    if E.section_of(gstart) is None or gsize < 4:
        return None
    n = min(gsize, limit) // 4 * 4
    gb = E.read(gstart, n)
    nb = N.read(scope, naddr, n)
    if gb is None or nb is None:
        return None
    match = comp = 0
    strings = 0
    for i in range(0, n, 4):
        a = gstart + i
        gw, = struct.unpack_from('>I', gb, i)
        nw, = struct.unpack_from('>I', nb, i)
        if a in E.ptr:
            gs = E.cstr(E.ptr[a])
            if gs is None:
                continue
            ns = N.cstr(scope, nw)
            comp += 1
            if ns == gs:
                match += 1
                strings += 1
            continue
        if gw == 0 and nw == 0:
            continue  # zero words carry no evidence
        comp += 1
        if gw == nw:
            match += 1
    return match, comp, strings


def search(gstart, gsize, scope, limit=256, min_literal=8, only=False):
    """N64 addresses (same scope + game + static) whose bytes equal the GC object's literal words; returns a list
    of addresses (at most 5) or None when the object has too little distinctive content."""
    E, N = _init()
    if E.section_of(gstart) is None or gsize < 8:
        return None
    n = min(gsize, limit) // 4 * 4
    gb = E.read(gstart, n)
    if gb is None:
        return None
    parts = []
    literal = 0
    distinct = set()
    strings = []
    for i in range(0, n, 4):
        a = gstart + i
        w = gb[i:i + 4]
        if a in E.ptr:
            parts.append(b'.{4}')
            s = E.cstr(E.ptr[a])
            if s:
                strings.append((i, s))
            continue
        parts.append(re.escape(w))
        if w != b'\0\0\0\0':
            literal += 4
            distinct.add(w)
    if literal < min_literal and not strings:
        return None
    if len(distinct) < 2 and not strings:
        return None
    pat = re.compile(b''.join(parts), re.S)
    hits = []
    blobs = N.regions[scope] if only else N.blobs(scope)
    for base, blob in blobs:
        for m in pat.finditer(blob):
            a = base + m.start()
            if a % 4:
                continue
            ok = True
            for i, s in strings:
                nw, = struct.unpack_from('>I', blob, m.start() + i)
                if N.cstr(scope, nw) != s:
                    ok = False
                    break
            if ok:
                hits.append(a)
            if len(hits) > 5:
                return hits
    return hits
