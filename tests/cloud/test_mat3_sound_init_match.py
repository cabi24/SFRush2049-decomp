"""One source/data-bound test; optional fresh stock IDO replay of the full group."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import pytest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/mat3_sound_init_match_20261017'

def test_sound_init_complete_group_and_controls(tmp_path):
    spec=importlib.util.spec_from_file_location('sound_init_replay',PACKET/'replay.py')
    replay=importlib.util.module_from_spec(spec);spec.loader.exec_module(replay)
    receipt=json.loads((PACKET/'verification.json').read_text())
    assert replay.source_bindings()==receipt['source_sha256']
    assert replay.target_bindings()==receipt['target_sha256']
    assert receipt['new_code_bytes']==1048 and receipt['prior_members']==24
    candidate=receipt['results']['candidate']
    assert len(candidate['all_members'])==25
    assert all(replay.complete(r) for r in candidate['all_members'].values())
    assert candidate['unclaimed_helper_residue_bytes']==8
    assert receipt['once_flag']['logical_bytes']==1
    assert receipt['once_flag']['verified_window_bytes']==4
    assert receipt['results']['without_count_conversion']['hud_setup']['differing']==24
    assert receipt['results']['external_once_flag']['hud_setup']['differing']==8
    assert set(receipt['results']['static_list_helper']['failed_old_members'])=={'func_800A370C'}
    for label in ['uninitialized_local_flag','wrong_initial_flag']:
        assert receipt['results'][label]['hud_setup']['unverified']
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
    if not (ido/'cc').is_file() or not shutil.which('mips-linux-gnu-objdump'):
        pytest.skip('IDO and MIPS binutils required for fresh replay; source/data receipt checks passed')
    assert replay.verify(tmp_path)==receipt
