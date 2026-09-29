"""007 callee-closure contract tests (contracts/closure-and-datasyms.md §1-§6)."""
import json
import shutil
import struct

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore
from tools.conveyor.pipeline import closure

BASE = closure.GAME_CODE_BASE
NOP = 0x00000000
JR_RA = 0x03E00008
HAS_AS = shutil.which("mips-linux-gnu-as") is not None


def _jal(target):
    return (3 << 26) | ((target >> 2) & 0x3FFFFFF)


def _j(target):
    return (2 << 26) | ((target >> 2) & 0x3FFFFFF)


def _image(*words):
    return b"".join(struct.pack(">I", w) for w in words)


# --- §1 decode -----------------------------------------------------------------

def test_decode_jal_and_j_only_with_page_rule():
    words = [_jal(0x80090000), _j(0x800A0004), 0x27BDFFE8, JR_RA, NOP]
    assert closure.decode_jumps(words, BASE) == [
        (BASE, 0x80090000), (BASE + 4, 0x800A0004)]


def test_decode_page_rule_uses_the_calling_pcs_256mb_page():
    # The 26-bit index cannot express the page; it comes from the pc.
    word = _jal(0x0FFFFFFC)                  # index only, page bits stripped
    assert closure.decode_jumps([word], 0x8FFFFFF0) == [(0x8FFFFFF0, 0x8FFFFFFC)]
    assert closure.decode_jumps([word], 0x9FFFFFF0) == [(0x9FFFFFF0, 0x9FFFFFFC)]
    # Region edge: a pc at the last word of a page still keys on its own page.
    assert closure.decode_jumps([word], 0x8FFFFFFC) == [(0x8FFFFFFC, 0x8FFFFFFC)]


# --- §1 candidate filter -------------------------------------------------------

def test_candidate_filter_static_known_and_dedup_by_first_discoverer():
    image = _image(
        _jal(0x80000100),        # static range: never a candidate
        _jal(BASE + 0x20),       # known: not a candidate
        _jal(BASE + 0x40),       # unknown in-blob: candidate
        _jal(BASE + 0x1000),     # beyond image: recorded (classifies invalid)
        JR_RA, NOP,
    )
    # (j/jal encode target>>2, so a decoded target is 4-aligned by
    # construction; `invalid: misaligned` is classify's defensive branch.)
    sources = [
        {"target_id": "b", "address": BASE + 0x8, "insn_count": 3},   # sees 0x40 second
        {"target_id": "a", "address": BASE, "insn_count": 6},
    ]
    found = closure.discover(sources, image, {BASE, BASE + 0x20},
                             BASE + len(image))
    assert list(found) == [BASE + 0x40, BASE + 0x1000]
    # Provenance is the first discoverer in (address, vaddr) order: "a".
    assert found[BASE + 0x40] == {"target_id": "a", "vaddr": BASE + 8}


# --- §2 outcome classes --------------------------------------------------------

def test_classify_each_outcome_class():
    image = _image(
        JR_RA, NOP,               # f0 @ BASE      (2 words)
        JR_RA, NOP,               # f1 @ BASE+8    (2 words)
        NOP, NOP, NOP, NOP,       # tail with no return -> scan_failure
    )
    extents = [(BASE, BASE + 8, "f0")]
    assert closure.classify(BASE + 8, image, extents) == (
        "registered", {"insn_count": 2})
    assert closure.classify(BASE + 4, image, extents) == (
        "inside_existing_extent", {"container": "f0"})
    assert closure.classify(BASE + 16, image, extents) == (
        "scan_failure", {"reason": "scan_overrun"})
    assert closure.classify(BASE + 2, image, extents)[0] == "invalid"
    assert closure.classify(BASE + 0x1000, image, extents)[0] == "invalid"


def test_classify_records_overlap_when_scan_swallows_a_registered_start():
    image = _image(
        NOP, NOP,                 # candidate @ BASE with no early return...
        JR_RA, NOP,               # ...runs into g @ BASE+8
    )
    extents = [(BASE + 8, BASE + 16, "g")]
    assert closure.classify(BASE, image, extents) == (
        "registered", {"insn_count": 4, "overlaps": ["g"]})


# --- §3-§5 fixpoint, caps, idempotency (fixture DB) ---------------------------

def _seed_db(path, image_len):
    conn = dbmod.connect(path)
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO n64_target (target_id,address,population,insn_count,"
            " target_o_sha,tier,gate_reason) VALUES ('root',?,'extracted',3,'x','raw_word',NULL)",
            (BASE,))
        conn.execute(
            "INSERT INTO n64_target (target_id,address,population,insn_count,"
            " target_o_sha,tier,gate_reason)"
            " VALUES ('conflicted',?,'extracted',2,'y','raw_word','extent_conflict:root')",
            (BASE + 4,))
        conn.execute(
            "INSERT INTO matrix_entry (target_id,candidate_id,flagset,toolkit_sha,score)"
            " VALUES ('root','cand','flags','tk',7)")
    return conn


@pytest.fixture
def chain(tmp_path):
    # root -> A -> B -> C ; C calls back into root (known) and into A's body.
    A, B, C = BASE + 0x0C, BASE + 0x18, BASE + 0x24
    image = _image(
        _jal(A), JR_RA, NOP,                    # root  @ BASE   (3)
        _jal(B), JR_RA, NOP,                    # A     @ +0x0C  (3)
        _jal(C), JR_RA, NOP,                    # B     @ +0x18  (3)
        _jal(BASE), _jal(A + 4), JR_RA, NOP,    # C     @ +0x24  (4)
    )
    img = tmp_path / "game_code.bin"
    img.write_bytes(image)
    conn = _seed_db(tmp_path / "conveyor.db", len(image))
    store = BlobStore(tmp_path / "blobs")
    return conn, store, img, (A, B, C)


@pytest.mark.skipif(not HAS_AS, reason="mips-linux-gnu-as absent")
def test_fixpoint_registers_chain_then_second_run_registers_zero(chain, tmp_path):
    conn, store, img, (A, B, C) = chain
    report = closure.run(conn, store, img, tmp_path / "r1.json")

    assert report["totals"]["registered"] == 3
    # C's call into A's body is discovered when C becomes a source (iter 4).
    assert [it["outcomes"] for it in report["iterations"]] == [
        {"registered": 1}, {"registered": 1}, {"registered": 1},
        {"inside_existing_extent": 1}]
    assert report["candidates"][f"{A:08X}"]["discovered_by"] == {
        "target_id": "root", "vaddr": f"{BASE:08X}"}
    assert report["candidates"][f"{A + 4:08X}"]["outcome"] == "inside_existing_extent"
    rows = conn.execute(
        "SELECT target_id,address,insn_count,gate_reason,tier,target_o_sha"
        " FROM n64_target WHERE gate_reason='discovered' ORDER BY address").fetchall()
    assert [(r["target_id"], r["insn_count"]) for r in rows] == [
        (f"func_{A:08X}", 3), (f"func_{B:08X}", 3), (f"func_{C:08X}", 4)]
    assert all(r["tier"] == "raw_word" and store.get(r["target_o_sha"]) for r in rows)
    assert conn.execute("SELECT count(*) FROM function_status"
                        " WHERE status='unmatched' AND target_id LIKE 'func_%'"
                        ).fetchone()[0] == 3
    # §5: no supersession side effect — root's evidence is intact.
    assert conn.execute("SELECT count(*) FROM matrix_entry").fetchone()[0] == 1
    assert not report["caps"]["iterations"]["hit"]
    assert not report["caps"]["registrations"]["hit"]

    before = {r["target_id"]: tuple(r) for r in conn.execute(
        "SELECT * FROM n64_target")}
    second = closure.run(conn, store, img, tmp_path / "r2.json")
    assert second["totals"]["registered"] == 0
    assert second["iterations"] == [{
        "iteration": 1, "sources": 4, "discovered": 1,
        "outcomes": {"inside_existing_extent": 1}}]
    after = {r["target_id"]: tuple(r) for r in conn.execute(
        "SELECT * FROM n64_target")}
    assert before == after
    assert conn.execute("SELECT count(*) FROM matrix_entry").fetchone()[0] == 1
    # Report is deterministic modulo its timestamp.
    r1, r2 = (json.loads((tmp_path / n).read_text()) for n in ("r1.json", "r2.json"))
    assert r1["stamp"]["image_sha"] == r2["stamp"]["image_sha"]
    assert list(r1["candidates"]) == sorted(r1["candidates"])


@pytest.mark.skipif(not HAS_AS, reason="mips-linux-gnu-as absent")
def test_iteration_cap_is_an_explicit_outcome(chain, tmp_path):
    conn, store, img, (A, B, C) = chain
    report = closure.run(conn, store, img, tmp_path / "r.json", max_iterations=1)

    assert report["caps"]["iterations"]["hit"] is True
    assert report["totals"] == {"registered": 1, "inside_existing_extent": 0,
                                "scan_failure": 0, "invalid": 0, "cap_hit": 1}
    assert report["candidates"][f"{B:08X}"] == {
        "discovered_by": {"target_id": f"func_{A:08X}", "vaddr": f"{A:08X}"},
        "iteration": 2, "outcome": "cap_hit", "reason": "iterations"}


@pytest.mark.skipif(not HAS_AS, reason="mips-linux-gnu-as absent")
def test_registration_cap_is_an_explicit_outcome(tmp_path):
    # One source calling three unknown leaves; cap at 2 registrations.
    L1, L2, L3 = BASE + 0x14, BASE + 0x1C, BASE + 0x24
    image = _image(
        _jal(L1), _jal(L2), _jal(L3), JR_RA, NOP,   # root (5 words)
        JR_RA, NOP, JR_RA, NOP, JR_RA, NOP,
    )
    img = tmp_path / "game_code.bin"
    img.write_bytes(image)
    conn = dbmod.connect(tmp_path / "conveyor.db")
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO n64_target (target_id,address,population,insn_count,"
            " target_o_sha,tier) VALUES ('root',?,'extracted',5,'x','raw_word')", (BASE,))
    report = closure.run(conn, store := BlobStore(tmp_path / "blobs"), img,
                         tmp_path / "r.json", max_registrations=2)

    assert report["caps"]["registrations"]["hit"] is True
    assert report["totals"]["registered"] == 2
    assert report["candidates"][f"{L3:08X}"]["outcome"] == "cap_hit"
    assert report["candidates"][f"{L3:08X}"]["reason"] == "registrations"
    assert conn.execute("SELECT count(*) FROM n64_target").fetchone()[0] == 3
    assert store.get(conn.execute(
        "SELECT target_o_sha FROM n64_target WHERE target_id=?",
        (f"func_{L1:08X}",)).fetchone()[0])


# --- amendment: inventory rows inside a discovered extent are suffixes --------

@pytest.mark.skipif(not HAS_AS, reason="mips-linux-gnu-as absent")
def test_inventory_row_inside_discovered_extent_becomes_extent_conflict(tmp_path):
    # root calls F @ BASE+0x0C; the inventory registered "late" @ F+8 (its
    # prologue scan skipped the hoisted lui/lw), so "late" is a suffix of F.
    F = BASE + 0x0C
    image = _image(
        _jal(F), JR_RA, NOP,                          # root (3)
        0x3C038013, 0x8C62E608, 0x27BDFFE8, JR_RA, NOP,  # F: lui, lw, addiu sp, jr, nop
    )
    img = tmp_path / "game_code.bin"
    img.write_bytes(image)
    conn = dbmod.connect(tmp_path / "conveyor.db")
    with dbmod.tx(conn):
        conn.execute("INSERT INTO n64_target (target_id,address,population,insn_count,"
                     "target_o_sha,tier) VALUES ('root',?,'extracted',3,'x','raw_word')", (BASE,))
        conn.execute("INSERT INTO n64_target (target_id,address,population,insn_count,"
                     "target_o_sha,tier,gate_reason)"
                     " VALUES ('late',?,'extracted',3,'y','raw_word','extent_repaired')", (F + 8,))
        conn.execute("INSERT INTO matrix_entry (target_id,candidate_id,flagset,toolkit_sha,score)"
                     " VALUES ('late','cand','flags','tk',9)")
    store = BlobStore(tmp_path / "blobs")

    report = closure.run(conn, store, img, tmp_path / "r.json")

    fid = f"func_{F:08X}"
    assert report["candidates"][f"{F:08X}"] == {
        "discovered_by": {"target_id": "root", "vaddr": f"{BASE:08X}"},
        "iteration": 1, "outcome": "registered", "insn_count": 5,
        "overlaps": ["late"], "target_id": fid}
    assert report["reclassified"] == {"late": {
        "container": fid, "address": f"{F + 8:08X}",
        "previous_gate_reason": "extent_repaired"}}
    row = conn.execute("SELECT gate_reason,target_o_sha FROM n64_target"
                       " WHERE target_id='late'").fetchone()
    assert row["gate_reason"] == f"extent_conflict:{fid}"
    assert row["target_o_sha"] == "y"                      # object untouched
    assert conn.execute("SELECT count(*) FROM matrix_entry").fetchone()[0] == 1

    # Idempotent: the second run registers nothing and reclassifies nothing.
    second = closure.run(conn, store, img, tmp_path / "r2.json")
    assert second["totals"]["registered"] == 0 and second["reclassified"] == {}


@pytest.mark.skipif(not HAS_AS, reason="mips-linux-gnu-as absent")
def test_populate_keeps_suffix_row_conflicted_against_discovered_extent(
        tmp_path, monkeypatch):
    """A later `matrix extract` must not undo the closure's suffix rule."""
    from tools.conveyor.pipeline import targets as T

    F = BASE + 0x0C
    image = _image(
        _jal(F), JR_RA, NOP,
        0x3C038013, 0x8C62E608, 0x27BDFFE8, JR_RA, NOP,
    )
    game_bin = tmp_path / "game_code.bin"
    game_bin.write_bytes(image)
    monkeypatch.setattr(T, "GAME_CODE_BIN", game_bin)
    T._image_cache.clear()
    inventory = [
        {"name": "root", "address": BASE, "category": "", "flags": "", "size": 12},
        {"name": "late", "address": F + 8, "category": "", "flags": "", "size": 12},
    ]
    monkeypatch.setattr(T, "load_work_inventory", lambda work_dir=None: inventory)
    monkeypatch.setattr(T, "index_asm_regions", lambda asm_dir=None: {})
    conn = dbmod.connect(tmp_path / "conveyor.db")
    store = BlobStore(tmp_path / "blobs")

    # Reproduce the older inventory state that closure must repair. Today's
    # populate can already recover this stranded head, so using it to seed
    # the DB would prevent closure from discovering F at all.
    with dbmod.tx(conn):
        for name, address in (("root", BASE), ("late", F + 8)):
            conn.execute(
                "INSERT INTO n64_target (target_id,address,population,insn_count,"
                "target_o_sha,tier) VALUES (?,?,'extracted',3,'old','raw_word')",
                (name, address))
    report = closure.run(conn, store, game_bin, tmp_path / "r.json")
    assert report["totals"]["registered"] == 1
    assert conn.execute("SELECT gate_reason FROM n64_target WHERE target_id='late'"
                        ).fetchone()[0] == f"extent_conflict:func_{F:08X}"
    T.populate(conn, store)                                  # re-extract
    assert conn.execute("SELECT gate_reason FROM n64_target WHERE target_id='late'"
                        ).fetchone()[0] == f"extent_conflict:func_{F:08X}"
    assert conn.execute("SELECT count(*) FROM n64_target WHERE gate_reason='discovered'"
                        ).fetchone()[0] == 1
