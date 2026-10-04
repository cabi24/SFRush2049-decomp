#!/usr/bin/env python3
"""Exercise actual sources against synthetic contract fixtures, never retail data."""
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile
import os
P = Path(__file__).resolve().parent
SOURCES = [P / 'nonmatch' / ('func_' + a + '.c') for a in ('80011104', '800114C0', '800139D4')]
def run():
    rows = []
    with tempfile.TemporaryDirectory(prefix='audio-final-host-') as directory:
        for kind, flags in [('c89', ['-O2']), ('asan_ubsan', ['-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-pie', '-no-pie'])]:
            exe = Path(directory) / kind
            command = ['gcc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-Wno-pointer-to-int-cast', '-Wno-int-to-pointer-cast', *flags, *map(str, SOURCES), str(P / 'host_behavior.c'), '-o', str(exe)]
            subprocess.run(command, check=True, capture_output=True, text=True)
            env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0', UBSAN_OPTIONS='halt_on_error=1')
            result = subprocess.run([str(exe)], capture_output=True, text=True, env=env)
            if result.returncode:
                raise RuntimeError(result.stdout + result.stderr)
            rows.append({'configuration': kind, 'result': 'PASS', 'stdout': result.stdout.strip(), 'flags': command[1:command.index(str(SOURCES[0]))]})
    return {'result': 'PASS', 'source_hashes': {str(p.relative_to(P)): hashlib.sha256(p.read_bytes()).hexdigest() for p in SOURCES + [P/'host_behavior.c', P/'test_host.py']}, 'configurations': rows,
            'limits': 'Synthetic external data values only; does not prove retail float/table values or callee behavior. Native pointer sizes/layouts are separately checked by IDO. Aligned cursor is observed as an integer, never dereferenced by the host.'}
if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
