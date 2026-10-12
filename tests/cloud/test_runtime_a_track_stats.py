"""One cheap input-bound replay, all ten complete members and causal controls."""
import importlib.util
from pathlib import Path
import sys

def test_runtime_a_track_stats():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_a_track_stats_20261012/verify.py'
    old_path=list(sys.path)
    spec=importlib.util.spec_from_file_location('track_stats_verify_test',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    previous=module.score.ASM_DIR
    try:result=module.verify()
    finally:sys.path[:]=old_path
    assert module.score.ASM_DIR==previous
    assert result['new_exact_code_bytes']==1184
    assert result['preserved_exact_code_bytes']==7512
