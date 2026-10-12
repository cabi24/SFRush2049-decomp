"""One bounded source-first whole-function proof; missing tools use shared skips."""
import importlib.util
from pathlib import Path
import shutil
import sys

import pytest


def test_runtime_b_nearest_steering():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_b_nearest_steering_20261012/verify.py'
    original_path = list(sys.path)
    try:
        spec = importlib.util.spec_from_file_location('dot_b_nearest_steering_packet', path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        if not shutil.which('mips-linux-gnu-ld'):
            pytest.skip('mips-linux-gnu linker missing')
        result = module.verify()
        assert result['claims'] == ['func_8038DDDC']
        assert result['new_strict_verified_bytes'] == 684
    finally:
        sys.path[:] = original_path
