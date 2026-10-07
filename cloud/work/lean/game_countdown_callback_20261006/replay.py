#!/usr/bin/env python3
"""Minimal strict-score replay with the genuine existing caller."""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
import re
from pathlib import Path
import subprocess
import sys
import tempfile
BASE = 'f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
HERE = Path(__file__).resolve().parent
parser = argparse.ArgumentParser()
parser.add_argument('--repo', type=Path, required=True)
args = parser.parse_args()
def frozen(path):
    return subprocess.check_output(['git', '-C', str(args.repo), 'show', BASE + ':' + path])
with tempfile.TemporaryDirectory(prefix='lean-hud-countdown-') as tmp:
    root = Path(tmp)
    paths = ['tools/cloud/score.py', 'tools/cloud/owndata.py']
    for region in ['blob', 'blob_data']:
        manifest = 'asm/us/' + region + '/SHA256SUMS'
        paths.append(manifest)
        paths += ['asm/us/' + region + '/' + line.split()[1] for line in frozen(manifest).decode().splitlines()]
    for path in paths:
        output = root / path
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_bytes(frozen(path))
    sys.path.insert(0, str(root / 'tools/cloud'))
    spec = importlib.util.spec_from_file_location('score', root / 'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec)
    sys.modules['score'] = score
    spec.loader.exec_module(score)
    baseline = root / 'baseline.c'
    seed = frozen('cloud/work/registered-heads/seeds/func_80106B3C/group.c').decode()
    baseline.write_text(seed[:seed.index('\ns32 slot_state_setup(s32 ipa_s2) {')])
    score.compile_single(baseline, FLAGS, root / 'baseline.o')
    group = root / 'group'
    group.mkdir()
    for name in ['callback.c','slot_context.c','replay_caller.c','group.json']:
        (group / name).write_bytes((HERE / name).read_bytes())
    score.compile_group(group, root / 'candidate.o')
    rows = []
    for mode, obj, functions in [('baseline_single', root / 'baseline.o', ['func_80106B3C']),
                                ('candidate_group', root / 'candidate.o', ['func_80106B3C','replay_save_prompt','slot_state_setup'])]:
        data, sections = score._elf(obj)
        symtab = next(i for i, sec in enumerate(sections) if sec['type'] == 2)
        symbols = score._symbol_table(data, sections, symtab)
        for name in functions:
            fn = next(s for s in symbols if s['name'] == name and s['type'] == 2)
            result = score.compare(obj, name, show=0)
            rows.append(dict(mode=mode, function=name, result={k: [re.sub(r'retail [0-9a-f]+, got [0-9a-f]+', 'retail [redacted], got [redacted]', v) for v in val] if k == 'errors' else val for k, val in dataclasses.asdict(result).items()},
                             function_bytes=fn['size'], target_bytes=len(score.targets()[name])*4))
    hashes = {p: hashlib.sha256((HERE / p).read_bytes()).hexdigest() for p in ['callback.c','slot_context.c','replay_caller.c','group.json']}
    print(json.dumps(dict(base=BASE, flags=FLAGS, automatic_backend_flag=score.R4300_CC,
                         source_sha256=hashes, rows=rows), indent=2))
