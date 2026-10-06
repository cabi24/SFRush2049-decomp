"""Portable research receipt and fail-closed replay regressions."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
from unittest import mock

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/boot_tail/BT03-voice-free-donor'


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    loaded = importlib.util.module_from_spec(spec)
    sys.modules[name] = loaded
    spec.loader.exec_module(loaded)
    return loaded


def receipt():
    return json.loads((PACKET / 'evidence.json').read_text())


def native():
    verify = module('voice_free_test_verifier', PACKET / 'verify.py')
    data = subprocess.check_output(['git', 'show', verify.BASE + ':' + verify.NATIVE_PATHS['func_8001F6EC']], cwd=ROOT)
    return verify.canonical_words(data)


def test_source_and_whole_extent_binding():
    verify = module('voice_free_test_verifier', PACKET / 'verify.py')
    record = receipt()
    assert hashlib.sha256((ROOT / verify.CANDIDATE).read_bytes()).hexdigest() == verify.FINAL_SOURCE_SHA
    assert record['candidate']['source_sha256'] == verify.FINAL_SOURCE_SHA
    assert record['candidate']['elf_entry'] == '0x8001f6ec'
    assert record['candidate']['elf_size'] == record['candidate']['text_size'] == 256
    assert record['candidate']['comparison']['differing'] == 28
    assert record['new_matching_bytes'] == record['accepted_byte_gain'] == 0


def test_donor_causal_control_results():
    rows = {row['label']: row for row in receipt()['controls']}
    assert {name: row['comparison']['differing'] for name, row in rows.items()} == {
        'baseline': 30, 'donor_index_lifetime_only': 30,
        'donor_queue_order_only': 28, 'donor_pointer_field': 28}
    assert rows['donor_index_lifetime_only']['gnu_linked_body_sha256'] == rows['baseline']['gnu_linked_body_sha256']
    assert rows['donor_queue_order_only']['gnu_linked_body_sha256'] == receipt()['candidate']['gnu_linked_body_sha256']


def test_current_accepted_context_and_census():
    record = receipt()
    assert {r['function']: r['elf_size'] for r in record['accepted_context']} == {
        'func_8001EE9C': 240, 'func_8001F9D0': 72, 'func_80021BC0': 48}
    for row in record['accepted_context']:
        assert row['comparison']['differing'] == 0
        assert len(row['current_normalized_lock_body_sha256']) == 64
        assert row['gnu_linked_body_sha256'] == row['native_sha256']
    assert record['direct_boot_tail_callers'] == {'func_8001F954': [92], 'func_8001F9D0': [48],
                                                'func_80021BC0': [20], 'func_80023E9C': [512]}


def test_behavior_coverage_and_actual_source():
    record = receipt()
    assert record['native_behavior']['native_covered_offsets'] == list(range(0, 256, 4))
    assert record['native_behavior']['linked_covered_offsets'] == list(range(0, 256, 4))
    assert record['native_behavior']['cases'] == record['actual_c89']['cases'] == 13824
    assert len(record['actual_c89']['negative_controls_rejected']) == 4
    assert len(record['native_behavior']['negative_controls_rejected']) == 4


def test_native_unaligned_and_callback_paths():
    behavior = module('voice_free_test_behavior', PACKET / 'native_behavior.py')
    words = native()
    for alignment in range(4):
        for active in (0, 1, 65535):
            for mutation in (0, 1):
                behavior.run(words, (31, active, 1, 255, 0, alignment, mutation))


def test_unknown_opcode_and_truncation_fail_closed():
    behavior = module('voice_free_test_behavior', PACKET / 'native_behavior.py')
    words = native()
    words[0] = 0xFC000000
    with pytest.raises(AssertionError, match='unknown opcode'):
        behavior.run(words, (7, 0, 1, 255, 0, 3, 1))
    with pytest.raises(AssertionError):
        behavior.run(native()[:-1], (7, 0, 1, 255, 0, 3, 1))


def test_optimized_python_rejected():
    result = subprocess.run([sys.executable, '-O', str(PACKET / 'verify.py')], capture_output=True, text=True)
    assert result.returncode != 0 and 'requires Python assertions' in result.stderr


def test_native_entry_address_drift_rejected(tmp_path):
    verify = module('voice_free_test_verifier', PACKET / 'verify.py')
    original = verify.load_module
    def drift(name, path):
        loaded = original(name, path)
        if name == 'voice_free_score':
            original_symbols = loaded.image_symbols
            def wrong_symbols():
                result = dict(original_symbols())
                result['func_8001F6EC'] += 4
                return result
            loaded.image_symbols = wrong_symbols
        return loaded
    with mock.patch.object(verify, 'load_module', side_effect=drift):
        with pytest.raises(AssertionError):
            verify.verify(ROOT, tmp_path)


def test_stale_source_rejected_before_compilation():
    verify = module('voice_free_test_verifier', PACKET / 'verify.py')
    original = subprocess.check_output
    with tempfile.TemporaryDirectory(prefix='voice-free-stale-') as temporary:
        temporary = Path(temporary)
        fake_root, scratch = temporary / 'source', temporary / 'scratch'
        candidate = fake_root / verify.CANDIDATE
        candidate.parent.mkdir(parents=True)
        candidate.write_text((ROOT / verify.CANDIDATE).read_text() + '\n/* stale */\n')
        scratch.mkdir()
        def bound_git(arguments, **options):
            assert arguments[0] == 'git', 'stale source unexpectedly reached a compiler'
            options['cwd'] = ROOT
            return original(arguments, **options)
        with mock.patch.object(verify.subprocess, 'check_output', side_effect=bound_git):
            with pytest.raises(AssertionError):
                verify.verify(fake_root, scratch)


def test_complete_portable_replay(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    if not (ido / 'cc').is_file():
        if os.environ.get('REQUIRE_TOOLCHAIN'):
            pytest.fail('REQUIRE_TOOLCHAIN set but IDO is unavailable')
        pytest.skip('IDO toolchain unavailable')
    unrelated = tmp_path / 'unrelated directory with spaces'
    unrelated.mkdir()
    subprocess.run([sys.executable, str(PACKET / 'verify.py'), '--check'], cwd=unrelated,
                   check=True, stdout=subprocess.DEVNULL)
