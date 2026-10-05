"""Relocation symbol splits named through reloc_addrs (wave 4).

A split name may be replaced only by base+addend naming the same address and
encoded by the candidate's own instruction field. A wrong global or addend is
refused, and every checked-in entry must encode its ROM word.
"""
import shutil
import subprocess

import pytest

from tools.conveyor.pipeline import reloc_split, targets

needs_as = pytest.mark.skipif(shutil.which("mips-linux-gnu-as") is None
                              or shutil.which("mips-linux-gnu-objdump") is None,
                              reason="MIPS binutils not installed")

TARGET = """\
    lui   $s0, %hi(D_80043EB8)
    lui   $s1, %hi(D_8004BE78)
    addiu $s1, $s1, %lo(D_8004BE78)
    addiu $s0, $s0, %lo(D_80043EB8)
"""


def assemble(tmp_path, name, body):
    src = tmp_path / f"{name}.s"
    src.write_text(".set noreorder\n.set noat\n.text\nfunc:\n" + body)
    obj = tmp_path / f"{name}.o"
    subprocess.run(["mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-o", str(obj), str(src)],
                   check=True)
    return obj


def candidate(tmp_path, end):
    return assemble(tmp_path, "cand", TARGET.replace("D_8004BE78", end))


@needs_as
def test_split_name_becomes_base_plus_addend(tmp_path):
    lines = reloc_split.derive(0x80018E78, assemble(tmp_path, "t", TARGET),
                               candidate(tmp_path, "D_80043EB8+0x7FC0"))
    assert lines == [
        "rom:0x19A7C reloc:MIPS_HI16 symbol:D_80043EB8 addend:0x7FC0 // was D_8004BE78",
        "rom:0x19A80 reloc:MIPS_LO16 symbol:D_80043EB8 addend:0x7FC0 // was D_8004BE78",
    ]


@needs_as
@pytest.mark.parametrize("end", ["D_80043EB8+0x7FC4",   # wrong addend
                                 "D_80043EB0+0x7FC0",   # wrong global
                                 "D_80043EB0"])         # wrong global, no addend
def test_wrong_global_or_addend_is_refused(tmp_path, end):
    with pytest.raises(reloc_split.SplitError):
        reloc_split.derive(0x80018E78, assemble(tmp_path, "t", TARGET), candidate(tmp_path, end))


@needs_as
def test_identical_relocations_need_no_entry(tmp_path):
    t = assemble(tmp_path, "t", TARGET)
    assert reloc_split.derive(0x80018E78, t, t) == []


def test_encodes_hi_carry_and_lui_only():
    address = 0x8004BE78  # low half >= 0x8000: %hi carries
    assert reloc_split.encodes("MIPS_HI16", 0x3C118005, address)
    assert not reloc_split.encodes("MIPS_HI16", 0x3C118004, address)
    assert not reloc_split.encodes("MIPS_HI16", 0x24118005, address)  # not a lui
    assert reloc_split.encodes("MIPS_LO16", 0x2631BE78, address)
    assert not reloc_split.encodes("MIPS_LO16", 0x2631BE7C, address)


def test_check_rejects_entries_that_do_not_encode_the_rom_word(tmp_path):
    rom = bytearray(0x19A88)
    rom[0x19A7C:0x19A80] = (0x3C118005).to_bytes(4, "big")
    rom[0x19A84:0x19A88] = (0x2631BE78).to_bytes(4, "big")
    good = ("rom:0x19A7C reloc:MIPS_HI16 symbol:D_80043EB8 addend:0x7FC0\n"
            "rom:0x19A84 reloc:MIPS_LO16 symbol:D_80043EB8 addend:0x7FC0 // ok\n")
    path = tmp_path / "reloc_addrs.txt"
    path.write_text("// header\n" + good)
    assert reloc_split.check(path, rom=bytes(rom)) == []
    # A HI16 word only pins the carried upper half, so the wrong addend moves it.
    path.write_text(good.replace("0x7FC0\n", "0x17FC0\n", 1)
                    + "rom:0x19A84 reloc:MIPS_LO16 symbol:D_80043EB0 addend:0x7FC0\n")
    problems = reloc_split.check(path, rom=bytes(rom))
    assert [n for n, _ in problems] == [1, 3, 3]  # wrong addend; duplicate + wrong global


@pytest.mark.skipif(not targets.BASEROM.is_file(), reason="baserom not present")
def test_checked_in_entries_encode_their_rom_words():
    assert reloc_split.parse(), "reloc_addrs.us.txt has no entries"
    assert reloc_split.check() == []
