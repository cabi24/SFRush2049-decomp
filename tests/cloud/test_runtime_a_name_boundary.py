"""One fast repository-only replay; no compiler or private ROM needed."""
import importlib.util
from pathlib import Path
import sys


def test_runtime_a_name_boundary():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_a_name_boundary_20261012/verify.py'
    old=list(sys.path)
    spec=importlib.util.spec_from_file_location('name_boundary_verify_test',path)
    module=importlib.util.module_from_spec(spec)
    try:
        spec.loader.exec_module(module)
        result=module.verify()
    finally:
        sys.path[:]=old
    assert result['claims']==[]
    assert result['new_strict_code_bytes']==0
    assert result['cases'][-1]['after_toggle_name_bytes']==19
    assert max(row['output_including_nul'] for row in result['formats'])==65
