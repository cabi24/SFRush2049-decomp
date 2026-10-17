"""Source-bound full-extent proof for the N64 window initializer."""
import importlib.util
import json
from pathlib import Path
import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_mat3_window_init_20261017'
spec = importlib.util.spec_from_file_location('mat3_window_init_packet', HERE / 'replay.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)

def test_window_init_source_and_receipt():
    packet.verify_source_change()
    saved = json.loads((HERE / 'proof.json').read_text())
    assert packet.inputs() == saved['inputs']
    assert packet.recipe()['claims'] == ['arb_rate_set']
    assert saved['new_individually_strict_bytes'] == 312
    assert saved['accepted_image_bytes'] == 0
    assert saved['positive']['first'] == saved['positive']['repeat']
    for group in saved['positive'].values():
        assert group['layout']['emitted_functions'] == sorted(packet.STRICT)
        assert group['layout']['root_frame_bytes'] == 56
        assert all(group['scores'][n]['strict_match'] for n in packet.STRICT)
        assert {n: group['layout']['function_extents'][n]['bytes'] for n in packet.STRICT} == {n: saved['inputs']['targets'][n]['bytes'] for n in packet.STRICT}
    assert set(saved['host_semantic_controls']) == {'wrong_frontclip','wrong_alpha','wrong_return'}

def test_window_init_stock_compiler_replay():
    if not (packet.score.IDO / 'cc').exists(): pytest.skip('stock IDO unavailable for exact compiler replay')
    assert packet.verify() == json.loads((HERE / 'proof.json').read_text())
