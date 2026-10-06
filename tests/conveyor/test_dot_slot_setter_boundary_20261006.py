"""Research packet receipt, complete-byte proof, and behavioral regression gates."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT/'cloud/work/frontier/dot_slot_setter_boundary_20261006'


def test_receipt_and_input_hashes():
    saved=json.loads((HERE/'verification.json').read_text())
    assert saved['claims'] == [] and saved['accepted_byte_gain'] == 0
    assert saved['status'].startswith('NONMATCH')
    for field,name in [('source_sha256','group.c'),('group_spec_sha256','group.json'),('verifier_sha256','verify.py'),('host_source_sha256','host.c')]:
        assert saved[field] == hashlib.sha256((HERE/name).read_bytes()).hexdigest()
    for path,digest in saved['input_hashes'].items():
        assert digest == hashlib.sha256((ROOT/path).read_bytes()).hexdigest()
    for label in ['original_helpers','normalized_helpers','kept_inline_control']:
        funcs=saved['experiments'][label]['functions']
        for name in ['func_8008E06C','func_80092BC8']:
            assert funcs[name]['status'] == 'MATCH'
            assert funcs[name]['elf_bytes'] == funcs[name]['target_bytes'] == 44
        assert funcs['func_80092BF4']['strict']['differing'] == 22
        assert funcs['func_80092BF4']['target_bytes'] == 100
        assert funcs['func_80092BF4']['elf_bytes'] == (84 if label == 'original_helpers' else 80)
    assert saved['native']['executions'] == 267904
    assert saved['host']['c89_strict_aliasing_ubsan_bounds_cases'] == 2880
    assert saved['direct_native_callers'] == {n:[] for n in ['func_8008E06C','func_80092BC8','func_80092BF4']}


def test_fresh_complete_replay():
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
    available=(ido/'cc').exists() and all(shutil.which(t) for t in ['cc','mips-linux-gnu-ld'])
    if not available:
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1': pytest.fail('required toolchain unavailable')
        pytest.skip('required toolchain unavailable')
    result=subprocess.run([sys.executable,str(HERE/'verify.py')],cwd=ROOT,text=True,capture_output=True,check=True)
    assert json.loads(result.stdout) == json.loads((HERE/'verification.json').read_text())


def test_native_address_only_drift_is_rejected(monkeypatch):
    spec=importlib.util.spec_from_file_location('dot_setter_boundary',HERE/'verify.py')
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    original=module.score.image_symbols()
    assert module.verify_address_anchors() == json.loads((HERE/'verification.json').read_text())['native_address_anchors']
    for name in module.ADDRESS_ANCHORS:
        shifted=dict(original)
        shifted[name] += 0x01000000
        with monkeypatch.context() as patch:
            patch.setattr(module.score,'image_symbols',lambda:shifted)
            with pytest.raises(AssertionError,match='native address drift: '+name):
                module.verify_address_anchors()
