"""The three w20c owned jump-table slots as committed, and logical-extent companions.

func_8001B9F8 owns, with co-owner func_8001BE14, lib_1a660's .rodata at
0x8002D8D0 (their two 6-entry jump tables, 0x30); func_80021548 owns lib_21210's
.rodata at 0x8002D930 (one table + IDO's 8-byte section pad, 0x20);
func_80023BDC owns the first 0x14 bytes of lib_22300's .rodata at 0x8002D950
(a 4-byte logical extent: func_80023E9C's table follows at 0x8002D964).

A logical-extent table is validated as a complete word-relocated table on
every build, so its passthrough companion spells each word `.word OWNER + off`.
"""
import hashlib
import json
from pathlib import Path

import pytest

from tools.conveyor.pipeline import owned_data as O

REPO = Path(__file__).resolve().parents[2]
SLOTS = [
    ("func_8001B9F8", ["func_8001B9F8", "func_8001BE14"], 0x8002D8D0, 0x30, False),
    ("func_80021548", ["func_80021548"], 0x8002D930, 0x20, False),
    ("func_80023BDC", ["func_80023BDC"], 0x8002D950, 0x14, True),
]


def test_committed_rows_match_their_companions_and_tus():
    rows = {r["owner"]: r for r in O.rom_slots(REPO, check_source=False)}
    for owner, members, address, size, logical in SLOTS:
        r = rows[owner]
        assert O.slot_members(r) == members and r["owner_section"] == ".rodata"
        assert r["container_vram"] + r["offset"] == address and r["size"] == size
        assert bool(r.get("logical_extent")) is logical
        companion = (REPO / r["passthrough_asm"]).read_text()
        assert ((".word " + owner + " + 0x") in companion) is logical
        if (REPO / r["source"]).is_file():
            assert companion == O.companion_source(r, REPO)
        text = (REPO / r["tu"]).read_text()
        if all(f'/{m}.s")' in text for m in members):
            assert text.count(O.companion_block(r)) == 1


def _repo(tmp_path, words, body_size=0x194):
    (tmp_path / "assets/us").mkdir(parents=True)
    data = b"\0" * 0x10 + b"".join(w.to_bytes(4, "big") for w in words)
    (tmp_path / "assets/us/data.bin").write_bytes(data)
    asm = tmp_path / "asm/us/nonmatchings/rom/lib_x"
    asm.mkdir(parents=True)
    (asm / "f.s").write_text(f"nonmatching f, {body_size:#x}\n\nglabel f\n"
                             "    /* 247DC 80023BDC 27BDFFD0 */  addiu      $sp, $sp, -0x30\n")
    row = {"owner": "f", "tu": "src/rom/lib_x.c", "source": "assets/us/data.bin",
           "owner_section": ".rodata", "offset": 0x10, "size": 4 * len(words),
           "passthrough_asm": "asm/us/nonmatchings/rom/lib_x/f.rodata.s",
           "alignment": 4, "logical_extent": True,
           "sha256": hashlib.sha256(data[0x10:]).hexdigest()}
    return row


def test_logical_companion_relocates_every_word_into_the_owner(tmp_path):
    row = _repo(tmp_path, [0x80023C90, 0x80023C9C])
    assert O.companion_source(row, tmp_path) == (
        ".section .rodata\n.balign 4\n.word f + 0xb4\n.word f + 0xc0\n")
    row16 = dict(row, logical_extent=False, alignment=16)
    assert ".incbin" in O.companion_source(row16, tmp_path)


@pytest.mark.parametrize("word", [0x80023BD8, 0x80023BDC + 0x194, 0x80023C92])
def test_logical_companion_refuses_a_word_outside_the_owner(tmp_path, word):
    with pytest.raises(O.OwnershipError):
        O.companion_source(_repo(tmp_path, [0x80023C90, word]), tmp_path)


def test_section_fits_admits_ido_padding_only_for_logical_extents():
    logical = {"size": 0x14, "logical_extent": True}
    plain = {"size": 0x20}
    assert O.section_fits(logical, 0x20) and O.section_fits(logical, 0x14)
    assert not O.section_fits(logical, 0x30)
    assert O.section_fits(plain, 0x20) and not O.section_fits(plain, 0x30)
    assert not O.section_fits({"size": 0x14}, 0x20)
