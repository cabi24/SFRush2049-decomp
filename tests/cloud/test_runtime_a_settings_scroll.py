"""One complete, input-bound research replay; missing IDO uses shared skips."""
import importlib.util
from pathlib import Path


def test_runtime_a_settings_scroll():
    path=Path(__file__).resolve().parents[2]/'cloud/work/frontier/dot_runtime_a_settings_scroll_20261012/verify.py'
    spec=importlib.util.spec_from_file_location('settings_scroll_verify',path)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    prior=module.score.ASM_DIR
    result=module.verify()
    assert module.score.ASM_DIR==prior
    assert result['inputs']['claims']==[]
    assert result['verification']['new_exact_bytes']==0
    assert result['verification']['comparison']['differing']==58
