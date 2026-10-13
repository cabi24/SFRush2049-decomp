#!/usr/bin/env python3
"""Struct-offset fingerprints: the (base class, offset, width) field accesses of each function.

A field access is a displacement load/store off a register that is neither the stack pointer nor a
small-data/%lo base: ``lwz r3, 0x1C(r31)`` on the GameCube, ``lw $v0, 0x1C($s0)`` on the N64. The
multiset of those (offset, width) keys is a fingerprint of the structs a function walks; rare offsets
(large members of big structs: a car record's 0x7C6, a pane's 0x1B0) identify the function's data even
when it has no strings. Used by correlate.py as the ``fld`` channel.

Base classes: ``p`` = any other register (struct/array pointer) -- the only class correlate scores;
``sp`` (r1 / $sp) and ``sd`` (GC r13/r2 small data, N64 $gp) are counted separately and ignored by the
score (frame layouts and small-data offsets differ between the two compilers). Accesses whose
displacement carries a relocation (GC ``@l``/``@sda21``, N64 ``%lo`` after a ``lui``) are symbol
references, not field offsets, and are skipped.

Standard library only: the GC side decodes the D-form words straight from rush_d.elf (relocated words
are known exactly from ``.rela.text``), so no capstone and no disassembly dump are needed.
"""
import math
import os
import struct
import sys
from collections import Counter, OrderedDict

if __package__ in (None, ''):
    sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..')))

# PowerPC D-form loads/stores: primary opcode -> access width (lmw/stmw excluded: register blocks)
PPC_MEM = {32: 4, 33: 4, 34: 1, 35: 1, 36: 4, 37: 4, 38: 1, 39: 1, 40: 2, 41: 2, 42: 2, 43: 2,
           44: 2, 45: 2, 48: 4, 49: 4, 50: 8, 51: 8, 52: 4, 53: 4, 54: 8, 55: 8}
# MIPS loads/stores: opcode -> width (lwl/lwr/swl/swr excluded: unaligned pairs)
MIPS_MEM = {32: 1, 33: 2, 35: 4, 36: 1, 37: 2, 39: 4, 40: 1, 41: 2, 43: 4, 49: 4, 53: 8, 55: 8,
            57: 4, 61: 8, 63: 8}
MAX_OFFSET = 0x8000


def key(offset, width):
    return '%X/%d' % (offset, width)


def gc_fields(prog=None, elf_path=None):
    """{gc function key: Counter(key -> count)} for the ``p`` class, decoded from the ELF."""
    if prog is None:
        from tools.gcref.disasm import GcProgram
        from tools.gcref.elf import DEFAULT_ELF
        prog = GcProgram(elf_path or DEFAULT_ELF)
    out = OrderedDict()
    for (addr, size, fkey, _name, _file) in prog.funcs:
        code = prog.elf.read(addr, size)
        c = Counter()
        if code is not None:
            for i in range(0, len(code) - 3, 4):
                a = addr + i
                if a in prog.relocs:
                    continue
                w = struct.unpack_from('>I', code, i)[0]
                width = PPC_MEM.get(w >> 26)
                if width is None:
                    continue
                ra = (w >> 16) & 31
                if ra in (0, 1, 2, 13):          # r0 = absolute, r1 stack, r2/r13 small data
                    continue
                d = w & 0xffff
                if d >= 0x8000:
                    continue                      # negative displacement: not a struct member
                c[key(d, width)] += 1
        out[fkey] = c
    return out


def n64_fields(img):
    """{N64 function name: Counter(key -> count)} for the ``p`` class of one N64Image."""
    out = OrderedDict()
    for addr, size, name in img.funcs:
        c = Counter()
        hi = set()
        for pc in range(addr, addr + size, 4):
            w = img.word(pc)
            if w is None:
                break
            op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
            if op == 15:                          # lui: rt now holds a %hi
                hi.add(rt)
                continue
            if op == 0:
                rd, funct = (w >> 11) & 31, w & 63
                if funct == 0x21 and (rs in hi or rt in hi):
                    hi.add(rd)                    # addu propagates the %hi base (array of a symbol)
                else:
                    hi.discard(rd)
                continue
            width = MIPS_MEM.get(op)
            if width is not None:
                d = w & 0xffff
                if rs not in hi and rs not in (0, 28, 29) and d < 0x8000:
                    c[key(d, width)] += 1
                if op < 40 or op in (49, 53):     # loads overwrite rt
                    hi.discard(rt)
                continue
            if op in (8, 9, 10, 11, 12, 13, 14):  # immediate ALU ops write rt
                if not (op == 9 and rs in hi and rt == rs):
                    hi.discard(rt)
                continue
        out[name] = c
    return out


class FieldIndex(object):
    """IDF-weighted set similarity between N64 and GC field fingerprints.

    A key's weight is sqrt(idf_n * idf_g), idf = ln(N / df) on each side, so common offsets weigh little;
    keys with an offset below ``min_offset`` (0, 4: every list walk) or, if ``max_df`` is set, present in
    more than ``max_df`` functions on either side are dropped, and a pair needs ``min_shared`` shared keys.
    (w29n measured: a df cap of 25-300 loses the HUD/screen families whose shared pane offsets are
    exactly the evidence; min_offset 8 + min_shared 4 gave the best held-out result.) Similarity = sum of shared weights / sqrt(sum N64 weights * sum GC weights), a
    cosine over binary sets."""

    def __init__(self, n64, gc, min_offset=0x8, max_df=None, min_shared=4):
        self.min_shared = min_shared
        dfn, dfg = Counter(), Counter()
        for c in n64.values():
            dfn.update(set(c))
        for c in gc.values():
            dfg.update(set(c))
        nn, ng = max(1, len(n64)), max(1, len(gc))
        self.w = {}
        for k in set(dfn) & set(dfg):
            off = int(k.split('/')[0], 16)
            if off < min_offset or (max_df and (dfn[k] > max_df or dfg[k] > max_df)):
                continue
            self.w[k] = math.sqrt(math.log(nn / float(dfn[k])) * math.log(ng / float(dfg[k])))
        self.n = {f: set(k for k in c if k in self.w) for f, c in n64.items()}
        self.g = {f: set(k for k in c if k in self.w) for f, c in gc.items()}
        # norms over ALL rare-on-own-side keys, so a function with many unshared fields is not inflated
        self.n_norm = {f: self._norm(c, dfn, nn, min_offset, max_df) for f, c in n64.items()}
        self.g_norm = {f: self._norm(c, dfg, ng, min_offset, max_df) for f, c in gc.items()}
        self.inv = {}
        for f, ks in self.g.items():
            for k in ks:
                self.inv.setdefault(k, set()).add(f)

    @staticmethod
    def _norm(c, df, n, min_offset, max_df):
        s = 0.0
        for k in c:
            if int(k.split('/')[0], 16) >= min_offset and (not max_df or df[k] <= max_df):
                s += math.log(n / float(df[k]))
        return s

    def similar(self, n):
        """{gc: (similarity, [shared keys])} for every GC function sharing >= min_shared keys."""
        ks = self.n.get(n)
        if not ks:
            return {}
        shared = {}
        for k in ks:
            for g in self.inv.get(k, ()):
                shared.setdefault(g, []).append(k)
        out = {}
        nn = self.n_norm[n]
        for g, lst in shared.items():
            if len(lst) < self.min_shared:
                continue
            den = (nn * self.g_norm[g]) ** 0.5
            if den <= 0:
                continue
            out[g] = (sum(self.w[k] for k in lst) / den, sorted(lst, key=lambda k: -self.w[k]))
        return out
