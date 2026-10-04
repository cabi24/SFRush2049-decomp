#!/usr/bin/env python3
"""Compile actual submitted sources with a separate host-only test driver."""
from pathlib import Path
import subprocess
import tempfile
from verify import ROOT, SUFFIXES

with tempfile.TemporaryDirectory(prefix='bt02-host-') as tmp:
    binary = Path(tmp) / 'checks'
    sources = [ROOT / 'cloud/matches/boot_tail' / ('func_800' + s + '.c') for s in SUFFIXES]
    subprocess.run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra',
                    '-Wno-unused-parameter', '-Werror', '-O2',
                    '-o', str(binary), str(Path(__file__).with_name('host_tests.c')),
                    *map(str, sources)], check=True)
    subprocess.run([str(binary)], check=True)
print('PASS: host behavior checks for all nine actual sources')
