#!/usr/bin/env python3
"""pre.py SRC.c [--flags ..] [-n N] [--fn object_render] [--show K]: positional/aligned prefix compare of first N words (reloc-masked)."""
import sys, argparse, tempfile, difflib, subprocess, struct
from pathlib import Path
ROOT = Path('/home/user/SFRush2049-decomp')
sys.path.insert(0, str(ROOT / 'tools' / 'cloud'))
import score
ap = argparse.ArgumentParser(); ap.add_argument('src'); ap.add_argument('-n', type=int, default=155)
ap.add_argument('--fn', default='object_render'); ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared'); ap.add_argument('--show', type=int, default=0); ap.add_argument('--from', dest='frm', type=int, default=0)
a = ap.parse_args()
want = score.targets()[a.fn]
with tempfile.TemporaryDirectory() as t:
    obj = Path(t) / 'o.o'
    score.compile_single(a.src, a.flags, obj)
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[a.fn]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks, *_ = score.relocate(obj, words, start, end, score.image_symbols())
    got = res[start // 4:end // 4]
    gm = [g & masks.get(start + 4 * i, 0xFFFFFFFF) for i, g in enumerate(got)]
wm = [w for w in want]
# reloc masked target: mask where got has mask
wmm = [w & masks.get(start + 4 * i, 0xFFFFFFFF) if i < len(got) else w for i, w in enumerate(want)]
N = a.n
pos = sum(1 for i in range(min(N, len(gm))) if gm[i] == wmm[i])
sm = difflib.SequenceMatcher(None, wmm[:N], gm[:N + 40], autojunk=False)
al = sum(b.size for b in sm.get_matching_blocks())
def shape(w):
    op = w >> 26
    if op == 0: return (0, w & 0x3f)
    if op == 1: return (1, (w >> 16) & 0x1f)
    return (op,)
s2 = difflib.SequenceMatcher(None, [shape(w) for w in wmm[:N]], [shape(w) for w in gm[:N + 40]], autojunk=False)
sh = sum(b.size for b in s2.get_matching_blocks())
print(f'got {len(got)} words; prefix {N}: positional {pos}, aligned-exact {al}, aligned-shape {sh}')
if a.show:
    def dis(ws):
        with tempfile.NamedTemporaryFile(suffix='.bin') as f:
            f.write(struct.pack('>%dI' % len(ws), *ws)); f.flush()
            o = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
        return [l.split('\t',2)[-1].strip() if '\t' in l else l for l in o.splitlines()[7:]]
    A = dis(wmm[:N]); B = dis(gm[:N+40])
    sm = difflib.SequenceMatcher(None, A, B, autojunk=False)
    k = 0
    for tag,i1,i2,j1,j2 in sm.get_opcodes():
        if tag == 'equal': continue
        if i1 < a.frm: continue
        print(f'@{i1*4:#x} {tag}'); [print('  - ', x) for x in A[i1:i2][:10]]; [print('  + ', x) for x in B[j1:j2][:10]]
        k += 1
        if k >= a.show: break
