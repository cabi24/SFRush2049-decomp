#!/usr/bin/env python3
"""set_claims.py [--write] [GROUP...]: rescore each IPA group and set its group.json
"claims" to exactly the members that score MATCH (strict, -r4300_mul). Tail-function groups
(a "targets" key) are scored with extscore.py. Groups whose match depends on a stand-in for a
retail function (STANDIN_DEPENDENT) get no claims. Prints old -> new; writes only with --write.
"""
import json, re, subprocess, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
G = ROOT / 'cloud/work/ipa-groups'
LINE = re.compile(r'^\s{2}(\w+)\s+(\(context\) )?size\s+(\d+)/(\d+)\s+(.*)$')
STANDIN_DEPENDENT = {'func_8008705C': 'needs the real func_80086A50 (387 words); a stand-in cannot be spliced'}
NOT_SCORED = {'car_cg_height_set', 'car_collision_update', 'car_crash_response', 'menu_back', 'highscore_entry_anim'}

def matched(d, spec):
    if 'targets' in spec:
        cmd = ['python3', str(ROOT / 'cloud/work/tools/extscore.py'), '--norm', str(d)]
    else:
        cmd = ['python3', str(ROOT / 'cloud/work/tools/zbuild.py'), str(d), '--as1=-r4300_mul']
    out = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, timeout=1200)
    text = out.stdout + out.stderr
    ok = set()
    for l in text.splitlines():
        m = LINE.match(l)
        if m and not m.group(2) and m.group(1) in spec['members'] and 'MATCH' in m.group(5):
            ok.add(m.group(1))
    return [m for m in spec['members'] if m in ok], text

def main():
    write = '--write' in sys.argv
    names = [a for a in sys.argv[1:] if not a.startswith('--')]
    for d in sorted(G.iterdir()):
        if not (d / 'group.json').exists() or (names and d.name not in names): continue
        p = d / 'group.json'; spec = json.loads(p.read_text())
        if d.name in NOT_SCORED or d.name in STANDIN_DEPENDENT:
            new = []
        else:
            new, _ = matched(d, spec)
        old = spec.get('claims')
        print(f'{d.name}: {old} -> {new}', flush=True)
        if write:
            spec['claims'] = new
            raw = p.read_text()
            m = re.search(r'^( +)"', raw, re.M)
            p.write_text(json.dumps(spec, indent=len(m.group(1)) if m else 2) + '\n')

if __name__ == '__main__':
    main()
