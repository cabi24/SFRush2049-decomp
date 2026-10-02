"""Research regression checks; success does not assert a byte match."""
import importlib.util
import os
from pathlib import Path
import shutil
import subprocess
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/dot_pool_object'
spec=importlib.util.spec_from_file_location('pool_object_proof',HERE/'verify.py')
proof=importlib.util.module_from_spec(spec)
spec.loader.exec_module(proof)

def test_host_semantics_ubsan(tmp_path):
    cc=shutil.which('cc')
    if not cc:pytest.skip('host C compiler unavailable')
    exe=tmp_path/'host'
    subprocess.run([cc,'-std=c99','-O1','-Wall','-Wextra','-Werror','-fsanitize=undefined',str(HERE/'host_test.c'),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)

def test_complete_target_candidate_differential():
    if not (proof.score.IDO/'cc').exists() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and GNU MIPS linker required')
    proof.run()

def test_interpreter_rejects_unknown_word():
    symbols=proof.score.image_symbols()
    symbols.update({n:proof.score.address_named(n) for n in ['D_80149788','D_80149450','func_800B362C','func_800A79F4','func_80094EC8']})
    with pytest.raises(AssertionError):
        proof.execute([0xffffffff],symbols,200,0,0,0,bytes(64),0,0,0)
