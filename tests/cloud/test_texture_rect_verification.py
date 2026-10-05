"""Small fail-closed and source-contract controls for the 80087110 verifier."""
import importlib.util
import json
import os
import shutil
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location(
    'rectangle_verification', ROOT / 'cloud/work/texture_rect_verification/replay.py')
REPLAY = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(REPLAY)


def test_exact_match_rejects_prefix_and_zero_suffix():
    target = [1] * (REPLAY.SIZE // 4)
    assert REPLAY.exact_match(REPLAY.SIZE, target, target)
    assert not REPLAY.exact_match(REPLAY.SIZE - 4, target[:-1], target)
    assert not REPLAY.exact_match(REPLAY.SIZE + 4, target + [0], target)
    assert not REPLAY.exact_match(REPLAY.SIZE, target + [0], target)
    assert not REPLAY.exact_match(REPLAY.SIZE, [0] + target[1:], target)


def test_linked_placement_is_exact_not_aligned_up():
    REPLAY.check_linked_placement(0x80087110, 1780, 0x80087110, 1780)
    with pytest.raises(AssertionError, match='placement/extent'):
        REPLAY.check_linked_placement(0x80087114, 1780, 0x80087110, 1780)
    with pytest.raises(AssertionError, match='placement/extent'):
        REPLAY.check_linked_placement(0x80087110, 1792, 0x80087110, 1780)


def test_zero_area_emits_but_inverted_area_rejects():
    state = [0, 0, 319, 0, 239, 0]
    assert len(REPLAY.oracle([2, 3, 2, 3, 4, 5], state)) == 6
    assert REPLAY.oracle([3, 3, 2, 3, 4, 5], state) == []
    assert REPLAY.oracle([2, 4, 2, 3, 4, 5], state) == []


def test_stretch_half_texel_applies_only_to_vertical_flip():
    args = [10, 20, 30, 40, 1, 2]
    for flip in [0, 4, 8, 12]:
        packet = REPLAY.oracle(args, [0x8000 | flip, 0, 319, 0, 239, 1])
        assert packet[-1] & 0xffff == ((-512 if flip & 8 else 512) & 0xffff)
        assert packet[3] & 31 == (16 if flip & 8 else 0)


def test_word_y_is_not_silently_narrowed_to_short():
    bounded = [0, 0, 319, 0, 239, 1]
    assert REPLAY.oracle([10, 65536, 20, 65537, 0, 0], bounded) == []
    assert REPLAY.oracle([10, 0, 20, 65537, 0, 0], bounded)


def test_machine_rejects_missing_instruction_extent():
    symbols = {name: 0x200000 + index * 4 for index, name in enumerate(REPLAY.machine.GLOBALS + ['D_80149438'])}
    with pytest.raises(AssertionError, match='escaped control flow'):
        REPLAY.machine.execute([], 0x80087110, symbols, [0] * 6, [0] * 6)


def test_native_parser_and_unknown_or_truncated_stream_fail_closed():
    words, manifest = REPLAY.independent_target()
    assert len(words) == 445 and 'symbols.json' in manifest
    symbols = REPLAY.score.image_symbols()
    args = (symbols[REPLAY.NAME], symbols, [10, 20, 30, 40, 0, 0], [0, 0, 319, 0, 239, 0])
    with pytest.raises(AssertionError, match='unsupported'):
        REPLAY.machine.execute([0xffffffff], *args)
    with pytest.raises(AssertionError, match='escaped control flow'):
        REPLAY.machine.execute(words[:-1], *args)


def test_full_independent_replay(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    if not (ido / 'cc').is_file() or not all(shutil.which(tool) for tool in
            ['cc', 'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy', 'mips-linux-gnu-nm', 'mips-linux-gnu-readelf']):
        pytest.skip('pinned IDO and GNU MIPS binutils required')
    output = REPLAY.run([sys.executable, str(ROOT / 'cloud/work/texture_rect_verification/replay.py'), '--build', str(tmp_path)])
    proof = json.loads(output)
    assert proof['exact_extent'] and proof['elf_function_bytes'] == 1780
    assert proof['alignment_padding_bytes'] == 12 and proof['alignment_padding_all_zero']
    assert proof['project_scorer_differing_words'] == 4 and not proof['accepted_exact_match']
    assert proof['residual_offsets'] == ['0x4c8', '0x4cc', '0x4d0', '0x4d4']
    assert proof['gnu_linker_equals_project_relocator']
    assert proof['semantics']['cases'] == 6828
    assert proof['semantics']['host_C_UBSan_cases'] == 3828
    assert proof['semantics']['native_instruction_offsets_executed'] == 445
    assert proof['semantics']['native_unexecuted_offsets'] == []
    assert all(item['expected_failure_observed'] for item in proof['semantics']['sanitizer_negative_controls'].values())


def test_published_receipt_binds_the_unmodified_source():
    proof = json.loads((ROOT / 'cloud/work/texture_rect_verification/verification.json').read_text())
    assert proof['source_sha256'] == REPLAY.sha(REPLAY.DEFAULT.read_bytes())
    assert proof['flags'] == REPLAY.FLAGS
    assert proof['claims'] == [] and proof['status'] == 'RESEARCH_ONLY'
    assert not proof['accepted_exact_match']
