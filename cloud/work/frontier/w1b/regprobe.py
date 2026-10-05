#!/usr/bin/env python3
"""regprobe.py DIR FN IMMHEX [--flags ...]: (builder) compile every DIR/*.c and histogram the rt register of the first
`lh/lw/lb rt, IMM(base)` in FN -- a quick probe of which register a global's value web received."""
import sys, argparse, tempfile, collections
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, 'tools/cloud')
import score
R = "zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp s8 ra".split()
ap = argparse.ArgumentParser(); ap.add_argument('dir'); ap.add_argument('fn'); ap.add_argument('imm'); ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared')
a = ap.parse_args(); imm = int(a.imm, 16) & 0xffff
def one(src):
    try:
        with tempfile.TemporaryDirectory() as t:
            obj = Path(t) / 'o.o'; score.compile_single(str(src), a.flags, obj)
            words = score.text_words(obj); fns = score.symbols(obj); start = fns[a.fn]
            end = min((o for o in fns.values() if o > start), default=len(words) * 4)
            res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, score.image_symbols())
            for w in res[start // 4:end // 4]:
                if (w >> 26) in (0x20, 0x21, 0x23, 0x24, 0x25) and (w & 0xffff) == imm: return (R[(w >> 16) & 31], src.name)
            return ('none', src.name)
    except SystemExit:
        return ('fail', src.name)
with ThreadPoolExecutor(4) as ex: res = list(ex.map(one, sorted(Path(a.dir).glob('*.c'))))
c = collections.Counter(r[0] for r in res); print(c)
for r in res:
    if r[0].startswith('s'): print(r)
