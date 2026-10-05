"""Reproducible source/ABI/behavior gates for the Hidden-based callback."""
import importlib.util
import json
from pathlib import Path
import shutil
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_hidden_callback_20261005'
sys.path.insert(0, str(ROOT))
spec = importlib.util.spec_from_file_location('hidden_callback_verify', PACKET / 'verify.py')
verify = importlib.util.module_from_spec(spec); spec.loader.exec_module(verify)


def receipt(): return json.loads((PACKET / 'verification.json').read_text())


def test_source_and_packet_binding():
    saved = receipt()
    assert saved['source_sha256'] == verify.sha(verify.SOURCE.read_bytes())
    for name, expected in saved['packet_sha256'].items():
        assert verify.sha((PACKET / name).read_bytes()) == expected
    assert saved['genuine_context']['context_sha256'] == verify.sha(verify.CONTEXT.read_bytes())


def test_claim_is_one_complete_callback():
    saved = receipt(); code = saved['code']
    assert saved['function'] == 'state_update_global' and saved['accepted_byte_gain'] == 0
    assert code['start'] == '0x8010b560' and code['end_exclusive'] == '0x8010b5d0'
    assert code['symbol_bytes'] == code['native_bytes'] == 112
    assert code['excluded_preceding_helper_bytes'] == 60 and code['excluded_zero_alignment_bytes'] == 4
    assert code['own_data_bytes'] == 0 and len(code['relocations']) == 6
    assert verify.complete(code)


@pytest.fixture(scope='module')
def fresh(tmp_path_factory):
    if not (verify.score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld') or not shutil.which('cc'):
        pytest.skip('Pinned IDO, GNU MIPS binutils and host C compiler required')
    return verify.verify(tmp_path_factory.mktemp('hidden-callback'))


def test_fresh_complete_elf_and_gnu_link(fresh):
    assert fresh['code'] == receipt()['code']


def test_genuine_context_and_old_controls(fresh):
    assert fresh['genuine_context'] == receipt()['genuine_context']
    assert all(verify.complete(row) for row in fresh['genuine_context']['bodies'].values())
    assert fresh['controls'] == receipt()['controls']


def test_native_linked_host_and_mutant_proof(fresh):
    assert fresh['behavior'] == receipt()['behavior']
    assert fresh['behavior']['cases'] == 15744
    assert fresh['behavior']['target_instructions_covered'] == 28
    assert all(item['rejected'] for item in fresh['behavior']['negative_controls'].values())


def test_native_unknown_instruction_fails_closed():
    words = list(verify.score.targets()[verify.FN]); words[0] = 0xffffffff
    with pytest.raises(AssertionError, match='unknown opcode'):
        verify.native.Machine(words, (0, -1, 0, 256, 0)).run()


def test_native_missing_callback_is_detected():
    words = list(verify.score.targets()[verify.FN]); words[9] = 0
    args = (0, -1, 0, 256, 0)
    assert verify.native.Machine(words, args).run() != verify.native.reference(args)


def test_receipt_rejects_extra_words_and_extent_shrink():
    clean = receipt()['code']; assert verify.complete(clean)
    assert not verify.complete(dict(clean, extra_words=1))
    assert not verify.complete(dict(clean, symbol_bytes=108))
