"""One cheap input-bound replay of all eight complete statistics bodies."""
import importlib.util
from pathlib import Path
import sys


def test_runtime_a_unlock_panel():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_a_unlock_panel_20261012/verify.py'
    old_path=list(sys.path)
    spec=importlib.util.spec_from_file_location('unlock_verify',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    previous=module.score.ASM_DIR
    try:result=module.verify()
    finally:sys.path[:]=old_path
    assert module.score.ASM_DIR==previous
    assert result['new_exact_code_bytes']==1664
    assert result['preserved_exact_code_bytes']==5136
