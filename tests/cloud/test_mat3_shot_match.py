"""Strict full-closure proof for the reconstructed ShotToWorldCollision body."""
import importlib.util
import json
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_mat3_shot_match_20261017'
spec = importlib.util.spec_from_file_location('mat3_shot_match_packet', HERE / 'replay.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_shot_match_source_and_receipt():
    packet.verify_source_change()
    saved = json.loads((HERE / 'proof.json').read_text())
    assert packet.inputs() == saved['inputs']
    assert saved['new_individually_strict_bytes'] == 3580
    assert saved['accepted_image_bytes'] == 0
    assert len(packet.strict_names()) == 24
    assert saved['positive']['first'] == saved['positive']['repeat']
    assert all(saved['positive']['first']['scores'][n]['strict_match'] for n in packet.strict_names())
    assert 'func_800AC9BC' not in packet.recipe()['claims']
    assert all(n not in packet.recipe()['keep'] for n in packet.REMOVED)


def test_shot_match_stock_compiler_replay():
    if not (packet.score.IDO / 'cc').exists():
        pytest.skip('stock IDO unavailable for exact compiler replay')
    assert packet.verify() == json.loads((HERE / 'proof.json').read_text())
