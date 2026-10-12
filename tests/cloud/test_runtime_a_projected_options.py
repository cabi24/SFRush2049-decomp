"""One bounded NONMATCH regression and preserved-helper/data/GNU check."""
import importlib.util
from pathlib import Path
import shutil

import pytest


def test_runtime_a_projected_options_research():
    root = Path(__file__).resolve().parents[2]
    if not (root/'assets/us/data.bin').is_file():
        pytest.skip('private image-A asset absent')
    for tool in ('mips-linux-gnu-as', 'mips-linux-gnu-ld'):
        if not shutil.which(tool):
            pytest.skip('MIPS binutils absent: ' + tool)
    path = root/'cloud/work/frontier/dot_runtime_a_projected_options_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('projected_options_research', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    result = module.verify()
    assert result['new_match_bytes'] == 0
