"""One cheap input-bound replay; shared conftest handles missing stock IDO."""
import importlib.util
from pathlib import Path


def test_runtime_a_delete_profile():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_a_delete_profile_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('delete_profile_verify',path)
    v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
    previous=v.score.ASM_DIR;result=v.verify()
    assert v.score.ASM_DIR==previous
    assert result['new_exact_code_bytes']==0
    assert result['preserved_exact_code_bytes']==2324
    assert result['checked']['func_803A9088']['comparison']['differing']==15
