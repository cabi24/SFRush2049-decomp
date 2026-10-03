"""Regression tests for NONMATCH cylinder-predicate research, not coverage."""
import importlib.util
from pathlib import Path
import shutil
import subprocess
import sys
import pytest
HERE=Path(__file__).resolve().parents[2]/'cloud/work/dot_cylinder_predicate'

def test_native_host_differential():
    if not shutil.which('cc'): pytest.skip('host C compiler unavailable')
    proc=subprocess.run([sys.executable,str(HERE/'verify_semantics.py')],capture_output=True,text=True)
    assert proc.returncode==0,proc.stdout+proc.stderr
    assert '"cases": 7620' in proc.stdout

def test_host_semantics_ubsan(tmp_path):
    if not shutil.which('cc'): pytest.skip('host C compiler unavailable')
    out=tmp_path/'host'
    subprocess.run(['cc','-std=c99','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-DHOST_MAIN','-fsanitize=undefined',str(HERE/'host_semantics.c'),'-o',str(out)],check=True)
    result=subprocess.run([str(out)],capture_output=True,text=True)
    assert result.returncode==0,result.stdout+result.stderr
    assert 'PASS 20000' in result.stdout
