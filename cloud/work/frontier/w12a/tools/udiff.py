#!/usr/bin/env python3
"""udiff.py FN [--obj OBJ] [--all] [--ops | --norm | --nosp] [--summary]   (Pi, any cwd)

Aligned diff of FN in a unit/snapshot object against the retail words (tools/cloud/score.py relocation, so
relocated operands compare as resolved addresses). Default object: build/blob_unit/$TAG/unit.o.
Rows are aligned with difflib on the chosen key:
  (default) exact words            -- the strict count: "differing rows N"
  --ops     mnemonic only          -- structure lens: register/offset changes vanish (w10b --mnem, w10d --ops)
  --norm    registers, sp offsets, immediates normalised (w10h ops.py): "same instruction shape"
  --nosp    sp offsets normalised  -- hides frame/home-slot shifts only (w10d)
Last line: want/got words, differing rows, frame (addiu sp) want/got, unverified/unresolved relocations.
--summary prints only that line. Consolidates w3a..w10e udiff.py, w10b udiffm.py, w10d udiff.py, w10h ops.py.
"""
import argparse, difflib, os, re, struct, subprocess, sys, tempfile
from pathlib import Path

REPO = Path('/home/cburnes/projects/rush2049-decomp')
sys.path.insert(0, str(REPO / 'tools' / 'cloud'))
CWD = os.getcwd()
os.chdir(REPO)
import score  # noqa: E402


def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack('>%dI' % len(words), *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:4300', '-EB', f.name],
                             capture_output=True, text=True).stdout
    r = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: r.append(re.sub(r'\s+', ' ', m.group(3)))
    return r


def norm_regs(x):
    x = re.sub(r'\$f\d+', 'F', x)
    x = re.sub(r'\b(zero|at|v[01]|a[0-3]|t\d|s[0-8]|ra|sp|gp|k[01])\b', 'R', x)
    x = re.sub(r'-?\d+\(R\)', 'M(R)', x)
    x = re.sub(r'0x[0-9a-f]+', 'N', x)
    return re.sub(r',-?\d+$', ',N', x)


def frame(lines):
    for x in lines:
        m = re.match(r'addiu sp,sp,(-?\d+)', x)
        if m: return -int(m.group(1))
    return 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('fn')
    ap.add_argument('--obj', default=None)
    ap.add_argument('--all', action='store_true')
    g = ap.add_mutually_exclusive_group()
    g.add_argument('--ops', '--mnem', dest='ops', action='store_true')
    g.add_argument('--norm', action='store_true')
    g.add_argument('--nosp', action='store_true')
    ap.add_argument('--summary', action='store_true')
    a = ap.parse_args()
    obj = Path(os.path.join(CWD, a.obj)) if a.obj else REPO / 'build/blob_unit' / os.environ.get('TAG', 'gate') / 'unit.o'
    want = score.targets()[a.fn]
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[a.fn]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, score.image_symbols())
    got = res[start // 4:end // 4]
    while got and got[-1] == 0 and len(got) > len(want): got.pop()
    dw, dg = dis(want), dis(got)
    if a.ops: kw, kg = [x.split(' ')[0] for x in dw], [x.split(' ')[0] for x in dg]
    elif a.norm: kw, kg = [norm_regs(x) for x in dw], [norm_regs(x) for x in dg]
    elif a.nosp:
        f = lambda x: re.sub(r'-?[0-9]+\(sp\)', 'N(sp)', re.sub(r'sp,sp,-?[0-9]+', 'sp,sp,N', x))
        kw, kg = [f(x) for x in dw], [f(x) for x in dg]
    else: kw, kg = want, got
    rows = []
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, kw, kg, autojunk=False).get_opcodes():
        if tag == 'equal':
            for k in range(i2 - i1): rows.append((' ', i1 + k, dw[i1 + k], dg[j1 + k]))
        else:
            for k in range(max(i2 - i1, j2 - j1)):
                rows.append(('|', i1 + k if i1 + k < i2 else None, dw[i1 + k] if i1 + k < i2 else '',
                             dg[j1 + k] if j1 + k < j2 else ''))
    if not a.summary:
        show = [a.all or r[0] == '|' for r in rows]
        for i, r in enumerate(rows):
            if show[i] or (i > 0 and show[i - 1]) or (i + 1 < len(rows) and show[i + 1]):
                print('%s %5s  %-32s %s' % (r[0], ('+%03x' % (r[1] * 4)) if r[1] is not None else '', r[2], r[3]))
    mode = 'ops' if a.ops else 'norm' if a.norm else 'nosp' if a.nosp else 'words'
    print('want %d words, got %d; differing rows %d (%s); frame %d/%d; unverified %d unresolved %d'
          % (len(want), len(got), sum(1 for r in rows if r[0] == '|'), mode, frame(dw), frame(dg),
             len(unverified), len(unresolved)))


if __name__ == '__main__':
    main()
