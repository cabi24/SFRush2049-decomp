"""008 TU generation and the image byte-identity gate (contract §7-§15).

Feature 004's ROM hash gate was found to have been vacuous since December —
it hashed the wrong file and swallowed failures. These tests exist so this
gate is known to fail when it should.
"""
import hashlib
import json
import shutil
import struct

import pytest

from tools.conveyor.pipeline import blob_build, blob_layout, blob_tu

BASE = blob_layout.BASE
HAS_BINUTILS = all(shutil.which(t) for t in
                   ("mips-linux-gnu-as", "mips-linux-gnu-ld", "mips-linux-gnu-objcopy"))
JR_RA, NOP = 0x03E00008, 0x00000000


def _fixture(tmp_path, words, functions):
    """(document, image_path) for an image of `words` with `functions` =
    [(target_id, word_index, word_count)]."""
    image = tmp_path / "image.bin"
    image.write_bytes(b"".join(struct.pack(">I", w) for w in words))
    items, cursor = [], BASE
    for target_id, start, count in functions:
        vaddr = BASE + start * 4
        if vaddr > cursor:
            items.append({"kind": "opaque", "vaddr": cursor, "size": vaddr - cursor})
        items.append({"kind": "function", "vaddr": vaddr, "size": count * 4,
                      "target_id": target_id})
        cursor = vaddr + count * 4
    end = BASE + len(words) * 4
    if cursor < end:
        items.append({"kind": "opaque", "vaddr": cursor, "size": end - cursor})
    regions = blob_layout.regions(items)
    return {
        "image": {"path": str(image), "base": f"{BASE:08X}",
                  "size": len(words) * 4,
                  "sha256": hashlib.sha256(image.read_bytes()).hexdigest()},
        "totals": {"regions": len(regions), "functions": len(functions),
                   "function_bytes": sum(c * 4 for _, _, c in functions),
                   "opaque_runs": sum(1 for i in items if i["kind"] == "opaque"),
                   "opaque_bytes": sum(i["size"] for i in items
                                       if i["kind"] == "opaque")},
        "regions": regions,
    }, image


def test_generated_region_is_all_passthrough_with_addressed_sections(tmp_path):
    document, image = _fixture(tmp_path, [0x27BDFFE8, JR_RA, NOP, 0xDEADBEEF],
                               [("fn", 0, 3)])
    written, script = blob_tu.generate(document, image, tmp_path / "asm",
                                       tmp_path / "blob", spliced={})

    text = written[0].read_text()
    assert ".section .text.fn" in text and ".globl fn" in text
    assert "    .word 0x27BDFFE8" in text and "    .word 0x03E00008" in text
    # the trailing word is data we do not claim to understand
    assert '.incbin "' in text and ", 12, 4" in text
    placement = script.read_text()
    assert f". = 0x{BASE:08X};" in placement
    assert placement.index("*(.text.fn)") < placement.index("*(.blobdata.op_")


def test_generation_is_byte_stable(tmp_path):
    document, image = _fixture(tmp_path, [JR_RA, NOP], [("fn", 0, 2)])
    written, script = blob_tu.generate(document, image, tmp_path / "asm",
                                       tmp_path / "blob", spliced={})
    first = (written[0].read_text(), script.read_text())
    blob_tu.generate(document, image, tmp_path / "asm", tmp_path / "blob", spliced={})
    assert (written[0].read_text(), script.read_text()) == first


@pytest.mark.skipif(not HAS_BINUTILS, reason="mips binutils absent")
def test_all_passthrough_build_reproduces_the_image(tmp_path):
    words = [0x27BDFFE8, 0xAFBF0014, JR_RA, NOP, 0x12345678, 0x9ABCDEF0]
    document, image = _fixture(tmp_path, words, [("fn", 0, 4)])
    blob_tu.generate(document, image, tmp_path / "asm", tmp_path / "blob", spliced={})

    ok, sha, message = blob_build.build(
        document, image, tmp_path / "asm", tmp_path / "blob" / "blob.ld",
        work_dir=tmp_path / "work")

    assert ok, message
    assert sha == document["image"]["sha256"]


@pytest.mark.skipif(not HAS_BINUTILS, reason="mips binutils absent")
def test_a_single_altered_word_fails_the_gate_and_names_its_owner(tmp_path):
    words = [0x27BDFFE8, 0xAFBF0014, JR_RA, NOP]
    document, image = _fixture(tmp_path, words, [("target_fn", 0, 4)])
    blob_tu.generate(document, image, tmp_path / "asm", tmp_path / "blob", spliced={})
    source = tmp_path / "asm" / f"{document['regions'][0]['name']}.s"
    source.write_text(source.read_text().replace("0xAFBF0014", "0xDEADBEEF"))

    ok, sha, message = blob_build.build(
        document, image, tmp_path / "asm", tmp_path / "blob" / "blob.ld",
        work_dir=tmp_path / "work")

    assert not ok and sha != document["image"]["sha256"]
    assert "offset 4" in message and "target_fn" in message
    assert "afbf0014" in message and "deadbeef" in message


@pytest.mark.skipif(not HAS_BINUTILS, reason="mips binutils absent")
def test_opaque_data_is_reproduced_verbatim(tmp_path):
    """20.5% of the image is data we never decompiled; it must survive the
    round trip untouched."""
    words = [0xCAFEBABE, 0x00000000, 0xFFFFFFFF, JR_RA, NOP]
    document, image = _fixture(tmp_path, words, [("fn", 3, 2)])
    blob_tu.generate(document, image, tmp_path / "asm", tmp_path / "blob", spliced={})

    ok, _sha, message = blob_build.build(
        document, image, tmp_path / "asm", tmp_path / "blob" / "blob.ld",
        work_dir=tmp_path / "work")
    assert ok, message


def test_build_refuses_an_image_that_does_not_match_the_map(tmp_path):
    document, image = _fixture(tmp_path, [JR_RA, NOP], [("fn", 0, 2)])
    document["image"]["sha256"] = "0" * 64
    with pytest.raises(blob_build.BuildError, match="re-derive the layout"):
        blob_build.build(document, image, tmp_path / "asm",
                         tmp_path / "blob" / "blob.ld", work_dir=tmp_path / "w")


def test_missing_region_source_is_reported_not_silently_skipped(tmp_path):
    document, image = _fixture(tmp_path, [JR_RA, NOP], [("fn", 0, 2)])
    with pytest.raises(blob_build.BuildError, match="blob_tu generate"):
        blob_build.build(document, image, tmp_path / "absent",
                         tmp_path / "blob" / "blob.ld", work_dir=tmp_path / "w")
