"""Isolated research regressions. No target compilation or acceptance claim."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKETS = ROOT / 'cloud/work'
AF06C = PACKETS / 'af06c_tagged_effect_service_20261006'
TICK = PACKETS / 'runtime_b_effect_tick_20261006'
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'


def _reference():
    root = Path(os.environ.get('SFRUSH_REFERENCE_ROOT', os.environ.get('RUSH_REFERENCE_ROOT', ROOT)))
    result = subprocess.run(['git', '-C', str(root), 'cat-file', '-e', BASE + ':assets/us/data.bin'], capture_output=True)
    if result.returncode:
        pytest.skip('historical reference commit and asset required')
    return root.resolve()


def _env(tmp_path):
    env = dict(os.environ, TMPDIR=str(tmp_path), PYTHONDONTWRITEBYTECODE='1')
    # Keep separately installed pytest, but never lend score its sibling imports.
    env['PYTHONPATH'] = os.pathsep.join(p for p in env.get('PYTHONPATH', '').split(os.pathsep)
                                      if p and not p.rstrip('/').endswith('/tools/cloud'))
    return env


def _run(script, tmp_path, env):
    result = subprocess.run([sys.executable, '-c', script], cwd=tmp_path, env=env,
                            capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr


def test_owned_receipt_bindings():
    receipts = [
        (AF06C, 'semantics.json', 'packet_sha256'),
        (AF06C, 'linked-semantics.json', 'packet_sha256'),
        (AF06C, 'independent-review.json', 'packet'),
        (AF06C, 'independent-linked-review.json', 'packet'),
        (TICK, 'packet.json', 'source_sha256'),
        (TICK, 'independent/receipt-current.json', 'packet_inputs_sha256'),
    ]
    for folder, receipt, key in receipts:
        for name, expected in json.loads((folder / receipt).read_text())[key].items():
            assert not name.startswith('test_')
            assert hashlib.sha256((folder / name).read_bytes()).hexdigest() == expected, (receipt, name)
    review = json.loads((AF06C / 'independent-review.json').read_text())
    contracts = json.loads((AF06C / 'independent/contract-receipt.json').read_text())
    verifier = hashlib.sha256((AF06C / 'independent/audit_contracts.py').read_bytes()).hexdigest()
    assert verifier == review['own_audit_source_sha256'] == contracts['own_verifier_sha256']
    historical = json.loads((TICK / 'independent/receipt.json').read_text())
    current = json.loads((TICK / 'independent/receipt-current.json').read_text())
    historical['packet_inputs_sha256']['native.py'] = current['packet_inputs_sha256']['native.py']
    assert historical == current  # All actual proof fields remain identical.


@pytest.mark.parametrize('packet', [AF06C, TICK], ids=['af06c', 'effect-tick'])
def test_isolated_packet_suite(packet, tmp_path):
    reference = _reference()
    env = _env(tmp_path)
    env.update(RUSH_REFERENCE_ROOT=str(reference), SFRUSH_REFERENCE_ROOT=str(reference))
    env.setdefault('SFRUSH_SCORE_ROOT', str(ROOT))
    result = subprocess.run([sys.executable, '-m', 'pytest', str(packet / 'test_packet.py'),
                             '-q', '-p', 'no:cacheprovider'], cwd=tmp_path, env=env,
                            capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr


def test_scorer_import_and_missing_ido_guards(tmp_path):
    scoreroot = Path(os.environ.get('SFRUSH_SCORE_ROOT', ROOT)).resolve()
    if not (scoreroot / 'tools/cloud/score.py').is_file():
        pytest.skip('canonical score.py and sibling owndata.py required')
    env = _env(tmp_path)
    env['IDO_DIR'] = str(tmp_path / 'absent-ido')
    _run(f'''
import sys
from pathlib import Path
sys.path.insert(0, {str(TICK)!r})
from baseline import load_score
root = Path({str(scoreroot)!r})
assert str(root / 'tools/cloud') not in sys.path
score = load_score(root)
assert Path(score.owndata.__file__).resolve() == (root / 'tools/cloud/owndata.py').resolve()
assert not (score.IDO / 'cc').is_file()
''', tmp_path, env)
    _run(f'''
import sys
from pathlib import Path
sys.path.insert(0, {str(AF06C)!r})
from build_once import run
work = Path({str(tmp_path / 'never-built')!r})
result = run(Path({str(tmp_path)!r}), Path({str(scoreroot)!r}), work)
assert result == {{'status': 'SKIP: pinned IDO and MIPS GNU linker required', 'target_compiler_invocations': 0}}
assert not (work / 'target_compile_started').exists()
''', tmp_path, env)
