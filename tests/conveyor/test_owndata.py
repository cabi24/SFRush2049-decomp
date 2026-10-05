"""Own-data verification: a function's literals, jump tables and local statics.

The module is tested directly, then through the scorer with the reviewed
patch applied (cloud/work/frontier/rodata/score_py.patch; once the maintainer
has applied it, the real tools/cloud/score.py is used), then through the
single-function splice link. No IDO: objects are built here byte by byte.
"""
import hashlib
import importlib.util
import shutil
import struct
import subprocess
import sys
from pathlib import Path

import pytest

from tools.cloud import owndata

REPO = Path(__file__).resolve().parents[2]
PATCH = REPO / "cloud" / "work" / "frontier" / "rodata" / "score_py.patch"

F = 0x80100000                      # image address of the function under test
RODATA = 0x80124000                 # where retail keeps its literals
DATA = 0x80116000

TEXT, RODATA_NDX, DATA_NDX, BSS_NDX = 1, 6, 8, 9
NAMES = ["", ".text", ".rel.text", ".symtab", ".strtab", ".shstrtab",
         ".rodata", ".rel.rodata", ".data", ".bss"]
SECTION_SYMBOLS = [(".text", 0, 3, TEXT, 0), (".rodata", 0, 3, RODATA_NDX, 0),
                   (".data", 0, 3, DATA_NDX, 0), (".bss", 0, 3, BSS_NDX, 0)]

LUI_AT, JR_RA = 0x3C010000, 0x03E00008
LWC1_F4, LWC1_F6, LDC1_F4, LW_T0, ADDIU_A0 = (0xC4240000, 0xC4260000, 0xD4240000,
                                              0x8C280000, 0x24240000)


def w(*words):
    return struct.pack(f">{len(words)}I", *words)


def build(tmp_path, words, rels, rodata=b"", rodata_rels=(), data=b"", functions=None,
          name="fixture.o"):
    """A minimal ELF32/MSB relocatable with .text, .rodata, .data and .bss."""
    functions = functions or [("f", 0, len(words) * 4)]
    symbols = SECTION_SYMBOLS + [(n, value, 2, TEXT, size) for n, value, size in functions]
    symbols += [("callee", 0, 2, 0, 0), ("D_WRONG", 0, 1, 0, 0), ("D_80110000", 0, 1, 0, 0)]
    shstrings, offsets = b"\0", [0]
    for section in NAMES[1:]:
        offsets.append(len(shstrings))
        shstrings += section.encode() + b"\0"
    strings, symbytes, indices = b"\0", bytes(16), {}
    for index, (sym, value, typ, section, size) in enumerate(symbols, 1):
        indices[sym] = index
        nameoff = 0 if typ == 3 else len(strings)
        if typ != 3:
            strings += sym.encode() + b"\0"
        symbytes += struct.pack(">IIIBBH", nameoff, value, size, typ, 0, section)
    rel = lambda entries: b"".join(struct.pack(">II", offset, indices[sym] << 8 | typ)
                                   for offset, sym, typ in entries)
    sections = [b"", w(*words), rel(rels), symbytes, strings, shstrings,
                rodata, rel(rodata_rels), data, b""]
    types = [0, 1, 9, 2, 3, 3, 1, 9, 1, 8]
    link = {2: 3, 3: 4, 7: 3}
    info = {2: TEXT, 7: RODATA_NDX}
    out = bytearray(52)
    headers = []
    for index, raw in enumerate(sections):
        out.extend(bytes(-len(out) % 4))
        size = 16 if index == BSS_NDX else len(raw)
        headers.append(struct.pack(">10I", offsets[index], types[index], 0, 0, len(out), size,
                                   link.get(index, 0), info.get(index, 0), 4,
                                   {2: 8, 3: 16, 7: 8}.get(index, 0)))
        out.extend(raw)
    out.extend(bytes(-len(out) % 4))
    shoff = len(out)
    out.extend(b"".join(headers))
    out[:16] = b"\x7fELF\x01\x02\x01" + bytes(9)
    struct.pack_into(">HHIIIIIHHHHHH", out, 16, 1, 8, 1, 0, 0, shoff, 0, 52, 0, 0, 40,
                     len(sections), 5)
    path = tmp_path / name
    path.write_bytes(out)
    return path


def hi(address):
    return ((address + 0x8000) >> 16) & 0xFFFF


def lo(address):
    return address & 0xFFFF


# Two float loads: literal 0 from .rodata+0, literal 1 from .rodata+4.
LITERAL_WORDS = [LUI_AT, LWC1_F4, LUI_AT, LWC1_F6 | 4, JR_RA, 0]
LITERAL_RELS = [(0, ".rodata", 5), (4, ".rodata", 6), (8, ".rodata", 5), (12, ".rodata", 6)]
FLOATS = w(0x3B23D70A, 0x44BB8000)          # 0.0025f, 1500.0f


def literal_want(first=RODATA, second=RODATA + 4):
    return [LUI_AT | hi(first), LWC1_F4 | lo(first),
            LUI_AT | hi(second), LWC1_F6 | lo(second), JR_RA, 0]


def literal_object(tmp_path, floats=FLOATS):
    return build(tmp_path, LITERAL_WORDS, LITERAL_RELS, rodata=floats + bytes(8))


def image(*runs):
    return owndata.ImageData(runs)


def verify(obj, want, img, **kw):
    return owndata.verify(obj, "f", want, address=F, image=img, **kw)


# --- the module ---------------------------------------------------------------

def test_correct_literals_verify_every_relocation_word(tmp_path):
    result = verify(literal_object(tmp_path), literal_want(), image((RODATA, FLOATS)))
    assert result.ok and result.sites == {0, 4, 8, 12}
    assert result.notes == ["own .rodata verified at 0x80124000..0x80124008"]
    assert result.bases() == {".rodata": RODATA}


def test_wrong_literal_value_fails_with_site_address_and_bytes(tmp_path):
    obj = literal_object(tmp_path, w(0x3B23D70A, 0x44BBA000))
    result = verify(obj, literal_want(), image((RODATA, FLOATS)))
    assert not result.ok and result.sites == {0, 4}
    message, = result.failures
    assert "own .rodata+0x4 referenced at +0xc" in message
    assert "retail 0x80124004" in message
    assert "retail 44bb8000, got 44bba000" in message


def test_right_value_but_retail_site_reads_another_address_fails(tmp_path):
    obj = literal_object(tmp_path)
    img = image((RODATA, FLOATS + w(0, 0, 0x3F800000)))
    # Retail's second load reads 0x80124010 (1.0f), not the 1500.0f we supply.
    result = verify(obj, literal_want(second=RODATA + 0x10), img)
    assert not result.ok and 12 not in result.sites and 8 not in result.sites
    assert "retail 0x80124010" in result.failures[0]
    assert "retail 3f800000, got 44bb8000" in result.failures[0]


def test_right_values_in_a_different_layout_fail(tmp_path):
    """Each load reads an equal value, but retail keeps the two literals in
    the other order: this source would not link to the retail words."""
    obj = literal_object(tmp_path)
    img = image((RODATA, w(0x44BB8000, 0x3B23D70A)))
    result = verify(obj, literal_want(first=RODATA + 4, second=RODATA), img)
    assert not result.ok and not result.sites
    assert "disagree on the section's image address" in result.failures[0]
    assert ".rodata" not in result.placements


def test_retail_address_without_known_bytes_stays_unverified(tmp_path):
    obj = literal_object(tmp_path)
    result = verify(obj, literal_want(second=0x80200000), image((RODATA, FLOATS)))
    assert not result.failures and len(result.unverified) == 1
    assert "0x80200000 are not available" in result.unverified[0]
    assert result.sites == {0, 4} and not result.ok
    result = verify(obj, literal_want(), None)
    assert not result.failures and len(result.unverified) == 2 and not result.sites


def test_retail_hi_words_must_agree_for_one_lo(tmp_path):
    obj = build(tmp_path, [LUI_AT, LUI_AT, LWC1_F4, JR_RA],
                [(0, ".rodata", 5), (4, ".rodata", 5), (8, ".rodata", 6)],
                rodata=FLOATS)
    want = [LUI_AT | hi(RODATA), LUI_AT | hi(RODATA) + 1, LWC1_F4 | lo(RODATA), JR_RA]
    result = verify(obj, want, image((RODATA, FLOATS), (RODATA + 0x10000, FLOATS)))
    assert not result.sites and "encode different addresses" in result.failures[0]


def test_shared_hi_serves_following_lo_words(tmp_path):
    obj = build(tmp_path, [LUI_AT, LWC1_F4, LWC1_F6 | 4, JR_RA],
                [(0, ".rodata", 5), (4, ".rodata", 6), (8, ".rodata", 6)], rodata=FLOATS)
    want = [LUI_AT | hi(RODATA), LWC1_F4 | lo(RODATA), LWC1_F6 | lo(RODATA + 4), JR_RA]
    assert verify(obj, want, image((RODATA, FLOATS))).sites == {0, 4, 8}
    bad = verify(obj, want, image((RODATA, w(0x3B23D70A, 0))))
    assert bad.failures and bad.sites == {4}        # the shared HI16 is not proven either


def test_double_load_compares_eight_bytes(tmp_path):
    words = [LUI_AT, LDC1_F4, JR_RA, 0]
    rels = [(0, ".rodata", 5), (4, ".rodata", 6)]
    want = [LUI_AT | hi(RODATA), LDC1_F4 | lo(RODATA), JR_RA, 0]
    one = w(0x3FF00000, 0)                          # 1.0: low word is all zero
    obj = build(tmp_path, words, rels, rodata=one + bytes(8))
    assert verify(obj, want, image((RODATA, one))).ok
    result = verify(obj, want, image((RODATA, w(0x3FF00000, 1))))
    assert "8 bytes compared" in result.failures[0]


def test_section_padding_is_not_compared_but_data_before_next_reference_is(tmp_path):
    # f reads +0; g (same object) reads +8. f's window is [0, 8): the second
    # word belongs to it even though no instruction of f names it.
    words = [LUI_AT, LWC1_F4, JR_RA, 0, LUI_AT, LWC1_F4 | 8, JR_RA, 0]
    rels = [(0, ".rodata", 5), (4, ".rodata", 6), (16, ".rodata", 5), (20, ".rodata", 6)]
    rodata = w(0x3B23D70A, 0x44BB8000, 0x3F000000, 0)
    obj = build(tmp_path, words, rels, rodata=rodata,
                functions=[("f", 0, 16), ("g", 16, 16)])
    want = [LUI_AT | hi(RODATA), LWC1_F4 | lo(RODATA), JR_RA, 0]
    assert verify(obj, want, image((RODATA, w(0x3B23D70A, 0x44BB8000, 0x12345678)))).ok
    result = verify(obj, want, image((RODATA, w(0x3B23D70A, 0x44BB8001))))
    assert "+0x4: retail 44bb8001, got 44bb8000; 8 bytes compared" in result.failures[0]
    # g's own literal ends the section: the zero padding after it is not
    # compared (retail has another owner's data there).
    want_g = [LUI_AT | hi(RODATA + 0x40), LWC1_F4 | lo(RODATA + 0x40), JR_RA, 0]
    result = owndata.verify(obj, "g", want_g, address=F + 16,
                            image=image((RODATA + 0x40, w(0x3F000000, 0xDEADBEEF))))
    assert result.ok and result.sites == {16, 20}


# A switch: `lui at; addu at,at,t0; lw t0,lo(at); jr t0` over a 3-entry table.
TABLE_WORDS = [LUI_AT, 0x00280821, LW_T0, 0x01000008, 0, JR_RA, 0, JR_RA]
TABLE_RELS = [(0, ".rodata", 5), (8, ".rodata", 6)]
TABLE = 0x80123870


def table_want():
    return [LUI_AT | hi(TABLE), 0x00280821, LW_T0 | lo(TABLE), 0x01000008, 0, JR_RA, 0, JR_RA]


def table_object(tmp_path, entries=(0x14, 0x18, 0x1C), symbol=".text"):
    return build(tmp_path, TABLE_WORDS, TABLE_RELS, rodata=w(*entries) + bytes(4),
                 rodata_rels=[(4 * i, symbol, 2) for i in range(len(entries))])


def test_jump_table_entries_map_to_the_function_image_address(tmp_path):
    retail = w(F + 0x14, F + 0x18, F + 0x1C)
    result = verify(table_object(tmp_path), table_want(), image((TABLE, retail)))
    assert result.ok and result.sites == {0, 8}
    assert result.placements[".rodata"] == [(0, 12, TABLE, "rodata")]
    # Entries may also be written against the function's own symbol.
    obj = table_object(tmp_path, symbol="f")
    assert verify(obj, table_want(), image((TABLE, retail))).ok


def test_wrong_jump_table_entry_fails(tmp_path):
    retail = w(F + 0x14, F + 0x18, F + 0x1C)
    obj = table_object(tmp_path, entries=(0x14, 0x1C, 0x1C))
    result = verify(obj, table_want(), image((TABLE, retail)))
    assert not result.sites
    assert "jump table entry 1: retail 80100018, got 8010001c" in result.failures[0]
    # A table one entry short: retail's third entry is not ours.
    obj = table_object(tmp_path, entries=(0x14, 0x18))
    assert verify(obj, table_want(), image((TABLE, retail))).ok is True  # 8 bytes equal
    obj = table_object(tmp_path, entries=(0x14, 0x18, 0x1C, 0x1C))
    result = verify(obj, table_want(), image((TABLE, retail + w(0x3F800000))))
    assert "jump table entry 3" in result.failures[0]


def test_jump_table_needs_addresses_for_everything_it_enters(tmp_path):
    retail = w(F + 0x14, F + 0x18, F + 0x1C)
    obj = table_object(tmp_path)
    result = owndata.verify(obj, "f", table_want(), address=None, image=image((TABLE, retail)))
    assert not result.sites and "image address is unknown" in result.unverified[0]
    # An entry leaving the function resolves only through a named owner.
    obj = build(tmp_path, TABLE_WORDS, TABLE_RELS, rodata=w(0x14, 0x18, 0x1C),
                rodata_rels=[(0, ".text", 2), (4, ".text", 2), (8, ".text", 2)],
                functions=[("f", 0, 0x18), ("g", 0x18, 8)])
    want = table_want()[:6]
    result = verify(obj, want, image((TABLE, w(F + 0x14, 0x80200000, 0x80200004))))
    assert "outside the function" in result.unverified[0] and not result.sites
    result = verify(obj, want, image((TABLE, w(F + 0x14, 0x80200000, 0x80200004))),
                    addresses={"g": 0x80200000}.get)
    assert result.ok
    # Anything but a word-sized absolute entry is refused outright.
    obj = build(tmp_path, TABLE_WORDS, TABLE_RELS, rodata=w(0x14, 0x18, 0x1C),
                rodata_rels=[(0, ".text", 2), (4, ".text", 4), (8, ".text", 2)])
    result = verify(obj, table_want(), image((TABLE, retail)))
    assert "unsupported relocation type 4" in result.failures[0]


DATA_WORDS = [LUI_AT, LW_T0, LUI_AT, ADDIU_A0 | 4, JR_RA, 0]
DATA_RELS = [(0, ".data", 5), (4, ".data", 6), (8, ".data", 5), (12, ".data", 6)]


def data_want(first=DATA, second=DATA + 4):
    return [LUI_AT | hi(first), LW_T0 | lo(first), LUI_AT | hi(second),
            ADDIU_A0 | lo(second), JR_RA, 0]


def test_own_data_is_verified_and_reported_as_its_own_class(tmp_path):
    statics = w(7, 0x41424300)
    obj = build(tmp_path, DATA_WORDS, DATA_RELS, data=statics + bytes(8))
    result = verify(obj, data_want(), image((DATA, statics)))
    assert result.ok and result.sites == {0, 4, 8, 12}
    assert result.notes == ["own .data verified at 0x80116000 (4 bytes; "
                            ".data is laid out per translation unit)",
                            "own .data verified at 0x80116004 (4 bytes; "
                            ".data is laid out per translation unit)"]
    # `static int n = 0;`: equal, but an all-zero object is weak evidence.
    zero = build(tmp_path, DATA_WORDS[:2] + [JR_RA, 0], DATA_RELS[:2], data=bytes(16),
                 name="zero.o")
    note, = verify(zero, data_want()[:2] + [JR_RA, 0], image((DATA, bytes(4)))).notes
    assert note.startswith("own .data verified at 0x80116000 (4 bytes, all zero;")
    result = verify(obj, data_want(), image((DATA, w(8, 0x41424300))))
    assert "own .data+0x0 referenced at +0x4 differs from retail 0x80116000" in result.failures[0]
    assert result.sites == {8, 12}


def test_own_data_objects_may_sit_at_independent_addresses(tmp_path):
    """.data is per translation unit: two statics of one function need not be
    neighbours in retail. Each is checked; no single section base exists."""
    statics = w(7, 0x41424300)
    obj = build(tmp_path, DATA_WORDS, DATA_RELS, data=statics)
    img = image((DATA, w(7)), (DATA + 0x100, w(0x41424300)))
    result = verify(obj, data_want(second=DATA + 0x100), img)
    assert result.ok and len(result.notes) == 2
    with pytest.raises(ValueError, match="independent image addresses"):
        result.bases()


# --- zero-initialised own data (.bss): verified by address only ----------------

BSS = 0x80156940                    # inside owndata.GAME_BSS
LH_T0, SW_T0, LB_T0 = 0x84280000, 0xAC280000, 0x80280000

# Three statics at .bss+0/+4/+8 (as IDO lays out `static u8 *op1, *sp; static
# s16 cnt;`): op1 stored, sp loaded twice (one shared lui), cnt halfword.
BSS_WORDS = [LUI_AT, SW_T0, LUI_AT, LW_T0 | 4, LW_T0 | 4, LUI_AT, LH_T0 | 8, JR_RA, 0]
BSS_RELS = [(0, ".bss", 5), (4, ".bss", 6), (8, ".bss", 5), (12, ".bss", 6),
            (16, ".bss", 6), (20, ".bss", 5), (24, ".bss", 6)]


def bss_want(op1=BSS, sp=BSS + 8, cnt=BSS + 0x10, sp2=None):
    sp2 = sp if sp2 is None else sp2
    return [LUI_AT | hi(op1), SW_T0 | lo(op1), LUI_AT | hi(sp), LW_T0 | lo(sp),
            LW_T0 | lo(sp2), LUI_AT | hi(cnt), LH_T0 | lo(cnt), JR_RA, 0]


def bss_object(tmp_path, words=BSS_WORDS, rels=BSS_RELS, **kw):
    return build(tmp_path, words, rels, **kw)


def test_bss_objects_are_placed_per_object_at_the_retail_addresses(tmp_path):
    """Retail keeps the three statics 8 apart where the object has them 4
    apart: placement is per object, never one section base."""
    result = verify(bss_object(tmp_path), bss_want(), image((F, bytes(36))))
    assert result.ok and result.sites == set(range(0, 28, 4))
    assert result.bss() == {".bss": {0: BSS, 4: BSS + 8, 8: BSS + 0x10}}
    assert result.bases() == {}                 # never a section base
    # extents: up to the next referenced offset; the last one by access width
    assert sorted(result.placements[".bss"]) == [(0, 4, BSS, "bss"), (4, 8, BSS + 8, "bss"),
                                                 (8, 10, BSS + 0x10, "bss")]
    assert result.notes[0] == ("own .bss placed at 0x80156940 (4 bytes; zero-initialised: "
                               "verified by address only, nothing to compare)")
    assert {(e[1], e[3], e[4]) for e in result.bss_sites} == {
        (0, 4, BSS), (4, 12, BSS + 8), (4, 16, BSS + 8), (8, 24, BSS + 0x10)}
    # no retail data is needed (or consulted) for .bss
    assert verify(bss_object(tmp_path), bss_want(), None).ok


def test_bss_object_read_at_two_addresses_fails(tmp_path):
    result = verify(bss_object(tmp_path), bss_want(sp2=BSS + 0xC), None)
    assert not result.ok
    assert result.failures == ["own .bss+0x4 (referenced at +0xc, +0x10): retail reads this "
                               "one object at different addresses 0x80156948, 0x8015694C"]
    assert result.sites == {0, 4, 20, 24}       # op1 and cnt still proven
    assert 4 not in result.bss().get(".bss", {})


def test_bss_address_outside_game_bss(tmp_path):
    # beyond the zeroed range, no retail bytes: unknown storage -> unverified
    result = verify(bss_object(tmp_path), bss_want(cnt=0x80180000), None)
    assert not result.failures and len(result.unverified) == 1
    assert "0x80180000 (+2 bytes) is not in a known game .bss range" in result.unverified[0]
    assert result.sites == set(range(0, 20, 4))
    # inside the image (retail has bytes there): it cannot be .bss -> failure
    result = verify(bss_object(tmp_path), bss_want(op1=DATA), image((DATA, w(7))))
    assert "retail has initialised bytes there" in result.failures[0]
    # straddling the end of the range: the access width counts
    end = owndata.GAME_BSS[0][1]
    assert verify(bss_object(tmp_path), bss_want(cnt=end - 2), None).ok
    words = BSS_WORDS[:6] + [LW_T0 | 8] + BSS_WORDS[7:]
    result = verify(bss_object(tmp_path, words=words), bss_want(cnt=end - 2)[:6]
                    + [LW_T0 | lo(end - 2), JR_RA, 0], None)
    assert "(+4 bytes) is not in a known game .bss range" in result.unverified[0]


def test_overlapping_bss_objects_fail(tmp_path):
    # sp (.bss+4, 4 bytes) placed two bytes into op1's storage
    result = verify(bss_object(tmp_path), bss_want(sp=BSS + 2), None)
    assert not result.ok and len(result.failures) == 1
    assert "own .bss+0x0 at 0x80156940..0x80156944 and own .bss+0x4 at " \
           "0x80156942..0x80156946 overlap" in result.failures[0]
    assert result.sites == {20, 24} and set(result.bss()[".bss"]) == {8}
    # adjacent is fine
    assert verify(bss_object(tmp_path), bss_want(sp=BSS + 4, cnt=BSS + 8), None).ok


def test_bss_offset_outside_the_section_fails(tmp_path):
    words = [LUI_AT, LW_T0 | 0x40, JR_RA, 0]
    obj = bss_object(tmp_path, words=words, rels=[(0, ".bss", 5), (4, ".bss", 6)])
    result = verify(obj, [LUI_AT | hi(BSS), LW_T0 | lo(BSS), JR_RA, 0], None)
    assert "offset is outside the section" in result.failures[0]


def test_bss_mixed_with_rodata_in_one_function(tmp_path):
    words = LITERAL_WORDS[:4] + [LUI_AT, LB_T0, JR_RA, 0]
    rels = LITERAL_RELS + [(16, ".bss", 5), (20, ".bss", 6)]
    obj = build(tmp_path, words, rels, rodata=FLOATS + bytes(8))
    want = literal_want() + [LUI_AT | hi(BSS), LB_T0 | lo(BSS), JR_RA, 0]
    want = want[:4] + want[6:]
    result = verify(obj, want, image((RODATA, FLOATS)))
    assert result.ok and result.sites == set(range(0, 24, 4))
    assert result.bases() == {".rodata": RODATA}
    assert result.bss() == {".bss": {0: BSS}}
    assert [n.split(" (")[0] for n in result.notes] == [
        "own .bss placed at 0x80156940", "own .rodata verified at 0x80124000..0x80124008"]
    # a wrong literal does not affect the static, and vice versa
    result = verify(obj, want, image((RODATA, w(0x3B23D70A, 0x44BBA000))))
    assert "differs from retail 0x80124004" in result.failures[0]
    assert result.bss() == {".bss": {0: BSS}} and {16, 20} <= result.sites
    result = verify(obj, want[:4] + [LUI_AT | hi(0x80180000), LB_T0 | lo(0x80180000)]
                    + want[6:], image((RODATA, FLOATS)))
    assert not result.ok and result.bases() == {".rodata": RODATA} and not result.bss()


def test_stray_and_unpaired_own_relocations_are_never_sites(tmp_path):
    obj = build(tmp_path, [LUI_AT, LWC1_F4, LUI_AT, JR_RA],
                [(4, ".rodata", 6), (8, ".rodata", 5)], rodata=FLOATS)
    result = verify(obj, [LUI_AT, LWC1_F4, LUI_AT, JR_RA], image((0, FLOATS)))
    assert not result.sites and len(result.unverified) == 2
    assert owndata.own_references(obj, "f") == 2
    assert owndata.own_references(literal_object(tmp_path), "f", limit=8) == 2
    plain = build(tmp_path, [0x0C000000, JR_RA], [(0, "callee", 4)])
    assert owndata.own_references(plain, "f") == 0
    assert verify(plain, [0x0C000000, JR_RA], None).ok


def test_unknown_function_and_bad_objects_fail_closed(tmp_path):
    result = owndata.verify(literal_object(tmp_path), "missing", literal_want(),
                            address=F, image=image((RODATA, FLOATS)))
    assert "not a defined function" in result.failures[0] and not result.sites
    junk = tmp_path / "junk.o"
    junk.write_bytes(b"not an object")
    assert owndata.verify(junk, "f", [0], image=None).failures


# --- the tracked artefact -----------------------------------------------------

def test_image_data_reads_only_fully_known_ranges():
    data = image((0x1000, b"abcd"), (0x1004, b"efgh"), (0x2000, b"zz"))
    assert data.read(0x1002, 4) == b"cdef"
    assert data.read(0x1006, 4) is None and data.read(0x0FFF, 2) is None
    with pytest.raises(ValueError, match="overlapping"):
        image((0x1000, b"abcd"), (0x1002, b"x"))


def _region(tmp_path, image_bytes):
    asm = tmp_path / "blob"
    asm.mkdir()
    (asm / "blob_80100000.s").write_text(
        "/* region blob_80100000: 0x80100000-0x80100030 */\n.set noreorder\n\n"
        '.section .text.f, "ax", @progbits\n.globl f\nf:\n'
        "    .word 0x03E00008\n    .word 0x00000000\n\n"
        '.section .blobdata.op_80100008, "ax", @progbits\n'
        '    .incbin "build/game_code.bin", 8, 40\n')
    return asm


def test_artifact_round_trip_and_integrity(tmp_path):
    blob = w(JR_RA, 0) + bytes(range(40))
    asm = _region(tmp_path, blob)
    runs = owndata.opaque_runs(asm, blob)
    assert runs == [(0x80100008, bytes(range(40)))]
    with pytest.raises(SystemExit, match="differs from the tracked word"):
        owndata.opaque_runs(asm, w(JR_RA, 1) + bytes(40))
    out = owndata.artifact_dir(asm)
    assert out == tmp_path / "blob_data"
    assert owndata.ImageData.from_artifact(out) is None       # not tracked: nothing verifies
    out.mkdir()
    rendered = owndata.render_artifact(runs)
    (out / owndata.ARTIFACT_NAME).write_bytes(rendered)
    (out / "SHA256SUMS").write_text(
        f"{hashlib.sha256(rendered).hexdigest()}  {owndata.ARTIFACT_NAME}\n")
    data = owndata.ImageData.from_artifact(out)
    assert data.read(0x80100008, 40) == bytes(range(40)) and data.read(0x80100000, 4) is None
    (out / owndata.ARTIFACT_NAME).write_bytes(rendered.replace(b"0001", b"0002", 1))
    with pytest.raises(SystemExit, match="SHA-256 mismatch"):
        owndata.ImageData.from_artifact(out)
    (out / "SHA256SUMS").unlink()
    with pytest.raises(SystemExit, match="integrity check failed"):
        owndata.ImageData.from_artifact(out)


def test_tracked_artifact_is_intact_and_covers_the_data_segment():
    directory = owndata.artifact_dir(REPO / "asm" / "us" / "blob")
    if not directory.is_dir():
        pytest.skip("asm/us/blob_data is not tracked yet")
    data = owndata.ImageData.from_artifact(directory)
    # .data + .rodata, 0x8010FD7C..0x801249F0 (type_model/REPORT.md section 1).
    assert data.read(0x8010FD7C, 0x801249F0 - 0x8010FD7C) is not None
    assert data.read(0x80123E74, 4) == bytes.fromhex("3b23d70a")      # 0.0025f


# --- the scorer, with the reviewed patch ---------------------------------------

@pytest.fixture(scope="module")
def score(tmp_path_factory):
    source = REPO / "tools" / "cloud" / "score.py"
    if "owndata" in source.read_text():
        from tools.cloud import score as module         # patch already applied
        return module
    if shutil.which("patch") is None or not PATCH.is_file():
        pytest.skip("needs patch(1) and score_py.patch to build the patched scorer")
    out = tmp_path_factory.mktemp("patched") / "tools" / "cloud" / "score.py"
    out.parent.mkdir(parents=True)
    subprocess.run(["patch", "-s", "-o", str(out), str(source), str(PATCH)], check=True)
    sys.modules.setdefault("owndata", owndata)           # its script-mode import
    spec = importlib.util.spec_from_file_location("score_patched", out)
    module = importlib.util.module_from_spec(spec)
    sys.modules["score_patched"] = module
    spec.loader.exec_module(module)
    return module


def scored(score, monkeypatch, obj, want, img, addresses=None):
    table = {"f": F}
    table.update(addresses or {})
    monkeypatch.setattr(score, "targets", lambda: {"f": want})
    monkeypatch.setattr(score, "image_symbols", lambda: table)
    monkeypatch.setattr(score, "own_data", lambda: img)
    return score.compare(obj, "f", show=0)


def test_scorer_verified_literals_are_a_strict_match(tmp_path, monkeypatch, score):
    result = scored(score, monkeypatch, literal_object(tmp_path), literal_want(),
                    image((RODATA, FLOATS)))
    assert result.accepted() and result.summary() == "MATCH"
    assert result.notes == ("own .rodata verified at 0x80124000..0x80124008",)


def test_scorer_wrong_literal_fails_even_when_unverified_is_allowed(tmp_path, monkeypatch, score):
    obj = literal_object(tmp_path, w(0x3B23D70A, 0x44BBA000))
    result = scored(score, monkeypatch, obj, literal_want(), image((RODATA, FLOATS)))
    assert not result.accepted() and not result.accepted(True)
    assert "MATCH" not in result.summary()
    assert "differs from retail 0x80124004" in result.summary()
    assert len(result.unverified) == 4          # nothing is silently promoted


def test_scorer_literal_at_another_retail_address_fails(tmp_path, monkeypatch, score):
    img = image((RODATA, FLOATS + w(0, 0, 0x3F800000)))
    result = scored(score, monkeypatch, literal_object(tmp_path),
                    literal_want(second=RODATA + 0x10), img)
    assert not result.accepted(True) and result.differing == 0
    result = scored(score, monkeypatch, literal_object(tmp_path),
                    literal_want(first=RODATA + 4, second=RODATA),
                    image((RODATA, w(0x44BB8000, 0x3B23D70A))))
    assert not result.accepted(True) and "disagree" in result.summary()


def test_scorer_without_retail_data_reports_unverified_as_before(tmp_path, monkeypatch, score):
    for img in (None, image((0x80200000, FLOATS))):
        result = scored(score, monkeypatch, literal_object(tmp_path), literal_want(), img)
        assert not result.accepted() and result.accepted(True)
        assert result.summary().startswith("MATCH (4 section-relative relocations unverified: "
                                           ".rodata+0x0 at +0x0, .rodata+0x0 at +0x4")
        assert all(note.startswith("not verified: ") for note in result.notes)


def test_scorer_jump_table_right_and_wrong(tmp_path, monkeypatch, score):
    retail = image((TABLE, w(F + 0x14, F + 0x18, F + 0x1C)))
    result = scored(score, monkeypatch, table_object(tmp_path), table_want(), retail)
    assert result.accepted() and result.summary() == "MATCH"
    obj = table_object(tmp_path, entries=(0x14, 0x14, 0x1C))
    result = scored(score, monkeypatch, obj, table_want(), retail)
    assert not result.accepted(True) and "jump table entry 1" in result.summary()


def test_scorer_own_data_static_right_and_wrong(tmp_path, monkeypatch, score):
    statics = w(7, 0x41424300)
    obj = build(tmp_path, DATA_WORDS, DATA_RELS, data=statics + bytes(8))
    result = scored(score, monkeypatch, obj, data_want(), image((DATA, statics)))
    assert result.accepted() and result.summary() == "MATCH"
    assert result.notes[0].startswith("own .data verified at 0x80116000")
    result = scored(score, monkeypatch, obj, data_want(), image((DATA, w(7, 0x41424400))))
    assert not result.accepted(True)


def test_scorer_bss_statics_are_a_strict_match(tmp_path, monkeypatch, score):
    result = scored(score, monkeypatch, bss_object(tmp_path), bss_want(), None)
    assert result.accepted() and result.summary() == "MATCH"
    assert len(result.notes) == 3 and "verified by address only" in result.notes[0]


def test_scorer_bss_wrong_address_is_never_accepted(tmp_path, monkeypatch, score):
    obj = bss_object(tmp_path)
    # one of sp's two loads reads elsewhere: a failure, even with unverified allowed
    result = scored(score, monkeypatch, obj, bss_want(sp2=BSS + 0xC), None)
    assert not result.accepted(True) and "different addresses" in result.summary()
    # overlapping objects
    result = scored(score, monkeypatch, obj, bss_want(sp=BSS + 2), None)
    assert not result.accepted(True) and "overlap" in result.summary()
    # outside every known .bss range: unverified, as before this rule existed
    result = scored(score, monkeypatch, obj, bss_want(cnt=0x80180000), None)
    assert not result.accepted() and result.accepted(True)
    assert any("not in a known game .bss range" in n for n in result.notes)
    # a .bss reference read from initialised retail data
    result = scored(score, monkeypatch, obj, bss_want(op1=DATA), image((DATA, w(7))))
    assert not result.accepted(True) and "initialised bytes" in result.summary()


def test_scorer_partly_verified_function_is_not_a_match(tmp_path, monkeypatch, score):
    """One literal proven, one static not: every masked word must be proven."""
    words = [LUI_AT, LWC1_F4, LUI_AT, LW_T0, JR_RA, 0]
    rels = [(0, ".rodata", 5), (4, ".rodata", 6), (8, ".bss", 5), (12, ".bss", 6)]
    obj = build(tmp_path, words, rels, rodata=FLOATS)
    outside = 0x80180000
    want = [LUI_AT | hi(RODATA), LWC1_F4 | lo(RODATA), LUI_AT | hi(outside),
            LW_T0 | lo(outside), JR_RA, 0]
    result = scored(score, monkeypatch, obj, want, image((RODATA, FLOATS)))
    assert not result.accepted() and result.accepted(True) and len(result.unverified) == 4
    assert "own .rodata verified" in result.notes[0]
    want[2:4] = [LUI_AT | hi(BSS), LW_T0 | lo(BSS)]
    result = scored(score, monkeypatch, obj, want, image((RODATA, FLOATS)))
    assert result.accepted() and result.summary() == "MATCH"


# Verified literals must not relax anything else in the same function.
MIXED_WORDS = LITERAL_WORDS[:4] + [0x0C000000, 0, LUI_AT, ADDIU_A0 | 0x10, JR_RA, 0]
MIXED_RELS = LITERAL_RELS + [(16, "callee", 4), (24, "D_80110000", 5), (28, "D_80110000", 6)]


def mixed_want(callee=0x80010000, data=0x80110010):
    return literal_want()[:4] + [0x0C000000 | (callee >> 2) & 0x03FFFFFF, 0,
                                 LUI_AT | hi(data), ADDIU_A0 | lo(data), JR_RA, 0]


def mixed(tmp_path, rels=MIXED_RELS):
    return build(tmp_path, MIXED_WORDS, rels, rodata=FLOATS + bytes(8))


def test_scorer_wrong_callee_global_and_addend_still_fail(tmp_path, monkeypatch, score):
    img = image((RODATA, FLOATS))
    known = {"callee": 0x80010000}
    result = scored(score, monkeypatch, mixed(tmp_path), mixed_want(), img, known)
    assert result.accepted() and result.summary() == "MATCH"
    # wrong callee: the retail word calls something else
    result = scored(score, monkeypatch, mixed(tmp_path), mixed_want(callee=0x80020000), img, known)
    assert result.differing == 1 and not result.accepted(True)
    # unknown callee: never a match, even though the literals verify
    result = scored(score, monkeypatch, mixed(tmp_path), mixed_want(), img)
    assert result.unresolved and not result.accepted(True) and "MATCH" not in result.summary()
    # wrong addend on the named global
    result = scored(score, monkeypatch, mixed(tmp_path), mixed_want(data=0x80110050), img, known)
    assert result.differing == 1 and not result.accepted(True)
    # wrong global: an unknown name where retail addresses D_80110000
    rels = LITERAL_RELS + [(16, "callee", 4), (24, "D_WRONG", 5), (28, "D_WRONG", 6)]
    result = scored(score, monkeypatch, mixed(tmp_path, rels), mixed_want(), img, known)
    assert result.unresolved and not result.accepted(True)
    # wrong known global
    result = scored(score, monkeypatch, mixed(tmp_path), mixed_want(), img,
                    {"callee": 0x80010000, "D_80110000": 0x80110200})
    assert result.differing > 0 and not result.accepted(True)
    # an instruction that differs outside the relocated bits of a literal load
    want = mixed_want()
    want[1] = LWC1_F6 | lo(RODATA)
    result = scored(score, monkeypatch, mixed(tmp_path), want, img, known)
    assert result.differing == 1 and not result.accepted(True)


def test_scorer_cli_prints_notes_and_exit_status(tmp_path, monkeypatch, capsys, score):
    obj = literal_object(tmp_path)
    monkeypatch.setattr(score, "compile_single", lambda source, flags, out: shutil.copy(obj, out))
    monkeypatch.setattr(score, "targets", lambda: {"f": literal_want()})
    monkeypatch.setattr(score, "image_symbols", lambda: {"f": F})
    monkeypatch.setattr(score, "own_data", lambda: image((RODATA, FLOATS)))
    monkeypatch.setattr("sys.argv", ["score.py", "fn", "ignored.c", "f"])
    assert score.main() == 0
    assert capsys.readouterr().out == ("f:\n  MATCH\n"
                                       "    own .rodata verified at 0x80124000..0x80124008\n")
    monkeypatch.setattr(score, "own_data", lambda: image((RODATA, w(1, 2))))
    monkeypatch.setattr("sys.argv", ["score.py", "fn", "ignored.c", "f", "--allow-unverified"])
    assert score.main() == 1
    assert "differs from retail" in capsys.readouterr().out


# --- the single-function splice link -------------------------------------------

binutils = pytest.mark.skipif(shutil.which("mips-linux-gnu-as") is None
                              or shutil.which("mips-linux-gnu-ld") is None,
                              reason="needs the mips binutils")
BASE = 0x80100000


def _assembled(tmp_path, body, rodata="", data=""):
    source = (".set noreorder\n.set noat\n.section .text.f,\"ax\",@progbits\n"
              ".globl f\n.type f,@function\nf:\n" + body + ".size f, .-f\n")
    if rodata:
        source += ".section .rodata\n" + rodata
    if data:
        source += ".data\n.balign 4\n" + data
    src, obj = tmp_path / "f.s", tmp_path / "f.o"
    src.write_text(source)
    subprocess.run(["mips-linux-gnu-as", "-EB", "-mips2", "-32", "-o", str(obj), str(src)],
                   check=True)
    return obj


def _retail(function_words, **data):
    blob = bytearray(0x25000)
    blob[:len(function_words) * 4] = w(*function_words)
    for address, raw in data.values():
        blob[address - BASE:address - BASE + len(raw)] = raw
    return bytes(blob), BASE


LITERAL_ASM = (" lui $at,%hi(lit)\n lwc1 $f4,%lo(lit)($at)\n"
               " lui $at,%hi(lit+4)\n lwc1 $f6,%lo(lit+4)($at)\n jr $ra\n nop\n")


@binutils
def test_splice_link_places_verified_literals_at_the_retail_address(tmp_path):
    from tools.conveyor.pipeline import blob_build, blob_splice
    obj = _assembled(tmp_path, LITERAL_ASM, rodata=".balign 16\nlit:\n .word 0x3B23D70A, 0x44BB8000\n")
    # As IDO does, the section is aligned to 16; retail keeps the literals at +4.
    at = RODATA + 4
    retail = _retail(literal_want(at, at + 4), lit=(at, FLOATS))
    work = tmp_path / "work"
    body = blob_splice.link_function(obj, "f", F, 24, provides={}, work=work, image=retail)
    assert body == w(*literal_want(at, at + 4))
    assert ".own0 0x80124004 : SUBALIGN(1) { *(.rodata) }" in (work / "f.ld").read_text()
    wrong = _retail(literal_want(at, at + 4), lit=(at, w(0x3B23D70A, 0x44BBA000)))
    with pytest.raises(blob_build.BuildError, match="differs from retail 0x80124008"):
        blob_splice.link_function(obj, "f", F, 24, provides={}, work=work, image=wrong)
    # The literal is right but retail's code reads it from somewhere else.
    moved = _retail(literal_want(at, at + 0x10), lit=(at, FLOATS + w(0, 0, 0x44BB8000)))
    with pytest.raises(blob_build.BuildError, match="disagree on the section's image address"):
        blob_splice.link_function(obj, "f", F, 24, provides={}, work=work, image=moved)


@binutils
def test_splice_link_verifies_jump_table_and_local_static(tmp_path):
    from tools.conveyor.pipeline import blob_build, blob_splice
    body = (" lui $at,%hi(table)\n addu $at,$at,$t0\n lw $t0,%lo(table)($at)\n jr $t0\n nop\n"
            "one:\n lui $at,%hi(counter)\n"
            "two:\n lw $t0,%lo(counter)($at)\n"
            "three:\n jr $ra\n nop\n")
    obj = _assembled(tmp_path, body, rodata="table:\n .word one, two, three\n",
                     data="counter:\n .word 7\n")
    want = [LUI_AT | hi(TABLE), 0x00280821, LW_T0 | lo(TABLE), 0x01000008, 0,
            LUI_AT | hi(DATA), LW_T0 | lo(DATA), JR_RA, 0]
    table = w(F + 0x14, F + 0x18, F + 0x1C)
    retail = _retail(want, table=(TABLE, table), counter=(DATA, w(7)))
    work = tmp_path / "work"
    assert blob_splice.link_function(obj, "f", F, 36, provides={}, work=work,
                                     image=retail) == w(*want)
    script = (work / "f.ld").read_text()
    assert ".own0 0x80116000 : SUBALIGN(1) { *(.data) }" in script
    assert ".own1 0x80123870 : SUBALIGN(1) { *(.rodata) }" in script
    bad = _retail(want, table=(TABLE, w(F + 0x14, F + 0x1C, F + 0x1C)), counter=(DATA, w(7)))
    with pytest.raises(blob_build.BuildError, match="jump table entry 1"):
        blob_splice.link_function(obj, "f", F, 36, provides={}, work=work, image=bad)
    bad = _retail(want, table=(TABLE, table), counter=(DATA, w(8)))
    with pytest.raises(blob_build.BuildError, match="own .data\\+0x0 referenced at \\+0x18"):
        blob_splice.link_function(obj, "f", F, 36, provides={}, work=work, image=bad)


@binutils
def test_splice_link_without_own_data_is_unchanged_and_never_reads_the_image(tmp_path,
                                                                             monkeypatch):
    from tools.conveyor.pipeline import blob_splice
    obj = _assembled(tmp_path, " lui $a0,%hi(D_80110000)\n jal callee\n"
                               " addiu $a0,$a0,%lo(D_80110000)\n jr $ra\n nop\n")
    monkeypatch.setattr(blob_splice, "_retail_image",
                        lambda: pytest.fail("image read for an object without own data"))
    work = tmp_path / "work"
    body = blob_splice.link_function(obj, "f", F, 20, provides={"callee": 0x80010000},
                                     work=work)
    assert body == w(0x3C048011, 0x0C004000, 0x24840000, JR_RA, 0)
    script = (work / "f.ld").read_text()
    assert ".own" not in script
    assert script == ("SECTIONS\n{\n    . = 0x80100000;\n    .out 0x80100000 : SUBALIGN(4) { *(.text.f) }\n"
                      "    PROVIDE(D_80110000 = 0x80110000);\n"
                      "    PROVIDE(callee = 0x80010000);\n"
                      "    /DISCARD/ : { *(.pdr) *(.mdebug*) *(.comment) *(.note*)"
                      " *(.reginfo) *(.options) *(.MIPS.abiflags) }\n}\n")


BSS_ASM = (" lui $at,%hi(op1)\n sw $t0,%lo(op1)($at)\n"
           " lui $at,%hi(sp)\n lw $t0,%lo(sp)($at)\n lw $t0,%lo(sp)($at)\n"
           " lui $at,%hi(cnt)\n lh $t0,%lo(cnt)($at)\n jr $ra\n nop\n")


def _bss_object(tmp_path, body=BSS_ASM, rodata=""):
    return _assembled(tmp_path, body, rodata=rodata,
                      data=".bss\n.balign 16\nop1: .space 4\nsp: .space 4\ncnt: .space 8\n")


@binutils
def test_splice_link_places_bss_objects_per_object_and_emits_nothing(tmp_path):
    from tools.conveyor.pipeline import blob_build, blob_splice
    obj = _bss_object(tmp_path)
    work = tmp_path / "work"
    for want in (bss_want(), bss_want(op1=BSS + 0x100, sp=BSS, cnt=BSS + 0x10),
                 bss_want(op1=BSS, sp=BSS + 4, cnt=BSS + 8)):
        body = blob_splice.link_function(obj, "f", F, 36, provides={}, work=work,
                                         image=_retail(want))
        assert body == w(*want)
    script = (work / "f.ld").read_text()
    # one NOLOAD section at the first object's delta; nothing else is emitted
    assert ".ownbss0 0x80156940 (NOLOAD) : SUBALIGN(1) { *(.bss) }" in script
    assert len(body) == 36
    # disagreeing references, overlap: refused, naming the object
    with pytest.raises(blob_build.BuildError, match=r"own \.bss\+0x4 .*different addresses"):
        blob_splice.link_function(obj, "f", F, 36, provides={}, work=work,
                                  image=_retail(bss_want(sp2=BSS + 0xC)))
    with pytest.raises(blob_build.BuildError, match="overlap"):
        blob_splice.link_function(obj, "f", F, 36, provides={}, work=work,
                                  image=_retail(bss_want(sp=BSS + 2)))


@binutils
def test_splice_link_bss_outside_game_bss_is_not_placed(tmp_path):
    """An unverified object is not placed: its words then differ from retail
    and the image gate refuses the body, exactly as before the .bss rule."""
    from tools.conveyor.pipeline import blob_splice
    obj = _bss_object(tmp_path)
    work = tmp_path / "work"
    want = bss_want(cnt=0x80180000)
    body = blob_splice.link_function(obj, "f", F, 36, provides={}, work=work,
                                     image=_retail(want))
    # op1 and sp are proven and placed; cnt is not, and its words differ
    got = struct.unpack(">9I", body)
    assert [i for i in range(9) if got[i] != want[i]] == [5, 6]


@binutils
def test_splice_link_bss_with_a_literal(tmp_path):
    from tools.conveyor.pipeline import blob_splice
    obj = _bss_object(tmp_path, body=LITERAL_ASM.replace(" jr $ra\n nop\n", "") + BSS_ASM,
                      rodata=".balign 16\nlit:\n .word 0x3B23D70A, 0x44BB8000\n")
    want = literal_want()[:4] + bss_want()
    work = tmp_path / "work"
    body = blob_splice.link_function(obj, "f", F, 52, provides={}, work=work,
                                     image=_retail(want, lit=(RODATA, FLOATS)))
    assert body == w(*want)
    script = (work / "f.ld").read_text()
    assert ".own0 0x80124000 : SUBALIGN(1) { *(.rodata) }" in script
    assert ".ownbss0 0x80156940 (NOLOAD)" in script
