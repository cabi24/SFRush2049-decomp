"""Runtime images: inflate (ovl_image), tiling and the image gate (ovl_layout)."""
import hashlib
import json
import shutil
import struct
import zlib

import pytest

from tools.conveyor.pipeline import blob_build, ovl_image, ovl_layout

BASE = ovl_image.BASE
HAS_BINUTILS = all(shutil.which(t) for t in
                   ("mips-linux-gnu-as", "mips-linux-gnu-ld", "mips-linux-gnu-objcopy"))
JR_RA, NOP = 0x03E00008, 0x00000000


def _words(words):
    return b"".join(struct.pack(">I", w) for w in words)


def _fixture(tmp_path, words, functions, text_end_words):
    """(image path, target dir) for a synthetic runtime image."""
    data = _words(words)
    image = tmp_path / "image.bin"
    image.write_bytes(data)
    target = tmp_path / "ovl_a"
    target.mkdir()
    (target / "extents.json").write_text(json.dumps({
        "image": "A", "size": len(data), "base": f"0x{BASE:08X}",
        "image_sha256": hashlib.sha256(data).hexdigest(),
        "text_end": f"0x{BASE + 4 * text_end_words:08X}",
        "functions": [{"name": n, "address": f"0x{BASE + 4 * s:08X}", "size": 4 * c,
                       "evidence": ["test"]} for n, s, c in functions]}))
    return image, target


def _derive(tmp_path, image, target):
    return ovl_layout.derive("a", image, target, output=tmp_path / "layout.json")


def test_image_name_from_targets_or_image():
    assert ovl_layout.image_name("asm/us/ovl_b") == "b"
    assert ovl_layout.image_name(image="a") == "a"
    with pytest.raises(ovl_layout.LayoutError):
        ovl_layout.image_name("asm/us/blob")
    with pytest.raises(ovl_layout.LayoutError):
        ovl_layout.image_name("asm/us/ovl_a", "b")


def test_tiling_is_complete_with_the_data_tail_opaque(tmp_path):
    words = [0x27BDFFE8, JR_RA, NOP, JR_RA, NOP, 0xDEADBEEF, 0x8038A400]
    image, target = _fixture(tmp_path, words, [("f0", 0, 3), ("f1", 3, 2)], 5)
    doc = _derive(tmp_path, image, target)
    entries = [e for r in doc["regions"] for e in r["entries"]]
    assert [(e["kind"], e["vaddr"] - BASE, e["size"]) for e in entries] == [
        ("function", 0, 12), ("function", 12, 8), ("opaque", 20, 8)]
    assert doc["image"]["base"] == "8038A400"
    assert doc["regions"][0]["name"] == "ovl_a_8038a400"
    assert json.loads((tmp_path / "layout.json").read_text()) == doc


@pytest.mark.parametrize("functions, text_end, why", [
    ([("f0", 0, 3), ("f1", 2, 2)], 4, "overlapping"),
    ([("f0", 0, 3)], 4, "text_end"),
    ([("f0", 0, 3), ("f0", 3, 1)], 4, "duplicate"),
    ([("f0", 0, 9)], 9, "past the image"),
])
def test_layout_refuses_to_guess(tmp_path, functions, text_end, why):
    image, target = _fixture(tmp_path, [NOP] * 6, functions, text_end)
    with pytest.raises(ovl_layout.LayoutError, match=why):
        _derive(tmp_path, image, target)


def test_layout_refuses_an_image_other_than_the_one_scanned(tmp_path):
    image, target = _fixture(tmp_path, [JR_RA, NOP], [("f0", 0, 2)], 2)
    image.write_bytes(_words([JR_RA, 1]))
    with pytest.raises(ovl_layout.LayoutError, match="sha256"):
        _derive(tmp_path, image, target)


def test_a_switch_table_inside_a_function_is_refused(tmp_path):
    # lui t6,0x8039; sltiu at,a0,2; lw t6,-0x5BF4(t6); jr t6 -> table at BASE+0xC
    words = [0x3C0E8039, 0x2C810002, 0x8DCEA40C, 0x01C00008, NOP, JR_RA, NOP, NOP]
    image, target = _fixture(tmp_path, words, [("f0", 0, 8)], 8)
    with pytest.raises(ovl_layout.LayoutError, match="switch table"):
        _derive(tmp_path, image, target)


def test_inflate_checks_slot_end_and_hash_and_ensure_extracts(tmp_path, monkeypatch):
    payload = _words([JR_RA, NOP, 0x12345678])
    comp = zlib.compressobj(9, zlib.DEFLATED, -15)
    stream = comp.compress(payload) + comp.flush()
    rom = bytearray(0x2000)
    offset = 0x100
    rom[offset:offset + len(stream)] = stream
    good = ovl_image.Stream("A", 0x80000410, offset, len(stream), len(payload),
                            hashlib.sha256(payload).hexdigest(), "", "test", BASE + 8)
    monkeypatch.setitem(ovl_image.IMAGES, "a", good)
    assert ovl_image.inflate_stream(bytes(rom), offset) == (payload, len(stream))
    assert ovl_image.inflate("a", bytes(rom)) == payload
    path = ovl_image.ensure("a", tmp_path, bytes(rom))
    assert path == ovl_image.image_path("a", tmp_path) and path.read_bytes() == payload
    path.write_bytes(b"stale")
    assert ovl_image.ensure("a", tmp_path, bytes(rom)).read_bytes() == payload
    monkeypatch.setitem(ovl_image.IMAGES, "a", good._replace(image_sha256="0" * 64))
    with pytest.raises(ovl_image.ImageError, match="not the recorded"):
        ovl_image.inflate("a", bytes(rom))
    monkeypatch.setitem(ovl_image.IMAGES, "a", good._replace(length=len(stream) + 4))
    with pytest.raises(ovl_image.ImageError, match="does not end"):
        ovl_image.inflate("a", bytes(rom))


@pytest.mark.skipif(not HAS_BINUTILS, reason="mips binutils absent")
def test_passthrough_gate_passes_and_a_wrong_body_fails_naming_its_owner(tmp_path, monkeypatch):
    words = [0x27BDFFE8, 0xAFBF0014, JR_RA, NOP, JR_RA, NOP, 0x12345678, 0x9ABCDEF0]
    image, target = _fixture(tmp_path, words, [("f0", 0, 4), ("f1", 4, 2)], 6)
    monkeypatch.setattr(ovl_layout, "work_dir", lambda image, build=None: tmp_path / "w")
    monkeypatch.setattr(ovl_layout, "asm_dir", lambda image, build=None: tmp_path / "w" / "asm")
    monkeypatch.setattr(ovl_layout, "script_path", lambda image, build=None: tmp_path / "w" / "a.ld")
    doc = _derive(tmp_path, image, target)
    ok, sha, message = ovl_layout.build(doc, {})
    assert ok, message
    assert sha == hashlib.sha256(image.read_bytes()).hexdigest()
    ok, _sha, message = ovl_layout.build(doc, {"f1": _words([JR_RA, 1])})
    assert not ok
    assert "owner:    f1" in message and f"vram 0x{BASE + 0x17:08X}" in message


@pytest.mark.skipif(not ovl_image.BASEROM.is_file(), reason="needs baserom.us.z64 (private input)")
@pytest.mark.skipif(not HAS_BINUTILS, reason="mips binutils absent")
@pytest.mark.parametrize("image", sorted(ovl_image.IMAGES))
def test_retail_all_passthrough_link_is_byte_identical(image, tmp_path, monkeypatch):
    path = ovl_image.ensure(image, tmp_path / "build")
    monkeypatch.setattr(ovl_layout, "work_dir", lambda i, build=None: tmp_path / i)
    monkeypatch.setattr(ovl_layout, "asm_dir", lambda i, build=None: tmp_path / i / "asm")
    monkeypatch.setattr(ovl_layout, "script_path", lambda i, build=None: tmp_path / i / "x.ld")
    doc = ovl_layout.derive(image, path, output=tmp_path / "layout.json")
    info = ovl_image.IMAGES[image]
    assert doc["totals"]["function_bytes"] == info.text_end - BASE
    ok, sha, message = ovl_layout.build(doc, {})
    assert ok, message
    assert sha == ovl_image.expected_sha256(image)


def test_base_reads_from_the_document():
    doc = {"image": {"base": "8038A400"}, "regions": []}
    assert blob_build.locate(doc, 0) == (None, None)
    assert "vram 0x8038A404" in blob_build.describe_difference(doc, b"\0" * 8, b"\0" * 4 + b"\1" * 4)


def test_publish_records_the_gated_image_and_withdraw_removes_it(tmp_path):
    built = tmp_path / "link.bin"
    built.write_bytes(b"gated image")
    sha = hashlib.sha256(b"gated image").hexdigest()
    entries = {"func_1": {"source_sha256": "x"}}
    path = ovl_layout.publish_rebuilt("a", built, sha, entries, {"func_1": b""}, tmp_path)
    assert path == ovl_image.rebuilt_path("a", tmp_path) and path.read_bytes() == b"gated image"
    record = json.loads(ovl_image.rebuilt_record("a", tmp_path).read_text())
    assert record["sha256"] == sha and record["bodies"] == ["func_1"]
    assert record["lock_sha256"] == ovl_image.lock_digest(entries) != ovl_image.lock_digest({})
    with pytest.raises(ovl_layout.LayoutError, match="changed after the gate"):
        ovl_layout.publish_rebuilt("a", built, "0" * 64, entries, {}, tmp_path)
    ovl_layout.withdraw_rebuilt("a", tmp_path)
    assert not path.exists() and not ovl_image.rebuilt_record("a", tmp_path).exists()
