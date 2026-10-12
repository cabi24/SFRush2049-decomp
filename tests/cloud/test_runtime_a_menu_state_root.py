"""One bounded compiler/data replay; shared conftest owns missing-tool handling."""
import importlib.util
from pathlib import Path

import pytest


def test_runtime_a_menu_state_root():
    root = Path(__file__).resolve().parents[2]
    if not (root / 'assets/us/data.bin').is_file():
        pytest.skip('private image-A asset absent')
    path = root / 'cloud/work/frontier/dot_runtime_a_menu_state_root_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('menu_state_root_proof', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.verify()
