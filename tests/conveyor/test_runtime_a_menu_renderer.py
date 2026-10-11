"""Freeze genuine image-A research without counting nonmatches as coverage."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_runtime_a_menu_renderer_20261011'

def load():
    spec=importlib.util.spec_from_file_location('menu_renderer_verify_test',HERE/'verify.py')
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module

def test_frozen_receipt_and_no_claims():
    receipt=json.loads((HERE/'verification.json').read_text())
    assert receipt['claims']==[] and receipt['matching_candidate_bytes']==receipt['accepted_bytes']==0
    assert json.loads((HERE/'group.json').read_text())['keep']==['func_803A7918']
    for name,digest in receipt['hashes'].items():
        assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest
    for name in ('func_8038F648','func_803A6F2C'):
        row=receipt['results'][name]
        assert row['score']['differing']==2
        assert row['compiled_bytes']==row['native_bytes']
        assert row['compiled_sha256']==row['gnu_sha256']!=row['native_sha256']

def test_fail_closed_guard():
    with pytest.raises(ValueError,match='rejected'):
        load().require(False,'rejected')

def test_complete_stock_replay():
    if os.environ.get('REQUIRE_TOOLCHAIN')!='1':
        pytest.skip('set REQUIRE_TOOLCHAIN=1 for canonical IDO/GNU replay')
    actual=load().verify()
    expected=json.loads((HERE/'verification.json').read_text())
    for receipt in (actual,expected):
        receipt.pop('seconds',None)
    assert actual==expected


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
