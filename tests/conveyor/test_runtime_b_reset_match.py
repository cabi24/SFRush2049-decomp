"""Runtime B reset: source/receipt identity, strict compile, bounded behavior."""
import importlib.util
import json
import shutil
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


def test_fresh_full_elf_gnu_native_host_replay():
    missing = [name for name in ['mips-linux-gnu-ld', 'mips-linux-gnu-objcopy', 'mips-linux-gnu-nm', 'gcc'] if not shutil.which(name)]
    if missing or not (verify.score.IDO / 'cc').exists():
        pytest.skip('requires IDO and MIPS GNU/host compiler tools: ' + ', '.join(missing))
    receipt = verify.prove()
    saved = json.loads((PACKET / 'verification.json').read_text())
    assert receipt == saved
