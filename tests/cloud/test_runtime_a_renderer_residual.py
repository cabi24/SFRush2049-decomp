"""One bounded stock-compiler/data replay; no raw image or compiler output retained."""
import importlib.util
from pathlib import Path

import pytest


def test_runtime_a_renderer_owned_data():
    root = Path(__file__).resolve().parents[2]
    if not (root / 'assets/us/data.bin').is_file():
        pytest.skip('private image-A asset absent')
    path = root / 'cloud/work/frontier/dot_runtime_a_renderer_residual_20261011/verify.py'
    spec = importlib.util.spec_from_file_location('renderer_owned_data', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.verify()
