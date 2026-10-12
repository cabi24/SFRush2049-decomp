"""One bounded stock/data replay; optional host/GNU experiments stay in packet."""
import importlib.util
from pathlib import Path
import pytest


def test_runtime_a_file_menu_writer_bounds():
    root=Path(__file__).resolve().parents[2]
    if not (root/'assets/us/data.bin').is_file():
        pytest.skip('private image-A asset absent')
    path=root/'cloud/work/frontier/dot_runtime_a_writer_bounds_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('file_menu_writer_bounds',path)
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.verify()
