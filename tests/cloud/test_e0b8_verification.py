"""Fail-closed native, semantic and exact-extent checks for E0B8 research."""
import importlib.util
import json
import math
import os
from pathlib import Path
import random
import shutil
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/e0b8_verification'
spec = importlib.util.spec_from_file_location('e0b8_replay', PACKET / 'replay.py')
replay = importlib.util.module_from_spec(spec)
spec.loader.exec_module(replay)
M = replay.machine


def test_exact_extent_rejects_padding_or_prefix_even_if_words_equal():
    target = [0] * 35
    assert replay.exact_match(140, target, target)
    for size in (128, 136, 144, 160):
        assert not replay.exact_match(size, target, target)
    assert not replay.exact_match(140, target + [0], target)
    assert not replay.exact_match(140, target[:-1], target)
    wrong = target.copy()
    wrong[17] = 1
    assert not replay.exact_match(140, wrong, target)


def test_native_extent_and_parser_integrity():
    target, manifest = replay.independent_target()
    assert len(target) == 35
    assert 'symbols.json' in manifest


def test_unknown_instruction_and_truncated_body_fail_closed():
    table = replay.score.image_symbols()
    args = (table[replay.NAME], 0x10000, table['D_8012394C'], [M.bits(3), M.bits(4), 0], M.bits(1e-5))
    with pytest.raises(AssertionError, match='unsupported'):
        M.execute([0xffffffff], *args)
    target, _ = replay.independent_target()
    with pytest.raises(AssertionError, match='escaped control flow'):
        M.execute(target[:-1], *args)


def native(xyz, threshold):
    target, _ = replay.independent_target()
    table = replay.score.image_symbols()
    return M.execute(target, table[replay.NAME], 0x10000, table['D_8012394C'], xyz, threshold)


def test_equality_is_early_return_and_preserves_signed_zero():
    xyz = [M.bits(3), M.bits(4), 0x80000000]
    result, out, events = native(xyz, M.bits(5))
    assert result == 0 and out == xyz
    assert not [event for event in events if event[0] == 'write']


def test_actual_epsilon_boundary_and_zero_underflow_preserve_inputs():
    epsilon = M.bits(1e-5)
    for component in (epsilon - 1, epsilon, 1, 0x80000001):
        xyz = [component, 0x80000000, 0]
        result, out, events = native(xyz, epsilon)
        assert result == 0 and out == xyz
        assert not any(kind == 'write' for kind, address in events)
    result, out, events = native([epsilon + 1, 0x80000000, 0], epsilon)
    assert result == epsilon + 1 and out == [M.bits(1), 0x80000000, 0]
    assert len([kind for kind, address in events if kind == 'write']) == 3


def test_unordered_comparison_takes_normalization():
    xyz = [0x7fc12345, M.bits(2), M.bits(-3)]
    result, out, events = native(xyz, M.bits(1e-5))
    assert all(math.isnan(M.value(word)) for word in [result] + out)
    assert len([event for event in events if event[0] == 'write']) == 3


def test_threshold_less_than_negative_control_is_detected():
    target, _ = replay.independent_target()
    # Mutate the one scalar <= comparison to < without embedding target words.
    comparisons = [i for i, word in enumerate(target)
                   if word >> 26 == 17 and ((word >> 21) & 31) == 16 and word & 63 == 0x3e]
    assert len(comparisons) == 1
    altered = list(target)
    altered[comparisons[0]] = (altered[comparisons[0]] & ~63) | 0x3c
    table = replay.score.image_symbols()
    xyz = [M.bits(3), M.bits(4), 0]
    result, out, _ = M.execute(altered, table[replay.NAME], 0x10000, table['D_8012394C'], xyz, M.bits(5))
    assert [result] + out != replay.oracle(xyz, M.bits(5))[0]


def test_semantic_negative_controls_have_finite_counterexamples():
    # Neither reassociation nor direct component division is interchangeable
    # with the native operation graph under binary32 round-to-nearest.
    rng = random.Random(0x8008e0b8)
    found_division = found_association = False
    for _ in range(1000):
        xyz = [M.bits(rng.uniform(-100, 100)) for _ in range(3)]
        expected, normalized = replay.oracle(xyz, M.bits(1e-5))
        assert normalized
        x, y, z = map(M.value, xyz)
        alternate_length = M.bits(math.sqrt(M.value(M.bits(M.value(M.bits(x * x)) +
            M.value(M.bits(M.value(M.bits(y * y)) + M.value(M.bits(z * z))))))))
        direct = [M.bits(value / M.value(expected[0])) for value in (x, y, z)]
        found_association |= alternate_length != expected[0]
        found_division |= direct != expected[1:]
    assert found_association and found_division


@pytest.mark.parametrize('before,after', [
    ('temp_f2 <= D_8012394C', 'temp_f2 < D_8012394C'),
    ('return temp_f2;', 'return temp_f14;'),
    ('return 0.0f;', 'arg0[0] = arg0[1] = arg0[2] = 0.0f; return 0.0f;'),
])
def test_host_negative_controls_are_rejected(tmp_path, before, after):
    if not shutil.which('cc'):
        pytest.skip('host C compiler unavailable')
    text = replay.SOURCE.read_text()
    assert text.count(before) == 1
    wrong = tmp_path / 'wrong.c'
    wrong.write_text(text.replace(before, after))
    host = replay.host_candidate(wrong, tmp_path)
    cases = [([M.bits(3), M.bits(4), 0], M.bits(5)),
             ([M.bits(3), M.bits(4), 0], M.bits(1e-5)),
             ([M.bits(1e-6), 0x80000000, 0], M.bits(1e-5))]
    assert any(host(xyz, threshold, -1) != replay.oracle(xyz, threshold)[0]
               for xyz, threshold in cases)


def test_full_four_way_replay(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    if not (ido / 'cc').is_file() or not all(shutil.which(tool) for tool in
            ['cc', 'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy', 'mips-linux-gnu-nm']):
        pytest.skip('pinned IDO and GNU MIPS tools required')
    output = replay.run([sys.executable, str(PACKET / 'replay.py'), '--source', str(replay.SOURCE), '--build', str(tmp_path)])
    proof = json.loads(output)
    assert '.text 0x8008e0b8 : SUBALIGN(4)' in (tmp_path / 'link.ld').read_text()
    assert proof['semantics']['result'] == 'PASS'
    assert proof['semantics']['cases'] == 7487
    assert proof['semantics']['categories']['threshold_alias'] == 12
    assert proof['elf_function_bytes'] == 128
    assert proof['target_bytes'] == 140
    assert not proof['accepted_exact_match']
    assert proof['project_scorer_differing_words'] == 35
    assert proof['gnu_linker_equals_project_relocator']
    assert proof['claims'] == []


def test_final_matching_group_four_way_replay(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    if not (ido / 'cc').is_file() or not all(shutil.which(tool) for tool in
            ['cc', 'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy', 'mips-linux-gnu-nm']):
        pytest.skip('pinned IDO and GNU MIPS tools required')
    output = replay.run([sys.executable, str(PACKET / 'replay.py'), '--build', str(tmp_path)])
    proof = json.loads(output)
    assert '.text 0x8008e098 : SUBALIGN(4)' in (tmp_path / 'link.ld').read_text()
    assert proof['semantics']['result'] == 'PASS'
    assert proof['semantics']['cases'] == 7487
    assert proof['elf_function_bytes'] == proof['target_bytes'] == 140
    assert proof['accepted_exact_match'] and proof['byte_equal']
    assert proof['project_scorer_differing_words'] == 0
    assert proof['gnu_linker_equals_project_relocator']
    helper = proof['context']['func_8008E098']
    assert helper['elf_function_bytes'] == helper['target_bytes'] == 32
    assert helper['byte_equal'] and helper['exact_extent']
    assert helper['project_scorer_differing_words'] == 0
    assert proof['target_sha256'] == proof['linked_body_sha256']
    assert proof['source_sha256'] == replay.sha((replay.GROUP / 'group.c').read_bytes())
    assert proof['group_manifest_sha256'] == replay.sha((replay.GROUP / 'group.json').read_bytes())


def test_published_source_retains_real_public_helper_and_only_live_locals():
    group = json.loads((replay.GROUP / 'group.json').read_text())
    assert group['claims'] == [replay.NAME]
    assert group['keep'] == group['members'] == ['func_8008E098', replay.NAME]
    source = (replay.GROUP / 'group.c').read_text()
    accepted = (ROOT / 'src/blob/groups/dot_vector_length/group.c').read_text()
    accepted_helper = accepted[accepted.index('f32 func_8008E098('):].strip()
    assert accepted_helper in source, 'real accepted helper body was changed'
    assert 'length = func_8008E098(v[0], v[1], v[2]);' in source
    assert 'f32 length, inverse_length;' in source
    assert 'M2C_ERROR' not in source and 'volatile' not in source and 'asm' not in source


@pytest.mark.parametrize('before,after', [
    ('length <= D_8012394C', 'length < D_8012394C'),
    ('return length;', 'return inverse_length;'),
    ('return 0.0f;', 'v[0] = v[1] = v[2] = 0.0f; return 0.0f;'),
])
def test_final_group_host_negative_controls_are_rejected(tmp_path, before, after):
    if not shutil.which('cc'):
        pytest.skip('host C compiler unavailable')
    text = (replay.GROUP / 'group.c').read_text()
    assert text.count(before) == 1
    wrong = tmp_path / 'wrong.c'
    wrong.write_text(text.replace(before, after))
    host = replay.host_candidate(wrong, tmp_path)
    cases = [([M.bits(3), M.bits(4), 0], M.bits(5)),
             ([M.bits(3), M.bits(4), 0], M.bits(1e-5)),
             ([M.bits(1e-6), 0x80000000, 0], M.bits(1e-5))]
    assert any(host(xyz, threshold, -1) != replay.oracle(xyz, threshold)[0]
               for xyz, threshold in cases)


def test_receipt_is_bound_to_frozen_final_source_and_group():
    proof = json.loads((PACKET / 'verification.json').read_text())
    assert proof['source_sha256'] == replay.sha((replay.GROUP / 'group.c').read_bytes())
    assert proof['group_manifest_sha256'] == replay.sha((replay.GROUP / 'group.json').read_bytes())
    assert proof['accepted_exact_match'] and proof['byte_equal'] and proof['exact_extent']
    assert proof['target_bytes'] == proof['elf_function_bytes'] == 140
    assert proof['context']['func_8008E098']['elf_function_bytes'] == 32
    assert proof['semantics']['cases'] == 7487
    assert proof['semantics']['result'] == 'PASS'


def test_subprocess_failure_exposes_command_and_both_output_streams():
    command = [sys.executable, '-c',
               "import sys; print('stdout detail'); print('stderr detail', file=sys.stderr); sys.exit(7)"]
    with pytest.raises(RuntimeError) as error:
        replay.run(command)
    message = str(error.value)
    assert 'exit 7' in message
    assert sys.executable in message
    assert 'stdout detail' in message and 'stderr detail' in message


def test_subprocess_success_returns_stdout():
    assert replay.run([sys.executable, '-c', "print('complete')"]) == 'complete\n'


@pytest.mark.parametrize('address,size', [(0x8008e0c0, 140), (0x8008e0b8, 144)])
def test_linked_extent_failure_reports_expected_and_actual_values(address, size):
    with pytest.raises(AssertionError) as error:
        replay.check_linked_extent(replay.NAME, 0x8008e0b8, 140, address, size)
    message = str(error.value)
    assert 'expected address=0x8008e0b8, size=140' in message
    assert f'got address=0x{address:08x}, size={size}' in message
