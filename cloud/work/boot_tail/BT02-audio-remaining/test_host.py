#!/usr/bin/env python3
"""Run the actual complete sources as separate translation units."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile
P = Path(__file__).resolve().parent
sources = [P / 'test_host.c', P / 'nonmatch/func_80013DEC.c', P / 'nonmatch/func_80014198.c']
rows = []
with tempfile.TemporaryDirectory(prefix='bt02-audio-remaining-host-') as directory:
    for mode, flags in [('c89', []), ('asan_ubsan', ['-fsanitize=address,undefined', '-fno-omit-frame-pointer'])]:
        executable = Path(directory) / mode
        command = ['gcc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-O1', '-g'] + flags
        command += [str(p) for p in sources] + ['-o', str(executable)]
        subprocess.run(command, check=True, capture_output=True, text=True)
        environment = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')
        result = subprocess.run([str(executable)], check=True, capture_output=True, text=True, env=environment)
        rows.append({'mode': mode, 'result': 'PASS', 'output': result.stdout.splitlines()})
print(json.dumps({'result': 'PASS', 'actual_sources': {p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in sources}, 'runs': rows}, indent=2))
