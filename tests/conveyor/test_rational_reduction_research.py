"""Research regression tests; none asserts a binary match."""
import importlib.util
from pathlib import Path
import shutil
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/dot_rational_reduction'
spec=importlib.util.spec_from_file_location('rational_reduction_proof',HERE/'verify.py')
proof=importlib.util.module_from_spec(spec);spec.loader.exec_module(proof)

def test_complete_differential_replay():
    if not (proof.score.IDO/'cc').exists() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and GNU MIPS tools needed for complete replay')
    proof.main()

def test_unknown_instruction_rejected():
    with pytest.raises(AssertionError,match='unsupported'):
        proof.execute([0xffffffff],[0]*13)

def test_uninitialized_stack_read_rejected():
    with pytest.raises(AssertionError,match='uninitialized'):
        proof.execute([0x8fa2fffc],[0]*13)

def test_defined_conversion_domain_enforced():
    inputs=[proof.bits(float('nan'))]+[proof.bits(x) for x in [100,1,1,0,.0001,.1,.1,.1,.1,.1,.1,.1]]
    with pytest.raises(AssertionError,match='conversion domain'):
        proof.execute(proof.score.targets()[proof.NAME],inputs)
