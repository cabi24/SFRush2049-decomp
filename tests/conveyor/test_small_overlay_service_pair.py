"""Host semantics only: no fake helper enters source matching or coverage."""
from pathlib import Path
import shutil
import subprocess
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/small_overlay_service_pair'


def test_small_overlay_service_pair_host(tmp_path):
    cc = shutil.which('cc')
    if cc is None:
        pytest.skip('C compiler unavailable')
    out = tmp_path / 'services'
    subprocess.run([
        cc, '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
        '-O2', '-ffp-contract=off', '-fno-fast-math',
        str(PACKET / 'service_pair.c'),
        str(ROOT / 'tests/conveyor/fixtures/small_overlay_service_pair/host_test.c'),
        '-o', str(out),
    ], check=True)
    subprocess.run([str(out)], check=True)


def test_independent_service_pair_oracle(tmp_path):
    import sys
    cc = shutil.which('cc')
    if cc is None:
        pytest.skip('C compiler unavailable')
    fixtures = ROOT / 'tests/conveyor/fixtures/small_overlay_service_pair'
    out = tmp_path / 'independent.so'
    subprocess.run([
        cc, '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
        '-O2', '-ffp-contract=off', '-fno-fast-math', '-fPIC', '-shared',
        str(PACKET / 'service_pair.c'), str(fixtures / 'independent_harness.c'),
        '-o', str(out),
    ], check=True)
    subprocess.run([sys.executable, str(fixtures / 'independent_oracle.py'),
                    str(out)], check=True)


def test_native_service_pair_provenance():
    import importlib.util
    spec = importlib.util.spec_from_file_location('small_service_audit', PACKET / 'audit.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    asset = ROOT / 'assets/us/data.bin'
    if not asset.is_file():
        pytest.skip('private native asset unavailable')
    result = module.audit(asset)
    assert [body['bytes'] for body in result['bodies']] == [244, 736]
    assert [body['sha256'] for body in result['bodies']] == [
        'bca5c5c0678b994919e5495be737f3c3bdcc6f00f021f93e3238634d0bda3b3e',
        'ef153feda709bc9a4a8c200237d0bfdb3faaf8c9240cfb553d6b3caff0781f55',
    ]
    assert result['accepted_bytes'] == 0


def test_residency_transition_model():
    import runpy
    namespace = runpy.run_path(str(PACKET / 'residency_model.py'))
    namespace['tests']()
