"""Portable compiler-free identity and bounded host checks; never builds target C."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import pytest
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(HERE))
from inputs import load,BASE
from verify import compare,build_host
from host_adapter import Host

@pytest.fixture(scope='module')
def reference():
    explicit=os.environ.get('RUSH_REFERENCE_ROOT')
    if explicit:
        root=Path(explicit).resolve()
    else:
        root=next((p for p in HERE.parents if (p/'tools/cloud/score.py').is_file() and (p/'.git').exists()),None)
    if root is None:pytest.skip('set RUSH_REFERENCE_ROOT to repository with full historical context')
    return root

def test_frozen_source_and_header():
    source=(HERE/'candidate.c').read_bytes();receipt=json.loads((HERE/'baseline.json').read_text())
    assert source.splitlines()[0]==b'/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
    assert hashlib.sha256(source).hexdigest()==receipt['source_sha256']
    assert receipt['target_compiler_invocations']==1
    assert receipt['comparisons']['save_write_data']['canonical']['differing']>0

def test_authenticated_native_targets(reference):
    code,image,targets=load(reference)
    assert len(code)==(1200+1128)//4
    assert targets['save_write_data']['size']==1200

def test_unchanged_host_smoke(reference,tmp_path):
    if not shutil.which('cc'):pytest.skip('host C compiler required')
    code,image,_=load(reference);output=tmp_path/'host.so';build_host(output);host=Host(output)
    for options in ({},{'mode':0},{'shortcut':True},{'shortcut':True,'alloc':(False,)},
                    {'old_scene':7},{'ring':49,'sound':1,'scale':.5}):
        compare(code,image,host,options)
