#!/usr/bin/env python3
"""Independently verify packet's exact source/target identities on an x86 builder."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import subprocess
import sys
import tempfile

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('--repo', type=Path, required=True)
p.add_argument('--toolkit', type=Path, required=True)
p.add_argument('--target-dir', type=Path, required=True)
p.add_argument('--output', type=Path)
a = p.parse_args()
root = Path(__file__).resolve().parent
os.environ['CONVEYOR_TOOLKIT'] = str(a.toolkit.resolve())
sys.path[:0] = [str(a.repo / 'tools/conveyor/jobs'), str(a.repo / 'tools/cloud')]
import scoring
import score as cloudscore

expected = {}
for name in ['verification.json']:
    expected.update({r['function']: r for r in json.loads((root / name).read_text())})
results = []
with tempfile.TemporaryDirectory(prefix='static-independent-') as scratch:
    for n, e in sorted(expected.items()):
        if e['strict_score'] != 0:
            continue  # blocked source is deliberately excluded
        src = root / (n + '.c')
        target = a.target_dir / (n + '.target.o')
        for path, key in [(src, 'source_sha256'), (target, 'target_sha256')]:
            if hashlib.sha256(path.read_bytes()).hexdigest() != e[key]:
                raise SystemExit('SHA-256 mismatch: ' + str(path))
        flags = src.read_text().splitlines()[0]
        if flags != '/* flags: ' + e['flags'] + ' */':
            raise SystemExit('flags line mismatch: ' + str(src))
        obj = Path(scratch) / (n + '.o')
        proc = subprocess.run([str(a.toolkit / 'ido/cc'), '-c'] + shlex.split(e['flags']) +
                              [str(src), '-o', str(obj)], capture_output=True, text=True,
                              timeout=120)
        if proc.returncode:
            raise SystemExit(proc.stderr)
        strict = scoring.score(target, obj, stack_differences=True)
        t, c = cloudscore.text_words(target), cloudscore.text_words(obj)
        raw = sum(x != y for x, y in zip(t, c)) + abs(len(t) - len(c))
        result = {'function': n, 'strict_score': strict, 'raw_word_diff': raw,
                  'source_sha256': e['source_sha256'], 'target_sha256': e['target_sha256'],
                  'flags': e['flags']}
        print(json.dumps(result), flush=True)
        results.append(result)
if a.output:
    a.output.write_text(json.dumps(results, indent=2) + '\n')
raise SystemExit(0 if len(results) == 5 and all(r['strict_score'] == r['raw_word_diff'] == 0
                                             for r in results) else 1)
