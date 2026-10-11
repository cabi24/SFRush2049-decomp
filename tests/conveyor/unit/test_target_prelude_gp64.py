"""Static targets assemble in the ROM build's assembler mode (w19c).

`targets.assemble_region` takes `.set noat/noreorder/gp=64` and the o32 float
aliases from tools/asm-processor/prelude.inc. Without `.set gp=64`, GNU as
(-mabi=32) expands a 64-bit `ld`/`sd` as a macro: `ld $t7, %lo(sym)($t7)`
refuses under `.set noat` (__fixunsdfdi/__fixunssfdi) and `ld $t7, 8($sp)`
becomes two `lw` (the libgcc helpers' gate refused with a length mismatch).
"""
import shutil

import pytest

from tools.conveyor.pipeline import targets as T

HAS_BINUTILS = bool(shutil.which("mips-linux-gnu-as") and shutil.which("mips-linux-gnu-objdump"))


def test_prelude_supplies_rom_build_modes_and_float_aliases():
    lines = T.target_prelude()
    assert {".set noat", ".set noreorder", ".set gp=64"} <= set(lines)
    assert ".set $fa0,          $f12" in lines
    # Only mode directives and register aliases: no macros or includes.
    assert all(l.startswith(".set ") for l in lines)


def test_prelude_requires_noat_and_noreorder(tmp_path):
    p = tmp_path / "prelude.inc"
    p.write_text(".set noreorder\n.set gp=64\n")
    with pytest.raises(T.AssembleError):
        T.target_prelude(p)
    p.write_text(".set noat\n.set noreorder\n.set $fa0, $f12\n.macro glabel x\n.endm\n")
    assert T.target_prelude(p) == [".set noat", ".set noreorder", ".set $fa0, $f12"]


@pytest.mark.skipif(not HAS_BINUTILS, reason="mips-linux-gnu binutils not available")
def test_64bit_load_of_symbol_is_one_instruction_with_hi_lo(tmp_path):
    """The __fixunsdfdi bias load: `lui; ld %lo(gInfinity)` (ROM words)."""
    from tools.conveyor.jobs import scoring

    region = T.Region("bias", 0x80009A88,
                      ["lui $t7, %hi(gInfinity)", "ld $t7, %lo(gInfinity)($t7)",
                       "jr $ra", "or $v0, $v0, $t7"],
                      ["3C0F8003", "DDEFD800", "03E00008", "004F1025"])
    out = tmp_path / "bias.o"
    T.assemble_region(region, "bias", out)
    assert T.gate_target(region.words, out) == (True, None)
    relocs = scoring._objdump(scoring._objdump_path(), "-r", str(out))
    assert relocs.count("R_MIPS_HI16") == 1 and relocs.count("R_MIPS_LO16") == 1
    assert "gInfinity" in relocs


@pytest.mark.skipif(not HAS_BINUTILS, reason="mips-linux-gnu binutils not available")
def test_64bit_stack_load_is_not_split_into_two_words(tmp_path):
    """__lshrdi3's `ld $t7, 8($sp)` is the one ROM word DFAF0008."""
    region = T.Region("ld_stack", 0x8000D900,
                      ["ld $t7, 8($sp)", "ld $t6, 0($sp)", "jr $ra", "nop"],
                      ["DFAF0008", "DFAE0000", "03E00008", "00000000"])
    out = tmp_path / "ld.o"
    T.assemble_region(region, "ld_stack", out)
    assert T.gate_target(region.words, out) == (True, None)
