"""One bounded source-first whole-function proof; missing tools use shared skips."""
import importlib.util
import json
from pathlib import Path
import shutil
import sys

import pytest


def test_runtime_b_trail():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_b_trail_20261012/verify.py'
    original_path = list(sys.path)
    try:
        spec = importlib.util.spec_from_file_location('dot_b_trail_packet', path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        assert module.inputs() == json.loads((path.parent / 'verification.json').read_text())['inputs']
        if not shutil.which('mips-linux-gnu-ld'):
            pytest.skip('mips-linux-gnu linker missing')
        result = module.verify()
        assert result['claims'] == ['func_8038CB20']
        assert result['new_strict_verified_bytes'] == 1332
    finally:
        sys.path[:] = original_path
