"""One bounded protocol-v2 replay; conftest handles absent IDO."""
import importlib.util
from pathlib import Path


def test_mat3_res_path_bounds():
    root = Path(__file__).resolve().parents[2]
    script = root / 'cloud/work/frontier/dot_mat3_res_path_bounds_20261017/verify.py'
    spec = importlib.util.spec_from_file_location('res_path_bounds_verify', script)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    result = module.verify(root)
    assert result['results']['candidate'] == 'MATCH'
