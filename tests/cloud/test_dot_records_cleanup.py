"""Focused portable replay and fail-closed controls for records_screen."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
REPO = Path(os.environ.get('RUSH_RECORDS_REPO', ROOT)).resolve()
HISTORY = Path(os.environ.get('RUSH_RECORDS_HISTORY', REPO)).resolve()
PACKET = ROOT / 'cloud/work/frontier/dot_records_cleanup_20261006'
SOURCE = ROOT / 'cloud/work/frontier/dot_records_cleanup_20261006/records_screen.c'
VERIFY = PACKET / 'verify.py'


def require_toolchain():
    ido = Path(os.environ.get('IDO_DIR', REPO / 'tools/cloud/ido'))
    if not (ido / 'cc').is_file():
        pytest.skip('IDO unavailable; no compiler replay performed')
    for tool in ('mips-linux-gnu-ld', 'mips-linux-gnu-readelf', 'mips-linux-gnu-objcopy', 'mips-linux-gnu-nm', os.environ.get('CC', 'cc')):
        if shutil.which(tool) is None:
            pytest.skip('required replay tool unavailable: ' + tool)


def replay(cwd, optimized=False, source=SOURCE, env=None):
    args = [sys.executable] + (['-O'] if optimized else [])
    args += [str(VERIFY), '--repo', str(REPO), '--history-repo', str(HISTORY), '--source', str(source)]
    return subprocess.run(args, cwd=cwd, env=env, text=True, capture_output=True)


def test_packet_bindings():
    bindings = json.loads((PACKET / 'bindings.json').read_text())
    paths = {'source': SOURCE, 'verifier': VERIFY, 'host_contract': PACKET / 'host_contract.c'}
    for label, path in paths.items():
        assert hashlib.sha256(path.read_bytes()).hexdigest() == bindings['packet_files'][label]


@pytest.mark.parametrize('optimized', [False, True])
def test_foreign_cwd_replay(tmp_path, optimized):
    require_toolchain()
    foreign = tmp_path / 'foreign cwd with spaces'
    foreign.mkdir()
    result = replay(foreign, optimized=optimized)
    assert result.returncode == 0, result.stdout + result.stderr
    receipt = json.loads(result.stdout)
    expected = json.loads((PACKET / 'verification.json').read_text())
    expected.pop('local_provenance', None)
    assert receipt == expected
    assert receipt['scorer_output'] == 'MATCH'
    assert receipt['strict']['total'] == 78
    assert receipt['gnu_linked_address'] == '0x800d58cc'
    assert receipt['gnu_linked_function_bytes'] == 312
    assert receipt['gnu_full_body_equal']
    assert 'local_provenance' not in receipt
    assert set(receipt['host_negative_controls'].values()) == {'rejected'}


@pytest.mark.parametrize('optimized', [False, True])
def test_source_mutation_rejected(tmp_path, optimized):
    mutated = tmp_path / 'modified source.c'
    mutated.write_text(SOURCE.read_text().replace('const int sentinel = -1;', 'const int sentinel = -2;'))
    result = replay(tmp_path, optimized=optimized, source=mutated)
    assert result.returncode != 0
    assert 'packet binding mismatch: source' in result.stderr


@pytest.mark.parametrize('optimized', [False, True])
def test_link_misplacement_rejected(tmp_path, optimized):
    require_toolchain()
    real_ld = shutil.which('mips-linux-gnu-ld')
    wrapper_dir = tmp_path / 'link wrappers with spaces'
    wrapper_dir.mkdir()
    wrapper = wrapper_dir / 'mips-linux-gnu-ld'
    wrapper.write_text('#!' + sys.executable + '\n'
                       'import os, pathlib, sys\n'
                       'args = sys.argv[1:]\n'
                       'p = pathlib.Path(args[args.index("-T") + 1])\n'
                       'p.write_text(p.read_text().replace(".text 0x800D58CC", ".text 0x800D58D0"))\n'
                       'os.execv(' + repr(real_ld) + ', [' + repr(real_ld) + '] + args)\n')
    wrapper.chmod(0o755)
    env = os.environ.copy()
    env['PATH'] = str(wrapper_dir) + os.pathsep + env['PATH']
    result = replay(tmp_path, optimized=optimized, env=env)
    assert result.returncode != 0
    assert 'linked function address mismatch' in result.stderr
