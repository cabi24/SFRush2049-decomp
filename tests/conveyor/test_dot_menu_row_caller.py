"""Check the bounded real-caller research packet without changing production."""
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/ipa-groups/dot_menu_row_caller_20261005'


def test_proof_is_full_extent_and_not_caller_credit():
    proof = json.loads((PACKET/'verification.json').read_text())
    spec = json.loads((PACKET/'group.json').read_text())
    assert spec['claims'] == proof['claims'] == ['func_8010A7A4']
    assert proof['source_sha256'] == hashlib.sha256((PACKET/'group.c').read_bytes()).hexdigest()
    assert proof['manifest_sha256'] == hashlib.sha256((PACKET/'group.json').read_bytes()).hexdigest()
    rows = {row['function']: row for row in proof['results']}
    helper = rows['func_8010A7A4']
    assert helper['elf_st_size'] == helper['extent_to_next_symbol_or_text_end'] == helper['target_bytes'] == 300
    assert helper['production_relocation_full_extent_equal']
    assert helper['independent_gnu_link_full_extent_equal']
    assert helper['fully_resolved_sha256'] == helper['target_sha256']
    assert helper['canonical_comparison']['differing'] == helper['canonical_comparison']['extra_words'] == 0
    assert not helper['unresolved_after_proof'] and not helper['unverified_after_proof']
    assert proof['own_literal']['verified']
    assert not rows['func_8010A8D0']['claimed']
    assert rows['func_8010A8D0']['canonical_comparison']['differing'] > 0
    assert not rows['func_800BEA3C']['claimed']
    assert rows['func_800BEA3C']['production_relocation_full_extent_equal']
    assert proof['host_behavior_cases'] == 1769 and proof['native_layout_checks'] == 9
    assert all(c['helper_comparison']['differing'] > 0 for c in proof['rejected_controls'])


def test_genuine_context_has_no_standins_or_pressure_locals():
    source = (PACKET/'group.c').read_text()
    assert 'volatile' not in source and '__inline' not in source
    assert 'caller_a' not in source and 'caller_b' not in source
    assert source.count('func_8010A7A4(') == 15
    assert 'D_80118E28.rgba = first.rgba;' in source
    assert 'D_80118E2C.rgba = second.rgba;' in source
    assert 'dispatch_handler(index == D_80116D9C ? 22 : 1);' in source


def test_source_behavior(tmp_path):
    cc = shutil.which('cc')
    if cc is None:
        pytest.skip('host C compiler unavailable')
    binary = tmp_path/'menu_row'
    subprocess.run([cc, '-std=c89', '-Wall', '-Wextra', '-Werror', '-O1',
        str(PACKET/'host_test.c'), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)


def test_toolchain_replays_full_extent_proof(tmp_path):
    """Normal CI must recreate the proof, rather than trust saved JSON."""
    from tools.cloud import score

    if not (score.IDO/'cc').is_file():
        pytest.skip('IDO compiler unavailable')
    for tool in ('mips-linux-gnu-ld', 'mips-linux-gnu-objcopy',
                 'mips-linux-gnu-objdump', 'cc'):
        if shutil.which(tool) is None:
            pytest.skip('C compiler or MIPS binutils unavailable: ' + tool)
    replay = tmp_path/'verification.json'
    subprocess.run([sys.executable, str(PACKET/'verify.py'), str(replay)],
                   cwd=ROOT, check=True, capture_output=True, text=True)
    assert json.loads(replay.read_text()) == json.loads(
        (PACKET/'verification.json').read_text())
