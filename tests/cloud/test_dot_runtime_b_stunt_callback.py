"""One cheap replay of the image-B callback's bounded NONMATCH research."""
import importlib.util
from pathlib import Path
import shutil
import pytest


def test_stunt_callback_research():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_b_stunt_callback_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('dot_runtime_b_stunt_callback_verify', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.score.ido('cc')
    if not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('GNU MIPS linker missing')
    actual = module.verify()
    assert actual['claims'] == []
    assert actual['comparison']['differing'] == 2
    assert actual['gnu_differing_offsets'] == [0x398, 0x39C]
    assert actual['new_strict_verified_bytes'] == actual['new_image_landed_bytes'] == actual['new_rom_verified_bytes'] == 0
