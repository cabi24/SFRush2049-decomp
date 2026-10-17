"""Small complete replay of the pause-title research packet."""
import importlib.util
from pathlib import Path


def test_pause_title_research():
    root = Path(__file__).resolve().parents[2]
    path = root / 'cloud/work/frontier/mat3_ui_pause_title_20261017/verify.py'
    spec = importlib.util.spec_from_file_location('pause_title_research', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    result = module.verify()
    assert result['members']['voice_stop'] == 'MATCH'
    assert result['new_accepted_bytes'] == 0
