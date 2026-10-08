"""Portable research tests. No target recompilation is hidden in this suite."""
import ctypes,hashlib,json,os,shutil,subprocess,sys
from pathlib import Path
import pytest
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(HERE))
from semantic import native
from native import verify_host
from contracts import audit
from controls import controls
from baseline import load_score,elf_image,compare_behavior
from elf import procedures,validate_link
REFERENCE=Path(os.environ.get('SFRUSH_REFERENCE_ROOT',str(HERE.parents[2]))).resolve()
SCORE_ROOT=Path(os.environ.get('SFRUSH_SCORE_ROOT',str(HERE.parents[2]))).resolve()

def test_packet_binding():
    receipt=json.loads((HERE/'packet.json').read_text())
    assert 'test_packet.py' not in receipt['source_sha256']
    for name,digest in receipt['source_sha256'].items():assert hashlib.sha256((HERE/name).read_bytes()).hexdigest()==digest,name
    assert (HERE/'effect_tick.c').read_text().splitlines()[0]=='/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
    result=json.loads((HERE/'baseline.json').read_text())
    assert result['group_compiler_invocations']==1
    assert result['procedures']['func_80390B10']['size']==8
    assert result['procedures']['func_80390D38']['size']==548
    assert result['procedures']['func_80390F60']['size']==1560
    assert result['canonical_root_comparison']['differing']==245

def test_native_contracts():
    assert audit(REFERENCE)==json.loads((HERE/'contracts.json').read_text())

def test_native_host(tmp_path):
    if not shutil.which('cc'):pytest.skip('host C compiler required')
    so=tmp_path/'host.so'
    subprocess.run(['cc','-std=c89','-Wall','-Wextra','-Werror','-O2','-fPIC','-shared','-ffp-contract=off',str(HERE/'host.c'),'-o',str(so)],check=True)
    code,data=native(REFERENCE)
    assert verify_host(code,data,ctypes.CDLL(str(so)))==json.loads((HERE/'host.json').read_text())

def test_host_mutants(tmp_path):
    if not shutil.which('cc'):pytest.skip('host C compiler required')
    assert controls(REFERENCE,tmp_path)==json.loads((HERE/'controls.json').read_text())['rejected']

def test_existing_canonical_object():
    score=load_score(SCORE_ROOT)
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    folder=os.environ.get('SFRUSH_EFFECT_TICK_ARTIFACTS')
    if not folder:pytest.skip('supply existing one-shot artifacts; test suite never recompiles the target')
    folder=Path(folder);obj,linked=folder/'effect.o',folder/'effect.elf';receipt=json.loads((HERE/'baseline.json').read_text())
    assert hashlib.sha256(obj.read_bytes()).hexdigest()==receipt['object_sha256']
    assert hashlib.sha256(linked.read_bytes()).hexdigest()==receipt['linked_sha256']
    procs,accounting=procedures(score,obj)
    assert accounting==receipt['text_accounting']
    assert {n:p['size'] for n,p in procs.items()}=={n:p['size'] for n,p in receipt['procedures'].items()}
    assert validate_link(score,obj,linked)==receipt['relocation_validation']
    nc,nd=native(REFERENCE);cc,cd,syms=elf_image(score,linked)
    assert compare_behavior(nc,nd,cc,cd,syms['func_80390F60']['value'])==receipt['behavior']
