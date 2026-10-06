"""Independent source fixtures plus stable packet/native bindings.

These are bounded semantic research tests. External helper bodies are hooks;
neither strict matching nor whole-game equivalence is claimed.
"""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
FRONTIER = ROOT / 'cloud/work/frontier'
GEAR = FRONTIER / 'dot_gear_label_ef288_20261006'
VIEWPORT = FRONTIER / 'dot_viewport_effect_fa9b4_20261006'
BATCH = FRONTIER / 'dot_callback_batch_20261006'

def run(args, work, **changes):
    env = dict(os.environ, PYTHONDONTWRITEBYTECODE='1', TMPDIR=str(work), **changes)
    subprocess.run([str(x) for x in args], env=env, cwd=work, check=True,
                   capture_output=True, text=True)

def roots():
    tools = Path(os.environ.get('RUSH_TOOL_ROOT', ROOT)).resolve()
    reference = Path(os.environ.get('RUSH_REFERENCE_ROOT', os.environ.get('RUSH_GIT_REPO', tools))).resolve()
    return tools, reference

def test_packet_sources_and_native_targets():
    spec = importlib.util.spec_from_file_location('callback_packet_binding', BATCH / 'verify_packet.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    result = module.verify(roots()[0])
    assert result['claims'] == [] and result['accepted_coverage_bytes'] == 0
    assert (GEAR / 'candidate.c').read_bytes() == (GEAR / 'review/candidate.c').read_bytes()
    assert (VIEWPORT / 'candidate.c').read_bytes() == (VIEWPORT / 'review/frozen_candidate.c').read_bytes()

def test_packet_binding_from_isolated_foreign_cwd(tmp_path):
    tools, _ = roots()
    env = dict(os.environ)
    env.pop('PYTHONPATH', None)
    result = subprocess.run(
        [sys.executable, '-I', '-B', str(BATCH / 'verify_packet.py'),
         '--tool-root', str(tools)],
        cwd=tmp_path, env=env, check=True, capture_output=True, text=True)
    assert json.loads(result.stdout) == {
        'authored_sources': 24, 'native_targets': 36,
        'claims': [], 'accepted_coverage_bytes': 0}

@pytest.mark.parametrize('failure', [None, 'import', 'targets'])
def test_packet_loader_restores_import_state(monkeypatch, failure):
    import types
    tools, _ = roots()
    spec = importlib.util.spec_from_file_location('callback_loader_state', BATCH / 'verify_packet.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    foreign = types.ModuleType('owndata')
    previous_score = types.ModuleType('callback_binding_score_blob')
    monkeypatch.setitem(sys.modules, 'owndata', foreign)
    monkeypatch.setitem(sys.modules, 'callback_binding_score_blob', previous_score)
    monkeypatch.delitem(sys.modules, 'callback_binding_score_ovl_b', raising=False)
    prior_path = sys.path[:]
    original_spec = importlib.util.spec_from_file_location
    loaded = []

    def fail():
        raise RuntimeError('deliberate loader-state control')

    def checked_spec(name, path):
        result = original_spec(name, path)
        original_exec = result.loader.exec_module

        def checked_exec(score):
            original_exec(score)
            assert score.owndata is not foreign
            assert Path(score.owndata.__file__).resolve() == tools / 'tools/cloud/owndata.py'
            loaded.append(name)
            if failure == 'import':
                fail()
            if failure == 'targets':
                score.targets = fail

        result.loader.exec_module = checked_exec
        return result

    monkeypatch.setattr(importlib.util, 'spec_from_file_location', checked_spec)
    if failure:
        with pytest.raises(RuntimeError, match='deliberate loader-state control'):
            module.verify(tools)
    else:
        assert module.verify(tools)['native_targets'] == 36
    assert loaded == (['callback_binding_score_blob'] if failure else
                      ['callback_binding_score_blob', 'callback_binding_score_ovl_b'])
    assert sys.path == prior_path
    assert sys.modules['owndata'] is foreign
    assert sys.modules['callback_binding_score_blob'] is previous_score
    assert 'callback_binding_score_ovl_b' not in sys.modules

def test_independent_gear_source_review(tmp_path):
    if not shutil.which('cc'):
        pytest.skip('host C compiler required')
    tools, _ = roots()
    review = GEAR / 'review'
    run(['cc', '-std=c99', '-O2', '-fPIC', '-shared', review / 'host_audit.c', '-o', tmp_path / 'host_audit.so'], tmp_path)
    env = {'RUSH_GEAR_REVIEW_WORK': str(tmp_path), 'RUSH_REVIEW_OUTPUT': str(tmp_path)}
    run([sys.executable, review / 'audit.py', '--source-root', tools], tmp_path, **env)
    run([sys.executable, review / 'negative.py', tools], tmp_path, **env)
    fresh = json.loads((tmp_path / 'review.json').read_text())
    saved = json.loads((review / 'review.json').read_text())
    for key in ('base', 'function', 'native_bytes', 'native_sha256', 'source_sha256',
                'reviewed_interpreter_sha256', 'cases', 'case_families',
                'native_instruction_words_visited', 'native_instruction_words',
                'unvisited_offsets', 'branches', 'trace_sha256', 'limitations'):
        assert fresh[key] == saved[key], key
    assert fresh['cases'] == 7281
    assert json.loads((tmp_path / 'negative_controls.json').read_text()) == json.loads((review / 'negative_controls.json').read_text())

@pytest.fixture(scope='module')
def viewport_object(tmp_path_factory):
    tools, _ = roots()
    sys.path.insert(0, str(tools / 'tools/cloud'))
    import score
    if not (score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    if not shutil.which('gcc'):
        pytest.skip('host C compiler required')
    if os.environ.get('RUSH_VIEWPORT_OBJECT'):
        return Path(os.environ['RUSH_VIEWPORT_OBJECT']).resolve()
    obj = tmp_path_factory.mktemp('viewport_review_object') / 'candidate.o'
    score.compile_single(VIEWPORT / 'candidate.c', '-g0 -O3 -mips2 -G 0 -non_shared', obj)
    return obj

def test_independent_viewport_source_review(viewport_object, tmp_path):
    tools, reference = roots()
    review = VIEWPORT / 'review'
    env = {'RUSH_TOOL_ROOT': str(tools), 'RUSH_REFERENCE_ROOT': str(reference),
           'RUSH_PACKET_ROOT': str(VIEWPORT), 'RUSH_VIEWPORT_OBJECT': str(viewport_object),
           'RUSH_VIEWPORT_REVIEW_WORK': str(tmp_path), 'RUSH_REVIEW_OUTPUT': str(tmp_path)}
    for script in ('build_review_host.py', 'review.py', 'mutations.py', 'static_review.py'):
        run([sys.executable, review / script], tmp_path, **env)
    for filename in ('review.json', 'mutations.json', 'static-review.json'):
        fresh = json.loads((tmp_path / filename).read_text())
        saved = json.loads((review / filename).read_text())
        # ELF container metadata are not the proof. Compiled words, comparison,
        # observations and fixture results remain bound and checked.
        fresh.pop('object_sha256', None)
        saved.pop('object_sha256', None)
        assert fresh == saved, filename
