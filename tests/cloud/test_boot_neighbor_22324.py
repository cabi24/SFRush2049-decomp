"""One cheap replay: a stock-IDO 7.1 match and current-TU promotion compatibility."""
import importlib.util
import json
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/frontier/dot_boot_neighbors_20261011"
spec = importlib.util.spec_from_file_location("boot_neighbor_packet", PACKET / "verify.py")
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_match_receipt_and_current_tu(monkeypatch):
    monkeypatch.setattr(packet.score, "IDO", packet.score.IDO)
    monkeypatch.setattr(packet.score, "ASM_DIR", packet.score.ASM_DIR)
    expected = json.loads((PACKET / "verification.json").read_text())
    assert packet.inputs() == expected["inputs"]
    assert expected["newly_matched_bytes"] == 172
    assert expected["landed_rom_bytes_claimed"] == 0
    if not all(shutil.which(name) for name in
               ("mips-linux-gnu-as", "mips-linux-gnu-objdump", "mips-linux-gnu-readelf")):
        pytest.skip("MIPS binutils absent")
    actual = packet.run()  # Missing IDO is handled by the shared conftest.
    for key in ("result", "population", "function", "compiler", "flags", "inputs", "canonical",
                "native_extent_bytes", "object_symbol_bytes", "zero_alignment_bytes",
                "gnu_symbolic_relocations", "gnu_object_equal", "own_data_sections",
                "control_differing_words", "current_whole_tu_equal"):
        assert actual[key] == expected[key], key
    # The live TU may grow when a neighboring function is promoted; no tool/TU hash pin.
    assert actual["rom_tu_context"]["status"] == "ok"
