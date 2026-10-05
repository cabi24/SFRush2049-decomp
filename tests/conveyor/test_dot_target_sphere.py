"""Matching sphere source, complete extents, aliases, and fail-closed proof."""
import importlib.util
import json
from pathlib import Path
import struct
import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_target_sphere_20261005'
NAME = 'steering_apply'


def module():
    spec = importlib.util.spec_from_file_location('sphere_verify', HERE / 'verify.py')
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def test_source_and_proof_receipt_bindings():
    v = module()
    receipt = json.loads((HERE / 'verification.json').read_text())
    assert receipt['claims'] == [NAME] and receipt['accepted_byte_gain'] == 0
    for filename, expected in receipt['proof_sources'].items():
        assert v.sha(HERE / filename) == expected
    row = receipt['targets'][NAME]
    assert v.sha(ROOT / 'cloud/matches' / (NAME+'.c')) == row['source_sha256']
    assert row['comparison']['differing'] == 0 and row['function_bytes'] == 260
    assert row['GNU_complete_body_differences'] == 0 and row['owned_data_bytes'] == 0
    assert row['remaining_section_bytes'] == 12 and row['remaining_section_all_zero']
    assert row['behavior']['executed_instruction_offsets'] == 65
    assert len(row['behavior']['mutants']) == 5
    assert all(x['rejected'] for x in row['behavior']['mutants'].values())
    assert receipt['native_selector_proof']['cases'] == 65536
    assert receipt['native_references']['aligned_pointer_data_sites'] == ['0x80117518']


def test_native_and_unchanged_host_source_contract(tmp_path):
    v = module()
    words = v.score.targets()[NAME]
    result = v.behavior(NAME, words, words, tmp_path, random_count=16, mutants=False)
    assert result['host_C89_UBSan'] == 'passed'
    assert result['executed_instruction_offsets'] == 65
    assert all(x == [False, True] for x in result['conditional_branch_outcomes'].values())


def build(tmp_path):
    v = module()
    if not (v.score.IDO / 'cc').is_file():
        pytest.skip('Pinned IDO compiler unavailable')
    (tmp_path / 'candidate.c').write_bytes((ROOT / 'cloud/matches' / (NAME+'.c')).read_bytes())
    obj = tmp_path / 'candidate.o'
    v.score.compile_single(tmp_path / 'candidate.c', v.FLAGS, obj)
    return v, obj


def test_complete_stock_IDO_and_GNU_body(tmp_path):
    v, obj = build(tmp_path)
    proof, words = v.full_body(obj, NAME, tmp_path)
    assert len(words) == 65 and proof['GNU_complete_body_differences'] == 0


@pytest.mark.parametrize('size', [256, 264])
def test_incomplete_or_alignment_inflated_ELF_extent_fails(tmp_path, size):
    v, obj = build(tmp_path)
    data, sections, syms = v.symbols(obj)
    patched = bytearray(data)
    for section in sections:
        if section['type'] != 2:
            continue
        strings = sections[section['link']]
        names = data[strings['off']:strings['off']+strings['size']]
        for offset in range(section['off'], section['off']+section['size'], 16):
            name_offset = struct.unpack_from('>I',data,offset)[0]
            if names[name_offset:].split(b'\0',1)[0] == NAME.encode():
                struct.pack_into('>I',patched,offset+8,size)
    obj.write_bytes(patched)
    with pytest.raises(AssertionError):
        v.full_body(obj, NAME, tmp_path)


def test_wrong_literal_instruction_fails_complete_proof(tmp_path):
    v, obj = build(tmp_path)
    data, sections, ignored = v.symbols(obj)
    text = next(s for s in sections if s['name'] == '.text')
    patched = bytearray(data)
    offset = text['off']+0x74
    word = struct.unpack_from('>I',patched,offset)[0]
    struct.pack_into('>I',patched,offset,word^1)
    obj.write_bytes(patched)
    with pytest.raises(AssertionError):
        v.full_body(obj, NAME, tmp_path)


def test_machine_rejects_invalid_words_and_memory():
    v = module()
    with pytest.raises(AssertionError):
        v.native.execute([0xffffffff], 0x1000, [], [0,0,0,0])
    with pytest.raises(AssertionError):
        v.native.execute(v.score.targets()[NAME],0x8010c6c8,[],[0,0,0,0])


def test_branch_likely_annuls_invalid_delay_slot():
    module().native.execute([0x54000001,0xffffffff,0x03e00008,0],0x1000,[],[0,0,0,0])


def test_redirected_stack_store_fails():
    v = module()
    words = list(v.score.targets()[NAME])
    words[0x44//4] = (words[0x44//4] & ~0xffff) | 0x30
    case = (0,1,[0,0,0,0,0,0,0,0],-1)
    with pytest.raises(AssertionError):
        v.native_case(words,0x8010c6c8,case)


def test_independent_oracle_sphere_boundary_nan_and_alias():
    v = module()
    bits = v.native.to_bits
    data = [0,0,0,bits(1.5),bits(2),bits(6),bits(3),bits(99)]
    result, output = v.oracle(data,1,6)
    assert result == 1 and output[6] == 0 and output[7] == bits(99)
    data[4] = bits(2.000001)
    assert v.oracle(data,1,-1)[0] == 0
    assert v.oracle(data,0,7) == (0,data)
    data[6] = 0x7fc12345
    result, output = v.oracle(data,1,7)
    assert result == 0 and (output[7]&0x7fffffff) > 0x7f800000


@pytest.mark.parametrize('index', [-32768,-1,0,1,32767])
def test_signed_selector_addressing_with_accessible_native_records(index):
    v = module()
    values = [0]*7+[v.native.to_bits(99)]
    case = (index,128,values,7)
    result, memory, seen, branches = v.native_case(v.score.targets()[NAME],0x8010c6c8,case)
    expected, output = v.oracle(values,128,7)
    assert result == expected and memory == output


def test_match_is_registered_for_existing_CI():
    from tools.cloud.check_submissions import commands
    jobs = list(commands(ROOT,['cloud/matches/steering_apply.c']))
    assert len(jobs) == 1 and jobs[0][4] == NAME
    assert jobs[0][5] == '--flags=-g0 -O3 -mips2 -G 0 -non_shared'
