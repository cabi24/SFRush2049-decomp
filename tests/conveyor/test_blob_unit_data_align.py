"""`data_align` (w21d): a .data pad so a data symbol sits at its image address modulo 16.

as1 shares one `lui at` across loads of a symbol defined in the unit only inside
one 16-byte-aligned chunk of .data (w20h, entity_render_setup's D_8011AD90). The
shadow unit's .data is emitted in LINK order (unlike .text), so the pad is an
initialised, kept char array linked immediately before the file that defines the
symbol; its size is recomputed after every build, like the text `align` pads.
"""
import json
from types import SimpleNamespace

import pytest

from tools.conveyor.pipeline import blob_unit as bu

from .test_blob_unit import _fixture

ENTRY = {"name": "D_8011AD90", "reason": "lui sharing"}


def test_data_align_entries_need_a_reason_and_an_address():
    ok = bu.validate_overrides({"data_align": [ENTRY]})
    assert ok["data_align"] == [ENTRY] and bu.data_align_target(ENTRY) == 0
    assert bu.data_align_target({"name": "D_8011AD94"}) == 4
    assert bu.data_align_target({"name": "table", "address": "8011AD98"}) == 8
    with pytest.raises(bu.UnitError, match="no reason"):
        bu.validate_overrides({"data_align": [{"name": "D_8011AD90"}]})
    with pytest.raises(bu.UnitError, match="needs an address"):
        bu.validate_overrides({"data_align": [{"name": "table", "reason": "r"}]})
    with pytest.raises(bu.UnitError, match="not hex"):
        bu.validate_overrides({"data_align": [{"name": "t", "reason": "r", "address": "x"}]})


def test_tracked_override_aligns_d_8011ad90():
    entry, = [e for e in bu.load_overrides()["data_align"] if e["name"] == "D_8011AD90"]
    assert bu.data_align_target(entry) == 0 and "entity_render_setup" in entry["reason"]


SRC = """# 1 "x.c"
typedef struct { float x, y, z; } Vec3;
extern Vec3 D_8011AD90[4];
extern Vec3 D_8011AD90[4];
Vec3 D_tentative[4];
struct S { int D_member; } s_init = { 1 };
static int local_data = 3;
Vec3 D_8011AD90[4] = { {1.0f, 1.0f, 0.0f}, {-1.0f, 1.0f, 0.0f},
                       {-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f} };
void f(void)
{
    static int D_inner = 1;
    int D_local = 2;
    D_8011AD90[0].x = D_local;
}
int D_after = 4, D_second[2] = { 1, 2 };
"""


def test_scan_data_defs_finds_initialised_file_scope_definitions_only():
    names = {"D_8011AD90", "D_tentative", "D_member", "D_inner", "D_local", "D_after",
             "D_second", "local_data"}
    assert bu.scan_data_defs(SRC, names) == {"D_8011AD90", "D_after", "D_second", "local_data"}
    assert bu.scan_data_defs(SRC.replace("Vec3 D_8011AD90[4] = {", "Vec3 X[4] = {"),
                             {"D_8011AD90"}) == set()
    assert bu.scan_data_defs(SRC, set()) == set()


def _derive(data_defs, data_pads, entries=(ENTRY,)):
    lock, specs, files, defs, addresses = _fixture()
    over = bu.validate_overrides({"data_align": list(entries)})
    return bu.derive(lock, specs, files, defs, addresses, over,
                     data_defs=data_defs, data_pads=data_pads)


def test_data_pad_is_linked_immediately_before_the_defining_file_and_kept():
    m = _derive({"g_grp__group.c": ["D_8011AD90"]}, {"D_8011AD90": 8})
    ids = [f["id"] for f in m["files"]]
    assert ids[ids.index("g_grp__group.c") - 1] == "q_D_8011AD90.c"
    pad = m["files"][ids.index("q_D_8011AD90.c")]
    assert pad["kind"] == "data_pad" and pad["pad_bytes"] == 8 and pad["defines"] == []
    assert pad["pad_symbols"] == ["__unit_dpad_D_8011AD90"]
    assert "__unit_dpad_D_8011AD90" in m["keep"]
    assert "__unit_dpad_D_8011AD90" not in m["functions"] and m["data_align"] == ["D_8011AD90"]
    # no pad needed: no file; the symbol is still tracked
    m = _derive({"g_grp__group.c": ["D_8011AD90"]}, {"D_8011AD90": 16})
    assert not any(f["kind"] == "data_pad" for f in m["files"]) and m["data_align"] == ["D_8011AD90"]


def test_undefined_and_doubly_defined_data_symbols():
    m = _derive({}, {"D_8011AD90": 8})
    assert m["data_align"] == [] and not any(f["kind"] == "data_pad" for f in m["files"])
    m = _derive({"s_leaf.c": ["D_8011AD90"], "g_grp__group.c": ["D_8011AD90"]}, {})
    assert "D_8011AD90" in m["problems"] and m["data_align"] == []


def test_stage_writes_the_zero_filled_data_pad(tmp_path):
    m = _derive({"g_grp__group.c": ["D_8011AD90"]}, {"D_8011AD90": 4})
    texts = {f["id"]: "" for f in m["files"] if f["kind"] not in ("pad", "data_pad")}
    out = bu.stage(m, texts, tmp_path / "stage")
    assert (out / "q_D_8011AD90.c").read_text() == "char __unit_dpad_D_8011AD90[4] = {0};\n"
    assert "q_D_8011AD90.c" in (out / "files.txt").read_text().split()
    assert "__unit_dpad_D_8011AD90" in (out / "keep.txt").read_text().split()


def _obj(**offsets):
    symbols = [dict(name=".data", value=0, type=bu.STT_SECTION, shndx=6)]
    symbols += [dict(name=n, value=v, type=1, shndx=6) for n, v in offsets.items()]
    symbols.append(dict(name="text_fn", value=0x48, type=bu.STT_FUNC, shndx=1))
    return SimpleNamespace(index={".text": 1, ".data": 6}, symbols=symbols)


def test_data_alignment_pads_converge_and_follow_earlier_data_changes():
    entries = [ENTRY]
    # w20h: the unit put the table at 72; 8 bytes move it to 80 = 0 mod 16
    assert bu.data_alignment_pads(_obj(D_8011AD90=0x48), entries, {}) == {"D_8011AD90": 8}
    assert bu.data_alignment_pads(_obj(D_8011AD90=0x50), entries, {"D_8011AD90": 8}) == {
        "D_8011AD90": 8}
    # an earlier file later gains 20 bytes of .data: built with the old pad
    # the table lands at 0x64; the pad shrinks to 4 (0x60)
    assert bu.data_alignment_pads(_obj(D_8011AD90=0x64), entries, {"D_8011AD90": 8}) == {
        "D_8011AD90": 4}
    # aligned without a pad: none
    assert bu.data_alignment_pads(_obj(D_8011AD90=0x40), entries, {}) == {}
    # not in this object (not defined in the unit): no pad
    assert bu.data_alignment_pads(_obj(), entries, {}) == {}


def test_data_alignment_pads_account_for_earlier_pads():
    entries = [ENTRY, {"name": "D_80120008", "reason": "r"}]
    # the first pad (8) moves the second symbol from 0x88 to 0x90 = 0 mod 16,
    # which wants 8: it needs 8 more
    pads = bu.data_alignment_pads(_obj(D_8011AD90=0x48, D_80120008=0x88), entries, {})
    assert pads == {"D_8011AD90": 8, "D_80120008": 8}
    assert bu.data_alignment_pads(_obj(D_8011AD90=0x50, D_80120008=0x98), entries, pads) == pads


def test_manifest_with_a_data_pad_round_trips_as_json():
    m = _derive({"g_grp__group.c": ["D_8011AD90"]}, {"D_8011AD90": 8})
    assert json.loads(json.dumps(m)) == m
