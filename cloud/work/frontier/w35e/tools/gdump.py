#!/usr/bin/env python3
"""gdump.py (builder-side) GROUPDIR FN IMG: print sp-relative offsets used in compiled FN (counts)"""
import sys, tempfile, subprocess, struct, re
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
d, fn, img = sys.argv[1:4]
score.ASM_DIR = Path(f'asm/us/ovl_{img}').resolve()
with tempfile.TemporaryDirectory() as t:
    obj = Path(t)/'o.o'; score.compile_group(Path(d), obj)
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[fn]; end = min((o for o in fns.values() if o > start), default=len(words)*4)
    got = words[start//4:end//4]
with tempfile.NamedTemporaryFile(suffix='.bin') as f:
    f.write(struct.pack(f'>{len(got)}I', *got)); f.flush()
    out = subprocess.run(['mips-linux-gnu-objdump','-D','-z','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
from collections import Counter
c = Counter()
for l in out.splitlines():
    m = re.search(r'\s(\w+)\s+[^,]*,?\s*(-?\d+)\(sp\)', l)
    m2 = re.search(r'(\w+)\s+(?:[$\w]+,)?(-?\d+)\(sp\)', l)
    mm = re.search(r'(-?\d+)\(sp\)', l)
    if mm:
        op = l.split('\t')[2] if len(l.split('\t'))>2 else ''
        c[(int(mm.group(1)), op.split()[0] if op.split() else '')] += 1
for (o, op), n in sorted(c.items()): print(f'{o:5d} {op:6s} x{n}')
