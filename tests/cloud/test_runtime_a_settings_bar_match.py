"""One cheap input/stock-toolchain replay for the complete settings-bar match."""
import importlib.util
from pathlib import Path
import shutil

import pytest


def test_settings_bar_match():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_a_settings_bar_match_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('settings_bar_match_proof', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.verify(inputs_only=True)
    if not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('MIPS binutils unavailable')
    # tests/conftest.py turns missing IDO into a skip, or a hard failure under
    # REQUIRE_TOOLCHAIN=1. Inputs are checked even without the compiler.
    result = module.verify()
    assert result['new_match_bytes'] == 772
    assert result['comparison']['differing'] == 0
    assert result['controls'] == {'missing_data_unverified': 6, 'corrupt_literal_rejected': True}
