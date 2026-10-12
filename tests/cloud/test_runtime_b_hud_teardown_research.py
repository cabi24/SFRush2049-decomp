"""One inexpensive input-bound research test; deep machine/GNU/host work is explicit."""
import importlib.util
from pathlib import Path


def test_hud_teardown_research():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_runtime_b_hud_teardown_20261012/verify.py'
    spec = importlib.util.spec_from_file_location('hud_teardown_proof', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.verify()
