#!/usr/bin/env python3
"""gscore.py: run on the builder inside the scratch dir.
  gscore.py group DIR [--targets asm/us/ovl_b] [--names A,B] [--diff NAME] [--norm] [--ctx N]
  gscore.py fn FILE.c NAME [--flags ...] [--diff NAME]
Prints per-function 'differing/total' (via score.compare), frame sizes, and an aligned diff
(difflib over disassembly text; --norm hides registers/immediates)."""
import argparse, difflib, os, re, struct, subprocess, sys, tempfile
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools' / 'cloud')) if False else None
sys.path.insert(0, os.getcwd() + '/tools/cloud')
import score

def dis(words, base=0):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
    res = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: res.append(re.sub(r'\s+', ' ', m.group(3)).strip())
    return res

def norm(s):
    s = re.sub(r'<[^>]*>', '', s)
    s = re.sub(r'\$f\d+', 'F', s)
    s = re.sub(r'\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|gp|sp|ra|k[01])\b', 'R', s)
    s = re.sub(r'-?0x[0-9a-f]+|-?\d+', 'N', s)
    return s

def fixed_got(obj, name):
    want = score.targets()[name]
    words = score.text_words(obj)
    fns = score.symbols(obj)
    start = fns[name]
    end = min((o for o in fns.values() if o > start), default=len(words)*4)
    tend = start + len(want)*4
    res, masks, unres, unver, errs = score.relocate(obj, words, start, min(tend,end), score.image_symbols())
    got = res[start//4:end//4]
    # mask section-relative words so they compare equal when only the reloc differs
    for off, m in masks.items():
        i = off//4 - start//4
        if 0 <= i < len(got): got[i] = got[i] & m
    wmask = list(want)
    for off, m in masks.items():
        i = off//4 - start//4
        if 0 <= i < len(wmask): wmask[i] = wmask[i] & m
    return wmask, got

def frame(lines):
    for l in lines[:40]:
        m = re.match(r'addiu sp,sp,(-?\d+)', l)
        if m: return int(m.group(1))
    return None

def show_diff(obj, name, normalize, ctx):
    want, got = fixed_got(obj, name)
    wl, gl = dis(want), dis(got)
    wn = [norm(x) if normalize else x for x in wl]
    gn = [norm(x) if normalize else x for x in gl]
    sm = difflib.SequenceMatcher(None, wn, gn, autojunk=False)
    print(f'== {name}: want {len(want)} words got {len(got)} words; frame want {frame(wl)} got {frame(gl)}')
    wi = gi = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            continue
        print(f'--- want[{i1}:{i2}] (+0x{4*i1:x}) got[{j1}:{j2}] (+0x{4*j1:x}) {tag}')
        for k in range(max(0, i1-ctx), i1):
            print(f'    {k:4d} {wl[k]}')
        for k in range(i1, i2): print(f'  - {k:4d} {wl[k]}')
        for k in range(j1, j2): print(f'  + {k:4d} {gl[k]}')

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('mode'); ap.add_argument('path'); ap.add_argument('name', nargs='?')
    ap.add_argument('--targets', default='asm/us/ovl_b')
    ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared')
    ap.add_argument('--names', default='')
    ap.add_argument('--diff', default='')
    ap.add_argument('--norm', action='store_true')
    ap.add_argument('--ctx', type=int, default=2)
    ap.add_argument('--side', default='')
    ap.add_argument('--got', default='')  # NAME:START:END print got rows  # NAME:START:END side-by-side window of want/got rows
    a = ap.parse_args()
    score.ASM_DIR = Path(a.targets).resolve()
    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp)/'out.o'
        if a.mode == 'obj':
            obj = Path(a.path); names = a.names.split(',')
        elif a.mode == 'group':
            spec = score.compile_group(Path(a.path), obj)
            names = a.names.split(',') if a.names else spec['members'] + spec.get('context', [])
        else:
            score.compile_single(a.path, a.flags, obj); names = [a.name]
        syms = score.symbols(obj)
        for n in names:
            try:
                r = score.compare(obj, n, show=0)
                w, g = fixed_got(obj, n)
                wl, gl = dis(w), dis(g)
                def rows(a, b):
                    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
                    return sum(max(i2-i1, j2-j1) for t, i1, i2, j1, j2 in sm.get_opcodes() if t != 'equal')
                rw = rows(wl, gl); rn = rows([norm(x) for x in wl], [norm(x) for x in gl])
                print(f'{n}: {r.summary()[:60]} | ALIGNED rows differ {rw} (norm {rn}) want {len(w)} got {len(g)} frame {frame(wl)}/{frame(gl)}')
            except SystemExit as e:
                print(f'{n}: ERROR {e}')
        print('compiled functions:', ' '.join(f'{k}@{v:x}' for k, v in sorted(syms.items(), key=lambda kv: kv[1])))
        if a.got:
            nm, st, en = a.got.split(':')
            w, g = fixed_got(obj, nm)
            gl = dis(g)
            for k in range(int(st), min(int(en), len(gl))): print(f'{k:4d} {gl[k]}')
        if a.side:
            nm, st, en = a.side.split(':')
            w, g = fixed_got(obj, nm)
            wl, gl = dis(w), dis(g)
            sm = difflib.SequenceMatcher(None, wl, gl, autojunk=False)
            print(f'== {nm} side-by-side (want index {st}..{en}; got aligned by matcher)')
            gi = {}
            for t, i1, i2, j1, j2 in sm.get_opcodes():
                if t == 'equal':
                    for k in range(i2-i1): gi[i1+k] = j1+k
            for k in range(int(st), min(int(en), len(wl))):
                print(f'{k:4d} {wl[k]:34s} | ' + (f'{gi[k]:4d} {gl[gi[k]]}' if k in gi else '-'))
        for d in [x for x in a.diff.split(',') if x]:
            show_diff(obj, d, a.norm, a.ctx)
if __name__ == "__main__": main()
