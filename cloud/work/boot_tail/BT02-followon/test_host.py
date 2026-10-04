#!/usr/bin/env python3
"""Compile actual matching files separately with host-only callback doubles."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
NAMES = ('105C4', '10D74', '11C84', '14374', '143C0', '14434')


def test():
    sources = [ROOT / 'cloud/matches/boot_tail' / ('func_800' + s + '.c') for s in NAMES]
    sources.append(PACKET / 'host_behavior.c')
    reports = []
    with tempfile.TemporaryDirectory(prefix='bt02-followon-host-') as tmp:
        for mode, options in [('c89', ['-O2']),
                              ('sanitizers', ['-O1', '-fsanitize=address,undefined',
                                              '-fno-omit-frame-pointer'])]:
            output = Path(tmp) / mode
            cmd = ['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                   *options, *map(str, sources), '-o', str(output)]
            subprocess.run(cmd, check=True, capture_output=True, text=True)
            env = dict(os.environ)
            env['ASAN_OPTIONS'] = 'detect_leaks=0'
            proc = subprocess.run([str(output)], env=env, capture_output=True, text=True)
            if proc.returncode:
                raise RuntimeError(mode + ': ' + proc.stdout + proc.stderr)
            reports.append({'mode': mode, 'result': 'PASS', 'stdout': proc.stdout.strip()})
    return {'result': 'PASS', 'reports': reports,
            'source_hashes': {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()
                              for p in sources},
            'leak_detection': 'Disabled: LeakSanitizer rejects the execution environment under ptrace; this driver allocates no heap objects.',
            'limits': 'Host callbacks model call contracts; no N64 hardware or concurrent scheduling is emulated.'}


if __name__ == '__main__':
    print(json.dumps(test(), indent=2))
