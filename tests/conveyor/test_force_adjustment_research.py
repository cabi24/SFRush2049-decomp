"""Fail-closed interpreter controls and native branch semantics for unclaimed research."""
import importlib.util
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
PATH=ROOT/'cloud/work/dot_force_adjustment/verify_semantics.py'
spec=importlib.util.spec_from_file_location('force_research',PATH)
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)

def native():
    import sys
    sys.path.insert(0,str(ROOT/'tools/cloud'))
    import score
    return score.targets()['func_800E1AA0']

def case():
    return [m.bits(2),m.bits(0),m.bits(1),m.bits(500),m.bits(.25),m.bits(99),m.bits(0),m.bits(1),m.bits(.25),m.bits(12000),8,8,1,0]

def test_native_modes_and_guards():
    words=native();x=case()
    assert m.execute(words,x)==(m.bits(300),1)
    x[12]=0
    assert m.execute(words,x)==(m.bits(450),1)
    x[13]=16
    assert m.execute(words,x)==(m.bits(500),0)
    x[13]=0;x[1]=m.bits(-1)
    assert m.execute(words,x)==(m.bits(500),0)
    x[1]=0x7fc00001
    assert m.execute(words,x)==(m.bits(450),1) # unordered is not negative

def test_native_interpreter_fails_closed():
    with pytest.raises(AssertionError):m.execute([0xffffffff],case())
    with pytest.raises(AssertionError):m.execute([0x8c880fff],case()) # unaligned lw
    with pytest.raises(AssertionError):m.execute([0xac800000],case()) # prohibited write
