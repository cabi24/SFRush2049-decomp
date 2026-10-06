"""Portable complete semantic replay; IDO is deliberately not a dependency."""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import pytest

HERE=Path(__file__).resolve().parent

def test_complete_semantic_callback(tmp_path,monkeypatch):
    if not shutil.which('cc'):
        pytest.skip('host C compiler required for native/semantic-C comparison')
    root=os.environ.get('SFRUSH_REFERENCE_ROOT') or os.environ.get('RUSH_REFERENCE_ROOT')
    if root:root=Path(root).resolve()
    else:root=next((p for p in HERE.parents if (p/'tools/cloud/score.py').is_file() and (p/'asm/us/ovl_b').is_dir()),None)
    if root is None:
        pytest.skip('set SFRUSH_REFERENCE_ROOT to a target-bearing Git checkout')
    monkeypatch.setenv('TMPDIR',str(tmp_path))
    monkeypatch.syspath_prepend(str(HERE))
    spec=importlib.util.spec_from_file_location('aa8c_complete_verify',HERE/'verify.py')
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    assert module.comparable(module.replay(root)) == module.comparable(json.loads((HERE/'verification.json').read_text()))
