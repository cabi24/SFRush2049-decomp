"""One cheap input-bound semantic prerequisite replay; no matching claim."""
import importlib.util
from pathlib import Path
import json

def test_runtime_b_radial_impulse():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_b_radial_impulse_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('radial_impulse_verify',path);v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
    result=v.verify(False);expected=json.loads((v.HERE/'verification.json').read_text())
    v.require(result['inputs']==expected['inputs'],'input receipt mismatch')
    v.require(result['proof']==expected['proof'],'output proof mismatch')
