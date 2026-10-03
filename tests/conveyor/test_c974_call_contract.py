"""ABI-proof tests; no compiler or gameplay equivalence claim."""
import importlib.util
import json
from pathlib import Path
import pytest

ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location('c974_contract', ROOT/'cloud/work/c974_call_contract/audit.py')
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)

def test_current_manifest_authenticated_contract():
    assert MODULE.audit() == json.loads((ROOT/'cloud/work/c974_call_contract/evidence.json').read_text())

def test_addiu_register_and_signed_offset_precision():
    assert MODULE.check_addiu((9<<26)|(29<<21)|(6<<16)|316,6,29,316)
    assert not MODULE.check_addiu((9<<26)|(29<<21)|(5<<16)|316,6,29,316)
    assert MODULE.signed16(65535)==-1

def test_reject_incoming_float_dependency():
    # trunc.w.s f8,f12: first-use read with no local definition.
    with pytest.raises(AssertionError, match='incoming f12'):
        MODULE.prove_no_incoming_f12([(17<<26)|(16<<21)|(12<<11)|(8<<6)|13],base=0,end=4)

def test_likely_annulled_delay_slot_is_not_unconditional_definition():
    branch=(17<<26)|(8<<21)|(2<<16)|1  # bc1fl taken -> 8; annul on fallthrough
    define=(17<<26)|(16<<21)|(12<<6)|6
    read=(17<<26)|(16<<21)|(12<<11)|(8<<6)|13
    with pytest.raises(AssertionError, match='incoming f12'):
        MODULE.prove_no_incoming_f12([branch,define,read],base=0,end=12)

def test_reject_unreviewed_calls():
    with pytest.raises(AssertionError, match='control transfer'):
        MODULE.prove_no_incoming_f12([3<<26],base=0,end=4)


def test_reject_unreviewed_cop1_formats():
    with pytest.raises(AssertionError, match='COP1 format'):
        MODULE.f12_access((17<<26)|(17<<21))
    assert MODULE.f12_access((17<<26)|(0<<21)|(12<<11)) == (True, False)
    assert MODULE.f12_access((17<<26)|(4<<21)|(12<<11)) == (False, True)
