"""Runtime B reset: source/receipt identity, strict compile, bounded behavior."""
import importlib.util
import json
import shutil
import struct
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/runtime_b_reset_20261006'
spec = importlib.util.spec_from_file_location('runtime_b_reset_verify', PACKET / 'verify.py')
verify = importlib.util.module_from_spec(spec)
spec.loader.exec_module(verify)


def test_saved_input_identity():
    receipt = json.loads((PACKET / 'verification.json').read_text())
    assert receipt['image'] == 'B' and receipt['address'] == '0x8038ca24'
    assert receipt['bytes'] == 236 and receipt['status'] == 'MATCH'
    assert verify.sha(verify.SOURCE) == receipt['source_sha256']
    for path, digest in receipt['inputs_sha256'].items():
        if path == 'asm/us/blob/SHA256SUMS':
            continue    # game-image manifest: changes with every game splice, unrelated to image B
        assert verify.sha(ROOT / path) == digest


def test_signed_low_halfword_address_contract():
    stack = 0x70000000
    expected = verify.oracle(0xabcdffff, stack)
    assert expected[0x80152818 - 0x3b8 + 0x384] == 8
    assert expected[0x80399118 - 1] == 9
    assert expected[0x80399550 - 0x148 + 0x138] == 255
    assert expected[0x80399120 - 0x10c + 0x108] == 0
    assert bytes(expected[stack + i] for i in range(4)) == bytes.fromhex('abcdffff')
    minimum = verify.oracle(0x8000, stack)
    assert minimum[0x80152818 - 32768 * 0x3b8 + 0x384] == 8


def test_write_footprint_and_unknown_decoder_rejection():
    expected = verify.oracle(0, 0x70000000)
    assert len(expected) == 50
    assert 0x80152818 + 0x3a1 not in expected
    assert 0x80399550 + 0x139 not in expected
    assert 0x80399120 + 20 not in expected
    with pytest.raises(AssertionError):
        verify.execute([0xffffffff], 0)


def require_toolchain():
    missing = [name for name in ['mips-linux-gnu-ld', 'mips-linux-gnu-objcopy', 'mips-linux-gnu-nm', 'gcc'] if not shutil.which(name)]
    if missing or not (verify.score.IDO / 'cc').exists():
        pytest.skip('requires IDO and MIPS GNU/host compiler tools: ' + ', '.join(missing))


def test_target_read_failure_restores_callers_target_directory(monkeypatch, tmp_path):
    previous = tmp_path / 'another-image'
    monkeypatch.setattr(verify.score, 'ASM_DIR', previous)

    def reject_targets():
        assert verify.score.ASM_DIR == ROOT / 'asm/us/ovl_b'
        raise AssertionError('injected target read failure')

    monkeypatch.setattr(verify.score, 'targets', reject_targets)
    with pytest.raises(AssertionError, match='injected target read failure'):
        verify.prove()
    assert verify.score.ASM_DIR == previous


def test_shifted_linked_address_fails_and_restores_blob_targets(monkeypatch):
    require_toolchain()
    previous = ROOT / 'asm/us/blob'
    monkeypatch.setattr(verify.score, 'ASM_DIR', previous)
    original_elf = verify.score._elf

    def shifted_linked_elf(path):
        data, sections = original_elf(path)
        if Path(path).name == 'linked.elf':
            data = bytearray(data)
            shoff = struct.unpack_from('>I', data, 0x20)[0]
            shentsize = struct.unpack_from('>H', data, 0x2e)[0]
            index = verify.score._text_index(sections)
            struct.pack_into('>I', data, shoff + index * shentsize + 12, verify.ADDRESS + 12)
        return data, sections

    monkeypatch.setattr(verify.score, '_elf', shifted_linked_elf)
    with pytest.raises(AssertionError, match='expected 0x8038ca24, got 0x8038ca30'):
        verify.prove()
    assert verify.score.ASM_DIR == previous
    # The original failure leaked ovl_b and made later seed-vector tests KeyError.
    assert 'func_8010C02C' in verify.score.targets()


@pytest.mark.parametrize('fail_blob_census', [False, True], ids=['success', 'blob_census_failure'])
def test_fresh_full_elf_gnu_native_host_replay(monkeypatch, tmp_path, fail_blob_census):
    require_toolchain()
    previous = tmp_path / 'another-image'
    monkeypatch.setattr(verify.score, 'ASM_DIR', previous)
    original_targets = verify.score.targets

    def census_targets():
        if fail_blob_census and verify.score.ASM_DIR == ROOT / 'asm/us/blob':
            raise AssertionError('injected blob census failure')
        return original_targets()

    monkeypatch.setattr(verify.score, 'targets', census_targets)
    if fail_blob_census:
        with pytest.raises(AssertionError, match='injected blob census failure'):
            verify.prove()
        assert verify.score.ASM_DIR == previous
        return
    receipt = verify.prove()
    assert verify.score.ASM_DIR == previous
    saved = json.loads((PACKET / 'verification.json').read_text())
    # The blob manifest changes with every splice; image-B inputs stay bound.
    for proof in (receipt, saved):
        proof['inputs_sha256'].pop('asm/us/blob/SHA256SUMS', None)
    assert receipt == saved
