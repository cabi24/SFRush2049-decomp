"""One cheap protocol-v2 receipt and strict static-match check."""
import importlib.util
from pathlib import Path


def test_static_10450_match():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_static_10450_20261013/verify.py'
    spec=importlib.util.spec_from_file_location('verify_static10450',path)
    packet=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(packet)
    packet.verify()
