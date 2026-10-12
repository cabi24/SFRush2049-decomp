"""One inexpensive input-bound group regression; shared missing-tool policy."""
import importlib.util
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_runtime_a_c93c_closure_20261012'


def test_packet_inputs_stock_gnu_and_private_abi():
    spec = importlib.util.spec_from_file_location('c93c_packet_verify', PACKET / 'verify.py')
    packet = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(packet)
    packet.verify(inputs_only=True)
    if shutil.which('mips-linux-gnu-ld') is None:
        pytest.skip('GNU MIPS binutils absent')
    packet.verify()
