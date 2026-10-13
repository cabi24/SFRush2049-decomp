"""One cheap input-bound strict replay; full GNU/host checks are explicit."""
import importlib.util
from pathlib import Path
import sys

def test_runtime_a_profile_controller():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_a_profile_controller_20261012/verify.py'
    old=list(sys.path);spec=importlib.util.spec_from_file_location('profile_controller_verify_test',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    previous=module.score.ASM_DIR
    try:result=module.verify()
    finally:sys.path[:]=old
    assert module.score.ASM_DIR==previous
    assert result['new_exact_code_bytes']==0
    assert result['preserved_exact_code_bytes']==7152
    assert result['checked'][module.NEW]['comparison']['differing']==31
