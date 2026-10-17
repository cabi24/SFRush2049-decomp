"""Complete group claims, strict relocation proof, and two source controls."""
import importlib.util
from pathlib import Path
import shutil
import pytest

ROOT = Path(__file__).resolve().parents[2]

def test_transition_real_allocator():
    if not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('MIPS binutils absent')
    path = ROOT/'cloud/work/frontier/mat3_ui_transition_20261017/verify.py'
    spec = importlib.util.spec_from_file_location('transition_real_allocator', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.main()
