"""Finite D9058 caller reconstruction and fail-closed nonmatch evidence."""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/dot_selector_d9058_20261006'
spec = importlib.util.spec_from_file_location('selector_packet', PACKET / 'verify.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)


def receipt():
    return json.loads((PACKET / 'verification.json').read_text())


def test_frozen_negative_and_full_extents():
    r = receipt()
    assert r['status'] == 'NONMATCH' and r['claims'] == []
    assert r['new_verified_matching_bytes'] == 0 and not r['parent_reconstructed']
    assert r['native_bytes'] == 328
    assert r['compile']['standalone']['functions'][v.FN]['symbol_bytes'] == 336
    assert r['compile']['clean_context']['functions'][v.FN]['symbol_bytes'] == 364
    assert r['behavior']['selector_argument_registers'] == {
        'native': 's2', 'standalone': 'a0', 'clean_context': 's1'}
    for result in r['compile'].values():
        assert result['owned_data_bytes'] == 0
        assert result['gnu_unmodified_object_text_address'] == '0x80000000'
        for body in result['functions'].values():
            assert body['gnu_equals_project_relocation']
            assert not any(body['canonical'][key] for key in ['errors', 'unresolved', 'unverified'])


def test_source_hashes_and_no_return_keeper():
    r = receipt()
    for name, expected in r['source_hashes'].items():
        assert v.sha((PACKET / name).read_bytes()) == expected
    source = v.SOURCE.read_text()
    assert '    slot_state_setup(11);' in source
    assert 'volatile' not in source and 'asm(' not in source
    context = (PACKET / 'context.c').read_text()
    assert 'void func_80096288(s32 a,s32 b,s32 c) {}' in context
    assert 'if (0)' not in context and 'switch' not in context


def test_native_arithmetic_boundaries_and_signed_index():
    code = v.score.targets()[v.FN]
    for height in [-0x80000000, -0x80000000 + 23, -1, 0, 23, 24, 231, 232, 247, 248, 0x7FFFFFFF]:
        for index in [-32768, -1, 0, 6, 32767]:
            for flags in [0, 1, 2, 0x80000000, 0xFFFFFFFF]:
                v.native.Machine(code, height, index, flags, 0).run()


def test_native_rejects_wrong_selector_or_abi():
    words = list(v.score.targets()[v.FN])
    site = (0x800D909C - 0x800D9058) // 4
    assert words[site] & 0xFFFF == 11
    words[site] = (words[site] & ~0xFFFF) | 12
    with pytest.raises(AssertionError):
        v.native.Machine(words, 232, 0, 0, 0).run()
    with pytest.raises(AssertionError):
        v.native.Machine(v.score.targets()[v.FN], 232, 0, 0, 0, 4).run()


def test_native_rejects_unmapped_field():
    m = v.native.Machine(v.score.targets()[v.FN], 232, 3, 0, 0)
    del m.mem[v.native.HEIGHT + 3 * 96]
    with pytest.raises(AssertionError):
        m.run()


def test_halfword_narrowing_is_before_cap():
    # The quotient wraps through the signed halfword before the cap test.
    assert v.native.oracle(24 + 32768 * 16, 0, 0) == -32768
    assert v.native.oracle(24 + 32768 * 16, 2, 0) == 13
    assert v.native.oracle(24 - 17, 0, 0) == -1
    assert v.native.oracle(248, 0, 0) == 13


def test_fresh_frozen_replay():
    if not (v.score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    required = ['mips-linux-gnu-ld', 'cc']
    ready = all(shutil.which(x) for x in required) and (v.score.IDO / 'cc').exists()
    if not ready:
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1':
            pytest.fail('Pinned IDO/MIPS/host tools required')
        pytest.skip('Pinned IDO/MIPS/host tools unavailable')
    assert v.portable(v.verify()) == v.portable(receipt())


def test_portability_keeps_packet_and_native_proof_strict():
    import copy
    import json
    saved = json.loads((PACKET / 'verification.json').read_text())
    changed = copy.deepcopy(saved)
    for field in ('protected_target_manifest_sha256','context_origin_sha256'):
        changed[field] = 'unrelated integration provenance'
    assert v.portable(changed) == v.portable(saved)
    assert saved == json.loads((PACKET / 'verification.json').read_text())
    for field in ['source_hashes', 'native_sha256', 'compile', 'behavior', 'negative_elf_drills', 'start', 'end', 'real_parent']:
        mutant = copy.deepcopy(changed)
        mutant[field] = 'proof drift'
        assert v.portable(mutant) != v.portable(saved), field
