"""Owned .bss slots (w21d): a TU's whole zero-initialised .bss at a fixed RAM address.

lib_16320's three dataGet*-shaped lookups (func_80016E68 owner, func_80016EE0 and
func_80016F80 co-owners; w20b ownbss) keep their search keys and results in
function-local statics that form the TU's whole .bss, 0x30 B at retail
0x80042670. A .bss slot has no ROM bytes: no container, split or hash. The
linker moves the TU's .bss input into a fixed NOLOAD section with address and
size ASSERTs; while the members are passthrough a `.space` companion keeps the
block occupied, so both states link to the same addresses.
"""
import json
from pathlib import Path

import pytest

from tools.conveyor.pipeline import owned_data as O

REPO = Path(__file__).resolve().parents[2]
MEMBERS = ["func_80016E68", "func_80016EE0", "func_80016F80"]

LD = """SECTIONS
{
    .main_bss (NOLOAD) : SUBALIGN(16)
    {
        build/us/src/rom/lib_a.o(.bss);
        build/us/src/rom/lib_x.o(.bss);
    }
    .data : AT(data_ROM_START)
    {
        build/us/assets/us/data.o(.data);
    }
    data_VRAM_END = .;

    /DISCARD/ :
    {
        *(*);
    }
}
"""


def _row(**kw):
    row = {"owner": "f", "co_owners": ["g"], "tu": "src/rom/lib_x.c", "owner_section": ".bss",
           "container_section": ".bss", "container_vram": "0x80042670", "offset": "0x0",
           "size": "0x30", "passthrough_asm": "asm/us/nonmatchings/rom/lib_x/f.bss.s"}
    row.update(kw)
    return {k: v for k, v in row.items() if v is not None}


def _repo(tmp_path, *rows, storage=()):
    (tmp_path / O.REGISTRY).write_text(json.dumps(
        {"schema": 1, "rom_slots": list(rows), "storage_blocks": list(storage)}))
    return tmp_path


def test_committed_lib_16320_row():
    rows = [r for r in O.rom_slots(REPO, check_source=False) if O.is_bss(r)]
    r, = [r for r in rows if r["owner"] == "func_80016E68"]
    assert O.slot_members(r) == MEMBERS and r["tu"] == "src/rom/lib_16320.c"
    assert (r["container_vram"], r["offset"], r["size"]) == (0x80042670, 0, 0x30)
    assert (REPO / r["passthrough_asm"]).read_text() == O.companion_source(r, REPO)
    assert O.companion_source(r) == ".section .bss\n.balign 4\n.space 0x30\n"
    text = (REPO / r["tu"]).read_text()
    passthrough = [m for m in MEMBERS if f'/{m}.s")' in text]
    # All members passthrough (companion present) or all promoted (companion gone).
    assert passthrough in ([], MEMBERS)
    assert text.count(O.companion_block(r)) == (1 if passthrough else 0)
    assert O.slots_for_function(REPO, "func_80016F80", "src/rom/lib_16320.c") == [r]


def test_committed_linker_script_places_the_block():
    text = (REPO / "rush2049.us.ld").read_text()
    assert "/* ROM_OWNED_SECTION_MOVED build/us/src/rom/lib_16320.o(.bss) */" in text
    assert ".rom_owned_bss_func_80016E68 0x80042670 (NOLOAD)" in text
    assert 'END - rom_owned_func_80016E68_START == 0x30, "owned BSS slot size changed")' in text


def test_bss_rows_need_no_container_and_are_not_split(tmp_path):
    repo = _repo(tmp_path, _row())
    r, = O.rom_slots(repo)  # check_source=True: no asset is read
    assert r["source"] is None and r["owner_object"] == "build/us/src/rom/lib_x.o"
    assert O._groups(repo) == {}
    assert O.section_fits(r, 0x30) and not O.section_fits(r, 0x40)


@pytest.mark.parametrize("bad", [
    {"offset": "0x10"},
    {"container_vram": "0x80042678"},
    {"size": "0x28"},
    {"size": "0x0"},
    {"container_vram": "0x00042670"},
    {"sha256": "0" * 64},
    {"source": "assets/us/data.bin"},
    {"storage_owner": "x"},
    {"logical_extent": True},
    {"alignment": 4},
    {"container_section": ".data"},
    {"tu": "src/blob/x.c"},
    {"passthrough_asm": None},
    {"co_owners": ["f"]},
])
def test_bss_row_refusals(tmp_path, bad):
    with pytest.raises(O.OwnershipError):
        O.rom_slots(_repo(tmp_path, _row(**bad)))


def test_bss_rows_refuse_overlap_and_a_second_block_per_tu(tmp_path):
    with pytest.raises(O.OwnershipError, match="overlaps"):
        O.rom_slots(_repo(tmp_path, _row(), _row(owner="h", co_owners=[], tu="src/rom/lib_y.c",
                                               container_vram="0x80042690")))
    with pytest.raises(O.OwnershipError, match="overlaps"):
        O.rom_slots(_repo(tmp_path, _row(), storage=[{"owner": "s", "tu": "src/rom/lib_s.c",
                                                      "vram_start": "0x80042680", "size": "0x10"}]))
    with pytest.raises(O.OwnershipError, match="duplicate"):
        O.rom_slots(_repo(tmp_path, _row(), _row(owner="h", co_owners=[], container_vram="0x80042700")))
    with pytest.raises(O.OwnershipError, match="one owned .bss block"):
        O.rom_slots(_repo(tmp_path, _row(), storage=[{"owner": "s", "tu": "src/rom/lib_x.c",
                                                      "vram_start": "0x80042700", "size": "0x10"}]))
    with pytest.raises(O.OwnershipError, match="duplicate"):
        O.rom_slots(_repo(tmp_path, _row(), _row(co_owners=[], tu="src/rom/lib_y.c",
                                               container_vram="0x80042700")))
    # Adjacent blocks of two TUs are fine.
    assert len(O.rom_slots(_repo(tmp_path, _row(), _row(owner="h", co_owners=[], tu="src/rom/lib_y.c",
                                                      container_vram="0x800426A0")))) == 2


def test_linker_moves_the_input_into_a_fixed_block_idempotently(tmp_path):
    repo = _repo(tmp_path, _row())
    once = O.rewrite_linker(LD, repo)
    assert O.rewrite_linker(once, repo) == once
    assert "        /* ROM_OWNED_SECTION_MOVED build/us/src/rom/lib_x.o(.bss) */\n" in once
    assert once.count("build/us/src/rom/lib_x.o(.bss);") == 1
    block = once.split("data_VRAM_END = .;\n", 1)[1].split("\n\n", 1)[0]
    assert block.splitlines() == [
        "    /* ROM_OWNED_BSS_BEGIN f */",
        "    .rom_owned_bss_f 0x80042670 (NOLOAD) : SUBALIGN(16)",
        "    {",
        "        rom_owned_f_START = ABSOLUTE(.);",
        "        build/us/src/rom/lib_x.o(.bss);",
        "        rom_owned_f_END = ABSOLUTE(.);",
        '        ASSERT(rom_owned_f_START == 0x80042670, "owned BSS slot address changed");',
        '        ASSERT(rom_owned_f_END - rom_owned_f_START == 0x30, "owned BSS slot size changed");',
        "    }",
        "    /* ROM_OWNED_BSS_END f */"]
    # Removing the row restores SPLAT's original script exactly.
    assert O.rewrite_linker(once, _repo(tmp_path)) == LD


def test_linker_refuses_a_missing_input_or_anchor(tmp_path):
    repo = _repo(tmp_path, _row())
    with pytest.raises(O.OwnershipError):
        O.rewrite_linker(LD.replace("        build/us/src/rom/lib_x.o(.bss);\n", ""), repo)
    with pytest.raises(O.OwnershipError):
        O.rewrite_linker(LD.replace("data_VRAM_END = .;", ""), repo)


def test_prepare_writes_the_space_companion(tmp_path):
    asm = tmp_path / "asm/us/nonmatchings/rom/lib_x"
    asm.mkdir(parents=True)
    tu = tmp_path / "src/rom/lib_x.c"
    tu.parent.mkdir(parents=True)
    tu.write_text('#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_x/f.s")\n'
                  '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_x/g.s")\n')
    _repo(tmp_path)
    row = O.prepare(tmp_path, _row())
    assert (asm / "f.bss.s").read_text() == ".section .bss\n.balign 4\n.space 0x30\n"
    assert tu.read_text().count(O.companion_block(row)) == 1
    assert O.strip_companion(tu.read_text(), "f", tmp_path, "src/rom/lib_x.c").count("f.bss.s") == 0
