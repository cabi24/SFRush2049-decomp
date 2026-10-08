"""Portable EB028 packet checks; compiler work is explicitly optional."""
import importlib.util,json,shutil,sys
from pathlib import Path
import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_camera_view_dispatch_20261006'
# Standalone packet development keeps the same test body without a full checkout.
if not PACKET.exists():
    PACKET=Path(__file__).resolve().parent
    ROOT=PACKET.parent/'rush-eb028-runtime'
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('eb028_packet_verify',PACKET/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

@pytest.fixture(scope='module')
def compiler_receipt():
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    return v.verify(ROOT)

def test_native_target_pin():
    w=score.targets()[v.NAME]
    assert len(w)==410 and v.sha(v.packed(w))==v.NATIVE_SHA

def test_receipt_has_complete_nonmatch_extents():
    r=json.loads((PACKET/'verification.json').read_text())
    assert r['status']=='COMPLETE-NONMATCH'
    assert r['builds']['exact_o3']['elf_function_bytes']==1636
    assert r['builds']['exact_o3']['complete_differing_positions']==285
    assert r['builds']['exact_o3']['missing_words']==1
    assert r['builds']['canonical_r4300_control']['complete_differing_positions']==156
    assert not any(k.endswith('.py') and k.startswith('test_') for k in r['packet_files'])
    assert not any(x in str(r['packet_files']) for x in ['src/blob','src/rom','SHA256SUMS','symbols.json','lock.json'])

def test_diagnostics_are_not_replay_invariants():
    r=json.loads((PACKET/'verification.json').read_text());s=json.loads(json.dumps(r))
    for build in s['builds'].values():build['canonical_diagnostic']={'newer_scorer':'different wording'}
    assert v.portable(r)==v.portable(s)
    s['builds']['exact_o3']['missing_words']=0
    assert v.portable(r)!=v.portable(s)

def test_whole_replay(compiler_receipt):
    assert v.portable(compiler_receipt)==v.portable(json.loads((PACKET/'verification.json').read_text()))

def test_all_native_instructions_and_mutants(compiler_receipt):
    b=compiler_receipt['behavior']
    assert b['native_instruction_coverage']==b['native_instructions']==410
    assert b['cases']==768 and b['compiled_executions']==1536
    assert b['model_slot_pairs_per_view']=={str(i):24 for i in range(11)}
    assert b['mutations_per_view']=={str(i):[0,1,2,3] for i in range(11)}
    assert len(b['source_mutants_rejected'])==4 and len(b['negative_controls'])==3

def test_corrupt_native_pin_fails_before_compile(monkeypatch):
    targets=dict(score.targets());targets[v.NAME]=list(targets[v.NAME]);targets[v.NAME][0]^=1
    monkeypatch.setattr(score,'targets',lambda:targets)
    with pytest.raises(AssertionError):v.verify(ROOT)

def test_misaligned_switch_target_refused():
    words=score.targets()[v.NAME];data=bytearray(score.own_data().read(v.RODATA,60));data[:4]=(v.START+1).to_bytes(4,'big')
    regions,mutation=v.fixture(1000,0,0)
    with pytest.raises(AssertionError,match='control flow'):
        v.execute(words,v.START,list(regions.items())+[(v.RODATA,data)],v.CAR,v.Hooks(mutation))

def test_transfer_in_delay_slot_refused():
    words=list(score.targets()[v.NAME]);words[45]=0x14000001 # bne zero,zero: deliberately not taken
    regions,mutation=v.fixture(1000,0,0)
    with pytest.raises(AssertionError,match='transfer in delay slot'):
        v.execute(words,v.START,list(regions.items())+[(v.RODATA,score.own_data().read(v.RODATA,60))],v.CAR,v.Hooks(mutation))

def test_truncated_native_tail_refused():
    words=score.targets()[v.NAME][:-1];regions,mutation=v.fixture(1000,255,0)
    with pytest.raises(AssertionError,match='control flow'):
        v.execute(words,v.START,list(regions.items())+[(v.RODATA,score.own_data().read(v.RODATA,60))],v.CAR,v.Hooks(mutation))
