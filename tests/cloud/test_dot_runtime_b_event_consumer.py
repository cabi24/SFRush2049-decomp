"""One bounded packet replay, including full relocation and negative-control checks."""
import importlib.util
from pathlib import Path
import shutil

import pytest


def test_event_consumer_complete_match():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_b_event_consumer_20261011/verify.py'
    spec = importlib.util.spec_from_file_location('dot_runtime_b_event_consumer_verify', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    # tests/conftest.py converts score.ido's actual missing-tool exit into a
    # toolchain skip, and turns that into failure when REQUIRE_TOOLCHAIN=1.
    module.score.ido('cc')
    if not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('mips-linux-gnu binutils linker missing')
    result = module.verify()
    assert result['new_strict_verified_bytes'] == 1164
    assert result['new_image_landed_bytes'] == result['new_rom_verified_bytes'] == 0
