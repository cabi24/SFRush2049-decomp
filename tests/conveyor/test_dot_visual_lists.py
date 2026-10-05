"""Source-bound strict evidence and executable native traversal regressions."""
import importlib.util
import json
from pathlib import Path
import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_visual_lists_20261005'
spec = importlib.util.spec_from_file_location('visual_lists_verify', HERE / 'verify.py')
proof = importlib.util.module_from_spec(spec)
spec.loader.exec_module(proof)


def test_source_and_native_bindings():
    result = proof.binding()
    assert result['status'] == 'MATCH' and result['accepted_byte_gain'] == 0
    assert result['target']['bytes'] == 140
    for field in ['comparison', 'o2_comparison']:
        row = result[field]
        assert row['differing'] == row['extra_words'] == 0
        assert row['elf_function_bytes'] == 140
        assert not row['unresolved'] and not row['unverified'] and not row['errors']


def test_failed_source_binding_is_detected(monkeypatch):
    actual = proof.sha
    monkeypatch.setattr(proof, 'sha', lambda path: '0' * 64 if Path(path) == proof.SOURCE else actual(path))
    with pytest.raises(AssertionError, match='visual_objects_update.c'): proof.binding()


def test_full_extent_relocations_alignment_and_context():
    result = proof.binding()
    assert result['gnu_link']['equal_bytes'] == 140
    assert result['gnu_link']['relocation_count'] == 5
    assert result['gnu_link']['zero_alignment_bytes'] == 4
    assert result['owned_data_sections'] == {}
    assert set(result['context']) == {proof.NAME, *proof.CONTEXT}
    for row in result['context'].values():
        assert row['differing'] == row['extra_words'] == 0
        assert not row['unresolved'] and not row['unverified'] and not row['errors']


def test_counted_loop_causal_controls():
    controls = proof.binding()['causal_controls']
    assert controls['raw_counted']['differing'] == controls['typed_counted']['differing'] == 0
    assert controls['raw_pointer_prior']['differing'] == controls['typed_pointer']['differing'] == 22
    assert all(row['elf_function_bytes'] == 140 for row in controls.values())


def test_native_list_mutations_and_every_instruction():
    words = proof.score.targets()[proof.NAME]
    covered = set()
    for seed in [0, 1, 65535, 0xAAAA, 0x5555, 0xDEADBEEF, 0x11223344]:
        for mode in range(7):
            for action in [0, 1, 0x80000000, 0xFFFFFFFF]:
                expected, expected_memory = proof.oracle(seed, mode, action)
                got, memory, visited = proof.execute(words, 0x800B55FC, seed, mode, action)
                assert got == expected and memory == expected_memory
                covered |= visited
    assert covered == set(range(0, 140, 4))


def test_full_behavior_and_wrong_contract_receipts():
    row = proof.binding()['behavior']
    assert row['cases'] == 8064 and row['native_runs'] == 16128
    assert row['all_instruction_offsets_executed'] == 35
    assert row['host_asan_ubsan'] == 'passed'
    assert set(row['mutants']) == {'omit_fourth_list', 'zero_action', 'invert_predicate', 'prefetch_next_before_operation'}
    assert all(m['rejected'] and m['differing_cases'] > 0 for m in row['mutants'].values())


def synthetic_elf(debug_path=b'/first/source.c\0', text=b'synthetic code', reloc=b'synthetic reloc', debug_flags=0):
    """Format-only ELF fixture; never contains retail or compiled instruction bytes."""
    import struct
    names = b'\0.shstrtab\0.text\0.rel.text\0.mdebug\0'
    content = [('', 0, 0, b''), ('.shstrtab', 3, 0, names),
               ('.text', 1, 6, text), ('.rel.text', 9, 0, reloc),
               ('.mdebug', 0x70000005, debug_flags, debug_path)]
    data, rows = bytearray(52), []
    for name, kind, flags, payload in content:
        offset = len(data)
        data.extend(payload)
        rows.append((names.index(name.encode() + b'\0'), kind, flags, 0,
                     offset, len(payload), 0, 0, 1, 0))
    shoff = len(data)
    for row in rows: data.extend(struct.pack('>10I', *row))
    data[:16] = b'\x7fELF\x01\x02\x01' + b'\0' * 9
    struct.pack_into('>HHIIIIIHHHHHH', data, 16, 1, 8, 1, 0, 0, shoff,
                     0x10000000, 52, 0, 0, 40, len(rows), 1)
    return data


def test_portable_fingerprint_rejects_code_reloc_and_allocated_debug():
    normal = proof.portable_elf(synthetic_elf())
    moved = proof.portable_elf(synthetic_elf(debug_path=b'/different/longer/source.c\0'))
    assert moved == normal
    assert proof.portable_elf(synthetic_elf(text=b'changed synthetic code')) != normal
    assert proof.portable_elf(synthetic_elf(reloc=b'changed synthetic relocation')) != normal
    with pytest.raises(AssertionError): proof.portable_elf(synthetic_elf(debug_flags=2))
    altered = synthetic_elf()
    altered[39] ^= 1
    assert proof.portable_elf(altered) != normal


def test_real_compiler_path_and_tamper_controls():
    row = proof.binding()['portability_controls']
    assert row['identical_source_at_two_absolute_paths']
    assert row['full_object_and_mdebug_hashes_differ']
    assert row['all_non_debug_sections_and_ABI_metadata_equal']
    assert row['each_mdebug_contains_exactly_one_source_path']
    assert set(row['semantic_mutations']) == {'.text', '.rel.text', '.symtab', '.reginfo', '.options', 'ELF_ABI_flags'}
    assert set(row['semantic_mutations'].values()) == {'rejected'}
