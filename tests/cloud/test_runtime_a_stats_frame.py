"""One input-bound complete-group proof with shared missing-IDO skips."""
import importlib.util
from pathlib import Path


def test_runtime_a_stats_frame():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_a_stats_frame_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('stats_frame_verify',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    previous=module.score.ASM_DIR;result=module.verify()
    assert module.score.ASM_DIR==previous
    assert result['new_exact_code_bytes']==1856
