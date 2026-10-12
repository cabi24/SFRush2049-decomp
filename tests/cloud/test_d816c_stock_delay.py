"""One cheap research replay; shared conftest turns absent IDO into a required-tool failure."""
import importlib.util
from pathlib import Path


def test_d816c_stock_delay():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_d816c_stock_delay_20261011/verify.py'
    spec = importlib.util.spec_from_file_location('d816c_stock_delay', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.verify()
