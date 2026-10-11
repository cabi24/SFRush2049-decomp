"""Group splice: own data verified and placed per reference, per member.

Retail lays .rodata out per function. Members of one group that are not
neighbours in the image have their literals and jump tables at unrelated
image addresses, so no single section base exists. Each own-data reference of
each output member must then (a) name object bytes equal to the image bytes
at the address the retail words encode at that site and (b) be relocated to
exactly that address. Context functions are not verified.
"""
import shutil
import struct
import subprocess

import pytest

from tools.conveyor.pipeline import blob_group

pytestmark = pytest.mark.skipif(shutil.which("mips-linux-gnu-as") is None,
                                reason="needs the mips binutils")

BASE = 0x80100000
LUI_AT, LWC1_F4, JR_RA = 0x3C010000, 0xC4240000, 0x03E00008
A, B, C = 0x3DCCCCCD, 0x3F4CCCCD, 0x3E19999A        # 0.1f, 0.8f, 0.15f


def _function(name, body):
    return f"    .globl {name}\n    .type {name}, @function\n{name}:\n{body}"


def _load(label):
    return (f"    lui $at, %hi({label})\n    lwc1 $f4, %lo({label})($at)\n"
            "    jr $ra\n    nop\n")


LITERALS = ("    .set noreorder\n    .set noat\n    .text\n"
            + _function("f", _load("lit_f")) + _function("g", _load("lit_g"))
            + _function("ctx", _load("lit_ctx"))
            + f"    .section .rodata\nlit_f: .word {A}\nlit_g: .word {B}\nlit_ctx: .word {C}\n")


def _assemble(tmp_path, text, name="own"):
    src, obj = tmp_path / f"{name}.s", tmp_path / f"{name}.o"
    src.write_text(text)
    subprocess.run(["mips-linux-gnu-as", "-EB", "-mips2", "-32", "-o", str(obj), str(src)],
                   check=True)
    return obj


def _hi(address):
    return ((address + 0x8000) >> 16) & 0xFFFF


def _pair(address, hi_op=LUI_AT, lo_op=LWC1_F4):
    return [hi_op | _hi(address), lo_op | (address & 0xFFFF)]


def _image(functions, data, size=0x400):
    """functions {offset: [words]}, data {address: [words]} -> image bytes."""
    image = bytearray(size)
    for offset, words in functions.items():
        struct.pack_into(f">{len(words)}I", image, offset, *words)
    for address, words in data.items():
        struct.pack_into(f">{len(words)}I", image, address - BASE, *words)
    return image


F_LIT, G_LIT, CTX_LIT = BASE + 0x100, BASE + 0x180, BASE + 0x1C0
EXTENTS = {"f": {"vaddr": BASE, "size": 16}, "g": {"vaddr": BASE + 0x20, "size": 16},
           "ctx": {"vaddr": BASE + 0x40, "size": 16}}


def _literal_image(f=A, g=B, ctx=C, g_at=G_LIT):
    return _image({0x00: _pair(F_LIT) + [JR_RA, 0], 0x20: _pair(g_at) + [JR_RA, 0],
                   0x40: _pair(CTX_LIT) + [JR_RA, 0]},
                  {F_LIT: [f], G_LIT: [g], CTX_LIT: [ctx]})


def _relocate(obj, image, members, extents=EXTENTS, names=None, data_runs=None):
    slices, ndx = blob_group.member_slices(obj, names or list(extents), extents)
    return blob_group.relocate(obj, slices, ndx, {}, members=members,
                               image=(bytes(image), BASE), data_runs=data_runs)


def _body(image, name, extents=EXTENTS):
    lo = extents[name]["vaddr"] - BASE
    return bytes(image[lo:lo + extents[name]["size"]])


def test_two_members_with_literals_at_non_adjacent_retail_addresses(tmp_path):
    obj, image = _assemble(tmp_path, LITERALS), _literal_image()
    bodies = _relocate(obj, image, ["f", "g"])
    assert bodies["f"] == _body(image, "f") and bodies["g"] == _body(image, "g")
    # each site carries exactly the address the retail words encode there
    assert struct.unpack(">2I", bodies["g"][:8]) == tuple(_pair(G_LIT))


def test_the_whole_section_proof_alone_refuses_that_group(tmp_path):
    obj, image = _assemble(tmp_path, LITERALS), _literal_image()
    slices, ndx = blob_group.member_slices(obj, list(EXTENTS), EXTENTS)
    rels, _others = blob_group._text_relocations(obj)
    read = lambda vaddr, n: bytes(image[vaddr - BASE:vaddr - BASE + n])
    with pytest.raises(blob_group.GroupError, match="disagree on its image address"):
        blob_group._local_data_bases(
            obj, rels, blob_group._symbols(obj), lambda o: o < 32,
            lambda o: struct.unpack(">I", read(BASE + (o if o < 16 else o + 16), 4))[0], read)


def test_wrong_member_literal_is_refused_naming_member_site_address_and_values(tmp_path):
    obj = _assemble(tmp_path, LITERALS)
    with pytest.raises(blob_group.GroupError) as refused:
        _relocate(obj, _literal_image(g=0x3F4CCCCE), ["f", "g"])
    message = str(refused.value)
    assert message.startswith("g: own .rodata+0x4 referenced at +0x4 differs from retail "
                              "0x80100180")
    assert "retail 3f4cccce, got 3f4ccccd" in message
    assert "whole-section placement was refused" in message


def test_right_value_read_from_another_retail_address_is_refused(tmp_path):
    # retail's g reads a word that is not g's literal
    obj = _assemble(tmp_path, LITERALS)
    with pytest.raises(blob_group.GroupError, match=r"g: own \.rodata\+0x4 .* retail 0x80100200"):
        _relocate(obj, _literal_image(g_at=BASE + 0x200), ["f", "g"])


def test_context_literal_is_not_verified_but_member_literals_are(tmp_path):
    obj = _assemble(tmp_path, LITERALS)
    wrong_context = _literal_image(ctx=0x12345678)
    bodies = _relocate(obj, wrong_context, ["f", "g"])
    assert bodies["f"] == _body(wrong_context, "f") and bodies["g"] == _body(wrong_context, "g")
    # the same wrong word under a member is a refusal
    with pytest.raises(blob_group.GroupError, match="^ctx: own .rodata"):
        _relocate(obj, wrong_context, ["f", "g", "ctx"])
    # and a context that is wrong never excuses a wrong member
    with pytest.raises(blob_group.GroupError, match="^f: own .rodata"):
        _relocate(obj, _literal_image(f=0x3DCCCCCC, ctx=0x12345678), ["f", "g"])


def test_one_member_and_one_context_literal(tmp_path):
    # the third blocked case: "multiple placements need complete jump table evidence"
    obj = _two_literal_object(tmp_path, _load("lit_f"))
    functions = {0x00: _pair(F_LIT) + [JR_RA, 0], 0x20: _pair(G_LIT) + [JR_RA, 0]}
    image = _image(functions, {F_LIT: [A], G_LIT: [0x12345678]})     # g is context: unchecked
    assert _relocate(obj, image, ["f"], names=["f", "g"])["f"] == _body(image, "f")
    with pytest.raises(blob_group.GroupError, match="^f: own .rodata.*complete jump table"):
        _relocate(obj, _image(functions, {F_LIT: [1], G_LIT: [B]}), ["f"], names=["f", "g"])


def test_adjacent_members_keep_the_whole_section_placement(tmp_path, monkeypatch):
    # Groups that pass today never reach the per-reference path.
    obj = _assemble(tmp_path, LITERALS)
    image = _image({0x00: _pair(F_LIT) + [JR_RA, 0], 0x20: _pair(F_LIT + 4) + [JR_RA, 0],
                    0x40: _pair(F_LIT + 8) + [JR_RA, 0]}, {F_LIT: [A, B, C]})
    monkeypatch.setattr(blob_group, "_member_own_data",
                        lambda *a, **k: pytest.fail("per-reference path used"))
    bodies = _relocate(obj, image, ["f", "g"])
    assert bodies["f"] == _body(image, "f") and bodies["g"] == _body(image, "g")


# --- jump tables ---------------------------------------------------------------

TABLE = ("    .set noreorder\n    .set noat\n    .text\n"
         + _function("f", "    lui $at, %hi(table)\n    lw $t0, %lo(table)($at)\n"
                          "    jr $t0\n    nop\nL0:\n    jr $ra\n    nop\nL1:\n"
                          "    jr $ra\n    nop\n")
         + _function("g", _load("lit_g"))
         + f"    .section .rodata\ntable: .word L0, L1\nlit_g: .word {B}\n")
T_EXTENTS = {"f": {"vaddr": BASE, "size": 32}, "g": {"vaddr": BASE + 0x40, "size": 16}}
T_TABLE = BASE + 0x100
LW_T0, JR_T0 = 0x8C280000, 0x01000008


def _table_image(entries=(BASE + 16, BASE + 24), g=B):
    return _image({0x00: _pair(T_TABLE, lo_op=LW_T0) + [JR_T0, 0, JR_RA, 0, JR_RA, 0],
                   0x40: _pair(G_LIT) + [JR_RA, 0]},
                  {T_TABLE: list(entries), G_LIT: [g]})


def test_member_table_and_member_literal_at_unrelated_addresses(tmp_path):
    obj, image = _assemble(tmp_path, TABLE), _table_image()
    bodies = _relocate(obj, image, ["f", "g"], T_EXTENTS)
    assert bodies["f"] == _body(image, "f", T_EXTENTS)
    assert bodies["g"] == _body(image, "g", T_EXTENTS)


@pytest.mark.parametrize("entries", [(BASE + 16, BASE + 28), (BASE + 20, BASE + 24),
                                     (BASE + 16, 0)])
def test_wrong_table_entry_is_refused(tmp_path, entries):
    obj = _assemble(tmp_path, TABLE)
    with pytest.raises(blob_group.GroupError,
                       match=r"^f: own \.rodata\+0x0 referenced at \+0x4 differs from retail "
                             r"0x80100100 \(jump table entry"):
        _relocate(obj, _table_image(entries), ["f", "g"], T_EXTENTS)


def test_wrong_literal_beside_a_right_table_is_refused(tmp_path):
    obj = _assemble(tmp_path, TABLE)
    with pytest.raises(blob_group.GroupError, match="^g: own .rodata"):
        _relocate(obj, _table_image(g=0), ["f", "g"], T_EXTENTS)


def test_table_entry_into_member_text_must_lie_in_a_verified_window(tmp_path):
    # A context function references the middle of f's table, so f's verified
    # window stops there. The second entry still enters f and is unproven.
    text = TABLE.replace(_function("g", _load("lit_g")),
                         _function("g", _load("lit_g")) + _function("ctx", _load("table+4")))
    obj = _assemble(tmp_path, text)
    extents = dict(T_EXTENTS, ctx={"vaddr": BASE + 0x60, "size": 16})
    with pytest.raises(blob_group.GroupError, match="outside every verified reference"):
        _relocate(obj, _table_image(), ["f", "g"], extents)


# --- protections that must survive ------------------------------------------------

def _two_literal_object(tmp_path, body_f, body_g=None, rodata=None, extra=""):
    return _assemble(tmp_path, "    .set noreorder\n    .set noat\n    .text\n"
                     + _function("f", body_f) + _function("g", body_g or _load("lit_g"))
                     + (rodata or f"    .section .rodata\nlit_f: .word {A}\nlit_g: .word {B}\n")
                     + extra)


def test_one_lo16_whose_retail_hi16_words_disagree_is_refused(tmp_path):
    obj = _two_literal_object(
        tmp_path, "    lui $at, %hi(lit_f)\n    lui $v0, %hi(lit_f)\n"
                  "    lwc1 $f4, %lo(lit_f)($at)\n    jr $ra\n")
    image = _image({0x00: [LUI_AT | _hi(F_LIT), 0x3C020000 | _hi(F_LIT + 0x10000),
                           LWC1_F4 | (F_LIT & 0xFFFF), JR_RA],
                    0x20: _pair(G_LIT) + [JR_RA, 0]}, {F_LIT: [A], G_LIT: [B]})
    with pytest.raises(blob_group.GroupError, match="^f: .*encode different addresses"):
        _relocate(obj, image, ["f", "g"], names=["f", "g"])


def test_one_object_literal_at_two_retail_addresses_is_refused(tmp_path):
    # g reads f's literal object; retail's g reads another (equal) word
    obj = _two_literal_object(tmp_path, _load("lit_f"), _load("lit_f"),
                              rodata=f"    .section .rodata\nlit_f: .word {A}\n")
    image = _image({0x00: _pair(F_LIT) + [JR_RA, 0], 0x20: _pair(G_LIT) + [JR_RA, 0]},
                   {F_LIT: [A], G_LIT: [A]})
    with pytest.raises(blob_group.GroupError, match="placed at different image addresses"):
        _relocate(obj, image, ["f", "g"], names=["f", "g"])


def test_two_object_literals_over_the_same_retail_word_are_refused(tmp_path):
    obj = _two_literal_object(
        tmp_path, _load("lit_f"),
        rodata=f"    .section .rodata\nlit_f: .word {A}\nlit_g: .word {A}\n")
    image = _image({0x00: _pair(F_LIT) + [JR_RA, 0], 0x20: _pair(F_LIT) + [JR_RA, 0]},
                   {F_LIT: [A]})
    with pytest.raises(blob_group.GroupError, match="placed over the same image bytes"):
        _relocate(obj, image, ["f", "g"], names=["f", "g"])


def test_unpaired_hi16_in_a_member_is_refused(tmp_path):
    # (the HI16 names another section: gas would otherwise attach it to a LO16)
    obj = _two_literal_object(
        tmp_path, _load("lit_f"),
        "    lui $at, %hi(lit_g)\n    lwc1 $f4, %lo(lit_g)($at)\n"
        "    lui $v0, %hi(stray)\n    jr $ra\n",
        extra="    .data\nstray: .word 0\n")
    image = _image({0x00: _pair(F_LIT) + [JR_RA, 0],
                    0x20: _pair(G_LIT) + [0x3C020000 | _hi(F_LIT), JR_RA]},
                   {F_LIT: [A], G_LIT: [B]})
    with pytest.raises(blob_group.GroupError, match="^g: .*unpaired HI16"):
        _relocate(obj, image, ["f", "g"], names=["f", "g"])


def test_lo16_without_hi16_in_a_member_is_refused(tmp_path):
    obj = _two_literal_object(
        tmp_path, "    lwc1 $f4, %lo(lit_f)($at)\n    jr $ra\n    nop\n    nop\n")
    image = _image({0x00: [LWC1_F4 | (F_LIT & 0xFFFF), JR_RA, 0, 0],
                    0x20: _pair(G_LIT) + [JR_RA, 0]}, {F_LIT: [A], G_LIT: [B]})
    with pytest.raises(blob_group.GroupError, match="has no HI16"):
        _relocate(obj, image, ["f", "g"], names=["f", "g"])


def test_bss_reference_outside_game_bss_is_refused(tmp_path):
    obj = _two_literal_object(tmp_path, _load("zeroed"), extra="    .bss\nzeroed: .space 4\n")
    image = _image({0x00: _pair(F_LIT) + [JR_RA, 0], 0x20: _pair(G_LIT) + [JR_RA, 0]},
                   {G_LIT: [B]})
    # F_LIT is inside the image: retail initialises it, so it is no .bss object
    with pytest.raises(blob_group.GroupError,
                       match=r"^f: own \.bss\+0x0 .*not in a known game \.bss range"):
        _relocate(obj, image, ["f", "g"], names=["f", "g"])


BSS = 0x80156940                    # inside tools/cloud/owndata.GAME_BSS
ZEROED = "    .bss\nzeroed_f: .space 4\nzeroed_g: .space 4\n"


def test_bss_statics_of_two_members_are_placed_per_object(tmp_path):
    obj = _two_literal_object(tmp_path, _load("zeroed_f"), _load("zeroed_g"), extra=ZEROED)
    # retail keeps them 8 apart, and in the other order: no section base exists
    image = _image({0x00: _pair(BSS + 8) + [JR_RA, 0], 0x20: _pair(BSS) + [JR_RA, 0]}, {})
    bodies = _relocate(obj, image, ["f", "g"], names=["f", "g"])
    assert bodies["f"] == _body(image, "f") and bodies["g"] == _body(image, "g")
    # a member with a literal and a context-free .bss member together
    obj = _two_literal_object(tmp_path, _load("zeroed_f"), extra=ZEROED)
    image = _image({0x00: _pair(BSS) + [JR_RA, 0], 0x20: _pair(G_LIT) + [JR_RA, 0]},
                   {G_LIT: [B]})
    bodies = _relocate(obj, image, ["f", "g"], names=["f", "g"])
    assert bodies["f"] == _body(image, "f") and bodies["g"] == _body(image, "g")


def test_bss_statics_of_two_members_must_not_overlap(tmp_path):
    obj = _two_literal_object(tmp_path, _load("zeroed_f"), _load("zeroed_g"), extra=ZEROED)
    image = _image({0x00: _pair(BSS) + [JR_RA, 0], 0x20: _pair(BSS + 2) + [JR_RA, 0]}, {})
    with pytest.raises(blob_group.GroupError, match="placed over the same image bytes"):
        _relocate(obj, image, ["f", "g"], names=["f", "g"])


def test_one_bss_static_read_at_two_addresses_is_refused(tmp_path):
    obj = _two_literal_object(tmp_path, _load("zeroed_f"), _load("zeroed_f"), extra=ZEROED)
    image = _image({0x00: _pair(BSS) + [JR_RA, 0], 0x20: _pair(BSS + 8) + [JR_RA, 0]}, {})
    with pytest.raises(blob_group.GroupError, match="placed at different image addresses"):
        _relocate(obj, image, ["f", "g"], names=["f", "g"])


def test_local_statics_of_two_members_at_unrelated_data_addresses(tmp_path):
    # .data is per translation unit in retail; a group can span two of them.
    obj = _two_literal_object(
        tmp_path, _load("lit_f"),
        rodata="    .data\nlit_f: .word 7\nlit_g: .word 9\n")
    image = _image({0x00: _pair(F_LIT) + [JR_RA, 0], 0x20: _pair(G_LIT) + [JR_RA, 0]},
                   {F_LIT: [7], G_LIT: [9]})
    bodies = _relocate(obj, image, ["f", "g"], names=["f", "g"])
    assert bodies["g"][:8] == struct.pack(">2I", *_pair(G_LIT))
    struct.pack_into(">I", image, G_LIT - BASE, 8)
    with pytest.raises(blob_group.GroupError, match=r"^g: own \.data\+0x4 .*retail 0x80100180"):
        _relocate(obj, image, ["f", "g"], names=["f", "g"])


def test_the_two_elf_readers_must_see_the_same_object(tmp_path, monkeypatch):
    # owndata reads the file itself: a relocation list or text that is not the
    # file's must not be verified by it.
    obj, image = _assemble(tmp_path, LITERALS), _literal_image()
    rels, others = blob_group._text_relocations(obj)
    monkeypatch.setattr(blob_group, "_text_relocations", lambda o: (rels[:-2], others))
    with pytest.raises(blob_group.GroupError, match="differ between the two ELF readers"):
        _relocate(obj, image, ["f", "g"])


# --- one member's .rodata in two retail regions ---------------------------------

TWO = ("    .set noreorder\n    .set noat\n    .text\n"
       + _function("f", "    lui $at, %hi(lit_a)\n    lwc1 $f4, %lo(lit_a)($at)\n"
                        "    lui $at, %hi(lit_b)\n    lwc1 $f6, %lo(lit_b)($at)\n"
                        "    jr $ra\n    nop\n    nop\n    nop\n")
       + f"    .section .rodata\nlit_a: .word {A}\nlit_b: .word {B}\n")
TWO_EXTENTS = {"f": {"vaddr": BASE, "size": 32}}
DATA_RUNS = [(BASE + 0x100, BASE + 0x400)]          # the image after the code
LWC1_F6 = 0xC4260000


def _two_region_image(a_at=F_LIT, b_at=G_LIT, b=B):
    return _image({0x00: _pair(a_at) + _pair(b_at, lo_op=LWC1_F6) + [JR_RA, 0, 0, 0]},
                  {a_at: [A], b_at: [b]})


def test_one_member_with_rodata_in_two_retail_regions(tmp_path):
    # func_800B59F0 (strings and floats apart in retail): placed per window
    # only with the image's data runs, never by the one-base rule alone
    obj, image = _assemble(tmp_path, TWO), _two_region_image()
    bodies = _relocate(obj, image, ["f"], TWO_EXTENTS, data_runs=DATA_RUNS)
    assert bodies["f"] == _body(image, "f", TWO_EXTENTS)
    with pytest.raises(blob_group.GroupError,
                       match="^f: own .rodata: this function's references disagree"):
        _relocate(obj, image, ["f"], TWO_EXTENTS)


def test_two_region_rodata_with_a_wrong_literal_is_refused(tmp_path):
    obj = _assemble(tmp_path, TWO)
    with pytest.raises(blob_group.GroupError, match=r"^f: own \.rodata\+0x4 .*0x80100180"):
        _relocate(obj, _two_region_image(b=0x3F4CCCCE), ["f"], TWO_EXTENTS,
                  data_runs=DATA_RUNS)


def test_two_region_rodata_inside_function_code_is_refused(tmp_path):
    # retail's second word pair points into the code: no data run holds it
    obj, image = _assemble(tmp_path, TWO), _two_region_image(b_at=BASE + 0x40)
    with pytest.raises(blob_group.GroupError,
                       match="not inside one non-function run of the image"):
        _relocate(obj, image, ["f"], TWO_EXTENTS, data_runs=DATA_RUNS)


def test_two_region_rodata_windows_must_not_overlap(tmp_path):
    obj = _assemble(tmp_path, TWO.replace(f"lit_b: .word {B}", f"lit_b: .word {A}"))
    image = _image({0x00: _pair(F_LIT) + _pair(F_LIT + 4, lo_op=LWC1_F6) + [JR_RA, 0, 0, 0]},
                   {F_LIT: [A, A]})
    # adjacent and equal: the one-base rule holds, nothing to refuse
    assert _relocate(obj, image, ["f"], TWO_EXTENTS, data_runs=DATA_RUNS)["f"] == \
        _body(image, "f", TWO_EXTENTS)
    image = _image({0x00: _pair(F_LIT + 4) + _pair(F_LIT, lo_op=LWC1_F6) + [JR_RA, 0, 0, 0]},
                   {F_LIT: [A, A]})
    bodies = _relocate(obj, image, ["f"], TWO_EXTENTS, data_runs=DATA_RUNS)
    assert bodies["f"] == _body(image, "f", TWO_EXTENTS)        # swapped, disjoint: fine
    image = _image({0x00: _pair(F_LIT) + _pair(F_LIT, lo_op=LWC1_F6) + [JR_RA, 0, 0, 0]},
                   {F_LIT: [A]})
    with pytest.raises(blob_group.GroupError, match="occupy the same image bytes"):
        _relocate(obj, image, ["f"], TWO_EXTENTS, data_runs=DATA_RUNS)
