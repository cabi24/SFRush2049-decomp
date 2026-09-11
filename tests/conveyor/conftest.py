import sys
from pathlib import Path

# Make `tools.conveyor` importable when pytest is run from the repo root.
REPO_ROOT = Path(__file__).resolve().parents[2]
if str(REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(REPO_ROOT))


import pytest


@pytest.fixture(autouse=True)
def _isolate_generated_symbol_layer(tmp_path, monkeypatch):
    """The generated data-symbol layer (build/m2c_datasyms.json) is a live
    artifact; tests must see the hand table only unless they install their
    own layer via disasm.DATASYMS_JSON."""
    from tools.conveyor.pipeline import disasm

    monkeypatch.setattr(disasm, "DATASYMS_JSON", tmp_path / "no-datasyms.json")
    monkeypatch.setattr(disasm, "_generated_cache", {})
