#!/usr/bin/env python3
"""C89 host behavior tests with test-only SDK/callback doubles."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent

def run():
    with tempfile.TemporaryDirectory(prefix='bt02-context-host-') as directory:
        output = Path(directory) / 'host'
        command = ['cc', '-std=c89', '-Wall', '-Wextra', '-Werror',
                   '-Wno-unused-parameter', '-fno-builtin', '-Dbzero=test_bzero',
                   str(ROOT / 'cloud/matches/boot_tail/func_80010A40.c'),
                   str(PACKET / 'nonmatch/func_80010E80.c'),
                   str(PACKET / 'host_behavior.c'), '-o', str(output)]
        subprocess.run(command, check=True)
        return subprocess.check_output([str(output)], text=True).strip()

if __name__ == '__main__':
    print(run())
