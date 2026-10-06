"""Domain and independent semantic controls for the D494 NONMATCH packet."""
import importlib.util
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_runtime_a_availability_boundary_20261006'
spec=importlib.util.spec_from_file_location('availability_native',HERE/'native.py')
n=importlib.util.module_from_spec(spec);spec.loader.exec_module(n)
def case(**kw):
    values=dict(item=0,player=0,unlock=1,network=1,track=7,active=1,mode=2,setting=1,selector=0,bits=7)
    values.update(kw);return tuple(values.values())
def test_permanent_disabled_items():
    assert [n.oracle(case(item=i)) for i in (12,15)]==[0,0]
    assert [n.oracle(case(item=i)) for i in (-2147483648,-1,19,2147483647)]==[1,1,1,1]
def test_nonzero_flags_are_true():
    for v in (-128,-1,1,127):assert n.oracle(case(item=13,unlock=v))==1
    assert n.oracle(case(item=13,unlock=0))==0
    for v in (-2147483648,-1,1,2147483647):assert n.oracle(case(item=16,active=v))==1
    assert n.oracle(case(item=16,active=0))==0
def test_track_signed_boundaries():
    assert [n.oracle(case(item=11,network=0,track=t)) for t in (-128,-1,0,5,6,127)]==[1,1,0,0,1,1]
def test_attribute_masks():
    for item,mask in ((7,1),(8,2),(9,4)):
        for bits in range(-128,128):assert n.oracle(case(item=item,bits=bits))==bool(bits&mask)
def test_mode_and_setting_gates():
    assert n.oracle(case(item=6,mode=2))==n.oracle(case(item=6,mode=6))==0
    assert n.oracle(case(item=10,mode=6))==1
    assert n.oracle(case(item=10,mode=2))==0
    assert n.oracle(case(item=7,mode=6,setting=0))==0
    assert n.oracle(case(item=7,mode=6,setting=-32768))==1
def test_exact_input_domain():
    samples=list(n.cases());assert len(samples)==8962
    assert {c[2] for c in samples}==set(range(-128,128))
    assert {c[3] for c in samples}==set(range(-128,128))
    assert {c[4] for c in samples}==set(range(-128,128))
    for c in samples:assert 0<=c[1]<4 and 0<=c[8]<128
def test_memory_is_fail_closed():
    for a,size in ((1,4),(0xDEADBEEF,1)):
        try:n.read({},a,size)
        except AssertionError:pass
        else:raise AssertionError('unmapped read accepted')
def test_decoder_is_fail_closed():
    try:n.run({n.ENTRY:[0xFC000000]},n.state(case()))
    except AssertionError as exc:assert 'unknown opcode' in str(exc)
    else:raise AssertionError('unknown opcode accepted')
