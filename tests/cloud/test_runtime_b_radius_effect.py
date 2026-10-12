"""One bounded input-bound NONMATCH packet replay; missing IDO uses shared skip."""
import importlib.util
from pathlib import Path
import shutil
import pytest


def test_runtime_b_radius_effect():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_b_radius_effect_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('runtime_b_radius_effect',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    module.selected_inputs()
    if shutil.which('mips-linux-gnu-ld') is None:
        pytest.skip('GNU MIPS binutils missing')
    module.verify()
