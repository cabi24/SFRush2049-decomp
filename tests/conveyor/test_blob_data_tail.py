"""The game image's tail after the last function is data, not unregistered code.

0x8010FD7C..0x801249F0 (85,108 B) is .data (initialised globals and every
string literal) followed by .rodata from 0x80123870 (float literals and switch
jump tables, starting with func_80086A50's table). blob_layout labels it as two
"data" entries and coverage reports game code against function bytes only;
this test is the evidence for both. If a later scan registers a function in
here, these assertions fail first.
"""
import struct

import pytest

from tools.conveyor.pipeline import blob_layout, layout, ovl_image

DATA_START = 0x8010FD7C
RODATA_START = 0x80123870
IMAGE_END = 0x801249F0

JR_RA = 0x03E00008

pytestmark = pytest.mark.skipif(
    not blob_layout.IMAGE.is_file() or not blob_layout.LAYOUT_JSON.exists(),
    reason="needs the extracted game image and the coordinator's blob layout")


def _words(data):
    return struct.unpack(f">{len(data) // 4}I", data[:len(data) // 4 * 4])


def _image():
    return blob_layout.IMAGE.read_bytes()


def _tail():
    return _words(_image()[DATA_START - blob_layout.BASE:])


def _jumps_into_tail(words, vaddr):
    """j/jal instructions whose target lies in the tail."""
    hits = []
    for i, word in enumerate(words):
        if word >> 26 in (2, 3):
            pc = vaddr + 4 * i
            target = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
            if DATA_START <= target < IMAGE_END:
                hits.append((hex(pc), hex(target)))
    return hits


def test_the_layout_ends_with_the_data_and_rodata_entries():
    entries = [e for region in blob_layout.load()["regions"] for e in region["entries"]]
    assert [(e["kind"], e.get("section"), e["vaddr"], e["vaddr"] + e["size"])
            for e in entries[-2:]] == [
        ("data", ".data", DATA_START, RODATA_START),
        ("data", ".rodata", RODATA_START, IMAGE_END)]
    assert entries[-3]["kind"] == "function"
    assert blob_layout.BASE + len(_image()) == IMAGE_END
    assert blob_layout.DATA_SECTIONS == ((".data", DATA_START), (".rodata", RODATA_START))


def test_the_tail_has_no_returns_or_stack_frames():
    """Every ordinary function returns with jr ra; game text has ~2.7 per KB."""
    tail = _tail()
    assert tail.count(JR_RA) == 0
    assert not [w for w in tail if w >> 16 == 0x27BD and w & 0x8000]   # addiu sp,sp,-N
    assert not [w for w in tail if w >> 16 in (0xAFBF, 0x8FBF)]        # sw/lw ra,N(sp)


def test_no_code_jumps_into_the_tail():
    image = _image()
    text = _words(image[:DATA_START - blob_layout.BASE])
    assert _jumps_into_tail(text, blob_layout.BASE) == []
    for region in layout.collect_regions().values():
        words = [int(w, 16) for w in region.words]
        assert _jumps_into_tail(words, region.vaddr) == [], region.name
    for name in ovl_image.IMAGES:
        path = blob_layout.REPO / "build" / "ovl" / name / "image.bin"
        if path.is_file():
            # j/jal keep the PC's top nibble; every image runs in KSEG0.
            assert _jumps_into_tail(_words(path.read_bytes()), 0x80000000) == [], name


def test_rodata_starts_with_the_first_functions_jump_table():
    image = _image()
    offset = RODATA_START - blob_layout.BASE
    assert image[offset - 8:offset] == b"Extra\x00\x00\x00"           # last .data string
    first = struct.unpack(">I", image[offset:offset + 4])[0]
    entries = [e for region in blob_layout.load()["regions"] for e in region["entries"]]
    owner = [e for e in entries if e["kind"] == "function"
             and e["vaddr"] <= first < e["vaddr"] + e["size"]]
    assert [e["target_id"] for e in owner] == ["func_80086A50"]
