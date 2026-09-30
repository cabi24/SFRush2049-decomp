"""Bounded head discovery on synthetic MIPS images (no cartridge data)."""
import json
import shutil
import struct

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore
from tools.conveyor.pipeline import closure, targets

BASE = targets.GAME_CODE_BASE
JR = targets.JR_RA
NOP = 0
FRAME = 0x27BDFFE8
RESTORE = 0x27BD0018
DATA = 0x00000001                 # reserved SPECIAL encoding on R4300


def image(*words):
    return struct.pack(f">{len(words)}I", *words)


def row(address, count):
    return {"address": address, "insn_count": count, "target_id": "known"}


@pytest.fixture
def decoder(monkeypatch):
    # Keep flow/discovery tests runnable without a binutils installation.
    def decodes(data, address, count):
        off = address - BASE
        return all(w != DATA for w, in struct.iter_unpack(
            ">I", data[off:off + count * 4]))
    monkeypatch.setattr(closure, "_decodes_as_code", decodes)


def test_fallthrough_head_cannot_consume_next_function():
    data = image(FRAME, NOP, FRAME, JR, RESTORE)
    assert targets.scan_head_extent(data, BASE, BASE + 8) == {
        "reason": "fallthrough_at_bound", "at": BASE + 8}


def test_two_returns_and_both_delay_slots_are_in_extent():
    # beq a0,zero,+3 -> second return; both branches restore the frame.
    data = image(FRAME, 0x10800003, NOP, JR, RESTORE, JR, RESTORE, NOP)
    proof = targets.scan_head_extent(data, BASE, BASE + len(data))
    assert proof["insn_count"] == 7
    assert proof["returns"] == [BASE + 12, BASE + 20]
    assert proof["reachable_words"] == 7


def test_head_running_into_data_is_refused(decoder):
    data = image(FRAME, DATA, JR, RESTORE)
    decisions = closure.find_heads(data, [])
    assert len(decisions) == 1
    assert decisions[0]["reason"] == "decodes_as_data"
    assert decisions[0]["decision"] == "refused"


def test_alternating_code_and_data_does_not_stop_the_sweep(decoder):
    data = image(FRAME, JR, RESTORE, DATA, FRAME, JR, RESTORE,
                 DATA, FRAME, JR, RESTORE, DATA)
    decisions = closure.find_heads(data, [])
    assert [(e["address"], e["insn_count"], e["decision"]) for e in decisions] == [
        (BASE, 3, "accepted"), (BASE + 16, 3, "accepted"),
        (BASE + 32, 3, "accepted")]
    assert "data_separator" in decisions[1]["evidence"]


def test_prologue_prefix_is_one_head_not_an_interior_extent_bound(decoder):
    # A referenced entry has two hoisted instructions before addiu sp.
    head = BASE + 8
    data = image(JR, NOP, 0x3C028012, 0x8C420000, FRAME, JR, RESTORE, head)
    decisions = closure.find_heads(data, [row(BASE, 2)])
    assert [(e["address"], e["insn_count"]) for e in decisions] == [(head, 5)]
    assert decisions[0]["evidence"]["prologue"] == [head + 8]


def test_called_interior_prologue_is_refused(decoder):
    head = BASE + 12
    jal = (3 << 26) | ((head >> 2) & 0x3FFFFFF)
    data = image(jal, NOP, 0x24020001, FRAME, JR, RESTORE)
    decisions = closure.find_heads(data, [row(BASE, 3)])
    assert len(decisions) == 1
    assert decisions[0]["reason"] == "incoming_fallthrough"


def test_known_function_bounds_candidate_and_is_never_absorbed(decoder):
    data = image(FRAME, NOP, JR, RESTORE)
    decisions = closure.find_heads(data, [row(BASE + 8, 2)])
    assert decisions[0]["reason"] == "fallthrough_at_bound"
    assert decisions[0]["bound"] == BASE + 8


def test_indirect_jump_requires_proven_destinations():
    data = image(FRAME, 0x00400008, NOP, JR, RESTORE)
    assert targets.scan_head_extent(data, BASE, BASE + len(data))["reason"] == "indirect_jump"


@pytest.mark.parametrize("branch", [0x10800007, 0x1080FFFE])
def test_branches_cannot_escape_either_extent_boundary(branch):
    data = image(branch, NOP, JR, NOP)
    assert targets.scan_head_extent(data, BASE, BASE + len(data))["reason"] == "escaping_branch"


def test_return_must_restore_stack():
    data = image(FRAME, JR, NOP)
    assert targets.scan_head_extent(data, BASE, BASE + len(data))["reason"] == "unbalanced_return"


def test_missing_delay_slot_is_not_a_complete_function():
    data = image(FRAME, JR)
    assert targets.scan_head_extent(data, BASE, BASE + len(data))["reason"] == "missing_delay_slot"


def test_branch_likely_annuls_stack_adjustment_on_untaken_arm():
    # beql a0,zero,+3. Taken arm restores in the slot; untaken arm restores
    # in its own return slot. Applying the likely slot on both arms is wrong.
    data = image(FRAME, 0x50800003, RESTORE, JR, RESTORE, JR, NOP)
    assert targets.scan_head_extent(data, BASE, BASE + len(data))["insn_count"] == 7


def test_switch_case_pointer_is_not_a_function_head(decoder):
    # j skips an internal basic block; a switch table points at that block.
    label = BASE + 16
    jump = (2 << 26) | (((BASE + 24) >> 2) & 0x3FFFFFF)
    data = image(FRAME, jump, NOP, NOP, 0x24020001, NOP, JR, RESTORE, label)
    candidates = closure.head_candidates(data, [])
    assert [e["address"] for e in candidates] == [BASE]


@pytest.mark.skipif(shutil.which("mips-linux-gnu-objdump") is None,
                    reason="requires MIPS binutils")
def test_real_decoder_rejects_reserved_word():
    assert not closure._decodes_as_code(image(DATA), BASE, 1)
    assert closure._decodes_as_code(image(JR, NOP), BASE, 2)


@pytest.mark.skipif(shutil.which("mips-linux-gnu-as") is None,
                    reason="requires MIPS binutils")
def test_registration_is_opt_in_unmatched_and_idempotent(tmp_path, decoder):
    path = tmp_path / "image.bin"
    path.write_bytes(image(FRAME, JR, RESTORE, NOP))
    conn = dbmod.connect(tmp_path / "db")
    store = BlobStore(tmp_path / "blobs")
    dry = closure.heads(conn, store, path)
    assert dry["registered"] == []
    assert conn.execute("SELECT count(*) FROM n64_target").fetchone()[0] == 0
    report_path = tmp_path / "heads.json"
    first = closure.heads(conn, store, path, apply=True, report_path=report_path)
    name = f"func_{BASE:08X}"
    assert first["registered"] == [name]
    r = conn.execute("SELECT population,insn_count,target_o_sha,gate_reason"
                     " FROM n64_target WHERE target_id=?", (name,)).fetchone()
    assert (r["population"], r["insn_count"], r["gate_reason"]) == ("extracted", 3, "discovered")
    assert store.get(r["target_o_sha"]).is_file()
    assert conn.execute("SELECT status FROM function_status WHERE target_id=?",
                        (name,)).fetchone()[0] == "unmatched"
    assert json.loads(report_path.read_text())["registered"] == [name]
    assert closure.heads(conn, store, path, apply=True)["registered"] == []


@pytest.mark.skipif(shutil.which("mips-linux-gnu-as") is None,
                    reason="requires MIPS binutils")
def test_failed_registration_rolls_back_the_entire_pass(tmp_path, decoder, monkeypatch):
    path = tmp_path / "image.bin"
    path.write_bytes(image(FRAME, JR, RESTORE, FRAME, JR, RESTORE))
    conn = dbmod.connect(tmp_path / "db")
    store = BlobStore(tmp_path / "blobs")
    assemble = targets.assemble_words
    calls = []

    def fail_second(*args):
        calls.append(args)
        if len(calls) == 2:
            raise RuntimeError("synthetic assembler failure")
        return assemble(*args)

    monkeypatch.setattr(targets, "assemble_words", fail_second)
    with pytest.raises(RuntimeError, match="synthetic assembler failure"):
        closure.heads(conn, store, path, apply=True)
    assert conn.execute("SELECT count(*) FROM n64_target").fetchone()[0] == 0
    assert conn.execute("SELECT count(*) FROM function_status").fetchone()[0] == 0
