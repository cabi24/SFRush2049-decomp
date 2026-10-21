"""Each runtime image's tail after text_end is data, not unregistered code.

Image A: .data 0x803AF978..0x803B92C8, .rodata ..0x803B9A50 (241 literals and
all 25 switch tables). Image B: .data 0x80393B34..0x80394D20, .rodata
..0x80394F70 (96 items, all 6 switch tables). Both images load at 0x8038A400,
so a call into one image's tail range from elsewhere is legitimate only when it
lands on a function entry of the other image. ovl_layout labels the tails as
"data" entries and coverage reports the images against function bytes first;
this test is the evidence for both.
"""
import json
import struct

import pytest

from tools.conveyor.pipeline import blob_layout, layout, ovl_image, ovl_layout

JR_RA = 0x03E00008
EXPECTED = {"a": (0x803AF978, 0x803B92C8, 0x803B9A50),
            "b": (0x80393B34, 0x80394D20, 0x80394F70)}


def _image(name):
    return ovl_layout.work_dir(name) / "image.bin"


pytestmark = pytest.mark.skipif(
    not all(_image(n).is_file() and ovl_layout.layout_path(n).is_file() for n in EXPECTED)
    or not blob_layout.IMAGE.is_file(),
    reason="needs the inflated runtime images, their layouts and the game image")


def _words(data):
    return struct.unpack(f">{len(data) // 4}I", data[:len(data) // 4 * 4])


def _jumps(words, vaddr, lo, hi):
    hits = []
    for i, word in enumerate(words):
        if word >> 26 in (2, 3):
            pc = vaddr + 4 * i
            target = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
            if lo <= target < hi:
                hits.append(target)
    return hits


def _entries(name):
    document = json.loads(ovl_layout.layout_path(name).read_text())
    return [e for region in document["regions"] for e in region["entries"]], document


@pytest.mark.parametrize("name", sorted(EXPECTED))
def test_the_layout_ends_with_the_data_and_rodata_entries(name):
    text_end, rodata, end = EXPECTED[name]
    entries, document = _entries(name)
    assert [(e["kind"], e.get("section"), e["vaddr"], e["vaddr"] + e["size"])
            for e in entries[-2:]] == [
        ("data", ".data", text_end, rodata), ("data", ".rodata", rodata, end)]
    assert entries[-3]["kind"] == "function"
    assert ovl_layout.RODATA_START[name] == rodata
    assert int(document["image"]["text_end"], 16) == text_end
    for table in document["switch_tables"]:
        start = int(table["vaddr"], 16)
        assert rodata <= start and start + table["size"] <= end


@pytest.mark.parametrize("name", sorted(EXPECTED))
def test_the_tail_has_no_returns_or_stack_frames(name):
    text_end, rodata, _end = EXPECTED[name]
    data = _image(name).read_bytes()
    tail = _words(data[text_end - 0x8038A400:])
    assert tail.count(JR_RA) == 0
    assert not [w for w in tail if w >> 16 == 0x27BD and w & 0x8000]   # addiu sp,sp,-N
    assert not [w for w in tail if w >> 16 in (0xAFBF, 0x8FBF)]        # sw/lw ra,N(sp)
    # .rodata follows the last .data string directly (NUL, then word padding).
    assert data[:rodata - 0x8038A400].rstrip(b"\x00")[-1:] != b"" \
        and data[rodata - 0x8038A400 - 1] == 0


@pytest.mark.parametrize("name", sorted(EXPECTED))
def test_no_code_jumps_into_the_tail(name):
    text_end, _rodata, end = EXPECTED[name]
    base = 0x8038A400
    own = _image(name).read_bytes()
    other = "b" if name == "a" else "a"
    other_entries = {e["vaddr"] for e in _entries(other)[0] if e["kind"] == "function"}
    assert _jumps(_words(own[:text_end - base]), base, text_end, end) == []
    elsewhere = _jumps(_words(blob_layout.IMAGE.read_bytes()), blob_layout.BASE, text_end, end)
    for region in layout.collect_regions().values():
        elsewhere += _jumps([int(w, 16) for w in region.words], region.vaddr, text_end, end)
    # The other image's code at the same addresses is its own (calls to its entries).
    other_text_end = int(ovl_image.STREAMS[other].text_end)
    other_words = _words(_image(other).read_bytes()[:other_text_end - base])
    for target in _jumps(other_words, base, text_end, end):
        assert target < other_text_end, f"image {other} jumps into {name}'s tail 0x{target:08X}"
    assert all(target in other_entries for target in elsewhere), \
        [hex(t) for t in elsewhere if t not in other_entries]
