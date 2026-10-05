"""Source/receipt binding and independent N64 HUD bar contract regressions."""
import hashlib
import importlib.util
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_blit_family_20261005'
SOURCE=ROOT/'cloud/matches/func_800EF62C.c'


def load(name):
    spec=importlib.util.spec_from_file_location(name,HERE/(name+'.py'))
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    return module


def receipt():return json.loads((HERE/'verification.json').read_text())
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def test_source_and_packet_bindings():
    r=receipt()
    assert sha(SOURCE)==r['source_sha256']
    for name,digest in r['packet_sha256'].items():assert sha(HERE/name)==digest
    for name,digest in r['context_source_sha256'].items():assert sha(ROOT/name)==digest


def test_complete_relocation_and_gnu_proof():
    r=receipt();o=r['object']
    assert r['candidate_bytes']==o['symbol_bytes']==o['native_bytes']==712
    assert o['total']==178 and o['differing']==o['extra_words']==0
    assert o['full_extent_equal'] and o['all_relocations_resolved']
    assert not (o['unresolved'] or o['unverified'] or o['errors'])
    assert len(r['relocations'])==35 and sum(x['type']==4 for x in r['relocations'])==5
    assert r['gnu_link']['full_symbol_equal'] and r['gnu_link']['own_literals_equal']
    assert r['alignment_bytes']=={'text_zero_outside_function':8,'rodata_zero_outside_literals':8}


def test_accepted_context_is_unchanged_and_exact():
    r=receipt();assert len(r['real_context'])==6
    assert all(x['canonical_verdict']=='MATCH' and x['full_extent_equal'] and x['all_relocations_resolved']
               for x in r['real_context'].values())
    assert r['controls']['archived_a70']['differing']==9
    assert r['controls']['o2']['canonical_verdict']=='MATCH'


def test_all_native_cases_and_instruction_offsets():
    s=load('verify_semantics');native=s.Native();cases=s.cases()
    assert len(cases)==4363
    for c in cases:assert native.run(c)==s.oracle(c)
    assert native.coverage==set(range(0,712,4))


def test_host_ubsan_cases(tmp_path):
    s=load('verify_semantics');host=s.host_function(tmp_path)
    for c in s.cases():assert host(c)==s.oracle(c)


def test_semantics_and_six_discriminators():
    result=receipt()['host_native_linked']
    assert result['native'] and result['gnu_linked'] and result['host_ubsan']
    assert result['instruction_offsets_executed']==178 and result['uncovered_offsets']==[]
    assert set(result['detected_wrong_contracts'])=={'wrong_coefficient','no_clamp','wrong_slot','wrong_half','wrong_mode','missing_final_update'}


def test_owned_literals_are_source_expressions():
    source=SOURCE.read_text()
    assert '1.35f' in source and '0.0001f' in source
    assert 'D_801245A4' not in source and 'D_801245A8' not in source
    r=receipt()['own_literals']
    assert r['address']=='0x801245A4' and r['bytes']==8 and r['verified']


def test_claim_is_narrow_and_has_no_coverage_credit():
    r=receipt();claim=json.loads((HERE/'claim.json').read_text())
    assert r['accepted_byte_gain']==0
    assert [c['function'] for c in claim['scope']]==['func_800EF62C']
    assert claim['handoff']['function']=='func_80108DA8'
    assert 'N64-specific decompilation' in (HERE/'README.md').read_text()
