#!/usr/bin/env python3
"""Regenerate this round's 45 seeds into a NEW scratch directory, without IDO.

Set GROUPGEN_M2C to an already patched m2c tree if the shared submodule has
patches applied. The shared tree is read only; compiler work belongs on watchman2.
"""
import argparse
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[3]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--out', type=Path, required=True)
    args = ap.parse_args()
    if args.out.exists():
        ap.error('--out must be a new scratch directory; curated candidates are not overwritten')
    sys.path.insert(0, str(ROOT / 'cloud/work/tools'))
    from ipakit import deps, groupgen, sigs

    if groupgen.ensure_m2c() is None:
        ap.error('patched m2c unavailable; set GROUPGEN_M2C explicitly, rather than generate stubs')
    dm = deps.model_cache()
    sm = sigs.SigModel(dm.corpus, dm.infos)
    initial = json.loads(Path(__file__).with_name('triage.json').read_text())
    args.out.mkdir(parents=True)
    inventory = []
    for item in initial:
        name = item['fn']
        cls = dm.classify(name)[0]
        directory = args.out / name
        directory.mkdir()
        plan = groupgen.make_plan([name], dmodel=dm)
        source, info = groupgen.build_sources(plan, dm, sm)
        flags = '-g0 %s -mips2 -G 0 -non_shared' % ('-O2' if cls == 'ABI' else '-O3')
        source = re.sub(r'/\* flags:[^\n]*\*/\n', '', source)
        source = '/* flags: %s */\n%s' % (flags, source)
        filename = 'seed.c' if cls == 'ABI' else 'group.c'
        (directory / filename).write_text(source)
        if cls != 'ABI':
            (directory / 'group.json').write_text(
                json.dumps(groupgen.group_json(plan, name), indent=2) + '\n')
        (directory / 'seed-info.json').write_text(json.dumps(info, indent=2) + '\n')
        inventory.append({'fn': name, 'class': cls, 'flags': flags, 'source': filename,
                          'target_m2c_output': info['m2c'].get(name, False)})
        print(name, cls, 'm2c output' if info['m2c'].get(name) else 'STUB', flush=True)
    (args.out / 'inventory.json').write_text(json.dumps(inventory, indent=2) + '\n')


if __name__ == '__main__':
    main()
