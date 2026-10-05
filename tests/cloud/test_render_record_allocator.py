"""Bounded contract and evidence guards for a NONMATCH renderer allocator."""
import importlib.util
import json
from pathlib import Path
import struct

import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_render_record_allocator_20261005'
spec=importlib.util.spec_from_file_location('allocator_verify',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def fixture(count=3,free=(1,)):
    pool=bytearray([0x6d]*6400)
    for i in range(200):pool[i*32+22]=1
    for i in free:pool[i*32+22]=2
    return bytes(pool),count,0,(0xffff,0x12345678,0xfedcba98,0x80000000,0xffffffff,0,0xffffffff)


def test_frozen_packet_explicitly_rejects_matching_credit():
    receipt=json.loads((HERE/'verification.json').read_text())
    assert receipt['status']=='NONMATCH' and receipt['accepted_byte_gain']==0
    assert receipt['source_sha256']==v.sha(v.SOURCE.read_bytes())
    assert receipt['object']['symbol_bytes']==receipt['object']['native_bytes']==240
    assert receipt['object']['differing']==23 and not receipt['object']['full_body_equal']
    assert len(receipt['object']['independent_gnu_differing_offsets'])==23


def test_only_state_two_is_reusable():
    for state in [0,1,3,127,128,255]:
        pool,count,high,args=fixture()
        pool=bytearray(pool);pool[22]=state
        assert v.native.reference(bytes(pool),count,high,args)[0]==1


def test_first_free_wins_without_growing_count():
    case=fixture(200,(7,9,199));result=v.native.reference(*case)
    assert result[0]==7 and result[2:]==(200,200)
    assert result[1][9*32:10*32]==case[0][9*32:10*32]


def test_full_capacity_refuses_without_writes():
    pool,count,_,args=fixture(200,())
    assert v.native.reference(pool,count,-1,args)==(-1,pool,200,-1)


def test_payload_and_crop_narrow_modulo_sixteen_bits():
    result=v.native.reference(*fixture())
    r=result[1][32:64]
    assert struct.unpack_from('>II',r)==(0xfedcba98,0x12345678)
    assert struct.unpack_from('>HHH',r,8)==(65535,0,65535)
    assert struct.unpack_from('>HH',r,28)==(65534,65535)
    assert r[23]==0x6d


def test_negative_count_native_behavior_is_documented():
    case=fixture(-3,());result=v.native.reference(*case)
    assert result[0]==0 and result[2]==-2


def test_native_replay_rejects_unknown_instruction():
    words=v.score.targets()[v.FN][:];words[0]=0xfc000000
    with pytest.raises(AssertionError,match='unknown opcode'):
        v.native.Machine(words,*fixture()).run()


def test_native_corpus_and_full_instruction_coverage():
    words=v.score.targets()[v.FN];seen=set()
    for case in v.cases():
        machine=v.native.Machine(words,*case)
        assert machine.run()==v.native.reference(*case)
        seen.update(machine.visited)
    assert seen==set(range(0,240,4))


def test_source_mutations_have_saved_counterexamples():
    receipt=json.loads((HERE/'verification.json').read_text())
    assert set(receipt['semantics']['wrong_contract_source_mutants'])==set(v.MUTANTS)
    assert all(x['rejected'] for x in receipt['semantics']['wrong_contract_source_mutants'].values())
