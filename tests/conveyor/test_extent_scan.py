"""Contract tests for extracted-target control-flow extent scanning."""
import struct

import pytest

from tools.conveyor.pipeline import targets as T


BASE = T.GAME_CODE_BASE
NOP = 0x00000000
JR_RA = 0x03E00008
JR_T9 = 0x03200008


def _image(*words):
    return b"".join(struct.pack(">I", word) for word in words)


def _branch(pc_index, target_index, opcode=0x04):
    """Encode a direct I-format branch between word indices."""
    displacement = target_index - (pc_index + 1)
    return (opcode << 26) | (displacement & 0xFFFF)


def _bc1(pc_index, target_index):
    displacement = target_index - (pc_index + 1)
    return (0x11 << 26) | (0x08 << 21) | (displacement & 0xFFFF)


def test_early_jr_ra_before_furthest_forward_target_does_not_terminate():
    image = _image(
        _branch(0, 4),
        JR_RA,
        NOP,
        NOP,
        NOP,
        JR_RA,
        NOP,
    )
    assert T.scan_extent(image, BASE) == 7


def test_branch_targeting_jr_itself_terminates_at_that_jr():
    # Shared-return leaf: beqz jumps directly to the jr $ra, so at the jr
    # furthest == pc; the extent must end there, not run into what follows.
    image = _image(
        _branch(0, 3),  # beqz -> the jr at index 3
        NOP,
        NOP,
        JR_RA,
        NOP,
        JR_RA,          # next function's return; must NOT be reached
        NOP,
    )
    assert T.scan_extent(image, BASE) == 5


def test_jump_table_jr_non_ra_does_not_terminate():
    image = _image(JR_T9, NOP, JR_RA, NOP)
    assert T.scan_extent(image, BASE) == 4


def test_terminating_delay_slot_is_included():
    image = _image(JR_RA, 0x24020001)
    assert T.scan_extent(image, BASE) == 2


def test_backward_branch_does_not_extend_extent():
    image = _image(NOP, _branch(1, 0), JR_RA, NOP, JR_RA, NOP)
    assert T.scan_extent(image, BASE) == 4


def test_scan_overrun_at_16_kib_cap():
    image = _image(*([NOP] * (4096 + 1)))
    assert T.scan_extent(image, BASE) == "scan_overrun"


def test_scan_is_pure_for_same_image_and_address():
    image = _image(_bc1(0, 2), NOP, NOP, JR_RA, NOP)
    first = T.scan_extent(image, BASE)
    assert first == 5
    assert T.scan_extent(image, BASE) == first


@pytest.mark.skipif(not __import__("shutil").which("mips-linux-gnu-as"),
                    reason="mips-linux-gnu-as not available")
def test_repair_reuses_supersession_and_marks_nested_target_conflict(
        tmp_path, monkeypatch):
    from tools.conveyor.coordinator import db as dbmod
    from tools.conveyor.coordinator.store import BlobStore

    game_bin = tmp_path / "game_code.bin"
    game_bin.write_bytes(_image(
        _branch(0, 4), JR_RA, NOP, NOP, NOP, JR_RA, NOP,
    ))
    monkeypatch.setattr(T, "GAME_CODE_BIN", game_bin)
    T._image_cache.clear()
    inventory = [
        {"name": "outer", "address": BASE, "category": "", "flags": "", "size": 8},
        {"name": "inner", "address": BASE + 8, "category": "", "flags": "", "size": 20},
    ]
    monkeypatch.setattr(T, "load_work_inventory", lambda work_dir=None: inventory)
    monkeypatch.setattr(T, "index_asm_regions", lambda asm_dir=None: {})

    data = tmp_path / "data"
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    with dbmod.tx(conn):
        for name, address, count in (("outer", BASE, 2), ("inner", BASE + 8, 5)):
            conn.execute(
                "INSERT INTO n64_target"
                " (target_id,address,population,insn_count,target_o_sha,tier)"
                " VALUES (?,?,'extracted',?,?,'raw_word')",
                (name, address, count, name[0] * 64),
            )
            conn.execute(
                "INSERT INTO matrix_entry"
                " (target_id,candidate_id,flagset,toolkit_sha,score)"
                " VALUES (?,'candidate','-O2',?,9)", (name, "t" * 64),
            )

    first = T.populate(conn, store)
    outer = conn.execute(
        "SELECT insn_count,gate_reason FROM n64_target WHERE target_id='outer'"
    ).fetchone()
    inner = conn.execute(
        "SELECT insn_count,gate_reason FROM n64_target WHERE target_id='inner'"
    ).fetchone()
    assert (outer["insn_count"], outer["gate_reason"]) == (7, "extent_repaired")
    assert (inner["insn_count"], inner["gate_reason"]) == (5, "extent_conflict:outer")
    assert first["superseded_targets"] == 2 and first["purged_rows"] == 2
    assert conn.execute("SELECT COUNT(*) AS n FROM matrix_entry").fetchone()["n"] == 0

    second = T.populate(conn, store)
    assert second["extents"].get("repaired", 0) == 0
    assert second["superseded_targets"] == 0 and second["purged_rows"] == 0
