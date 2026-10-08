"""Compiler-free portable tests for the native selector-address research."""
import importlib.util
import json
import os
from pathlib import Path
import pytest

HERE = Path(__file__).resolve().parents[2] / 'cloud/work/runtime_b_aa8c_selector_bounds_20261006'


def test_selector_replay():
    root = os.environ.get('SFRUSH_REFERENCE_ROOT')
    if root is None:
        # The installed packet is cloud/work/<name>, with ROOT three levels up.
        candidate = HERE.parents[2]
        if (candidate / 'tools/cloud/score.py').is_file():
            root = candidate
        else:
            pytest.skip('SFRUSH_REFERENCE_ROOT or repository-installed packet required')
    spec = importlib.util.spec_from_file_location('selector_packet', HERE / 'verify.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    fresh = module.verify(Path(root).resolve())
    receipt = json.loads((HERE / 'verification.json').read_text())
    assert fresh == receipt
