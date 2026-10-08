"""Compiler-independent service contract replay."""
import importlib.util
import os
from pathlib import Path
import pytest


def test_native_service_contracts():
    root = os.environ.get('SFRUSH_REFERENCE_ROOT')
    if not root:
        pytest.skip('SFRUSH_REFERENCE_ROOT is required for this standalone research packet')
    path = Path(__file__).with_name('verify_native_effects.py')
    spec = importlib.util.spec_from_file_location('native_service_contracts',path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    result = module.verify(Path(root).resolve())
    assert result['status'] == 'BOUNDED_ACTUAL_NATIVE_SERVICE_CHECKS'
    assert result['cases'] == 61
