"""One cheap replay of the inherited complete image-B initializer delivery."""
import importlib.util
from pathlib import Path
import shutil
import pytest


def test_position_initializer_inherited_match():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_b_position_initializer_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('dot_runtime_b_position_initializer_verify', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.score.ido('cc')
    if not shutil.which('mips-linux-gnu-ld') or not shutil.which('cc'):
        pytest.skip('GNU MIPS linker or host C compiler missing')
    result = module.verify()
    assert result['new_strict_verified_bytes'] == 0
    assert result['inherited_strict_verified_bytes'] == 340
    assert result['preserved_or_integrated_bytes'] == 0
    assert result['new_image_landed_bytes'] == result['new_rom_verified_bytes'] == 0
