"""Complete research bodies and switch ownership must not become match credit."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_runtime_a_menu_renderer_closure_20261011'

def load():
    spec=importlib.util.spec_from_file_location('menu_renderer_closure_test',HERE/'verify.py')
    m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m

def test_frozen_sources_and_nonmatches():
    r=json.loads((HERE/'verification.json').read_text())
    assert r['claims']==[] and r['matching_candidate_bytes']==r['accepted_bytes']==0
    for p,h in r['hashes'].items():assert hashlib.sha256((ROOT/p).read_bytes()).hexdigest()==h
    assert [x['score']['differing'] for x in r['results'].values()]==[2,6,4,7]
    for x in r['results'].values():
        assert x['native_bytes']==x['compiled_bytes']
        assert x['compiled_sha256']==x['gnu_sha256']!=x['native_sha256']
        assert not any(x['score'][k] for k in ('unresolved','unverified','errors','extra_words'))
    assert r['owned_switch_table']['bytes']==20
    assert set(r['owned_switch_table'])=={'address','bytes','entry_count','sha256','gnu_sha256','gnu_relocations'}
    assert r['owned_switch_table']['sha256']==r['owned_switch_table']['gnu_sha256']
    assert r['stock_ugen_metadata']['canonical_object_identical'] is True

def test_fail_closed_guard():
    with pytest.raises(ValueError,match='rejected'):load().require(False,'rejected')

def test_stock_complete_replay():
    if os.environ.get('REQUIRE_TOOLCHAIN')!='1':pytest.skip('canonical compiler opt-in')
    actual=load().verify();frozen=json.loads((HERE/'verification.json').read_text())
    actual.pop('seconds');frozen.pop('seconds');assert actual==frozen

def test_optimized_python_checks():
    if os.environ.get('REQUIRE_TOOLCHAIN')!='1':pytest.skip('canonical compiler opt-in')
    p=subprocess.run([sys.executable,'-O',str(HERE/'verify.py')],capture_output=True,text=True,cwd='/tmp')
    assert p.returncode==0,p.stderr
    assert json.loads(p.stdout)['claims']==[]


def test_import_state_preserved(monkeypatch):
    if os.environ.get('REQUIRE_TOOLCHAIN')!='1':
        pytest.skip('canonical compiler opt-in')
    import sys
    import types
    sentinels={name:types.ModuleType(name) for name in ('native','elf_reader')}
    for name,value in sentinels.items():monkeypatch.setitem(sys.modules,name,value)
    before=list(sys.path)
    load().verify()
    assert sys.path==before
    assert all(sys.modules[name] is value for name,value in sentinels.items())
