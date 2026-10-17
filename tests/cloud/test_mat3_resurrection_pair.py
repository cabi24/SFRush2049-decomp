"""Bounded protocol-v2 replay of the explicitly empty-claim research packet."""
import importlib.util
from pathlib import Path


def test_mat3_resurrection_pair():
    root = Path(__file__).resolve().parents[2]
    script = root / 'cloud/work/frontier/dot_mat3_resurrection_pair_20261017/verify.py'
    spec = importlib.util.spec_from_file_location('resurrection_pair_verify', script)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    result = module.verify(root)
    assert result['claims'] == []
    assert result['results']['natural']['scores']['camera_follow_path'] == 'MATCH'
    assert result['results']['natural']['scores']['func_800C4F68'] == '17/354 words differ'
