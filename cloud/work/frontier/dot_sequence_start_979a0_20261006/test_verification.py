"""Scoped replay and fail-closed packet guards; no image/ROM gate."""
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import pytest

HERE = Path(__file__).resolve().parent
REPO = Path(os.environ.get('RUSH_SEQUENCE_REPO', HERE.parents[3])).resolve()


def run(packet, *options):
    return subprocess.run([sys.executable, *options, str(packet / 'verify.py'),
                           '--repo', str(REPO), '--check'],
                          cwd='/tmp', text=True, capture_output=True)


def copy_packet(destination):
    destination.mkdir()
    for name in ('verify.py', 'verification.json'):
        shutil.copy2(HERE / name, destination / name)
    shutil.copytree(HERE / 'group', destination / 'group')
    return destination


def test_frozen_receipt_replays():
    result = run(HERE)
    assert result.returncode == 0, result.stdout + result.stderr


def test_replay_from_different_directory_under_optimized_python(tmp_path):
    packet = copy_packet(tmp_path / 'portable packet')
    result = run(packet, '-O')
    assert result.returncode == 0, result.stdout + result.stderr


@pytest.mark.parametrize('change,expected', [
    ('claim', 'Research packet must not claim'),
    ('context', 'Accepted source copy changed'),
    ('order', 'Context kept roots changed'),
])
def test_invalid_packet_rejected(tmp_path, change, expected):
    packet = copy_packet(tmp_path / 'negative')
    spec_path = packet / 'group/group.json'
    spec = json.loads(spec_path.read_text())
    if change == 'claim':
        spec['claims'] = ['func_800979A0']
    elif change == 'order':
        spec['keep'] = list(reversed(spec['keep']))
    else:
        path = packet / 'group/group.c'
        path.write_text(path.read_text() + '\n/* changed accepted context */\n')
    spec_path.write_text(json.dumps(spec, indent=2) + '\n')
    result = run(packet, '-O')
    assert result.returncode != 0
    assert expected in result.stderr
