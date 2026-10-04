#!/usr/bin/env python3
"""Compile actual matching sources separately from host-only contract doubles."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile
ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
NAMES = ('800107E0', '80013D70')
SOURCES = [ROOT / 'cloud/matches/boot_tail' / ('func_' + name + '.c') for name in NAMES]
SOURCES += [PACKET / 'nonmatch' / ('func_' + name + '.c') for name in ('80010840', '800108E0')]
SOURCES.append(PACKET / 'host_behavior.c')
def run():
    rows = []
    with tempfile.TemporaryDirectory(prefix='bt02-final-small-host-') as tmp:
        for label, flags in [('c89', ['-O2']), ('asan_ubsan', ['-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer'])]:
            executable = Path(tmp) / label
            command = ['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Wno-unused-parameter'] + flags + list(map(str, SOURCES)) + ['-o', str(executable)]
            subprocess.run(command, check=True, capture_output=True, text=True)
            env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0', UBSAN_OPTIONS='halt_on_error=1')
            output = subprocess.check_output([str(executable)], env=env, text=True, stderr=subprocess.STDOUT)
            rows.append({'configuration': label, 'flags': command[1:command.index(str(SOURCES[0]))], 'result': 'PASS', 'output': output.strip()})
    return {'result': 'PASS', 'runs': rows, 'leak_detection': 'disabled: runtime ptrace limitation', 'source_sha256': {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest() for p in SOURCES}}
if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
