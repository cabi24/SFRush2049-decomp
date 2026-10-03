"""Portable regressions for filtered-vertex bounds research; no match claims."""
import importlib.util
from pathlib import Path
import shutil
import subprocess
import sys
import pytest
ROOT=Path(__file__).resolve().parents[2]
WORK=ROOT/'cloud/work/dot_vertex_bounds'

def module():
    spec=importlib.util.spec_from_file_location('vertex_bounds_proof',WORK/'verify.py')
    result=importlib.util.module_from_spec(spec);spec.loader.exec_module(result);return result

def test_host_selection_and_bounds(tmp_path):
    cc=shutil.which('cc')
    if not cc:pytest.skip('host C compiler unavailable')
    binary=tmp_path/'bounds'
    subprocess.run([cc,'-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2',str(WORK/'host_test.c'),'-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)

def test_oracle_empty_and_sentinel():
    oracle=module().oracle
    assert oracle([],0,[])==[[32767]*3,[-32767]*3]
    assert oracle([[-32768]*3],1,[])==[[-32768]*3,[-32767]*3]

def test_ranges_half_open_and_kind():
    assert module().oracle([[9]*3,[5]*3,[-4]*3,[99]*3],0,[(1,2,1),(0,4,2)])==[[-4]*3,[5]*3]

def test_mips_unknown_instruction_rejected():
    m=module();symbols={n:0x1000+i*0x100 for i,n in enumerate(m.NAMES)};symbols[m.NAME]=0x80000000
    with pytest.raises(AssertionError):m.execute([0xffffffff],symbols,[],0,[],1)

def test_native_linked_oracle_replay():
    import os
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
    if not (ido/'cc').exists() or not all(shutil.which(n) for n in ['mips-linux-gnu-ld','mips-linux-gnu-objcopy','mips-linux-gnu-nm']):
        pytest.skip('pinned IDO and GNU MIPS binutils unavailable')
    module().run()
