"""One inexpensive packet test; no GNU, host matrices or compiler traces in CI."""
import importlib.util
from pathlib import Path


def test_resource_callbacks():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_b_resource_callbacks_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('b_resource_callbacks', path)
    packet = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(packet)
    packet.verify()
