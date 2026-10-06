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
    fresh, saved = json.loads(receipt.read_text()), json.loads((PACKET / 'verification.json').read_text())
    # Game-blob manifest files and the game lock list change with every splice;
    # image-B inputs stay bound.
    for proof in (fresh, saved):
        proof['inputs_sha256'] = {k: v for k, v in proof['inputs_sha256'].items()
                                  if not k.startswith('asm/us/blob/')
                                  and k != 'blob_matched.lock.json'}
    assert fresh == saved


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


def test_gnu_explicit_entry_and_misplacement_rejection(tmp_path, monkeypatch):
    toolchain_or_skip()
    original_run = PROOF.run
    scripts = []

    def inspect_link(args, **kwargs):
        if args[0] == 'mips-linux-gnu-ld':
            script = Path(args[args.index('-T') + 1])
            text = script.read_text()
            assert '.text 0x8039244C : SUBALIGN(4)' in text
            scripts.append(text)
        return original_run(args, **kwargs)

    monkeypatch.setattr(PROOF, 'run', inspect_link)
    previous = PROOF.score.ASM_DIR
    PROOF.elf_proof(tmp_path)
    assert scripts and PROOF.score.ASM_DIR == previous

    def misplaced_link(args, **kwargs):
        if args[0] == 'mips-linux-gnu-ld':
            script = Path(args[args.index('-T') + 1])
            script.write_text(script.read_text().replace('.text 0x8039244C', '.text 0x80392450'))
        return original_run(args, **kwargs)

    monkeypatch.setattr(PROOF, 'run', misplaced_link)
    with pytest.raises(AssertionError, match='GNU placement/extent.*80392450'):
        PROOF.elf_proof(tmp_path)
    assert PROOF.score.ASM_DIR == previous


def test_target_context_restores_after_nested_exception(monkeypatch):
    previous = PROOF.score.ASM_DIR

    def fail():
        with PROOF.target_image(ROOT / 'asm/us/blob'):
            raise RuntimeError('forced helper failure')

    monkeypatch.setattr(PROOF, '_prove', fail)
    with pytest.raises(RuntimeError, match='forced helper failure'):
        PROOF.prove()
    assert PROOF.score.ASM_DIR == previous
