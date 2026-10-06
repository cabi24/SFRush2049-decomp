#!/usr/bin/env python3
"""Minimal strict-score replay with the genuine existing caller."""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile
BASE = 'f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
OLD = 'cloud/work/near-miss/func_800DA0BC/base.c'
HERE = Path(__file__).resolve().parent
parser = argparse.ArgumentParser()
parser.add_argument('--repo', type=Path, required=True)
args = parser.parse_args()
def frozen(path):
    return subprocess.check_output(['git', '-C', str(args.repo), 'show', BASE + ':' + path])
with tempfile.TemporaryDirectory(prefix='lean-da0bc-') as tmp:
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
    baseline.write_bytes(frozen(OLD))
    score.compile_single(baseline, FLAGS, root / 'baseline.o')
    group = root / 'group'
    group.mkdir()
    for name in ['caller.c', 'cleanup.c', 'group.json']:
        (group / name).write_bytes((HERE / name).read_bytes())
    score.compile_group(group, root / 'candidate.o')
    rows = []
    for mode, obj, functions in [('baseline_single', root / 'baseline.o', ['func_800DA0BC']),
                                ('candidate_group', root / 'candidate.o', ['func_800DA0BC', 'func_800DB1E0'])]:
        data, sections = score._elf(obj)
        symtab = next(i for i, sec in enumerate(sections) if sec['type'] == 2)
        symbols = score._symbol_table(data, sections, symtab)
        for name in functions:
            fn = next(s for s in symbols if s['name'] == name and s['type'] == 2)
            result = score.compare(obj, name, show=0)
            rows.append(dict(mode=mode, function=name, result=dataclasses.asdict(result),
                             function_bytes=fn['size'], target_bytes=len(score.targets()[name])*4))
    hashes = {p: hashlib.sha256((HERE / p).read_bytes()).hexdigest() for p in ['caller.c','cleanup.c','group.json']}
    print(json.dumps(dict(base=BASE, flags=FLAGS, automatic_backend_flag=score.R4300_CC,
                         source_sha256=hashes, rows=rows), indent=2))
