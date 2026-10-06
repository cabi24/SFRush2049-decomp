#!/usr/bin/env python3
"""Minimal frozen-input strict-score replay; no acceptance or ROM claim."""
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
FUNCTION = 'func_8001671C'
OLD = 'cloud/work/boot_tail/BT03-low-insert-pair/func_8001671C_NONMATCH.c'
HERE = Path(__file__).resolve().parent
parser = argparse.ArgumentParser()
parser.add_argument('--repo', type=Path, required=True)
args = parser.parse_args()
def frozen(path):
    return subprocess.check_output(['git', '-C', str(args.repo), 'show', BASE + ':' + path])
with tempfile.TemporaryDirectory(prefix='lean-1671c-') as tmp:
    root = Path(tmp)
    paths = ['tools/cloud/score.py', 'tools/cloud/owndata.py']
    paths += ['asm/us/boot_tail/' + p for p in ['SHA256SUMS', 'symbols.json', 'extents.json', 'boot_tail_8000f3a4.s']]
    for path in paths:
        output = root / path
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_bytes(frozen(path))
    sys.path.insert(0, str(root / 'tools/cloud'))
    spec = importlib.util.spec_from_file_location('score', root / 'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec)
    sys.modules['score'] = score
    spec.loader.exec_module(score)
    score.ASM_DIR = root / 'asm/us/boot_tail'
    rows = []
    for name, content in [('baseline', frozen(OLD)), ('candidate', (HERE / (FUNCTION + '.c')).read_bytes())]:
        source, obj = root / (name + '.c'), root / (name + '.o')
        source.write_bytes(content)
        score.compile_single(source, FLAGS, obj)
        result = score.compare(obj, FUNCTION, show=0)
        data, sections = score._elf(obj)
        symtab = next(i for i, sec in enumerate(sections) if sec['type'] == 2)
        fn = next(s for s in score._symbol_table(data, sections, symtab) if s['name'] == FUNCTION)
        rows.append(dict(source=name, source_sha256=hashlib.sha256(content).hexdigest(),
                         result=dataclasses.asdict(result), function_bytes=fn['size']))
    print(json.dumps(dict(base=BASE, flags=FLAGS, automatic_backend_flag=score.R4300_CC, function=FUNCTION,
                         target_bytes=len(score.targets()[FUNCTION])*4, rows=rows), indent=2))
