"""One cheap protocol-v2 replay; no instrumented compiler or GNU dependency."""
import importlib.util
from pathlib import Path


def test_static_11910_research_receipt():
    root=Path(__file__).resolve().parents[2]
    path=root/'cloud/work/frontier/dot_static_11910_20261013/verify.py'
    spec=importlib.util.spec_from_file_location('static11910_verify',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    result=module.verify()
    assert result['status']=='CODE_EQUAL_STORAGE_BLOCKED' and result['inputs']['claims']==[]
    assert result['geometry']['candidate_bytes']==208
