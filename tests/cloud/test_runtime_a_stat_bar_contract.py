"""Focused bounded oracle/domain controls; full verifier compiles and runs C/MIPS."""
import importlib.util
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_runtime_a_stat_bar_20261006'
spec=importlib.util.spec_from_file_location('stat_bar_semantics',HERE/'semantics.py')
s=importlib.util.module_from_spec(spec);spec.loader.exec_module(s)
def case(**kw):
    c=dict(init=1,player=0,bar=0,segment=1,count=1,mode=0,hidden=0,hide=0,width=100,height=55,bits=s.fbits(.5),hook=0,junk=0)
    c.update(kw);return tuple(c.values())
def events(trace):return [trace[i:i+14] for i in range(0,len(trace),14)]
def test_signed_division():
    assert [(x,s.div8(x)) for x in (-9,-8,-7,0,7,8,9)]==[(-9,-1),(-8,-1),(-7,0),(0,0),(7,0),(8,1),(9,1)]
def test_visible_fraction_and_minimum():
    assert events(s.oracle(case()))[-1][9]==49
    assert events(s.oracle(case(width=1,bits=s.fbits(1))))[-1][9]==2
    assert events(s.oracle(case(width=1,bits=s.fbits(32768))))[-1][9]==32767
def test_hidden_return_and_disable():
    r=events(s.oracle(case(hidden=1)));assert [x[0] for x in r]==[2,3,4]
    r=events(s.oracle(case(init=0,player=15)));assert r[-1][11]==0
    assert [x[0] for x in r]==[2,3,4]
def test_rename_then_reload_real_captured_fields():
    r=events(s.oracle(case(init=0,count=3,bar=3,segment=0,hook=1)))
    assert r[0][0]==1 and r[-1][2:4]==[10,12]
    assert r[-1][6:10]==[-42,-49,0,122]
    assert r[-1][12]==s.signed(0x92345678)
def test_native_only_wide_conversion():
    r=events(s.oracle(case(width=1,bits=s.fbits(-32768))))
    assert r[-1][9]==32767
    assert not any(c[10]==s.fbits(-32768) for c in s.cases())
def test_domain_and_count():
    allcases=list(s.cases());assert len(allcases)==5105
    for c in allcases:
        init,player,bar,seg,count,mode,hidden,hide,width,height,bits,hook,junk=c
        if init:assert player<4 and (bar<4 or seg!=1)
        assert 0<=player<=15 and 0<=bar<=15 and 0<=seg<=15
