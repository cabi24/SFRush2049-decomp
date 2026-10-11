"""One bounded protocol-v2 group regression; deep research is opt-in."""
import importlib.util
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_runtime_a_available_list_20261011'

def test_packet_inputs_and_strict_members():
    spec = importlib.util.spec_from_file_location('dot_runtime_a_available_list_20261011_quick', PACKET / 'quick_verify.py')
    packet = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(packet)
    packet.verify()
