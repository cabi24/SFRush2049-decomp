"""One cheap input-bound replay of the complete image-B damage submission."""
import importlib.util
from pathlib import Path
import shutil
import pytest


def test_damage_complete_match():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_b_damage_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('dot_runtime_b_damage_verify', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.score.ido('cc')
    if not shutil.which('mips-linux-gnu-ld') or not shutil.which('cc'):
        pytest.skip('GNU MIPS linker or host C compiler missing')
    result = module.verify()
    assert result['new_strict_verified_bytes'] == 244
    assert result['preserved_or_integrated_bytes'] == 0
    assert result['new_image_landed_bytes'] == result['new_rom_verified_bytes'] == 0
