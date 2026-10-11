"""One cheap research replay; shared conftest turns absent IDO into a required-tool failure."""
import importlib.util
from pathlib import Path


def test_game_caller_cluster():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/dot_game_caller_cluster_20261011/verify.py'
    spec = importlib.util.spec_from_file_location('game_caller_cluster', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.verify()
