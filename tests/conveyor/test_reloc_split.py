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
def test_negative_addend_is_written_so_check_and_splat_can_parse_it(tmp_path):
    # func_8001661C: IDO reaches the split label D_80042228 as D_80042230-8 (a loop end pointer).
    body = "    lui   $t3, %hi(D_80042228)\n    addiu $t3, $t3, %lo(D_80042228)\n"
    lines = reloc_split.derive(0x800166C0, assemble(tmp_path, "t", body),
                               assemble(tmp_path, "c", body.replace("D_80042228", "D_80042230-8")))
    assert lines == [
        "rom:0x172C0 reloc:MIPS_HI16 symbol:D_80042230 addend:-0x8 // was D_80042228",
        "rom:0x172C4 reloc:MIPS_LO16 symbol:D_80042230 addend:-0x8 // was D_80042228",
    ]
    f = tmp_path / "reloc_addrs.txt"
    f.write_text("\n".join(lines) + "\n")
    assert [int(a["addend"], 0) for _, a in reloc_split.parse(f)] == [-8, -8]  # splat: int(attr_val, 0)


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


# w20c: relocation present on one side only, and own-section sites.

@needs_as
def test_constant_paired_as_an_address_becomes_a_custom_constant(tmp_path):
    # func_800114C0: splat paired the float->u32 `lui at,0x8000` with an unrelated
    # `addiu ...,63` as D_8000003F; IDO emits no relocation at either site.
    body = ("    lui   $at, %hi(D_8000003F)\n    or    $t1, $t1, $at\n"
            "    addiu $t3, $t1, %lo(D_8000003F)\n")
    cand = body.replace("%hi(D_8000003F)", "0x8000").replace("%lo(D_8000003F)", "63")
    lines = reloc_split.derive(0x8001176C, assemble(tmp_path, "t", body),
                               assemble(tmp_path, "c", cand), resolve=lambda s: 0x8000003F)
    assert lines == [
        "rom:0x1236C reloc:CUSTOM_CONSTANT_HI symbol:0x80000000 // was D_8000003F",
        "rom:0x12374 reloc:CUSTOM_CONSTANT_LO symbol:0x3F // was D_8000003F",
    ]
    rom = bytearray(0x12378)
    rom[0x1236C:0x12370] = (0x3C018000).to_bytes(4, "big")
    rom[0x12374:0x12378] = (0x252B003F).to_bytes(4, "big")
    f = tmp_path / "reloc_addrs.txt"
    f.write_text("\n".join(lines) + "\n")
    assert reloc_split.check(f, rom=bytes(rom), resolve=lambda s: None) == []
    # The HI constant is rendered `(value >> 16)`: no %hi carry, so 0x7FFF8000 != 0x8000.
    f.write_text("rom:0x1236C reloc:CUSTOM_CONSTANT_HI symbol:0x7FFF8000\n"
                 "rom:0x12374 reloc:CUSTOM_CONSTANT_LO symbol:D_8000003F\n")
    assert [n for n, _ in reloc_split.check(f, rom=bytes(rom), resolve=lambda s: None)] == [1, 2]


@needs_as
def test_target_addend_on_a_constant_site_is_read_from_its_field(tmp_path):
    # func_80011104: the same artifact named g_tlb_exception_vector + 0x1.
    body = ("    lui   $at, %hi(g_tlb_exception_vector + 0x1)\n"
            "    addiu $t4, $t2, %lo(g_tlb_exception_vector + 0x1)\n")
    cand = "    lui   $at, 0x8000\n    addiu $t4, $t2, 1\n"
    lines = reloc_split.derive(0x8001127C, assemble(tmp_path, "t", body),
                               assemble(tmp_path, "c", cand), resolve=lambda s: 0x80000000)
    assert [l.split(" //")[0] for l in lines] == [
        "rom:0x11E7C reloc:CUSTOM_CONSTANT_HI symbol:0x80000000",
        "rom:0x11E80 reloc:CUSTOM_CONSTANT_LO symbol:0x1",
    ]


@needs_as
def test_a_constant_that_is_not_the_target_half_is_refused(tmp_path):
    body = "    lui   $at, %hi(D_8000003F)\n"
    with pytest.raises(reloc_split.SplitError):
        reloc_split.derive(0x8001176C, assemble(tmp_path, "t", body),
                           assemble(tmp_path, "c", "    lui   $at, 0x8001\n"),
                           resolve=lambda s: 0x8000003F)


@needs_as
def test_unpaired_raw_immediate_is_named_by_the_candidate_symbol(tmp_path):
    # func_80013DEC: splat left `lui s0,0x8004; lhu s0,-0x7D6E(s0)` unpaired.
    raw = "    lui   $s0, 0x8004\n    nop\n    lhu   $s0, -0x7D6E($s0)\n"
    cand = "    lui   $s0, %hi(D_80038292)\n    nop\n    lhu   $s0, %lo(D_80038292)($s0)\n"
    lines = reloc_split.derive(0x80013E3C, assemble(tmp_path, "t", raw),
                               assemble(tmp_path, "c", cand))
    assert [l.split(" //")[0] for l in lines] == [
        "rom:0x14A3C reloc:MIPS_HI16 symbol:D_80038292 addend:0x0",
        "rom:0x14A44 reloc:MIPS_LO16 symbol:D_80038292 addend:0x0",
    ]
    # A symbol whose halves the raw words do not encode is refused.
    with pytest.raises(reloc_split.SplitError):
        reloc_split.derive(0x80013E3C, assemble(tmp_path, "t2", raw),
                           assemble(tmp_path, "c2", cand.replace("D_80038292", "D_80038294")))


@needs_as
def test_own_section_sites_are_listed_not_proposed(tmp_path):
    # A jump table the candidate holds in its own .rodata: an owned data slot, not a name.
    body = "    lui   $at, %hi(jtbl_8002D930_main)\n    lw    $t6, %lo(jtbl_8002D930_main)($at)\n"
    cand = (".section .rodata\ntbl:\n.word 0\n.text\n"
            "    lui   $at, %hi(tbl)\n    lw    $t6, %lo(tbl)($at)\n")
    lines = reloc_split.derive(0x80021560, assemble(tmp_path, "t", body),
                               assemble(tmp_path, "c", cand))
    assert len(lines) == 2 and all(l.startswith("// rom:") and "owned data slot" in l for l in lines)
