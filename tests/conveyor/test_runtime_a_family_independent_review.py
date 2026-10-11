"""Independent availability-family review and fail-closed harness controls."""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_runtime_a_leaf_review_20261011'
GROUP=Path(os.environ.get('RUNTIME_A_FAMILY_GROUP',ROOT/'cloud/work/frontier/dot_runtime_a_callers_deep_20261011'))


def load_native():
    spec=importlib.util.spec_from_file_location('availability_review_native_test',PACKET/'native.py')
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    return module


def toolchain():
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
    ready=(ido/'cc').is_file() and (GROUP/'group.c').is_file() and all(shutil.which(n) for n in ('gcc','mips-linux-gnu-as','mips-linux-gnu-ld'))
    if not ready:
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('required group or toolchain unavailable')
        pytest.skip('group or independent verification toolchain unavailable')


def test_memory_and_opcode_guards():
    native=load_native()
    with pytest.raises(ValueError):native.read({},0x1000,4)
    with pytest.raises(ValueError):native.write({0:0},0,4,1)
    with pytest.raises(ValueError):native.run({0x1000:[0xFC000000]}, {},0x1000,0,[])
    with pytest.raises(ValueError):native.run({0x1000:[0]}, {},0x1000,0,[])
    assert native.signed(0xFFFF,16)==-1
    assert native.signed(0x8000,16)==-32768


def test_native_guards_under_optimized_python():
    code="import sys;sys.path.insert(0,sys.argv[1]);import native;native.require(False,'guard survived optimization')"
    result=subprocess.run([sys.executable,'-O','-c',code,str(PACKET)],capture_output=True,text=True)
    assert result.returncode!=0 and 'guard survived optimization' in result.stderr


def test_full_five_body_review(tmp_path):
    toolchain();output=tmp_path/'review.json'
    result=subprocess.run([sys.executable,str(PACKET/'review.py'),'--group',str(GROUP),'--output',str(output)],capture_output=True,text=True)
    assert result.returncode==0,result.stdout+'\n'+result.stderr
    receipt=json.loads(output.read_text())
    assert receipt['status']=='PASS' and receipt['new_candidate_bytes']==928 and receipt['accepted_bytes']==0
    assert all(not r['differing'] and not r['extra_words'] and not r['errors'] and not r['unverified'] for r in receipt['scores'].values())
    assert receipt['semantics']['host_C89_UBSan_bounds_cases']==7103
    assert receipt['cleanup_semantics']['host_C89_UBSan_bounds_cases']==1364
    assert receipt['standalone_negative_control']['differing']==2
    assert all(r['differing']>0 for r in receipt['compiled_negative_controls'].values())


def test_stock_compiler_metadata_fidelity(tmp_path):
    toolchain();output=tmp_path/'compiler.json'
    result=subprocess.run([sys.executable,str(PACKET/'compiler_evidence.py'),'--group',str(GROUP),'--output',str(output)],capture_output=True,text=True)
    assert result.returncode==0,result.stdout+'\n'+result.stderr
    receipt=json.loads(output.read_text());rows=receipt['observations']
    assert rows['standalone']['external']==1 and rows['genuine_group']['external']==0
    assert rows['standalone']['differing_words']==2 and rows['genuine_group']['differing_words']==0
    assert all(r['full_object_diagnostic_identical'] and r['full_object_canonical_identical'] for r in rows.values())


def test_record_navigation_peer_review(tmp_path):
    toolchain()
    group=Path(os.environ.get('RUNTIME_A_RECORD_GROUP',ROOT/'cloud/work/frontier/dot_runtime_a_record_context_20261011'))
    if not (group/'group.c').is_file():
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('required record-navigation source unavailable')
        pytest.skip('record-navigation source unavailable')
    output=tmp_path/'record.json'
    result=subprocess.run([sys.executable,str(PACKET/'review_record.py'),'--group',str(group),'--output',str(output)],capture_output=True,text=True)
    assert result.returncode==0,result.stdout+'\n'+result.stderr
    receipt=json.loads(output.read_text())
    assert receipt['status']=='PASS' and receipt['new_candidate_bytes']==444
    assert receipt['independent_gnu_relocations']['func_8039E200']==15
    assert receipt['separately_reexecuted_author_cases']==6546
    assert receipt['independently_constructed_topology_cases']==2250
    assert receipt['linked_six_node_permutations']==720
    assert receipt['topology_instructions_covered']==111
