"""One bounded full-object replay; no private ROM or runtime fixture required."""
import importlib.util
from pathlib import Path
import shutil

import pytest


def test_initializer_complete_match():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_b_initializer_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('dot_runtime_b_initializer_verify', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.score.ido('cc')
    if not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('GNU MIPS linker missing')
    result = module.verify()
    assert result['new_strict_verified_bytes'] == 560
    assert result['preserved_or_integrated_bytes'] == 0
    assert result['new_image_landed_bytes'] == result['new_rom_verified_bytes'] == 0
