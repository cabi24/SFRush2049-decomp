"""Frozen research packet replay; never counts this nonmatch as accepted source."""
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/runtime_b_teardown_20261006'
SPEC = importlib.util.spec_from_file_location('teardown_proof', PACKET / 'verify.py')
PROOF = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PROOF)


def toolchain_or_skip():
    needed = [PROOF.score.ido('cc'), PROOF.shutil.which('gcc'), PROOF.shutil.which('mips-linux-gnu-ld')]
    present = all(p and Path(p).is_file() for p in needed)
    if not present and os.environ.get('REQUIRE_TOOLCHAIN') == '1':
        pytest.fail('required toolchain is missing')
    if not present:
        pytest.skip('set REQUIRE_TOOLCHAIN=1 for required compiler replay')


@pytest.mark.parametrize('optimized', [False, True])
def test_frozen_portable_replay(tmp_path, optimized):
    toolchain_or_skip()
    elsewhere = tmp_path / 'unrelated directory with spaces'
    elsewhere.mkdir()
    receipt = elsewhere / 'replayed.json'
    cmd = [sys.executable] + (['-O'] if optimized else [])
    result = subprocess.run(cmd + [str(PACKET / 'verify.py'), '--output', str(receipt)],
                            cwd=elsewhere, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    assert json.loads(receipt.read_text()) == json.loads((PACKET / 'verification.json').read_text())


def test_unknown_instruction_and_bad_memory_fail_closed():
    memory = PROOF.fixture(0, 1, False)
    with pytest.raises(AssertionError, match='unsupported opcode'):
        PROOF.execute([0xffffffff], memory, 0)
    with pytest.raises(AssertionError, match='unaligned access'):
        memory.get(PROOF.BASE + 1, 4)
    with pytest.raises(AssertionError, match='unmapped access'):
        memory.get(0, 4)


def test_helper_contract_rejects_bad_handles_and_destinations():
    memory = PROOF.fixture(0, 1, False)
    with pytest.raises(AssertionError, match='sign extended'):
        PROOF.hook(memory, 0x80090254, 0x00008000, 0, [])
    with pytest.raises(AssertionError, match='unknown helper'):
        PROOF.hook(memory, 0x80090258, 0, 0, [])


def test_zero_count_after_resource_callback_skips_arrays():
    initial = PROOF.fixture(2, 4, True)
    final, events = PROOF.oracle(initial, 2)
    assert len(events) == 1
    assert final[0][:0xe14] == initial.snapshot()[0][:0xe14]
    assert final[1] == bytes(2)


def test_packet_is_research_only():
    data = json.loads((PACKET / 'verification.json').read_text())
    assert data['status'] == 'NONMATCH'
    assert data['accepted_bytes'] == 0
    assert data['elf']['differing_words'] == 34
    assert not (ROOT / 'cloud/matches/ovl_b/func_8039244C.c').exists()
