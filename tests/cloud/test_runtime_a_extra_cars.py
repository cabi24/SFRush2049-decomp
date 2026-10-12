"""One cheap input-bound replay of the complete statistics group."""
import importlib.util
from pathlib import Path


def test_runtime_a_extra_cars():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_a_extra_cars_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('extra_cars_verify',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    previous=module.score.ASM_DIR;result=module.verify()
    assert module.score.ASM_DIR==previous
    assert result['new_exact_code_bytes']==588
    assert result['preserved_exact_code_bytes']==4548
