"""008 image layout map (contracts/image-layout-and-gate.md §1-§6)."""
import json

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.pipeline import blob_layout

BASE = blob_layout.BASE


def _db(tmp_path, rows):
    conn = dbmod.connect(tmp_path / "db.sqlite")
    with dbmod.tx(conn):
        for target_id, address, insns, gate in rows:
            conn.execute(
                "INSERT INTO n64_target (target_id,address,population,insn_count,"
                " target_o_sha,tier,gate_reason) VALUES (?,?,'extracted',?,'s','raw_word',?)",
                (target_id, address, insns, gate))
    return conn


def test_entries_cover_every_byte_with_opaque_runs(tmp_path):
    conn = _db(tmp_path, [("a", BASE + 8, 2, None), ("b", BASE + 24, 2, None)])
    items = blob_layout.entries(conn, 64)

    assert [(e["kind"], e["vaddr"] - BASE, e["size"]) for e in items] == [
        ("opaque", 0, 8), ("function", 8, 8),
        ("opaque", 16, 8), ("function", 24, 8), ("opaque", 32, 32)]
    assert sum(e["size"] for e in items) == 64


def test_conflicted_and_overrun_extents_are_excluded(tmp_path):
    conn = _db(tmp_path, [
        ("real", BASE, 2, None),
        ("suffix", BASE + 4, 1, "extent_conflict:real"),
        ("overrun", BASE + 16, 2, "scan_overrun"),
    ])
    items = blob_layout.entries(conn, 32)
    named = [e.get("target_id") for e in items if e["kind"] == "function"]
    assert named == ["real"]


def test_overlapping_extents_are_a_hard_error_naming_both(tmp_path):
    conn = _db(tmp_path, [("first", BASE, 4, None), ("second", BASE + 8, 2, None)])
    with pytest.raises(blob_layout.LayoutError, match="overlapping extents: second"):
        blob_layout.entries(conn, 32)


def test_extents_outside_the_image_are_dropped(tmp_path):
    conn = _db(tmp_path, [("inside", BASE, 2, None), ("past_end", BASE + 24, 4, None)])
    items = blob_layout.entries(conn, 32)
    assert [e.get("target_id") for e in items if e["kind"] == "function"] == ["inside"]


def test_regions_split_on_large_gaps_and_function_count(tmp_path):
    conn = _db(tmp_path, [("a", BASE, 2, None), ("b", BASE + 8 + 4096, 2, None)])
    items = blob_layout.entries(conn, 4096 + 32)
    grouped = blob_layout.regions(items, split_gap=4096, max_funcs=60)
    assert len(grouped) == 2
    assert grouped[0]["entries"][0]["target_id"] == "a"
    assert grouped[1]["entries"][0]["kind"] == "opaque"     # the gap starts it

    grouped = blob_layout.regions(items, split_gap=1 << 30, max_funcs=1)
    assert len(grouped) == 2                                # split on count

    for region in grouped:
        assert region["vaddr_end"] > region["vaddr_start"]
        assert region["flagset"] == blob_layout.DEFAULT_FLAGSET


def test_derive_is_deterministic_and_carries_the_image_hash(tmp_path):
    import hashlib

    image = tmp_path / "image.bin"
    image.write_bytes(bytes(64))
    conn = _db(tmp_path, [("a", BASE + 8, 2, None)])
    out = tmp_path / "map.json"

    first = blob_layout.derive(conn, image, out)
    text = out.read_text()
    second = blob_layout.derive(conn, image, out)

    assert first == second and out.read_text() == text   # no timestamp inside
    assert first["image"]["sha256"] == hashlib.sha256(bytes(64)).hexdigest()
    assert first["totals"]["functions"] == 1
    assert first["totals"]["function_bytes"] + first["totals"]["opaque_bytes"] == 64


def test_the_live_map_covers_the_whole_image():
    """Regression guard: the committed map must stay complete and ordered."""
    document = blob_layout.load()
    size = document["image"]["size"]
    cursor = blob_layout.BASE
    functions = 0
    for region in document["regions"]:
        for entry in region["entries"]:
            assert entry["vaddr"] == cursor, "gap or overlap in the map"
            cursor += entry["size"]
            functions += entry["kind"] == "function"
    assert cursor == blob_layout.BASE + size
    assert functions == document["totals"]["functions"]
