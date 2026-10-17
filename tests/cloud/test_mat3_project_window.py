"""Genuine-helper full-extent proof for the N64 window projection routine."""
import importlib.util
import json
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_mat3_project_window_20261017'
spec = importlib.util.spec_from_file_location('mat3_project_window_packet', HERE / 'replay.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_project_window_source_and_receipt():
    packet.verify_source_change()
    saved = json.loads((HERE / 'proof.json').read_text())
    assert packet.inputs() == saved['inputs']
    assert saved['new_individually_strict_bytes'] == 448
    assert saved['accepted_image_bytes'] == 0
    assert packet.recipe()['claims'] == ['brake_light_update']
    assert saved['positive']['first'] == saved['positive']['repeat'] == saved['positive']['getter_after_inverse']
    assert all(saved['positive']['first']['scores'][n]['strict_match'] for n in packet.STRICT)
    assert saved['positive']['first']['layout']['root_frame_bytes'] == 72
    for label, group in saved['positive'].items():
        assert group['layout']['emitted_functions'] == sorted(packet.STRICT)
        assert {n: group['layout']['function_extents'][n]['bytes'] for n in packet.STRICT} == {n: saved['inputs']['targets'][n]['bytes'] for n in packet.STRICT}
    assert '[redacted]' in saved['negative_controls']['flattened_getter']['scores'][packet.NAME]['errors'][0]


def test_project_window_stock_compiler_replay():
    if not (packet.score.IDO / 'cc').exists():
        pytest.skip('stock IDO unavailable for exact compiler replay')
    assert packet.verify() == json.loads((HERE / 'proof.json').read_text())
