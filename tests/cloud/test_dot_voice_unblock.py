"""Source, provenance, fail-closed and full replay tests for 1F954."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/boot_tail/BT03-voice-unblock-topology'
SOURCE = ROOT / 'cloud/matches/boot_tail/func_8001F954.c'


def receipt():
    return json.loads((PACKET / 'evidence.json').read_text())


def test_final_source_binding():
    row = receipt()['rows'][0]
    assert row['source_sha256'] == hashlib.sha256(SOURCE.read_bytes()).hexdigest()
    assert row['function'] == 'func_8001F954'
    assert (row['elf_size'], row['text_size'], row['alignment_zero_bytes']) == (124, 128, 4)
    assert row['comparison'] == dict(differing=0, total=31, unresolved=[], unverified=[], errors=[], extra_words=0)


def test_real_current_caller_and_helper_contracts():
    rows = {row['function']: row for row in receipt()['rows']}
    assert rows['func_8001C7F4']['source_path'] == 'cloud/work/boot_tail_promotion/sources/func_8001C7F4.c'
    assert rows['func_80014AF0']['source_path'] == 'cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014AF0.c'
    assert all(rows[name]['exact_current_lock_body'] for name in ('func_8001C7F4', 'func_80014AF0'))
    assert 'include/boot_tail_sample_contract.h' in receipt()['input_sha256']
    assert 'include/boot_tail_audio_record.h' in receipt()['input_sha256']


def test_bounded_native_proof_and_mutations():
    behavior = receipt()['rows'][0]['behavior']
    assert behavior['cases'] == 2112
    assert behavior['native_and_linked_executions'] == 4224
    assert behavior['all_instruction_offsets_covered'] == list(range(0, 124, 4))
    assert len(behavior['negative_controls_rejected']) == 3
    assert len(receipt()['actual_c_negative_controls_rejected']) == 3
    assert receipt()['accepted_byte_gain'] == 0


def test_causal_controls():
    rows = {row['source']: row for row in receipt()['controls']}
    assert rows['baseline']['complete_body_diff_offsets'] == [96, 100]
    assert rows['named_pointer_restored']['complete_body_diff_offsets'] == [96, 100]
    assert rows['signed_boundary']['complete_body_diff_offsets'] == []
    assert rows['nested_guard']['complete_body_diff_offsets'] == []
    assert rows['final_O1']['complete_body_diff_offsets']


@pytest.mark.parametrize('filename', ['verify.py', 'native_behavior.py'])
def test_optimized_python_is_rejected(filename):
    result = subprocess.run([sys.executable, '-O', str(PACKET / filename)],
                            text=True, capture_output=True)
    assert result.returncode != 0
    assert 'assertions' in result.stderr + result.stdout


def test_reject_invalid_replay_opcode():
    spec = importlib.util.spec_from_file_location('test_unblock_native', PACKET / 'native_behavior.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    with pytest.raises(AssertionError, match='unknown opcode'):
        module.run([0xFC000000] * 31, 0, 1, 0)


def test_full_replay_from_unrelated_working_directory(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    if not (ido / 'cc').exists() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('IDO or MIPS binutils unavailable')
    result = subprocess.run([sys.executable, str(PACKET / 'verify.py'), '--check'],
                            cwd=tmp_path, text=True, capture_output=True)
    assert result.returncode == 0, result.stdout + result.stderr
    assert json.loads(result.stdout)['status'] == 'PASS'
