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
spec=importlib.util.spec_from_file_location('slot_portable_proof',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def test_receipt_and_input_hashes():
    saved=json.loads((HERE/'verification.json').read_text())
    assert saved['claims'] == [] and saved['accepted_byte_gain'] == 0
    assert saved['status'].startswith('NONMATCH')
    for field,name in [('source_sha256','group.c'),('group_spec_sha256','group.json'),('verifier_sha256','verify.py'),('host_source_sha256','host.c')]:
        assert saved[field] == hashlib.sha256((HERE/name).read_bytes()).hexdigest()
    for path,digest in saved['input_hashes'].items():
        assert digest == hashlib.sha256(v.base_bytes(path)).hexdigest()
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
    if not (v.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    available=(ido/'cc').is_file() and all(shutil.which(t) for t in ['cc','mips-linux-gnu-ld'])
    if not available:
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1': pytest.fail('required toolchain unavailable')
        pytest.skip('required toolchain unavailable')
    result=subprocess.run([sys.executable,str(HERE/'verify.py')],cwd=ROOT,text=True,capture_output=True,check=True)
    assert v.portable(json.loads(result.stdout)) == v.portable(json.loads((HERE/'verification.json').read_text()))


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


def test_portability_keeps_packet_and_native_proof_strict():
    import copy
    import json
    saved = json.loads((HERE / 'verification.json').read_text())
    changed = copy.deepcopy(saved)
    for field in ():
        changed[field] = 'unrelated integration provenance'
    for name in ('src/blob/func_8008E06C.c','src/blob/func_80092BC8.c','src/blob/sfx_stop.c'):
        changed['input_hashes'][name] = 'different accepted context revision'
    assert v.portable(changed) == v.portable(saved)
    assert saved == json.loads((HERE / 'verification.json').read_text())
    for field in ['source_sha256', 'group_spec_sha256', 'verifier_sha256', 'host_source_sha256', 'flags', 'experiments', 'native', 'host', 'native_address_anchors', 'toolchain_sha256']:
        mutant = copy.deepcopy(changed)
        mutant[field] = 'proof drift'
        assert v.portable(mutant) != v.portable(saved), field


def test_archived_caller_binding_is_not_provenance():
    import copy
    saved = json.loads((HERE / 'verification.json').read_text())
    changed = copy.deepcopy(saved)
    changed['input_hashes']['cloud/work/tiny_A44/func_80092BF4.c'] = 'changed archive'
    assert v.portable(saved) != v.portable(changed)
