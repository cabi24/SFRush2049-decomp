#!/usr/bin/env python3
"""extscore.py GROUP_DIR [--as1=-r4300_mul] [--only a,b]

Like cloud/work/tools/zbuild.py, but for members whose retail words are NOT
a `.text.<name>` section of asm/us/blob (functions that live inside the opaque
`.incbin` runs, e.g. unregistered function heads). Their target words are read
from the game-code image, which this script inflates from the committed
assets/us/data.bin (raw DEFLATE, 326,180 bytes at ROM 0xB0CB10, image base
0x80086A50). group.json lists them under "targets": {"name": {"addr": "0x..", "words": N}}.
Compared with the same strict score.compare. Cloud Lane A helper (hand-written groups).
"""
import sys, tempfile, io, contextlib, json, shlex, re, argparse, zlib
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
ap = argparse.ArgumentParser(); ap.add_argument('g'); ap.add_argument('--as1', default='-r4300_mul'); ap.add_argument('--only', default='')
ap.add_argument('--flags', default=''); ap.add_argument('--dis', default=''); ap.add_argument('--umerge', default=''); ap.add_argument('--uopt', default=''); ap.add_argument('--ugen', default=''); ap.add_argument('--show', type=int, default=12)
a = ap.parse_args(); g = Path(a.g); spec = json.loads((g/'group.json').read_text())
data = (ROOT/'assets/us/data.bin').read_bytes()
img = zlib.decompressobj(-15).decompress(data[0xB0CB10-0x10000:0xB0CB10-0x10000+326180])
assert len(img) == 647072
import struct
orig_targets = score.targets
extra = {}
for n, t in spec.get('targets', {}).items():
    o = int(t['addr'], 16) - 0x80086A50
    extra[n] = list(struct.unpack('>%dI' % t['words'], img[o:o+4*t['words']]))
def targets():
    d = dict(orig_targets()); d.update(extra); return d
score.targets = targets
def build(out):
    with tempfile.TemporaryDirectory() as tmp:
        work = Path(tmp)
        for name in spec['files']: (work/name).write_text((g/name).read_text())
        (work/'keep.txt').write_text(''.join(k+'\n' for k in spec['keep']))
        units = [re.sub(r'\.c$', '.u', f) for f in spec['files']]
        common = ['-mips2', '-EB', '-g0', '-O3']
        flags = a.flags or spec['flags']
        steps = [[score.ido('cc'), '-j', *shlex.split(flags), *spec['files']],
                 [score.ido('uld'), '-L/usr/lib/mips2/nonshared', '-_SYSTYPE_SVR4', '-mips2', '-non_shared', '-g0', '-no_AutoGnum', '-kp', 'keep.txt', *units, '-ko', 'linked'],
                 [score.ido('usplit'), '-mips2', '-o', 'split', '-t', 'st', 'linked'],
                 [score.ido('umerge'), *shlex.split(a.umerge), '-Olimit', '5000', *common, 'split', '-o', 'merged', '-t', 'st'],
                 [score.ido('uopt'), '-G', '0', '-Olimit', '5000', *common, *shlex.split(a.uopt), 'merged', 'opt', '-t', 'st', 'optlog'],
                 [score.ido('ugen'), '-G', '0', *common, *shlex.split(a.ugen), 'opt', '-o', 'gen', '-t', 'st', '-temp', 'ugtmp'],
                 [score.ido('as1'), '-elf', '-G', '0', '-p0', *common, *shlex.split(a.as1), '-Olimit', '5000', 'gen', '-o', str(out), '-t', 'st']]
        for s in steps:
            p = score._run(s, cwd=work)
            if '-v' in a.umerge and Path(s[0]).name=='umerge': print(p.stdout, p.stderr)
            if p.returncode: sys.exit(Path(s[0]).name + ' failed: ' + (p.stderr or p.stdout)[:2000])
with tempfile.TemporaryDirectory() as t:
    obj = Path(t)/'o.o'; build(obj)
    fns = score.symbols(obj); words = score.text_words(obj)
    order = sorted(fns.items(), key=lambda x: x[1])
    print('emitted:', ' '.join(f"{n}({((order[i+1][1] if i+1 < len(order) else len(words)*4) - o)//4})" for i, (n, o) in enumerate(order)))
    if a.dis:
        import subprocess, struct as _s
        for n in a.dis.split(','):
            st = fns[n]; end = min([o for o in fns.values() if o > st] or [len(words)*4])
            with tempfile.NamedTemporaryFile(suffix='.bin') as f:
                f.write(_s.pack('>%dI' % ((end-st)//4), *words[st//4:end//4])); f.flush()
                print(subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout.split('<.data>:')[-1])
    tot = ok = 0
    for n in spec['members'] + spec.get('context', []):
        if a.only and n not in a.only.split(','): continue
        tg = score.targets().get(n)
        if tg is None: continue
        want = len(tg); ctx = n not in spec['members']
        if not ctx: tot += want
        if n not in fns:
            print(f'  {n:28s} MISSING  target {want}'); continue
        st = fns[n]; end = min([o for o in fns.values() if o > st] or [len(words)*4])
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            c = score.compare(obj, n, show=a.show)
        if c.differing == 0 and not c.extra_words and not ctx: ok += want
        print(f'  {n:28s}{" (context)" if ctx else ""} size {(end-st)//4:4d}/{want:<4d} {c.summary()[:110]}')
        if c.differing: print(buf.getvalue())
    print(f'matched words {ok}/{tot}')
