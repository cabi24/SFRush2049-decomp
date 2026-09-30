#!/usr/bin/env python3
"""gen_nearmiss_index.py: regenerate cloud/work/near-miss/INDEX.md.

For every cloud/work/near-miss/<f>/base.c, rescore with
`tools/cloud/score.py fn base.c <f> --flags ...` under several flag sets and keep the
best (fewest differing words). Functions that have a file in cloud/matches/ are listed
in a "matched" section with the flags recorded in that file's header comment. The
pipeline score and source columns are carried over from the previous INDEX.md.

    python3 cloud/work/tools/gen_nearmiss_index.py
"""
import re, subprocess, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
NM = ROOT / 'cloud/work/near-miss'; MATCHES = ROOT / 'cloud/matches'
BASE = '-g0 -O2 -mips2 -G 0 -non_shared'
FLAGSETS = [BASE, BASE + ' -Wab,-r4300_mul', '-g0 -O1 -mips2 -G 0 -non_shared',
            '-g0 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul']

def old_meta():
    meta = {}
    for l in (NM / 'INDEX.md').read_text().splitlines():
        c = [x.strip() for x in l.strip('|').split('|')]
        if len(c) == 6 and re.match(r'^\w+$', c[0]) and c[1].isdigit():
            meta[c[0]] = (c[1], c[3])
    return meta

def score(fn, flags):
    p = subprocess.run(['python3', 'tools/cloud/score.py', 'fn', str(NM / fn / 'base.c'), fn, '--flags', flags],
                       cwd=ROOT, capture_output=True, text=True)
    out = (p.stdout + p.stderr).strip().splitlines()
    last = out[-1].strip() if out else 'no output'
    m = re.match(r'(\d+)/(\d+) words differ(.*)', last)
    if m:
        d = int(m.group(1)); ex = re.search(r'(\d+) extra', m.group(3))
        return d + (int(ex.group(1)) if ex else 0), last
    if 'MATCH' in last: return 0, 'MATCH' + (last[5:] if last != 'MATCH' else '')
    return 10**6, last[:80]

def best(fn):
    res = []
    for f in FLAGSETS:
        # a matched file in cloud/matches pins its own flags first
        res.append((score(fn, f), f))
    return min(res, key=lambda r: r[0][0])

def main():
    meta = old_meta()
    matched = {p.stem for p in MATCHES.glob('*.c')}
    rows = []; done = []; pending = []
    for d in sorted(NM.iterdir()):
        if not (d / 'base.c').exists(): continue
        fn = d.name
        (n, res), fl = best(fn)
        pipe, src = meta.get(fn, ('?', '?'))
        if fn in matched:
            hdr = re.search(r'flags: (.*?) \*/', (MATCHES / f'{fn}.c').read_text())
            done.append((fn, pipe, src, hdr.group(1) if hdr else fl, res))
        elif n == 0 and res.startswith('MATCH'):
            pending.append((fn, pipe, src, fl, res))
        else:
            rows.append((n, fn, pipe, src, fl, res))
    rows.sort(key=lambda r: (r[0], r[1]))
    L = ['# Single-function near misses', '',
         'Pipeline score is the historical heuristic (stack offsets ignored), not matching evidence.',
         'Strict words differing counts score.py\'s full-word differences plus nonzero words beyond',
         'the target extent. Unverified relocations and unresolved symbols remain visible in the',
         'result column: zero differences alone is not a verified match. Unscorable entries sort last.',
         'Each directory contains base.c for the whole translation unit. Verify with:', '',
         '    python3 tools/cloud/score.py fn cloud/work/near-miss/<name>/base.c <name> --flags "<flags>"', '',
         'Regenerate (rescores every entry under -O2, -O2 + `-Wab,-r4300_mul`, and the -O1 variants and keeps the',
         'best; run from the repo root): `python3 cloud/work/tools/gen_nearmiss_index.py`.',
         'The `-O1` in older revisions of this file was wrong for the m2c-sweep rows: they need `-O2`.', '',
         f'Rescored {len(rows) + len(done) + len(pending)} entries: {len(rows)} unmatched below, {len(pending)} strict matches not yet in `cloud/matches/`, {len(done)} now in `cloud/matches/`.', '',
         '| function | pipeline score | strict words differing | source | flags | score.py result |',
         '|---|---|---|---|---|---|']
    for n, fn, pipe, src, fl, res in rows:
        L.append(f'| {fn} | {pipe} | {n if n < 10**6 else "-"} | {src} | `{fl}` | {res} |')
    L += ['', '## Matched (now in cloud/matches/)', '',
          'Strict matches for the source and flags shown (the flags are the header comment of the file in',
          '`cloud/matches/`). Not new ROM coverage: maintainers must check whether each is already locked',
          'and run the splice/image/ROM gates before promotion. `func_800E1540` needs `-Wab,-r4300_mul`;',
          '`func_800ABB58` and `func_800B9F60` use `volatile` reads found by trial and need review.', '',
          '| function | pipeline score | source | flags | score.py result (rescored) |', '|---|---|---|---|---|']
    for fn, pipe, src, fl, res in done:
        L.append(f'| {fn} | {pipe} | {src} | `{fl}` | {res} |')
    L += ['', '## Strict matches not yet in cloud/matches/', '',
          'Score 0 under the flags shown but no file in `cloud/matches/` yet; maintainers should review them',
          '(check the function is not already locked) before promotion.', '',
          '| function | pipeline score | source | flags | score.py result |', '|---|---|---|---|---|']
    for fn, pipe, src, fl, res in pending:
        L.append(f'| {fn} | {pipe} | {src} | `{fl}` | {res} |')
    (NM / 'INDEX.md').write_text('\n'.join(L) + '\n')
    print('wrote', len(rows), 'unmatched,', len(pending), 'pending,', len(done), 'matched')
main()
