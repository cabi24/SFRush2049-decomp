#!/usr/bin/env python3
"""gdiff.py (builder-side) MODE SRC FN IMG [flags]: MODE=group|fn. Compile, relocate and print a full aligned diff of FN
against the overlay target (disassembled). Run from the lane scratch root."""
import sys, tempfile, difflib, struct, subprocess, shlex
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
mode, src, fn, img = sys.argv[1:5]
flags = sys.argv[5] if len(sys.argv) > 5 else "-g0 -O3 -mips2 -G 0 -non_shared"
score.ASM_DIR = Path(f'asm/us/ovl_{img}').resolve()
want = score.targets()[fn]
with tempfile.TemporaryDirectory() as t:
    obj = Path(t) / 'o.o'
    if mode == 'group': score.compile_group(Path(src), obj)
    else: score.compile_single(src, flags, obj)
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[fn]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    syms = score.image_symbols()
    res, masks, unres, unver, errs = score.relocate(obj, words, start, end, syms)
    got = res[start // 4:end // 4]
def dis(ws, base):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(ws)}I', *ws)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:4300', '-EB', f'--adjust-vma={base}', '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
    import re
    r = []
    for l in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l)
        if m: r.append(m.group(3))
    return r
# mask unverified relocs
wm = [w & masks.get(start + 4 * i, 0xFFFFFFFF) for i, w in enumerate(want)]
gm = [g & masks.get(start + 4 * i, 0xFFFFFFFF) for i, g in enumerate(got)]
wd = dis(want, 0); gd = dis(got, 0)
import os, re as _re
if os.environ.get('NOSP'):
    def norm(l):
        l = _re.sub(r'-?\d+\(sp\)', '?(sp)', l)
        l = _re.sub(r'(addiu\s+\w+,sp,)-?\d+', r'\1?', l)
        l = _re.sub(r'\b0x[0-9a-f]+$', 'T', l) if l.split()[0].startswith('b') or l.split()[0].startswith('j') else l
        return l
    wm = [norm(l) for l in wd]; gm = [norm(l) for l in gd]
sm = difflib.SequenceMatcher(None, wm, gm, autojunk=False)
n = 0
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal':
        continue
    n += max(i2 - i1, j2 - j1)
    print(f'--- want[{i1}:{i2}] (+0x{i1*4:x})  got[{j1}:{j2}]  {tag}')
    for k in range(max(i2 - i1, j2 - j1)):
        a = wd[i1 + k] if i1 + k < i2 else ''
        b = gd[j1 + k] if j1 + k < j2 else ''
        print(f'   {a:48s} | {b}')
print(f'want {len(want)} got {len(got)} words; non-equal run words {n}; frame want/got: ', end='')
import re
def fr(d):
    for l in d[:3]:
        m = re.search(r'addiu\s+sp,sp,-(\d+)', l)
        if m: return int(m.group(1))
print(fr(wd), fr(gd))
