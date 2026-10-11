"""Research checkpoint guards, complete root ABI checks, and isolated replay."""
import importlib.util
import json
import os
from pathlib import Path
import sys
import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_runtime_a_roots_20261011'
spec=importlib.util.spec_from_file_location('runtime_a_roots_verify',PACKET/'verify.py')
verify=importlib.util.module_from_spec(spec);spec.loader.exec_module(verify)


def test_research_only_scope_and_archived_bodies():
    source=(PACKET/'group.c').read_text();config=json.loads((PACKET/'group.json').read_text())
    verify.source_contract(source,config)
    with pytest.raises(ValueError,match='no claims'):
        verify.source_contract(source,dict(config,claims=['func_80394058']))
    with pytest.raises(ValueError,match='genuine root'):
        verify.source_contract(source,dict(config,keep=['func_803923B0']))
    with pytest.raises(ValueError,match='member drift'):
        verify.source_contract(source,dict(config,members=config['members'][:-1]))
    with pytest.raises(ValueError,match='context drift'):
        verify.source_contract(source,dict(config,context=[]))
    with pytest.raises(ValueError,match='archived body drift'):
        verify.source_contract(source.replace('slot < 4','slot < 3'),config)
    with pytest.raises(ValueError,match='witnessed mode'):
        verify.source_contract(source.replace('s32 mode;','s32 mode;\n    s32 pressure;'),config)


def test_fail_closed_runner_and_optimization_independent_guards():
    native=verify.load('test_root_native',PACKET/'native.py')
    with pytest.raises(ValueError,match='sentinel'):
        verify.require(False,'sentinel')
    with pytest.raises(ValueError,match='unmapped'):
        native.read({},0,4)
    with pytest.raises(ValueError,match='unaligned'):
        native.write(dict.fromkeys(range(8),0),1,4,0)
    initial,seed=next(verify.cases(native))
    with pytest.raises(ValueError,match='unknown opcode'):
        native.run([0xFC000000],initial,seed)


def test_complete_research_replay_and_import_isolation():
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))/'cc'
    if not ido.is_file():
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('IDO required')
        pytest.skip('IDO unavailable')
    incoming_path=list(sys.path)
    sentinel=object();old=sys.modules.get('native');sys.modules['native']=sentinel
    try:
        result=verify.verify()
        assert sys.modules['native'] is sentinel
        assert sys.path==incoming_path
    finally:
        if old is None:sys.modules.pop('native',None)
        else:sys.modules['native']=old
    assert result['new_candidate_bytes']==result['accepted_bytes']==0
    assert result['scores']['func_80394058']['differing']==2
    assert result['bindings']['func_80394058']['compiled_bytes']==1292
    assert result['literal_section']['bytes']==48
    assert result['gnu_relocations']['func_80394058']==84
    assert result['root_boundary_cases']==1024
    assert result['root_instructions_covered']==307
    assert result['negative_controls']['missing_initializer']['differing']==2
    assert result['negative_controls']['missing_renderer']['differing']>2


def test_stock_metadata_fidelity_and_storage_layout():
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))/'cc'
    if not ido.is_file():
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('IDO required')
        pytest.skip('IDO unavailable')
    compiler=verify.load('test_roots_compiler_evidence',PACKET/'compiler_evidence.py')
    incoming_path=list(sys.path)
    result=compiler.inspect()
    assert sys.path==incoming_path
    assert result['whole_object_diagnostic_identical']
    assert result['observations']['func_80394058']['memory_areas']=={'T':16,'P':4,'M':2}
    assert result['native_layout']['sizeof(MenuRecord)']==64


def test_failed_public_calls_restore_search_path(monkeypatch):
    compiler=verify.load('test_roots_compiler_failure',PACKET/'compiler_evidence.py')
    incoming_path=list(sys.path)
    def fail():
        sys.path.insert(0,'/tmp/root-failure-sentinel')
        raise ValueError('injected failure')
    monkeypatch.setattr(verify,'_verify',fail)
    monkeypatch.setattr(compiler,'_inspect',fail)
    for call in (verify.verify,compiler.inspect):
        with pytest.raises(ValueError,match='injected failure'):call()
        assert sys.path==incoming_path
