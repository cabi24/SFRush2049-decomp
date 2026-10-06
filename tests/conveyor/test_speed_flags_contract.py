"""Self-contained source/native checks; compiler replay is explicit in the packet."""
import importlib.util
from pathlib import Path
import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/ipa-groups/dot_speed_flags_c9210_20261006'
spec=importlib.util.spec_from_file_location('speed_flags_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

def test_source_and_target_binding():
    r=v.binding()
    assert r['target']['bytes']==204
    assert r['current_comparisons']['speed_set']['complete_extent_differing_positions']==0
    assert r['current_comparisons']['vsync_wait']['complete_extent_differing_positions']==0
    assert r['original_s8_control']['speed_set']['elf_extent_bytes']==228
    assert r['original_s8_control']['speed_set']['complete_extent_differing_positions']==53

def test_only_the_two_flag_formals_change():
    old=v.git(v.BASE_SOURCE)
    assert old.replace(b's8 first,s8 second',b's32 first,s32 second')==(HERE/'group.c').read_bytes()

def test_relocation_extent_and_no_owned_data():
    r=v.binding()
    assert r['target_relocations']==8 and r['gnu_total_relocations']==20
    assert r['owned_data_sections']=={} and r['zero_alignment_bytes']==4
    for name in ['speed_mode0_wrapper','speed_mode1_wrapper','continue_prompt']:
        assert r['current_comparisons'][name]['differing']>0

def test_full_word_flags_have_only_low_byte_effects():
    words=v.score.targets()['speed_set']
    for low in range(256):
        c=(0x3F000000,0x40000000,0xFFFFFF00|low,0x80000000|low,0)
        actual,_,_=v.native.execute(words,c)
        assert actual==v.native.oracle(c)
        assert actual[12]==actual[13]==low

def test_clamps_zero_and_mutation_trace():
    words=v.score.targets()['speed_set']
    for blend in [0,0x80000000,0xBF800000,0x3F800000,0x40000000]:
        for amount in [0,0x80000000,0xBF800000,0x40000000]:
            for mutation in range(3):
                c=(blend,amount,1,0xFFFFFFFF,mutation)
                assert v.native.execute(words,c)[0]==v.native.oracle(c)

def test_truncation_is_rejected():
    with pytest.raises(AssertionError):v.native.execute(v.score.targets()['speed_set'][:-1],(0,0,0,0,0))

def test_unknown_instruction_is_rejected():
    words=v.score.targets()['speed_set'][:];words[0]=0xFFFFFFFF
    with pytest.raises(AssertionError):v.native.execute(words,(0,0,0,0,0))

def test_wrong_call_contract_is_rejected():
    words=v.score.targets()['speed_set'][:]
    words[6]=(words[6]&0xFC000000)|((0x80001234>>2)&0x03FFFFFF)
    with pytest.raises(AssertionError):v.native.execute(words,(0,0,0,0,0))


def test_address_only_identity_drift_is_rejected(monkeypatch):
    addresses=v.score.image_symbols()
    addresses['speed_set']+=4
    monkeypatch.setattr(v.score,'image_symbols',lambda:addresses)
    with pytest.raises(AssertionError):v.binding()


def test_original_recipe_and_roots_are_enforced():
    import json
    cfg=json.loads((HERE/'group.json').read_text())
    v.check_recipe(cfg)
    cfg['keep']=cfg['keep']+['speed_set']
    with pytest.raises(AssertionError):v.check_recipe(cfg)


def test_all_direct_callers_are_accounted_for():
    census=v.caller_audit()
    assert census['direct_jal_sites']==21 and census['distinct_callers']==13
    assert census['all_witnessed_flag_values']==[0,1]
