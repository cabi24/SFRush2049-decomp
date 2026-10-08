"""Source-only AA8C semantic research, independently runnable after integration.

Subprocesses isolate generic packet module names. Native-only tests have no C,
IDO, or MIPS linker dependency. Host-C tests skip before invoking the verifier
when the host compiler is absent. No test-file hash enters a proof receipt.
"""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

REPO = Path(__file__).resolve().parents[2]
PACKET = REPO / 'cloud/work/runtime_b_aa8c_semantic_20261006'


def require_host_compiler():
    if not shutil.which('cc'):
        pytest.skip('host C compiler required for native/semantic-C comparison')


@pytest.fixture
def reference():
    value = os.environ.get('SFRUSH_REFERENCE_ROOT') or os.environ.get('RUSH_REFERENCE_ROOT')
    root = Path(value).resolve() if value else REPO
    if not (root / 'tools/cloud/score.py').is_file() or not (root / 'asm/us/ovl_b').is_dir():
        pytest.skip('target-bearing Git checkout required; set SFRUSH_REFERENCE_ROOT')
    return root


def replay(relative_script, reference, tmp_path, *arguments):
    output = tmp_path / 'replay.json'
    environment = dict(os.environ, TMPDIR=str(tmp_path), PYTHONDONTWRITEBYTECODE='1')
    # The selected checkout supplies dependencies; invocation cwd is irrelevant.
    environment['PYTHONPATH'] = str(reference) + os.pathsep + environment.get('PYTHONPATH', '')
    result = subprocess.run([sys.executable, str(PACKET / relative_script),
                             '--reference-root', str(reference), '--output', str(output), *arguments],
                            cwd=tmp_path, env=environment, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    return json.loads(output.read_text())


def receipt(path):
    return json.loads((PACKET / path).read_text())


def test_producer_semantic_replay(reference, tmp_path):
    require_host_compiler()
    fresh = replay('packet/verify.py', reference, tmp_path)
    expected = receipt('packet/verification.json')
    # Compiler identification is provenance, not part of the behavior proof.
    for result in (fresh, expected):
        result['behavior'].pop('host_compiler', None)
    assert fresh == expected
    assert fresh['behavior']['paired_fixtures'] == 1759
    assert fresh['behavior']['IDO_invocations'] == 0


def test_independent_semantic_review(reference, tmp_path):
    require_host_compiler()
    fresh = replay('review/verify_independent.py', reference, tmp_path,
                   '--packet', str(PACKET / 'packet'))
    assert fresh == receipt('review/final-review.json')
    assert fresh['extra_pairs_per_host_configuration'] == 2766


def test_per_operation_rounding_control(reference, tmp_path):
    require_host_compiler()
    fresh = replay('review/verify_rounding.py', reference, tmp_path,
                   '--packet', str(PACKET / 'packet'))
    assert fresh == receipt('review/rounding-control.json')


def test_actual_native_services(reference, tmp_path):
    fresh = replay('effects/verify_native_effects.py', reference, tmp_path)
    assert fresh == receipt('effects/verification.json')
    assert fresh['cases'] == 61


def test_owner_domain_and_object_blockers(reference, tmp_path):
    fresh = replay('objects/verify.py', reference, tmp_path)
    assert fresh == receipt('objects/verification.json')
    assert fresh['fixture_counts'] == {
        'setup_full_native': 105, 'flag_projection': 630, 'UI_scan_clamp': 567}
    assert fresh['counterexamples'] == [
        {'count': 5, 'active_owners': list(range(5))},
        {'count': 6, 'active_owners': list(range(6))}]


def test_host_compiler_absence_guard(monkeypatch):
    monkeypatch.setattr(shutil, 'which', lambda name: None)
    with pytest.raises(pytest.skip.Exception, match='host C compiler required'):
        require_host_compiler()


def test_source_and_harness_bindings():
    sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    producer = receipt('packet/verification.json')
    review = receipt('review/final-review.json')
    for report in (producer, review):
        for name, expected in report['packet_sha256'].items():
            assert not name.startswith('test_')
            assert sha(PACKET / 'packet' / name) == expected
    assert sha(PACKET / 'review/verify_independent.py') == review['review_sha256']
    for name, expected in receipt('objects/verification.json')['owned_files'].items():
        assert not name.startswith('test_')
        assert sha(PACKET / 'objects' / name) == expected
    assert sha(PACKET / 'effects/verify_native_effects.py') == receipt('effects/verification.json')['verifier_sha256']
