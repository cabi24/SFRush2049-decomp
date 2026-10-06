import importlib.util,json,os
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parent

def test_packet():
 reference=os.environ.get('SFRUSH_REFERENCE_ROOT')
 if not reference:pytest.skip('SFRUSH_REFERENCE_ROOT required')
 spec=importlib.util.spec_from_file_location('source_object_verify_test',ROOT/'verify.py')
 module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
 result=module.verify(Path(reference))
 assert result==json.loads((ROOT/'verification.json').read_text())
 assert result['counterexamples']==[{'count':5,'active_owners':list(range(5))},{'count':6,'active_owners':list(range(6))}]
