"""One cheap current-input replay; shared conftest handles missing IDO."""
import importlib.util
from pathlib import Path


def test_runtime_a_profile_owner():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_a_profile_owner_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('profile_owner_verify',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    previous=module.score.ASM_DIR;result=module.verify()
    assert module.score.ASM_DIR==previous
    assert result['new_exact_code_bytes']==104
    assert result['preserved_exact_code_bytes']==2220
