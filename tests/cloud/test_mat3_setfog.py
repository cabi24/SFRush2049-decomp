"""Exact fog-setter source boundary, helper preservation and extent proof."""
import importlib.util
import json
from pathlib import Path
import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_mat3_setfog_20261017'
spec=importlib.util.spec_from_file_location('mat3_setfog_packet',HERE/'replay.py')
packet=importlib.util.module_from_spec(spec);spec.loader.exec_module(packet)

def test_setfog_source_and_receipt():
    packet.verify_source_change();saved=json.loads((HERE/'proof.json').read_text())
    assert packet.inputs()==saved['inputs']
    assert packet.recipe()['claims']==['func_800A7480']
    assert saved['new_individually_strict_bytes']==136 and saved['accepted_image_bytes']==0
    assert saved['positive']['first']==saved['positive']['repeat']==saved['positive']['direct_index']
    for group in saved['positive'].values():
        assert group['layout']['emitted_functions']==sorted(packet.STRICT)
        assert group['layout']['root_frame_bytes'] is None
        assert all(group['scores'][n]['strict_match'] for n in packet.STRICT)
        assert {n:group['layout']['function_extents'][n]['bytes'] for n in packet.STRICT}=={n:saved['inputs']['targets'][n]['bytes'] for n in packet.STRICT}
    assert set(saved['host_semantic_controls'])=={'wrong_alpha','wrong_return','wrong_green_mask','wrong_min'}

def test_setfog_stock_compiler_replay():
    if not (packet.score.IDO/'cc').exists():pytest.skip('stock IDO unavailable for exact compiler replay')
    assert packet.verify()==json.loads((HERE/'proof.json').read_text())
