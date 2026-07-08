"""Reloc-aware target assembly + round-trip gate (`pipeline.targets`, 003).

Region parsing and the gate decision are pure and drive without an assembler.
One integration test runs the real `mips-linux-gnu-as` on the osCreateMesgQueue
region (skipped where mips binutils are absent, e.g. CI-less environments),
per contracts/target-assembly.md.
"""
import shutil

import pytest

from tools.conveyor.pipeline import targets as T

HAS_AS = shutil.which("mips-linux-gnu-as") is not None
HAS_OBJDUMP = shutil.which("mips-linux-gnu-objdump") is not None

# The osCreateMesgQueue region as splat emits it in asm/us/7600.s: two %hi/%lo
# pairs (D_8002C3D0) then stores, jr, delay-slot store.
OSCREATE_S = """\
.include "macro.inc"

.section .text, "ax"

nonmatching func_80006A00, 0x2C

glabel func_80006A00
    /* 7600 80006A00 3C0E8003 */  lui        $t6, %hi(D_8002C3D0)
    /* 7604 80006A04 3C0F8003 */  lui        $t7, %hi(D_8002C3D0)
    /* 7608 80006A08 25CEC3D0 */  addiu      $t6, $t6, %lo(D_8002C3D0)
    /* 760C 80006A0C 25EFC3D0 */  addiu      $t7, $t7, %lo(D_8002C3D0)
    /* 7610 80006A10 AC8E0000 */  sw         $t6, 0x0($a0)
    /* 7614 80006A14 AC8F0004 */  sw         $t7, 0x4($a0)
    /* 7618 80006A18 AC800008 */  sw         $zero, 0x8($a0)
    /* 761C 80006A1C AC80000C */  sw         $zero, 0xC($a0)
    /* 7620 80006A20 AC860010 */  sw         $a2, 0x10($a0)
    /* 7624 80006A24 03E00008 */  jr         $ra
    /* 7628 80006A28 AC850014 */   sw        $a1, 0x14($a0)
endlabel func_80006A00
    /* 762C 80006A2C 00000000 */  nop
"""

# A synthetic two-region file with an interior local label.
TWO_REGION_S = """\
.section .text

glabel func_80000100
    /* 0100 80000100 24020001 */  addiu      $v0, $zero, 1
.L80000104:
    /* 0104 80000104 03E00008 */  jr         $ra
    /* 0108 80000108 00000000 */   nop
endlabel func_80000100

glabel func_80000200
    /* 0200 80000200 24030002 */  addiu      $v1, $zero, 2
    /* 0204 80000204 03E00008 */  jr         $ra
    /* 0208 80000208 00000000 */   nop
endlabel func_80000200
"""


# --- region parsing ---------------------------------------------------------

def test_index_two_regions_keyed_by_first_vaddr(tmp_path):
    (tmp_path / "a.s").write_text(TWO_REGION_S)
    regions = T.index_asm_regions(tmp_path)
    assert set(regions) == {0x80000100, 0x80000200}
    r = regions[0x80000100]
    assert r.name == "func_80000100"
    assert r.vaddr == 0x80000100
    # words are the instruction words only (interior label contributes none).
    assert r.words == ["24020001", "03E00008", "00000000"]
    # the interior label line is kept verbatim for the assembler.
    assert any(line.strip() == ".L80000104:" for line in r.lines)
    # instruction lines are comment-stripped down to the mnemonic.
    assert any("addiu" in line and "/*" not in line for line in r.lines)


def test_index_oscreate_region_words(tmp_path):
    (tmp_path / "7600.s").write_text(OSCREATE_S)
    regions = T.index_asm_regions(tmp_path)
    assert set(regions) == {0x80006A00}
    r = regions[0x80006A00]
    # 11 instruction words between glabel/endlabel; the trailing nop after
    # endlabel (ROM alignment padding) is NOT part of the region.
    assert len(r.words) == 11
    assert r.words[0] == "3C0E8003" and r.words[-1] == "AC850014"
    # the %hi/%lo operands survive into the assembler lines.
    assert any("%hi(D_8002C3D0)" in line for line in r.lines)
    assert any("%lo(D_8002C3D0)" in line for line in r.lines)


def test_unterminated_region_is_dropped(tmp_path):
    (tmp_path / "b.s").write_text(
        "glabel func_80000300\n"
        "    /* 0300 80000300 24020001 */  addiu $v0, $zero, 1\n"
    )  # no endlabel
    assert T.index_asm_regions(tmp_path) == {}


# --- gate decision (pure) ---------------------------------------------------

HI16 = 0xFFFF0000


def test_gate_pass_ignores_masked_reloc_bits():
    # Same kept (high) bits; the low 16 bits (the relocation field) differ and
    # are masked away at the HI16 site -> pass.
    rom = [0x3C0E8003, 0xAC8E0000]
    new = [0x3C0E0000, 0xAC8E0000]
    assert T._gate_decide(rom, new, [(0, HI16)]) == (True, None)


def test_gate_pass_ignores_trailing_nop_padding():
    # Assembler-padded trailing nop on one side is stripped before comparison.
    rom = [0x24020001, 0x03E00008]
    new = [0x24020001, 0x03E00008, 0x00000000]
    assert T._gate_decide(rom, new, []) == (True, None)


def test_gate_word_mismatch_at_reloc_site_kept_bits():
    # Difference in the KEPT bits at a reloc site is a real mismatch.
    rom = [0x3C0E8003]
    new = [0x3C0F0000]  # differs in bit 16 (register field), which HI16 keeps
    ok, reason = T._gate_decide(rom, new, [(0, HI16)])
    assert ok is False and reason == "word_mismatch@0"


def test_gate_word_mismatch_reports_first_index():
    rom = [0x24020001, 0x24030002, 0x24040003]
    new = [0x24020001, 0xDEADBEEF, 0x24040003]
    ok, reason = T._gate_decide(rom, new, [])
    assert ok is False and reason == "word_mismatch@1"


def test_gate_length_mismatch():
    ok, reason = T._gate_decide([1, 2, 3], [1, 2], [])
    assert ok is False and reason == "length_mismatch 2 != 3"


# --- real-assembler integration (needs mips binutils) -----------------------

@pytest.mark.skipif(not (HAS_AS and HAS_OBJDUMP),
                    reason="mips-linux-gnu binutils not available")
def test_oscreate_assembles_with_relocations_and_passes_gate(tmp_path):
    from tools.conveyor.jobs import scoring

    (tmp_path / "7600.s").write_text(OSCREATE_S)
    region = T.index_asm_regions(tmp_path)[0x80006A00]
    out_o = tmp_path / "osCreateMesgQueue.o"
    T.assemble_region(region, "osCreateMesgQueue", out_o)

    binary = scoring._objdump_path()
    words = scoring._parse_text_words(scoring._objdump(binary, "-dz", str(out_o)))
    sites = dict(scoring._parse_relocs(scoring._objdump(binary, "-r", str(out_o))))
    # 11 instructions + one alignment nop = 12 words in the object.
    assert len(words) == 12
    # HI16 relocs at word indices 0,1; LO16 at 2,3 — the two %hi/%lo pairs.
    assert sites == {0: 0xFFFF0000, 1: 0xFFFF0000, 2: 0xFFFF0000, 3: 0xFFFF0000}
    r = scoring._objdump(binary, "-r", str(out_o))
    assert r.count("R_MIPS_HI16") == 2 and r.count("R_MIPS_LO16") == 2
    # Gate passes against the region's raw ROM words.
    assert T.gate_target(region.words, out_o) == (True, None)


@pytest.mark.skipif(not HAS_AS, reason="mips-linux-gnu-as not available")
def test_assemble_error_raises_deterministic_reason(tmp_path):
    # A bogus mnemonic fails to assemble; the reason carries no temp-file path.
    bad = T.Region(name="func_x", vaddr=0x80000000,
                   lines=["    not_an_instruction $t0, $t1"], words=["00000000"])
    with pytest.raises(T.AssembleError) as e:
        T.assemble_region(bad, "func_x", tmp_path / "x.o")
    msg = str(e.value)
    assert ".s:" not in msg and "/tmp" not in msg
    assert "Error" in msg


# --- determinism (T007) -----------------------------------------------------

@pytest.mark.skipif(not HAS_AS, reason="mips-linux-gnu-as not available")
def test_assembly_is_byte_identical_across_runs(tmp_path):
    (tmp_path / "7600.s").write_text(OSCREATE_S)
    region = T.index_asm_regions(tmp_path)[0x80006A00]
    a, b = tmp_path / "a.o", tmp_path / "b.o"
    T.assemble_region(region, "osCreateMesgQueue", a)
    T.assemble_region(region, "osCreateMesgQueue", b)
    assert a.read_bytes() == b.read_bytes()


@pytest.mark.skipif(not (HAS_AS and HAS_OBJDUMP),
                    reason="mips-linux-gnu binutils not available")
def test_populate_second_pass_is_a_noop(tmp_path, monkeypatch):
    """A one-target fabricated inventory, extracted twice into a tmp store:
    the second pass supersedes nothing and leaves tier/sha unchanged (SC-007).
    Uses tmp dirs only — never touches ~/.conveyor."""
    from tools.conveyor.coordinator import db as dbmod
    from tools.conveyor.coordinator.store import BlobStore

    # Fabricate work/<name>/info.txt for one static libultra function whose
    # region lives in a fixture asm dir we point ASM_DIR at.
    asm_dir = tmp_path / "asm"
    asm_dir.mkdir()
    (asm_dir / "7600.s").write_text(OSCREATE_S)
    monkeypatch.setattr(T, "ASM_DIR", asm_dir)

    # Its ROM words come from the real baserom via function_words(); reuse the
    # real address/size so the raw fallback path also works. If the baserom is
    # absent, skip (this is a Pi-local determinism check).
    if not T.BASEROM.is_file():
        pytest.skip("baserom.us.z64 not present")
    work = tmp_path / "work" / "osCreateMesgQueue"
    work.mkdir(parents=True)
    (work / "info.txt").write_text(
        "name: osCreateMesgQueue\naddress: 0x80006A00\n"
        "category: libultra/os\ncomment: (48 bytes)\n"
    )

    data = tmp_path / "conveyor"
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")

    s1 = T.populate(conn, store, work_dir=tmp_path / "work")
    row1 = conn.execute(
        "SELECT tier, gate_reason, target_o_sha FROM n64_target"
        " WHERE target_id='osCreateMesgQueue'").fetchone()
    assert row1["tier"] == "reloc_aware" and row1["gate_reason"] is None
    assert s1["tiers"]["reloc_aware"] == 1
    assert s1["superseded_targets"] == 1  # NULL -> sha first build

    s2 = T.populate(conn, store, work_dir=tmp_path / "work")
    row2 = conn.execute(
        "SELECT tier, gate_reason, target_o_sha FROM n64_target"
        " WHERE target_id='osCreateMesgQueue'").fetchone()
    # Nothing changed: byte-identical object, no supersession, no purge.
    assert row2["target_o_sha"] == row1["target_o_sha"]
    assert row2["tier"] == "reloc_aware"
    assert s2["superseded_targets"] == 0 and s2["purged_rows"] == 0
