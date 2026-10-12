"""Fast stock-compiler replay of the one complete image-A callback packet."""
import importlib.util
from pathlib import Path


def test_menu_home_complete_match():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_a_menu_home_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('runtime_a_menu_home_packet', path)
    packet = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(packet)
    result = packet.run()
    assert result['verification']['comparison']['differing'] == 0
    assert result['verification']['complete_bytes'] == 428
    assert result['verification']['owned_bytes'] == 0
