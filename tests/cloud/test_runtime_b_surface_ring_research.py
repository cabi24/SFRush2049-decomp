"""Bounded stock controls; full GNU extents and missing-tool behavior."""
import importlib.util
import json
from pathlib import Path
import shutil
import sys
import pytest


def test_surface_ring_research():
    root=Path(__file__).resolve().parents[2]
    path=root/'cloud/work/frontier/dot_runtime_b_surface_ring_20261012/verify.py'
    prior=list(sys.path)
    try:
        spec=importlib.util.spec_from_file_location('surface_ring_research',path)
        v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
        expected=json.loads((path.parent/'verification.json').read_text())
        assert v.inputs()==expected['inputs']
        if not shutil.which('mips-linux-gnu-ld'):pytest.skip('GNU MIPS binutils missing')
        assert v.replay()==expected
        assert expected['claims']==[] and expected['new_strict_verified_bytes']==0
    finally:sys.path[:]=prior
