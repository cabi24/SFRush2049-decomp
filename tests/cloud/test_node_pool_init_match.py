"""Complete pool initialization proof, genuine consumer, and fail-closed controls."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import struct
import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_node_pool_init_20261005'
spec = importlib.util.spec_from_file_location('node_pool_proof', HERE / 'verify.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def saved(): return json.loads((HERE / 'verification.json').read_text())


def toolchain():
    available = (packet.score.IDO / 'cc').is_file() and all(shutil.which(n) for n in ['cc', 'mips-linux-gnu-ld'])
    if not available:
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1': pytest.fail('required IDO/GNU/host compiler unavailable')
        pytest.skip('requires IDO/GNU/host compiler')


def test_source_bound_receipt_and_registered_single():
    receipt = saved()
    assert receipt['status'] == 'MATCH' and receipt['claims'] == [packet.FN]
    assert receipt['accepted_byte_gain'] == 0 and receipt['candidate_bytes'] == 216
    for path, expected in receipt['source_sha256'].items():
        assert hashlib.sha256((ROOT / path).read_bytes()).hexdigest() == expected
    from tools.cloud import check_submissions
    jobs = list(check_submissions.commands(ROOT, ['cloud/matches/func_800B2BDC.c']))
    assert len(jobs) == 1 and jobs[0][4] == packet.FN


def test_full_body_relocations_and_zero_alignment():
    receipt = saved()
    for name in ['candidate', 'o2']:
        result = receipt[name]
        assert result['complete_gnu_equal'] and result['complete_differing_offsets'] == []
        assert result['elf_function_bytes'] == result['native_bytes'] == 216
        assert result['standalone_trailing_text_bytes'] == 8 and result['standalone_tail_all_zero']
        assert len(result['relocations']) == 33
        assert result['canonical']['differing'] == 0
        assert all(not result['canonical'][key] for key in ['unresolved', 'unverified', 'errors'])


def test_compatible_actual_consumer_and_owned_pool():
    receipt = saved()
    assert receipt['abi_assertions'] == 9
    assert receipt['active_head_contract']['role'] == 'active Node list head'
    assert receipt['active_head_contract']['full_constructor_replayed'] is False
    assert receipt['bss']['bytes'] == 2400
    assert receipt['bss']['address'] == '0x80138880' and receipt['bss']['end_exclusive'] == '0x801391e0'
    for name, length in [(packet.FN, 216), (packet.POP, 132)]:
        proof = receipt['compatible_context'][name]
        assert proof['complete_gnu_equal'] and proof['elf_function_bytes'] == length
    lock = json.loads((ROOT / 'blob_matched.lock.json').read_text())[packet.POP]
    assert lock['source_sha256'] == packet.sha(packet.ACCEPTED)
    assert receipt['native']['direct_callers'] == [{'name': 'func_800BB9B0', 'address': '0x800bc104'}]


def test_native_host_full_state_and_every_instruction():
    sem = saved()['semantics']
    assert sem['cases'] == 320 and sem['native_and_gnu_invocations'] == 30528
    assert sem['init_instruction_offsets'] == list(range(0, 216, 4))
    assert sem['pop_instruction_offsets'] == list(range(0, 132, 4))
    assert sem['init_branch_outcomes'] == [[180, False], [180, True]]
    for key in ['full_memory_canaries', 'native_gnu_access_traces', 'saved_registers_and_stack',
                'unchanged_host_c89_ubsan', 'host_full_object_bytes_preserved', 'repeated_init']:
        assert sem[key] == 'passed'


def test_actual_source_ablation_and_wrong_contracts():
    controls = saved()['source_controls']
    assert controls['archived_pointer']['proof']['elf_function_bytes'] == 80
    assert len(controls['archived_pointer']['proof']['complete_differing_offsets']) == 53
    assert controls['extern_array']['proof']['elf_function_bytes'] == 228
    assert len(controls['extern_array']['proof']['complete_differing_offsets']) == 51
    assert len(controls['id_before_next']['proof']['complete_differing_offsets']) == 14
    assert controls['two_stores_same_line']['proof']['complete_gnu_equal']
    assert len(saved()['wrong_contracts']) == 4
    assert all(m['behavioral_difference_bytes'] and m['complete_differing_words'] for m in saved()['wrong_contracts'].values())
    assert saved()['adverse_controls'] == {'bad_store_address': 'rejected', 'unknown_opcode': 'rejected'}


def test_initializer_has_no_hidden_reads_or_stack_activity():
    mem = packet.native.case(7, -1)
    before = mem.copy()
    packet.native.oracle_init(before)
    result = packet.native.execute(packet.score.targets()[packet.FN], packet.native.INIT, mem)
    assert mem == before and result[3] == [] and len(result[2]) == 203
    with pytest.raises(AssertionError, match='unsupported instruction'):
        packet.native.execute([0xffffffff], packet.native.INIT, packet.native.case(1, 0))


def test_complete_inspector_rejects_object_corruption(tmp_path):
    toolchain()
    obj = tmp_path / 'original.o'
    packet.score.compile_single(packet.SOURCE, packet.FLAGS, obj)
    data, sections, syms = packet.elf(obj)
    fn, = [s for s in syms if s['name'] == packet.FN and s['type'] == 2]
    text = sections[fn['section']]
    variants = {}
    altered = bytearray(data); altered[text['off'] + fn['value'] + 3] ^= 1
    variants['instruction'] = altered
    altered = bytearray(data)
    for index, sec in enumerate(sections):
        if sec['type'] != 2: continue
        table = packet.score._symbol_table(data, sections, index)
        for row, sym in enumerate(table):
            if sym['name'] == packet.FN:
                struct.pack_into('>I', altered, sec['off'] + row * 16 + 8, 212)
    variants['extent'] = altered
    altered = bytearray(data)
    rel, = [s for s in sections if s['type'] == 9 and s['info'] == fn['section']]
    table = packet.score._symbol_table(data, sections, rel['link'])
    wrong = next(i for i, s in enumerate(table) if s['name'] == 'D_801391F0')
    for offset in [rel['off'], rel['off'] + 8]:
        info, = struct.unpack_from('>I', altered, offset + 4)
        struct.pack_into('>I', altered, offset + 4, (wrong << 8) | (info & 255))
    variants['relocation'] = altered
    for label, content in variants.items():
        path = tmp_path / (label + '.o'); path.write_bytes(content)
        with pytest.raises(AssertionError): packet.inspect(path, packet.FN, tmp_path)


def test_fresh_frozen_receipt_replay(tmp_path):
    toolchain()
    fresh, recorded = packet.verify(tmp_path), saved()
    for value in [fresh, recorded]: value.pop('target_manifest_sha256')
    assert fresh == recorded
