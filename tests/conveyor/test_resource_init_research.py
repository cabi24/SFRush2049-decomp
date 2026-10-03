"""Regression coverage for the unclaimed resource-initializer reconstruction."""
import importlib.util
import os
from pathlib import Path
import shutil
import subprocess
import sys
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/dot_resource_init'
spec=importlib.util.spec_from_file_location('resource_init_research',HERE/'verify.py')
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)

def native():
    sys.path.insert(0,str(ROOT/'tools/cloud'))
    import score
    return score.targets()[m.NAME]

def test_native_guards_and_callback_rereads():
    x=[1,0,0,1,0,1,255,0,2,7,0xFEDCBA98]
    r=m.execute(native(),x)
    assert r[:6]==[0xffcf,0,1,0x81,10,7]
    assert r[13]==0xFEDCBA98 and r[14:]==[3,10,22,12]
    x[0]=0x10000
    assert m.execute(native(),x)[14:]==[1,0]
    x[0]=1;x[1]=1
    assert m.execute(native(),x)[14:]==[0]
    x[1]=0;x[2]=1
    r=m.execute(native(),x)
    assert r[:6]==[0x5432,0,1,1,0x4321,2] and r[14:]==[0]

def test_replay_fails_closed():
    x=[1,0,0,1,0,1,48,1,2,7,42]
    with pytest.raises(AssertionError):m.execute([0xffffffff],x)
    with pytest.raises(AssertionError):m.execute([0x8c880001],x)
    with pytest.raises(AssertionError):m.execute([0xac800020],x)

def test_host_sanitized(tmp_path):
    if not shutil.which('cc'):pytest.skip('host C compiler unavailable')
    exe=tmp_path/'host'
    subprocess.run(['cc','-std=c99','-O1','-g','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=all','-DHOST_MAIN',str(HERE/'host.c'),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)

def test_full_native_candidate_host_replay(tmp_path):
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
    if not (ido/'cc').exists() or not all(shutil.which(x) for x in ['cc','mips-linux-gnu-ld','mips-linux-gnu-objcopy']):pytest.skip('pinned IDO/GNU MIPS toolchain unavailable')
    subprocess.run([sys.executable,str(HERE/'verify.py'),str(tmp_path/'proof.json')],check=True)
