"""One bounded input/stock-IDO packet check; shared missing-tool policy applies."""
import importlib.util
from pathlib import Path


def test_runtime_a_capacity_evidence():
    root=Path(__file__).resolve().parents[2]
    path=root/'cloud/work/frontier/dot_runtime_a_capacity_evidence_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('capacity_evidence_verify',path)
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    result=module.verify()
    assert result['new_match_bytes']==964
