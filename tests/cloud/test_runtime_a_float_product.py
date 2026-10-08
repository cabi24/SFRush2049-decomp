"""Fast source packet guards; full replay uses the optional installed toolchain."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import random
import shutil
import struct
import subprocess
import sys
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT/'cloud/work/runtime_a_float_product'
spec = importlib.util.spec_from_file_location('runtime_a_float_native', PACKET/'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)

def test_exact_normal_product_rounding_and_signed_zero():
    rng = random.Random(0x8039D300)
    for _ in range(10000):
        a,b = [(rng.randrange(2)<<31)|(rng.randrange(111,143)<<23)|rng.getrandbits(23) for _ in range(2)]
        fa,fb = [struct.unpack('>f', struct.pack('>I', x))[0] for x in (a,b)]
        expected = struct.unpack('>I',struct.pack('>f',fa*fb))[0]
        assert native.mul(a,b) == expected
    for a in (0,0x80000000):
        for b in (0,0x80000000,0x3f800000,0xbf800000):
            assert native.mul(a,b) == ((a^b)&0x80000000)

@pytest.mark.parametrize('bad',[1,0x80000001,0x7f800000,0xff800000,0x7fc00000,0x7f800001,0x7fbfffff])
def test_refuses_unmodeled_exceptional_arithmetic(bad):
    with pytest.raises(ValueError): native.mul(bad,0x3f800000)

def test_refuses_result_outside_normal_domain():
    with pytest.raises(ValueError): native.mul(0x00800000,0x00800000)
    with pytest.raises(ValueError): native.mul(0x7f7fffff,0x7f7fffff)

def test_invalid_backing_and_unknown_instruction_fail_closed():
    with pytest.raises(AssertionError): native.state(-1,0,(0,)*5,[0]*20)
    with pytest.raises(AssertionError): native.state(0,13,(0,)*5,[0]*20)
    with pytest.raises(AssertionError): native.load({},0x80001000,4)
    with pytest.raises(AssertionError): native.store({},0x80001000,4,0)
    with pytest.raises(AssertionError): native.run([0xffffffff],{},0)
    with pytest.raises(AssertionError): native.run([0],{},0)

def test_optimized_python_refuses_verification():
    p = subprocess.run([sys.executable,'-O',str(PACKET/'verify.py')],capture_output=True,text=True)
    assert p.returncode and 'Verification assertions must remain enabled' in p.stderr

def test_full_source_bound_replay():
    repo = Path(os.environ.get('RUSH_RECOVERY_ROOT',ROOT))
    ido = Path(os.environ.get('IDO_DIR',repo/'tools/cloud/ido'))
    if not (ido/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    env = dict(os.environ, PYTHONDONTWRITEBYTECODE='1')
    p = subprocess.run([sys.executable,str(PACKET/'verify.py'),'--repo',str(repo),'--check'],env=env,capture_output=True,text=True)
    assert p.returncode == 0, p.stdout+p.stderr


def test_receipt_binds_packet_verifier_and_native_words():
    receipt = json.loads((PACKET/'verification.json').read_text())
    assert receipt['verifier_sha256'] == hashlib.sha256((PACKET/'verify.py').read_bytes()).hexdigest()
    assert receipt['native_sha256'] == 'e0bf3d0731f618da18552bced27d621bec55be5f5cc274d1554c3a9656b046bc'
