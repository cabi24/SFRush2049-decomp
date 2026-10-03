"""R16: runtime-image target discovery (tools.conveyor.pipeline.ovl_targets)."""
import json
import struct
from pathlib import Path

import pytest

from tools.conveyor.pipeline import ovl_targets as ovl
from tools.conveyor.pipeline import targets

REPO = Path(__file__).resolve().parents[2]
JR_RA, NOP = 0x03E00008, 0


def _image(words):
    return b"".join(struct.pack(">I", w) for w in words)


def test_scan_extent_honours_an_explicit_base():
    img = _image([0x27BDFFE8, JR_RA, 0x27BD0018])
    assert targets.scan_extent(img, ovl.BASE, base=ovl.BASE) == 3
    with pytest.raises(ValueError):
        targets.scan_extent(img, ovl.BASE)          # not inside the game image


def test_discovery_splits_on_evidence_and_merges_bare_tiles():
    base = ovl.BASE
    jal = lambda target: (3 << 26) | ((target >> 2) & 0x3FFFFFF)
    words = [
        0x27BDFFE8, jal(base + 0x14), NOP, JR_RA, 0x27BD0018,   # 0x00 prologue (base)
        JR_RA, NOP,                                             # 0x14 called leaf
        JR_RA, NOP,                                             # 0x1C empty function
        0x24020001, JR_RA, NOP,                                 # 0x24 bare tile: merged
        0x27BDFFE8, JR_RA, 0x27BD0018,                          # 0x30 prologue
    ]
    found = ovl.discover(_image(words), game=b"")
    starts = [f["address"] - base for f in found["functions"]]
    assert starts == [0x00, 0x14, 0x1C, 0x30]
    assert found["functions"][2]["size"] == 0x14       # empty fn + merged tile
    assert [a - base for a in found["unproven_merges"]] == [0x24]
    assert found["text_end"] == base + len(words) * 4


@pytest.mark.parametrize("image", ["a", "b"])
def test_committed_targets_cover_their_manifest(image):
    out = REPO / "asm" / "us" / f"ovl_{image}"
    sums = dict(reversed(line.split("  ")) for line in
                (out / "SHA256SUMS").read_text().splitlines())
    assert set(sums) == {p.name for p in out.iterdir() if p.name != "SHA256SUMS"}
    extents = json.loads((out / "extents.json").read_text())
    assert extents["base"] == "0x8038A400"
    assert not extents["call_targets_inside_functions"]
    for fn in extents["functions"]:
        assert fn["evidence"], fn


def test_ovl_rom_compose_splices_bodies_and_keeps_the_data_tail(monkeypatch):
    from tools.conveyor.pipeline import ovl_rom

    base = ovl.BASE
    extents = {"base": f"0x{base:08X}", "text_end": f"0x{base + 16:08X}",
               "functions": [{"name": "f", "address": f"0x{base:08X}", "size": 8},
                             {"name": "g", "address": f"0x{base + 8:08X}", "size": 8}]}
    words = {"f": [0x11111111, 0x22222222], "g": [0x33333333, 0x44444444]}
    monkeypatch.setattr(ovl_rom, "load_targets", lambda image: (extents, {}, words))
    monkeypatch.setattr(ovl_rom.blob_splice, "link_function",
                        lambda obj, name, *a, **k: b"\xAA" * 8)
    original = _image([0x11111111, 0x22222222, 0x33333333, 0x44444444, 0xDA7A0000])

    built, spliced = ovl_rom.compose("b", original, {"g": "g.o"})
    assert built == original[:8] + b"\xAA" * 8 + original[16:]   # data tail carried through
    assert set(spliced) == {"g"}
    with pytest.raises(ovl_rom.OvlRomError, match="not a function"):
        ovl_rom.compose("b", original, {"h": "h.o"})
