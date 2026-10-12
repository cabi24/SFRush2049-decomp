"""One cheap protocol-v2 whole-group proof; missing IDO uses shared conftest."""
import importlib.util
from pathlib import Path
import shutil
import pytest


def test_status_scene_full_member():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_b_status_scene_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('dot_b_status_scene_verify', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.require(module.inputs() == __import__('json').loads(path.with_name('verification.json').read_text())['inputs'], 'source/target receipt changed')
    module.score.ido('cc')
    if not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('GNU MIPS linker missing')
    result = module.verify()
    assert result['new_strict_verified_bytes'] == 532
    assert result['preserved_candidate_bytes'] == 0
    assert result['new_image_landed_bytes'] == result['new_rom_verified_bytes'] == 0
