"""One inexpensive, expected-NONMATCH research check with input-bound receipts."""
import importlib.util
from pathlib import Path

def test_callback_initializer_research():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_b_callback_init_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('callback_init_verify',path)
    packet=importlib.util.module_from_spec(spec);spec.loader.exec_module(packet)
    packet.verify()
