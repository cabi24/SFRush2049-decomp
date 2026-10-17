"""Complete genuine-caller teardown proof, with both negative controls."""
import importlib.util
import json
from pathlib import Path
import shutil
import sys
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT/'cloud/work/frontier/mat3_ui_20261017'

def test_genuine_teardown_context(monkeypatch):
    spec = importlib.util.spec_from_file_location('ui_teardown_verify', PACKET/'verify.py')
    v = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(v)
    assert v.evidence() == json.loads((PACKET/'RECEIPT.json').read_text())
    if not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('MIPS binutils missing: mips-linux-gnu-ld')
    monkeypatch.setattr(sys, 'argv', ['verify.py'])
    v.main()
