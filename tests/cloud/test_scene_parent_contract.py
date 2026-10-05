"""Full native-function, typed graph and source-bound regression checks."""
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_scene_parent_20261005'
spec = importlib.util.spec_from_file_location('scene_parent_verify', PACKET / 'verify.py')
proof = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = proof
spec.loader.exec_module(proof)


@pytest.fixture(scope='module')
def receipt(): return json.loads((PACKET / 'verification.json').read_text())


@pytest.fixture(scope='module')
def replay(tmp_path_factory):
    if not (proof.score.IDO / 'cc').exists() or not shutil.which('mips-linux-gnu-ld') or not shutil.which('cc'):
        pytest.skip('Pinned IDO, GNU MIPS linker and host compiler are required')
    return proof.verify(tmp_path_factory.mktemp('scene-parent-proof'))


def test_sources_and_inputs_are_bound(receipt):
    assert receipt['source_sha256'] == proof.sha(proof.SOURCE.read_bytes())
    for name, digest in receipt['packet_sha256'].items():
        assert proof.sha((PACKET / name).read_bytes()) == digest
    for name, digest in receipt['context']['source_sha256'].items():
        assert proof.sha((ROOT / name).read_bytes()) == digest
    assert receipt['scorer_sha256'] == proof.sha(Path(proof.score.__file__).read_bytes())
    assert receipt['accepted_byte_gain'] == 0


def test_fresh_portable_receipt_replay(replay, receipt):
    for key in ['object', 'context', 'controls', 'behavior', 'source_sha256', 'target_sha256', 'packet_sha256', 'direct_callers']:
        assert replay[key] == receipt[key]


def test_complete_extent_relocations_and_owned_data(replay):
    result = replay['object']
    assert proof.complete(result)
    assert result['symbol_bytes'] == result['native_bytes'] == 164
    assert result['gnu_linked_full_body_equal']
    assert result['own_data_bytes'] == 0 and result['zero_alignment_bytes'] == 12
    assert len(result['relocations']) == 6
    assert {r['symbol'] for r in result['relocations']} == {'D_8012E700', 'D_80156990'}


def test_natural_record_control_and_real_accessors(replay):
    assert all(proof.complete(v) for v in replay['context']['bodies'].values())
    assert len(replay['context']['bodies']) == 3
    assert replay['controls']['unnamed_record']['differing'] == 11
    assert replay['controls']['unnamed_record']['symbol_bytes'] == 164
    assert proof.complete(replay['controls']['o2'])
    assert replay['controls']['archived_m2c']['differing'] == 35
    assert replay['controls']['archived_full_width_counter']['differing'] == 34
    treatment = replay['controls']['accepted_accessor_calls']
    assert not proof.complete(treatment[proof.FN])
    assert all(proof.complete(treatment[n]) for n in proof.CONTEXT)


def test_native_linked_host_exhaustive_graph_behavior(replay):
    result = replay['behavior']
    assert result['cases'] == 9220 and result['native_executions'] == 18440
    assert result['instruction_offsets_covered'] == 41
    assert result['all_conditional_outcomes_covered'] and result['branch_sites'] == 6
    assert result['host_c89_ubsan'] == 'passed'
    assert len(result['negative_controls']) == 5
    assert all(r['rejected'] for r in result['negative_controls'].values())


def test_native_sentinel_and_narrowing_contract():
    words = proof.score.targets()[proof.FN]
    for argument in [-1, 65535, 0x1234FFFF]:
        case = proof.make_case([-1], [-1], argument)
        assert proof.oracle(case) == 0
        assert proof.native.Machine(words, case['records'], case['count'], case['argument']).run() == 0
    case = proof.make_case([1, -1], [-1, -1], 0x12340001)
    assert proof.oracle(case) == 0
    assert proof.native.Machine(words, case['records'], case['count'], case['argument']).run() == 0


def test_corrupt_decoder_and_write_fail_closed():
    case = proof.make_case([-1], [-1], 1)
    words = list(proof.score.targets()[proof.FN])
    words[0] = 0xFFFFFFFF
    with pytest.raises(AssertionError, match='opcode'):
        proof.native.Machine(words, case['records'], 1, 1).run()
    words = list(proof.score.targets()[proof.FN])
    words[2] |= 4  # Redirect the real argument-home store beyond its allowed location.
    with pytest.raises(AssertionError, match='write outside argument home'):
        proof.native.Machine(words, case['records'], 1, 1).run()


def test_short_elf_extent_is_never_accepted(replay):
    result = dict(replay['object'])
    result['symbol_bytes'] -= 4
    assert not proof.complete(result)
    result = dict(replay['object'])
    result['unverified'] = ['unknown reference']
    assert not proof.complete(result)


def test_actual_caller_is_identified_without_closure_claim(receipt):
    assert receipt['direct_callers'] == [['transmission_shift', '0x800abc34'], ['transmission_shift', '0x800abc98']]
    assert any('transmission_shift execution' in limit for limit in receipt['limits'])
