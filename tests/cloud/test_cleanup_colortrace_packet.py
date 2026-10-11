"""One bounded stock replay; deep compiler/semantic experiments are opt-in."""
import importlib.util
import json
from pathlib import Path
import shutil
import sys
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/frontier/dot_cleanup_colortrace_20261011"
spec = importlib.util.spec_from_file_location("cleanup_colortrace_packet", PACKET / "verify.py")
v = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = v
spec.loader.exec_module(v)

def test_cleanup_source_inputs_and_stock_full_extent():
    receipt = json.loads((PACKET / "verification.json").read_text())
    inputs, _ = v.inputs()
    assert all(receipt[k] == value for k, value in inputs.items())
    for tool in ("mips-linux-gnu-as", "mips-linux-gnu-ld"):
        if not shutil.which(tool):
            pytest.skip("MIPS binutils missing: " + tool)
    assert v.verify() == receipt
