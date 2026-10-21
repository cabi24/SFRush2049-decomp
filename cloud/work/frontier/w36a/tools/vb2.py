#!/usr/bin/env python3
"""vb2.py BASE_GROUP_DIR VARIANT_DIR [--fn NAME] [--targets asm/us/ovl_a]   (run in builder scratch root)
BASE_GROUP_DIR: group.c with func_803A1EAC present + group.json. VARIANT_DIR/*.c: replacement function texts.
Prints per variant: strict differing words (score.compare) / aligned differing rows / word counts. Sorted best first."""
import sys, os, re, json, shutil, tempfile, difflib, struct, subprocess
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
args = sys.argv[1:]
fn = 'func_803A1EAC'
if '--fn' in args:
    i = args.index('--fn'); fn = args[i+1]; del args[i:i+2]
tg = os.environ.get('TARGETS', 'asm/us/ovl_a')
LIM = int(os.environ.get('LIM') or 0)
score.ASM_DIR = Path(tg).resolve()
base, vdir = Path(args[0]), Path(args[1])
b = (base / 'group.c').read_text()
i = b.index('\nvoid %s(void)\n' % fn) + 1
j = b.index('\n}\n', i) + 3
head, tail = b[:i], b[j:]

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
want = score.targets()[fn]
rows = []
for v in sorted(vdir.glob('*.c')):
    with tempfile.TemporaryDirectory() as t:
        t = Path(t)
        (t / 'group.c').write_text(head + v.read_text() + tail)
        shutil.copy(base / 'group.json', t / 'group.json')
        obj = t / 'o.o'
        try:
            score.compile_group(t, obj)
        except SystemExit as e:
            rows.append((10**9, v.name, 'FAIL ' + str(e)[:200].replace('\n', ' '))); continue
        words = score.text_words(obj); fns = score.symbols(obj)
        st = fns[fn]; en = min((o for o in fns.values() if o > st), default=len(words) * 4)
        res, masks, unresolved, unverified, errors = score.relocate(obj, words, st, en, score.image_symbols())
        got = res[st // 4:en // 4]
        while got and got[-1] == 0 and len(got) > len(want): got.pop()
        if LIM:
            want_l, got_l = want[:LIM], got[:LIM]
        else:
            want_l, got_l = want, got
        dw, dg = dis(want), dis(got)
        def norm_regs(x):
            x = re.sub(r'\$f\d+', 'F', x)
            x = re.sub(r'\b(zero|at|v[01]|a[0-3]|t\d|s[0-8]|ra|sp|gp|k[01])\b', 'R', x)
            x = re.sub(r'-?\d+\(R\)', 'M(R)', x)
            x = re.sub(r'0x[0-9a-f]+', 'N', x)
            return re.sub(r',-?\d+$', ',N', x)
        def adist(a, b):
            sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
            m = sum(x.size for x in sm.get_matching_blocks())
            return len(a) + len(b) - 2 * m
        dwd = adist(want_l, got_l) if not LIM else sum(1 for a, c in zip(want_l, got_l) if a != c)
        dno = adist([norm_regs(x) for x in dw], [norm_regs(x) for x in dg])
        dop = adist([x.split(' ')[0] for x in dw], [x.split(' ')[0] for x in dg])
        fr = lambda l: next((-int(re.match(r'addiu sp,sp,(-?\d+)', x).group(1)) for x in l if re.match(r'addiu sp,sp,(-?\d+)', x)), 0)
        import collections
        def lay(l):
            c = collections.Counter()
            for x in l:
                for m in re.finditer(r'(-?\d+)\(sp\)', x):
                    if int(m.group(1)) >= 116: c[int(m.group(1))] += 1
                for m in re.finditer(r'addiu \w+,sp,(\d+)', x): c['a' + m.group(1)] += 1
            return ' '.join('%s' % k for k in sorted(c, key=lambda z: (isinstance(z, str), int(str(z).lstrip('a')))))
        if os.environ.get('LAYOUT'): print(v.name, 'LAYOUT', lay(dg))
        rows.append((dwd, v.name, 'words=%d norm=%d ops=%d  len %d/%d frame %d' % (dwd, dno, dop, len(want), len(got), fr(dg))))
for k, n, s in sorted(rows):
    print(n, s)
