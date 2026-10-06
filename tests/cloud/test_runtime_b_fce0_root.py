"""Portable tests for the source-only, nonmatching FCE0 root packet."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys

import pytest
from tools.cloud import score

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/runtime_b_fce0_root_20261006'


def test_packet_source_bindings():
    receipt = json.loads((PACKET / 'verification.json').read_text())
    for name, expected in receipt['sources_sha256'].items():
        assert hashlib.sha256((PACKET / name).read_bytes()).hexdigest() == expected
    assert (PACKET / 'root.c').read_text().splitlines()[0] == \
        '/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
    assert 'RESEARCH-ONLY' in receipt['status']
    assert receipt['negative_controls']


def test_current_selected_native_words(monkeypatch):
    monkeypatch.setattr(score, 'ASM_DIR', ROOT / 'asm/us/ovl_b')
    words = score.targets()['func_8038FCE0']
    native = struct.pack('>' + str(len(words)) + 'I', *words)
    receipt = json.loads((PACKET / 'verification.json').read_text())
    assert len(native) == receipt['target']['size']
    assert hashlib.sha256(native).hexdigest() == receipt['target']['sha256']


def test_complete_behavior_replay():
    if not (score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    reference = Path(os.environ.get('RUSH_REFERENCE_ROOT', str(ROOT)))
    result = subprocess.run([sys.executable, str(PACKET / 'verify.py'),
                             '--reference-root', str(reference), '--check'],
                            text=True, capture_output=True)
    assert result.returncode == 0, result.stdout + result.stderr


def test_rejects_unsupported_alternate_source_option():
    result = subprocess.run([sys.executable, str(PACKET / 'verify.py'),
                             '--source', 'unsupported-input.c'],
                            text=True, capture_output=True)
    assert result.returncode == 2
    assert 'unrecognized arguments: --source' in result.stderr
