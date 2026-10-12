"""One cheap input-bound whole-group replay; shared conftest handles missing IDO."""
import importlib.util
from pathlib import Path


def test_runtime_a_product_pre():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_a_product_pre_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('product_pre_verify',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    before=module.score.ASM_DIR;result=module.verify()
    assert module.score.ASM_DIR==before
    assert result['new_exact_code_bytes']==404
    assert result['preserved_exact_code_bytes']==2264
