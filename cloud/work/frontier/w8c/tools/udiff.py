#!/usr/bin/env python3
"""udiff.py NAME [unit.o]: aligned diff of NAME in the w8c blob_unit object vs retail (relocs shown raw)."""
import sys, re, subprocess, difflib, struct, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
name = sys.argv[1]
obj = sys.argv[2] if len(sys.argv) > 2 else str(ROOT / 'build/blob_unit/w8c/unit.o')
def dis_words(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB',
                              '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
    r = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m:
            ins = re.sub(r'\s+', ' ', m.group(3))
            ins = re.sub(r'^(jal|j) .*', r'\1 X', ins)
            ins = re.sub(r'(b\w*) (.*),0x[0-9a-f]+$', r'\1 \2,L', ins)
            ins = re.sub(r'^b 0x[0-9a-f]+$', 'b L', ins)
            ins = re.sub(r'^(lui \w+),0x[0-9a-f]+$', r'\1,HI', ins)
            r.append(ins)
    return r
want = score.targets()[name]
words = score.text_words(obj); syms = score.symbols(obj)
start = syms[name]; end = min([o for o in syms.values() if o > start] + [len(words)*4])
got = words[start//4:end//4]
a, b = dis_words(want), dis_words(got)
sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
n = 0
for op, i1, i2, j1, j2 in sm.get_opcodes():
    for k in range(max(i2-i1, j2-j1)):
        x = a[i1+k] if i1+k < i2 else ''; y = b[j1+k] if j1+k < j2 else ''
        mark = ' ' if op == 'equal' else '|'
        if op != 'equal': n += 1
        print(f'{mark} {x:34s} {y}')
print(f'want {len(a)} got {len(b)} diff rows {n}')
