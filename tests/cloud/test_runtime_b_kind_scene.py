"""One cheap full two-member native/GNU replay; deep host/controls are opt-in."""
import importlib.util
import json
from pathlib import Path
import shutil
import pytest


def test_kind_scene_packet():
    root=Path(__file__).resolve().parents[2]
    path=root/'cloud/work/frontier/dot_runtime_b_kind_scene_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('kind_scene_verifier',path)
    v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
    v.require(v.inputs()==json.loads(path.with_name('verification.json').read_text())['inputs'],'input binding')
    v.score.ido('cc')
    if not shutil.which('mips-linux-gnu-ld'):pytest.skip('GNU MIPS linker missing')
    result=v.verify()
    assert result['new_strict_verified_bytes']==668
    assert result['preserved_candidate_bytes']==532
    assert result['new_owned_bytes']==40
    assert result['new_image_landed_bytes']==result['new_rom_verified_bytes']==0
