"""Final matching source, native alias contract and strict-proof regressions."""
import importlib.util
import json
from pathlib import Path
import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_target_overlap_20261005'

def module():
    spec = importlib.util.spec_from_file_location('overlap_verify', HERE / 'verify.py')
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result

def test_source_and_proof_receipt_bindings():
    v = module()
    receipt = json.loads((HERE / 'verification.json').read_text())
    assert receipt['claims'] == list(v.NAMES) and receipt['accepted_byte_gain'] == 0
    for filename, expected in receipt['proof_sources'].items():
        assert v.sha(HERE / filename) == expected
    for name, row in receipt['targets'].items():
        assert v.sha(ROOT / 'cloud/matches' / (name+'.c')) == row['source_sha256']
        assert row['comparison']['differing'] == 0 and row['function_bytes'] == 320
        assert row['GNU_linked_equal'] and row['owned_data_bytes'] == 0
        assert row['behavior']['executed_instruction_offsets'] == 79
        assert len(row['behavior']['mutants']) == 5
        assert all(x['rejected'] for x in row['behavior']['mutants'].values())

@pytest.mark.parametrize('name', ['func_8010C448', 'func_8010C588'])
def test_native_final_host_source_contract(name, tmp_path):
    v = module()
    words = v.score.targets()[name]
    result = v.behavior(name, words, words, tmp_path, random_count=16, mutants=False)
    assert result['host_C89_UBSan'] == 'passed'
    assert result['executed_instruction_offsets'] == 79

@pytest.mark.parametrize('name', ['func_8010C448', 'func_8010C588'])
def test_complete_stock_IDO_and_GNU_body(name, tmp_path):
    v = module()
    if not (v.score.IDO / 'cc').is_file():
        pytest.skip('Pinned IDO compiler unavailable')
    (tmp_path / 'candidate.c').write_bytes((ROOT / 'cloud/matches' / (name+'.c')).read_bytes())
    obj = tmp_path / 'candidate.o'
    v.score.compile_single(tmp_path / 'candidate.c', v.FLAGS, obj)
    proof, words = v.full_body(obj, name, tmp_path)
    assert len(words) == 80 and proof['GNU_linked_equal']

def test_machine_rejects_invalid_memory_and_words():
    v = module()
    with pytest.raises(AssertionError):
        v.native.execute([0xffffffff], 0x1000, [], [0, 0, 0, 0])
    with pytest.raises(AssertionError):
        v.native.execute(v.score.targets()['func_8010C448'], 0x8010c448, [], [0, 0, 0, 0])

def test_branch_likely_annuls_invalid_delay_slot():
    v = module()
    v.native.execute([0x54000001, 0xffffffff, 0x03e00008, 0], 0x1000, [], [0, 0, 0, 0])

def test_independent_oracle_boundaries_and_alias():
    v = module()
    bits = v.native.to_bits
    # Radius zero plus the native margin accepts exact radial equality.
    data = [0, 0, 0, bits(3.5), bits(0), 0, 0, bits(99)]
    result, output = v.oracle(data, 1, 6, 18)
    assert result == 1 and output[6] == 0 and output[7] == bits(99)
    for bound in [-2, 18]:
        data[4] = bits(bound)
        assert v.oracle(data, 1, -1, 18)[0] == 0
    assert v.oracle(data, 0, 7, 18) == (0, data)
