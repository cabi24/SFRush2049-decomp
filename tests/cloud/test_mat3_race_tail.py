"""One bounded protocol-v2 replay; shared conftest handles absent IDO."""
import importlib.util
from pathlib import Path


def test_mat3_race_tail():
    root=Path(__file__).resolve().parents[2]
    script=root/'cloud/work/frontier/dot_mat3_race_tail_20261017/verify.py'
    spec=importlib.util.spec_from_file_location('mat3_race_tail_verify',script)
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    result=module.verify(root)
    assert len(result['results']['candidate'])==25
    assert set(result['results']['candidate'].values())=={'MATCH'}
