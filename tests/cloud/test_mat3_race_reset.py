"""One fast protocol-v2 test for the complete settings/race reset group."""
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_mat3_physics_20261017/groups/codex_vsync_a145'
spec = importlib.util.spec_from_file_location('race_reset_replay', HERE / 'replay.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_complete_race_reset_packet(tmp_path):
    receipt = json.loads((HERE / 'proof.json').read_text())
    assert packet.inputs() == receipt['inputs']
    for name in ('group.c', 'func_800D6530.c'):
        assert (HERE/name).read_bytes() == (ROOT/'src/blob/groups/codex_vsync_a145'/name).read_bytes()
    compiler = shutil.which('cc')
    if not compiler:
        pytest.skip('host C compiler unavailable')
    exe = tmp_path / 'semantics'
    subprocess.run([compiler, '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                    '-fsanitize=undefined', str(HERE/'semantics.c'), '-o', str(exe)], check=True)
    checked = subprocess.run([str(exe)], check=True, capture_output=True, text=True)
    assert checked.stdout == '256 bounded host cases passed\n'
    if not (packet.score.IDO/'cc').exists():
        pytest.skip('IDO missing for strict compiler replay')
    obj = tmp_path / 'group.o'
    packet.score.compile_group(HERE, obj)
    assert packet.group_result(obj) == receipt['positive']['group_first']
    assert packet.object_layout(obj) == receipt['object_layout']
