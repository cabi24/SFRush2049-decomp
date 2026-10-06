"""Bounded regression for the D197C constructor's accepted list contract."""
import importlib.util
import json
from pathlib import Path
import shutil
import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_constructor_prepend_20261005'
spec=importlib.util.spec_from_file_location('constructor_packet',PACKET/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def test_receipt_has_no_matching_claim():
    r=json.loads((PACKET/'verification.json').read_text())
    c=json.loads((PACKET/'claim.json').read_text())
    assert r['status']=='NONMATCH' and r['accepted_byte_gain']==0 and c['claims']==[]
    assert r['start']=='0x800d197c' and r['end']=='0x800d1ab0'
    assert all(x['functions'][v.FN]['differing_words']==24 for x in r['compile'].values())


def test_source_hash_and_real_list_members():
    r=json.loads((PACKET/'verification.json').read_text())
    assert v.sha(v.SOURCE.read_bytes())==r['source_sha256']
    source=v.SOURCE.read_text()
    assert 'u32 count, head, tail;' in source
    assert 'func_80091FBC(&D_80149860, obj, D_80149860.head);' in source
    assert 'volatile' not in source and '__standin' not in source


def test_complete_context_and_no_owned_data():
    r=json.loads((PACKET/'verification.json').read_text())
    result=r['compile']['context']
    assert result['owned_data_bytes']==0
    for n,size in [('func_800D18D8',164),('func_80091FBC',352),('func_8009211C',348)]:
        f=result['functions'][n]
        assert f['symbol_bytes']==size==f['native_bytes'] and f['differing_words']==0
        assert f['gnu_equals_project_relocation']


def test_native_bit_patterns_and_boundaries():
    code=v.score.targets()[v.FN]
    for seed in [0,1,0xffffffff,0x80000000,0x7fffffff,0xdeadbeef]:
        for count in [-0x80000000,-3,0,1,3,4]:
            v.native.Machine(code,v.native.fixture(seed,count)).run()


def test_native_unmapped_read_rejected():
    m=v.native.Machine(v.score.targets()[v.FN],v.native.fixture(1,2))
    del m.mem[v.native.STACK+20]
    with pytest.raises(AssertionError):m.run()


def test_fresh_frozen_replay():
    required=['mips-linux-gnu-ld','cc']
    if not all(shutil.which(x) for x in required):pytest.skip('Needs native/host compiler tools')
    if not Path(v.score.ido('cc')).exists():pytest.skip('Needs pinned IDO')
    assert v.verify()==json.loads((PACKET/'verification.json').read_text())
