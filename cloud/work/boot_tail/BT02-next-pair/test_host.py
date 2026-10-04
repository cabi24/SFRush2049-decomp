#!/usr/bin/env python3
"""Compile and exercise the actual complete nonmatches as separate C89 units."""
import json
import os
from pathlib import Path
import subprocess
import tempfile

P = Path(__file__).resolve().parent
sources = [P / 'host_behavior.c'] + sorted((P / 'nonmatch').glob('*.c'))
results = []
with tempfile.TemporaryDirectory(prefix='bt02-next-host-') as temp:
    for label, flags in [('C89 O2', ['-O2']),
                         ('ASan UBSan O1', ['-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer'])]:
        out = Path(temp) / 'host_test'
        subprocess.run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                        *flags, *(str(p) for p in sources), '-o', str(out)], check=True)
        result = subprocess.check_output([str(out)], text=True,
                    env={**os.environ, 'ASAN_OPTIONS': 'detect_leaks=0'})
        results.append({'mode': label, 'output': result.strip()})
print(json.dumps({'result': 'PASS', 'tests': results}, indent=2))
