"""Reloc-aware extracted targets: the fix for scores that could never reach 0.

A raw-word target bakes absolute addresses into its words, so a compiled
candidate — which emits relocations with zeroed fields — always differed on
every call and global reference. Evidence:
specs/007-population-closure/research/reloc-scoring-finding.md.
"""
import shutil
import struct

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore
from tools.conveyor.pipeline import targets as T

HAS_AS = shutil.which("mips-linux-gnu-as") is not None
HAS_OBJDUMP = shutil.which("mips-linux-gnu-objdump") is not None
BASE = T.GAME_CODE_BASE

# addiu sp,sp,-24 / sw ra,20(sp) / jal <> / nop / lw ra,20(sp) / addiu sp,sp,24
# / jr ra / nop  — the shape of object_process_thunk, the function that
# exposed all of this.
THUNK = [0x27BDFFE8, 0xAFBF0014, 0x0C0296CF, 0x00000000,
         0x8FBF0014, 0x27BD0018, 0x03E00008, 0x00000000]
DERIVED = """glabel thunk
.L800A7DF0:
    addiu   $sp,$sp,-24
.L800A7DF4:
    sw      $ra,20($sp)
.L800A7DF8:
    jal     func_800A5B3C
.L800A7DFC:
    nop
.L800A7E00:
    lw      $ra,20($sp)
.L800A7E04:
    addiu   $sp,$sp,24
.L800A7E08:
    jr      $ra
.L800A7E0C:
    nop
"""


def test_synthetic_instructions_are_dropped_real_ones_kept():
    """m2c re-emits `lui`s to fix its own %hi/%lo binding. They are not ROM
    instructions; assembling them would give the target words the ROM never
    had. The real instruction is the last in each label block."""
    derived = (
        "glabel sample\n"
        ".L80090000:\n"
        "    lui     $t0,%hi(D_80140D70)\n"       # synthetic
        "    lw      $t8,%lo(D_80140D70)($t0)\n"  # real
        ".L80090004:\n"
        "    jr      $ra\n"
        ".L80090008:\n"
        "    nop\n"
    )
    asm = T.target_asm_from_derived(derived, "sample")

    body = [l for l in asm.splitlines() if l.startswith("    ")]
    assert body == ["    lw      $t8,%lo(D_80140D70)($t0)",
                    "    jr      $ra", "    nop"]
    assert ".L80090000:" in asm            # labels survive: branches need them
    assert asm.splitlines()[:5] == [
        ".set noreorder", ".set noat", ".section .text",
        ".globl sample", "sample:"]


@pytest.mark.skipif(not (HAS_AS and HAS_OBJDUMP), reason="mips binutils absent")
def test_assembled_target_carries_a_relocation_and_passes_the_gate():
    import tempfile
    from pathlib import Path

    out = Path(tempfile.mkdtemp()) / "t.o"
    T.assemble_text(T.target_asm_from_derived(DERIVED, "thunk"), out)

    from tools.conveyor.jobs import scoring
    relocs = scoring._parse_relocs(
        scoring._objdump(scoring._objdump_path(), "-r", str(out)))
    assert len(relocs) == 1                      # the jal
    ok, why = T.gate_target([f"{w:08X}" for w in THUNK], out)
    assert (ok, why) == (True, None)


@pytest.mark.skipif(not (HAS_AS and HAS_OBJDUMP), reason="mips binutils absent")
def test_gate_rejects_a_target_whose_words_do_not_match_the_rom():
    import tempfile
    from pathlib import Path

    out = Path(tempfile.mkdtemp()) / "t.o"
    T.assemble_text(T.target_asm_from_derived(DERIVED, "thunk"), out)
    wrong = list(THUNK)
    wrong[5] = 0x27BD0020                        # different stack adjust
    ok, why = T.gate_target([f"{w:08X}" for w in wrong], out)
    assert not ok and why.startswith("word_mismatch")


@pytest.mark.skipif(not (HAS_AS and HAS_OBJDUMP), reason="mips binutils absent")
def test_upgrade_supersedes_evidence_once_then_is_idempotent(tmp_path, monkeypatch):
    image = tmp_path / "game_code.bin"
    image.write_bytes(b"".join(struct.pack(">I", w) for w in THUNK))
    monkeypatch.setattr(T, "GAME_CODE_BIN", image)
    T._image_cache.clear()
    conn = dbmod.connect(tmp_path / "conveyor.db")
    store = BlobStore(tmp_path / "blobs")
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO n64_target (target_id,address,population,insn_count,"
            " target_o_sha,tier) VALUES ('thunk',?,'extracted',8,'rawsha','raw_word')",
            (BASE,))
        conn.execute(
            "INSERT INTO matrix_entry (target_id,candidate_id,flagset,toolkit_sha,score)"
            " VALUES ('thunk','cand','flags','tk',5)")

    asm = tmp_path / "thunk.s"
    asm.write_text(DERIVED)
    monkeypatch.setattr("tools.conveyor.pipeline.disasm.derive",
                        lambda *a, **k: asm)

    summary, reasons = T.relocate_extracted(conn, store)

    assert summary["reloc_aware"] == 1 and summary["upgraded"] == 1
    assert summary["superseded_targets"] == 1
    # the score that could never have been right is purged, not kept
    assert summary["purged_evidence_rows"] == 1
    assert conn.execute("SELECT count(*) FROM matrix_entry").fetchone()[0] == 0
    row = conn.execute("SELECT tier,target_o_sha FROM n64_target").fetchone()
    assert row["tier"] == "reloc_aware" and store.get(row["target_o_sha"])

    again, _ = T.relocate_extracted(conn, store)
    assert again["reloc_aware"] == 1 and again.get("upgraded", 0) == 0
    assert again["unchanged"] == 1 and again["superseded_targets"] == 0


def test_conflicted_extents_are_skipped_not_assembled(tmp_path, monkeypatch):
    conn = dbmod.connect(tmp_path / "conveyor.db")
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO n64_target (target_id,address,population,insn_count,"
            " target_o_sha,tier,gate_reason)"
            " VALUES ('suffix',?,'extracted',4,'s','raw_word','extent_conflict:outer')",
            (BASE,))
    monkeypatch.setattr("tools.conveyor.pipeline.disasm.derive",
                        lambda *a, **k: pytest.fail("must not derive a conflicted extent"))

    summary, _ = T.relocate_extracted(conn, BlobStore(tmp_path / "blobs"))

    assert summary["skipped_gate"] == 1 and summary.get("attempted", 0) == 0


def test_fp_control_register_is_renumbered_for_the_assembler():
    """objdump prints `cfc1 $t6,c1_fcsr`; GNU as rejects the name and takes
    only the number. Thirty targets failed to assemble on this alone."""
    derived = ("glabel fp\n.L80090000:\n    cfc1    $t6,c1_fcsr\n"
               ".L80090004:\n    ctc1    $t6,c1_fcsr\n")
    asm = T.target_asm_from_derived(derived, "fp")
    assert "c1_fcsr" not in asm
    assert "    cfc1    $t6,$31" in asm and "    ctc1    $t6,$31" in asm
