"""One cheap input-bound static match check; deep behavior replay stays opt-in."""
import importlib.util
from pathlib import Path


def test_static_1558c_match():
    path = Path(__file__).resolve().parents[2] / 'cloud/work/frontier/dot_static_1558c_20261013/verify.py'
    spec = importlib.util.spec_from_file_location('verify_static1558c', path)
    packet = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(packet)
    packet.verify()
