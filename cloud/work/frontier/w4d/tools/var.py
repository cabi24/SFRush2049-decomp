#!/usr/bin/env python3
"""var.py PRELUDE BASE NAME OUTDIR 'label::old=>new[|||old2=>new2]' ... [-- blob_unit args]
Applies each replacement set to BASE (body), writes OUTDIR/v_label.c (= PRELUDE + body), blob_unit-scores NAME
(tag w4d, or $TAG) and prints the differing-word line. Runs sequentially. An argument @FILE reads specs separated by lines containing only %%."""
import sys, subprocess, os
args = sys.argv[1:]
extra = []
if '--' in args:
    i = args.index('--'); extra = args[i+1:]; args = args[:i]
prelude, base, name, outdir = args[:4]
pre = open(prelude).read() if prelude != '-' else ''
b = open(base).read()
tag = os.environ.get('TAG', 'w4d')
specs = []
for a in args[4:]:
    if a.startswith('@'):
        specs += [x.strip('\n') for x in open(a[1:]).read().split('\n%%\n') if x.strip()]
    else:
        specs.append(a)
for spec in specs:
    label, _, reps = spec.partition('::')
    s = b
    ok = True
    for rep in reps.split('|||') if reps else []:
        old, _, new = rep.partition('=>')
        old = old.encode().decode('unicode_escape'); new = new.encode().decode('unicode_escape')
        if old not in s:
            print(f'{label}: MISSING {old!r}'); ok = False; break
        s = s.replace(old, new, 1)
    if not ok: continue
    p = os.path.join(outdir, f'v_{label}.c')
    open(p, 'w').write(pre + s)
    r = subprocess.run([sys.executable, '-m', 'tools.conveyor.pipeline.blob_unit', '--tag', tag, 'score', name, '--with', os.path.abspath(p)] + extra,
                       cwd='/home/cburnes/projects/rush2049-decomp', capture_output=True, text=True)
    line = [l for l in (r.stdout + r.stderr).splitlines() if 'EQUAL' in l or 'FAIL' in l or 'rror' in l]
    print(f'{label}: {line[0].strip() if line else (r.stdout+r.stderr)[-300:]}')
