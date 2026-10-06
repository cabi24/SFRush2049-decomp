"""Portable checks for the real-caller B66B0 matching packet."""
import importlib.util
import json
import os
import shutil
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_text_measure_b66b0_20261006'
spec = importlib.util.spec_from_file_location('dot_b66b0_proof', PACKET / 'verify.py')
proof = importlib.util.module_from_spec(spec)
spec.loader.exec_module(proof)


def test_recipe_and_own_source_binding():
    receipt = json.loads((PACKET / 'verification.json').read_text())
    assert receipt['result'] == 'MATCH'
    assert receipt['native_bytes'] == 152
    assert receipt['elf_function_bytes'][proof.FN] == 152
    assert receipt['target_relocations'] == 0
    assert receipt['owned_data_bytes'] == 0
    assert receipt['context']['base_commit'] == proof.BASE
    for name, expected in receipt['own_files'].items():
        assert proof.digest((ROOT / name).read_bytes()) == expected
    assert proof.GROUP.joinpath('group.c').read_text().splitlines()[0] == '/* flags: ' + proof.FLAGS + ' */'


def test_native_string_contract_and_maximum_narrowing():
    words = proof.score.targets()[proof.FN]
    cases = [(b'\0', 0), (b'abc\0', 0), (b'abc\0', 1), (b'abc\0', 0xffff),
             (b'\xff\0\0', 0), (b'\xff\1\0\0\1\0\0', 0x1234ffff)]
    for data, raw in cases:
        assert proof.execute(words, data, raw) == proof.oracle(data, raw)
    assert proof.oracle(b'abc\0', 0) == 1
    assert proof.oracle(b'\xff\1\2\0\0', 0) == 3


def test_unknown_and_truncated_native_fail_closed():
    words = list(proof.score.targets()[proof.FN])
    words[0] = 0xffffffff
    with pytest.raises(AssertionError, match='unsupported'):
        proof.execute(words, b'abc\0', 0xffff)
    with pytest.raises(AssertionError, match='instruction fetch'):
        proof.execute(proof.score.targets()[proof.FN][:-1], b'\0', 0xffff)


def test_historical_real_caller_body():
    history = Path(os.environ.get('RUSH_B66B0_HISTORY_REPO', ROOT))
    result = proof.historical_source(history)
    assert result['caller_body_unchanged'] and result['helper_body_unchanged']


def test_fresh_complete_compiler_and_behavior_replay():
    if not (proof.score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    if not shutil.which('gcc'):
        pytest.skip('host GCC required for unchanged-source behavioral proof')
    history = Path(os.environ.get('RUSH_B66B0_HISTORY_REPO', ROOT))
    result = proof.proof(history)
    assert result == json.loads((PACKET / 'verification.json').read_text())


def byte_proof_toolchain():
    if not (proof.score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')


@pytest.mark.parametrize('change', ['caller_removed', 'caller_renamed', 'caller_words_changed'])
def test_live_unclaimed_caller_drift_preserves_proof(monkeypatch, change):
    byte_proof_toolchain()
    original = proof.score.targets()
    changed = dict(original)
    if change == 'caller_removed':
        changed.pop(proof.CALLER)
    elif change == 'caller_renamed':
        changed['renamed_unclaimed_caller'] = changed.pop(proof.CALLER)
    else:
        changed[proof.CALLER] = [0] * len(changed[proof.CALLER])
    assert changed[proof.FN] == original[proof.FN]
    monkeypatch.setattr(proof.score, 'targets', lambda: changed)
    # The proof's context must not consult any mutable live symbol map either.
    def unexpected_live_symbols():
        raise AssertionError('unclaimed context must use historical base symbols')
    monkeypatch.setattr(proof.score, 'image_symbols', unexpected_live_symbols)
    history = Path(os.environ.get('RUSH_B66B0_HISTORY_REPO', ROOT))
    expected = json.loads((PACKET / 'verification.json').read_text())
    expected.pop('behavior')
    assert proof.proof(history, behavior=False) == expected


@pytest.mark.parametrize('change', ['word_changed', 'truncated', 'extended'])
def test_live_claimed_target_mutation_is_rejected(monkeypatch, change):
    byte_proof_toolchain()
    changed = dict(proof.score.targets())
    words = list(changed[proof.FN])
    if change == 'word_changed':
        words[0] ^= 1
    elif change == 'truncated':
        words.pop()
    else:
        words.append(0)
    changed[proof.FN] = words
    monkeypatch.setattr(proof.score, 'targets', lambda: changed)
    history = Path(os.environ.get('RUSH_B66B0_HISTORY_REPO', ROOT))
    with pytest.raises(AssertionError, match='native identity changed|full target words differ'):
        proof.proof(history, behavior=False)
