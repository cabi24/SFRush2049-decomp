"""One bounded source/input/stock-IDO check; shared missing-tool policy applies."""
import importlib.util
from pathlib import Path


def test_runtime_a_b560_trace():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_a_b560_trace_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('b560_trace_verify', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.verify()
