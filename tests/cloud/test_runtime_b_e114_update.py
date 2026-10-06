"""Portable tests for the source-only, nonmatching E114 private-body packet."""
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
PACKET = ROOT / 'cloud/work/runtime_b_e114_update_20261006'


def test_packet_source_bindings():
    receipt = json.loads((PACKET / 'verification.json').read_text())
    for name, expected in receipt['sources_sha256'].items():
        assert hashlib.sha256((PACKET / name).read_bytes()).hexdigest() == expected
    assert (PACKET / 'update.c').read_text().splitlines()[0] == \
        '/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
    assert 'RESEARCH-ONLY' in receipt['status']
    assert receipt['negative_controls']


def test_current_selected_native_words(monkeypatch):
    monkeypatch.setattr(score, 'ASM_DIR', ROOT / 'asm/us/ovl_b')
    words = score.targets()['func_8038E114']
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


def test_no_artificial_private_context():
    source = (PACKET / 'update.c').read_text()
    assert 'f32 radius;' in source
    assert 'f32 radius =' not in source
    assert 'private_DA78(record, record->previous_position)' in source
    assert 'private_DA78(record, previous)' in source
    assert 'BQuad *quad;' in source
    assert 'NOT an original exported/kept IPA root' in source


def test_documented_native_extent_and_boundaries():
    receipt = json.loads((PACKET / 'verification.json').read_text())
    assert receipt['target']['size'] == 5196
    assert len(receipt['behavior']['native_executed_offsets']) == 1292
    assert len(receipt['behavior']['native_unexecuted_offsets']) == 7
    assert len(receipt['layout_facts']) == 42
    assert set(receipt['external_bindings']) >= {
        'private_DA78', 'private_D498', 'private_D328', 'private_D200', 'private_E088'}


def test_alternate_source_cli_is_rejected(tmp_path):
    alternate = tmp_path / 'alternate.c'
    alternate.write_text('/* A different input must never inherit this packet receipt. */\n')
    output = tmp_path / 'untrusted-receipt.json'
    result = subprocess.run([sys.executable, str(PACKET / 'verify.py'),
                             '--source', str(alternate), '--output', str(output)],
                            text=True, capture_output=True, cwd=tmp_path)
    assert result.returncode == 2
    assert 'unrecognized arguments: --source' in result.stderr
    assert not output.exists()
