"""Native signed index and ABI homes for the independently matched setter."""
import importlib.util
from pathlib import Path
import sys
import pytest
ROOT=Path(__file__).resolve().parents[2]
PATH=ROOT/'cloud/work/dot_record_word_set/verify_semantics.py'
spec=importlib.util.spec_from_file_location('record_word_set',PATH)
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)

def native():
    sys.path.insert(0,str(ROOT/'tools/cloud'))
    import score
    return score.targets()['func_800A7830']

@pytest.mark.parametrize('word,index',[(0,0),(0x7fff,32767),(0x8000,-32768),(0xffffffff,-1),(0xdead0003,3)])
def test_native_index_and_homes(word,index):
    assert m.execute(native(),word,0x89abcdef,0xfedcba98)==[(m.STACK,word),(m.STACK+8,0xfedcba98),((m.BASE+index*68+52)&0xffffffff,0x89abcdef)]

def test_native_interpreter_rejects_unknown_operation():
    with pytest.raises(AssertionError):m.execute([0xffffffff],0,0,0)

def test_host_layout_and_differential():
    assert m.main()['status']=='PASS'
