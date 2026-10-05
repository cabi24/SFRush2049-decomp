"""Guard the bounded setter research without granting matching credit."""
import hashlib
import importlib.util
import json
from pathlib import Path
import re

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_audio_bus_reopen'

def load(name):
    spec = importlib.util.spec_from_file_location('audio_reopen_' + name, HERE / (name+'.py'))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module

def test_packet_stays_nonmatching_and_unclaimed():
    manifest = json.loads((HERE/'group.json').read_text())
    proof = json.loads((HERE/'verification.json').read_text())
    assert manifest['claims'] == proof['claims'] == []
    assert manifest['keep'] == ['audio_bus_mix']
    assert proof['status'] == 'NONMATCH_ABI_UNPROVED'
    assert proof['coverage_delta_functions'] == proof['coverage_delta_bytes'] == 0
    assert proof['source_sha256'] == hashlib.sha256((HERE/'group.c').read_bytes()).hexdigest()
    caller = proof['comparisons']['audio_bus_mix']
    assert (caller['differing'], caller['elf_symbol_bytes'], caller['target_bytes']) == (141, 712, 720)
    assert caller['exact_extent'] is False and caller['full_relocated_equal'] is False

def test_accepted_checksum_body_is_unchanged():
    verification = load('verify')
    source = (HERE/'group.c').read_text()
    accepted = (ROOT/'src/blob/groups/codex_hash_a80/group.c').read_text()
    assert verification.definition(source,'format_string_parse') == verification.definition(accepted,'format_string_parse')
    params = re.search(r'void voice_stop_2\(([^)]*)\)',source).group(1)
    assert params.split(',') == ['Handle *handle','u8 selector','s8 value']
    assert not re.search(r'\b(asm|__asm__|volatile)\b',source)

def test_native_page_home_has_no_callee_consumer():
    verification = load('verify')
    facts = verification.native_facts()
    voice = facts['voice_stop_2']
    assert {x['function'] for x in voice['native_callsites']} == {'audio_bus_mix'}
    assert [x['offset'] for x in voice['native_callsites']] == [164,204]
    assert {x['stack_offset'] for x in voice['stack_loads']} == {20}
    stores = facts['audio_bus_mix']['stack_stores']
    assert [x['offset'] for x in stores if x['stack_offset'] == 12] == [168,208]
    for name in ('func_800B4720','func_800B4728','func_800B4730'):
        assert facts[name]['native_callsites'] == [] and facts[name]['bytes'] == 8
    receipt = json.loads((HERE/'verification.json').read_text())
    for name in ('audio_bus_mix','voice_stop_2'):
        assert facts[name] == receipt['native'][name]

def test_full_helper_evidence_is_not_promoted_to_group_claim():
    proof = json.loads((HERE/'verification.json').read_text())
    voice = proof['comparisons']['voice_stop_2']
    assert voice['exact_extent'] and voice['elf_symbol_bytes'] == 224
    assert voice['full_relocated_equal']
    assert voice['relocated_sha256'] == proof['native']['voice_stop_2']['target_sha256']
    assert voice['own_data']['verified_reference_sites'] == 2
    assert sum(hi-lo for lo,hi,_,_ in voice['own_data']['placements']['.rodata']) == 44
    assert voice['status'] == 'INDIVIDUAL_BODY_PROOF_ONLY_NO_CLAIM'
    assert all(proof['negative_controls'].values())

def test_native_boundary_modes_follow_independent_record_map():
    replay = load('verify_semantics')
    native = replay.Native()
    initial = bytes((i*17+3)&255 for i in range(160))
    mode_a = dict(zip((21,22,23,24,26,27,28,29,30,31),range(80,90)))
    primary = dict(zip((23,28,29,30,37,38,39,40),range(100,108)))
    secondary = dict(zip((23,28,29,30,32,33,34,35,36),range(116,125)))
    fallback = dict(zip((23,28,29,30),range(136,140)))
    for mode in (-128,-1,0,5,6,13,14,17,18,19,24,25,127):
        for selector in range(256):
            got,calls=native.run(mode,selector,-128,initial)
            if selector < 21: offset,record,hash_size,total=32+selector,28,21,25
            else:
                if 0 <= mode < 6 or 19 <= mode < 25: mapping,record,hash_size,total=mode_a,72,16,20
                elif 6 <= mode < 14: mapping,record,hash_size,total=primary,92,12,16
                elif 14 <= mode < 18: mapping,record,hash_size,total=secondary,108,16,20
                else: mapping,record,hash_size,total=fallback,128,8,12
                offset=mapping.get(selector)
            expected=bytearray(initial)
            if offset is None or expected[offset] == 128:
                assert got == initial and calls == []
            else:
                expected[offset]=128
                expected[record:record+4]=replay.checksum(expected[record+4:record+4+hash_size]).to_bytes(4,'big')
                assert got == bytes(expected)
                assert calls == [(record,total,bytes(expected[record:record+total]))]

def test_semantic_receipt_covers_full_signed_mode_and_selector_space():
    receipt=json.loads((HERE/'semantics.json').read_text())
    assert receipt['status']=='PASS' and receipt['native_vs_host_cases']==204562
    assert receipt['change_then_same_value_sequences']==6524
    assert receipt['source_sha256']==hashlib.sha256((HERE/'group.c').read_bytes()).hexdigest()
    assert receipt['persisted_record_shapes']==[[0,-1,0],[1,28,25],[1,72,20],[1,92,16],[1,108,20],[1,128,12]]


@pytest.mark.skipif(not (load('verify').score.IDO / 'cc').is_file(),
                    reason='requires pinned local IDO and MIPS binutils')
def test_fresh_compile_preserves_extent_own_data_and_no_credit():
    verification = load('verify')
    fresh = verification.verify()
    saved = json.loads((HERE/'verification.json').read_text())
    assert fresh['claims'] == [] and fresh['status'] == 'NONMATCH_ABI_UNPROVED'
    assert fresh['source_sha256'] == saved['source_sha256']
    assert json.loads(json.dumps(fresh['comparisons'])) == saved['comparisons']
    assert fresh['negative_controls'] == saved['negative_controls']
