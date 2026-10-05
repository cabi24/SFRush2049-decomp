"""Whole-program shadow unit: the pure parts (no IDO, no builder)."""
import json
import shutil
import struct
import subprocess

import pytest

from tools.conveyor.pipeline import blob_unit as bu


# --- scanning and staging transforms ---------------------------------------

SRC = """# 1 "x.c"
typedef int s32;
struct P { int a; };
extern int table[];
int data[2] = { 1, 2 };
void proto(int a);
static int helper(int a) { return a + 1; }
void first(void)
{
    char *s = "}{ (";   /* } */
    if (table[0]) { proto(1); }
}
int second(a, b)
int a;
int b;
{
    return helper(a) + b;
}
"""


def test_scan_defs_finds_only_top_level_function_definitions():
    defs = bu.scan_defs(SRC)
    assert [(d["name"], d["static"]) for d in defs] == [
        ("helper", True), ("first", False), ("second", False)]
    first = defs[1]
    assert SRC[first["open"]] == "{" and SRC[first["close"]] == "}"
    assert SRC[first["close"] + 1:].lstrip().startswith("int second")
    assert defs[2]["kr"] is not None and defs[1]["kr"] is None


def test_transform_strips_other_definitions_to_prototypes():
    text, kept = bu.transform(SRC, {"first"})
    assert kept == ["helper", "first"]            # statics always stay
    assert "int second();" in text                # K&R: unprototyped declaration
    assert "return helper(a) + b" not in text
    assert "proto(1)" in text
    assert [d["name"] for d in bu.scan_defs(text)] == ["helper", "first"]


def test_transform_blocker_goes_inside_the_kept_body_only():
    text, _ = bu.transform(SRC, {"first", "second"}, block={"first", "absent"})
    assert text.count("if (0) {") == 1
    assert text.startswith(f"extern int {bu.DEAD_SYMBOL}[];")
    body = text[text.index("void first"):text.index("int second")]
    assert "if (0) {" in body and body.rstrip().endswith("}")
    # a blocker for a function this file does not keep changes nothing
    stripped, _ = bu.transform(SRC, {"second"}, block={"first"})
    assert bu.DEAD_SYMBOL not in stripped and "void first(void)\n;" in stripped


def test_transform_renames_a_stand_in_everywhere():
    src = "void standin(void) { f(); }\nvoid f(void) { standin(); }\n"
    text, kept = bu.transform(src, {"standin__u1", "f"}, renames={"standin": "standin__u1"})
    assert kept == ["standin__u1", "f"]
    assert "standin(" not in text.replace("standin__u1(", "")


# --- overrides ----------------------------------------------------------------

def test_overrides_need_a_reason_and_known_keys():
    ok = bu.validate_overrides({"force_keep": [{"name": "f", "reason": "because"}]})
    assert ok["force_keep"][0]["name"] == "f" and ok["inline_blockers"] == []
    with pytest.raises(bu.UnitError, match="no reason"):
        bu.validate_overrides({"inline_blockers": [{"name": "f"}]})
    with pytest.raises(bu.UnitError, match="unknown keys"):
        bu.validate_overrides({"blockers": []})
    with pytest.raises(bu.UnitError, match="both force_keep and force_internal"):
        bu.validate_overrides({"force_keep": [{"name": "f", "reason": "a"}],
                               "force_internal": [{"name": "f", "reason": "b"}]})
    with pytest.raises(bu.UnitError, match="needs a file"):
        bu.validate_overrides({"prefer_definition": [{"name": "f", "reason": "a"}]})
    with pytest.raises(bu.UnitError, match="twice"):
        bu.validate_overrides({"align": [{"name": "f", "reason": "a"},
                                         {"name": "f", "reason": "b"}]})


def test_tracked_overrides_file_is_valid():
    doc = bu.load_overrides()
    assert all(e["reason"] for key in bu.OVERRIDE_KEYS for e in doc[key])


# --- manifest -------------------------------------------------------------------

def _g(*names, static=()):
    return [dict(name=n, static=n in static) for n in names]


def _fixture():
    """Two singles, one group with a member, a private copy of a locked single
    as context, an unmatched context function and a stand-in; a second group
    with its own stand-in of the same name."""
    lock = {
        "leaf": {"source": "src/blob/leaf.c", "flagset": "-O2"},
        "caller": {"source": "src/blob/caller.c", "flagset": "-O2 -Xcpluscomm"},
        "member": {"group": "grp", "source": "src/blob/groups/grp/group.json", "flagset": "-O3"},
        "other": {"group": "grp2", "source": "src/blob/groups/grp2/group.json",
                  "flagset": "-O3"},
    }
    specs = {"grp": {"files": ["group.c"], "keep": ["member", "__standin"], "flags": "-O3"},
             "grp2": {"files": ["a.c"], "keep": ["other", "__standin"], "flags": "-O3"}}
    files, problems = bu.unit_files(lock, specs)
    assert not problems
    defs = {
        "s_leaf.c": _g("leaf"),
        "s_caller.c": _g("caller", "local", static=("local",)),
        "g_grp__group.c": _g("leaf", "ctx", "member", "__standin"),
        "g_grp2__a.c": _g("other", "ctx", "__standin"),
    }
    addresses = {"leaf": 0x80090000, "caller": 0x80091000, "member": 0x80092000,
                 "ctx": 0x80093000, "other": 0x80094000}
    return lock, specs, files, defs, addresses


def test_unit_files_names_and_missing_group():
    lock, specs, files, _, _ = _fixture()
    assert [f["id"] for f in files] == ["s_caller.c", "s_leaf.c", "g_grp__group.c",
                                        "g_grp2__a.c"]
    assert [f["xcpluscomm"] for f in files] == [True, False, False, False]
    del specs["grp2"]
    files, problems = bu.unit_files(lock, specs)
    assert problems == {"other": "group grp2 has no group.json"}
    assert "g_grp2__a.c" not in [f["id"] for f in files]


def test_one_definition_per_function():
    lock, specs, files, defs, addresses = _fixture()
    m = bu.derive(lock, specs, files, defs, addresses)
    fn = m["functions"]
    # the locked single owns its name; the group's private copy is stripped
    assert fn["leaf"] == "s_leaf.c"
    assert m["duplicates"]["leaf"]["stripped_from"] == ["g_grp__group.c"]
    assert fn["member"] == "g_grp__group.c" and fn["other"] == "g_grp2__a.c"
    # unmatched context defined twice: first file by name, the other stripped
    assert fn["ctx"] == "g_grp2__a.c"
    assert m["duplicates"]["ctx"]["stripped_from"] == ["g_grp__group.c"]
    # stand-ins that share a name are different procedures
    assert "__standin" not in fn
    assert fn["__standin__u0"] == "g_grp2__a.c" and fn["__standin__u1"] == "g_grp__group.c"
    by_id = {f["id"]: f for f in m["files"]}
    assert by_id["g_grp__group.c"]["renames"] == {"__standin": "__standin__u1"}
    assert by_id["g_grp__group.c"]["stripped"] == ["ctx", "leaf"]
    assert by_id["g_grp__group.c"]["defines"] == ["__standin__u1", "member"]
    # every global name is defined by exactly one staged file
    owners = [n for f in m["files"] for n in f["defines"]]
    assert len(owners) == len(set(owners)) == len(fn)
    assert m["members"] == sorted(lock) and not m["problems"]
    assert m["counts"]["duplicates_locked"] == 1


def test_keep_list_and_internal_set():
    lock, specs, files, defs, addresses = _fixture()
    m = bu.derive(lock, specs, files, defs, addresses)
    # internal = what a locked group defines and does not keep; that includes a
    # locked single a group lists as internal context
    assert m["internal"] == ["ctx", "leaf"]
    assert {"caller", "member", "other", "__standin__u0", "__standin__u1"} <= set(m["keep"])
    over = bu.validate_overrides({
        "force_keep": [{"name": "leaf", "reason": "standalone body is emptied otherwise"}],
        "force_internal": [{"name": "caller", "reason": "test"}],
        "inline_blockers": [{"name": "leaf", "reason": "retail jal"},
                            {"name": "gone", "reason": "stale"}]})
    m = bu.derive(lock, specs, files, defs, addresses, over)
    assert m["internal"] == ["caller", "ctx"]
    assert m["forced_keep"] == ["leaf"] and m["blockers"] == ["leaf"]
    assert m["unused_overrides"] == ["gone"]
    by_id = {f["id"]: f for f in m["files"]}
    assert by_id["s_leaf.c"]["blocked"] == ["leaf"]
    assert by_id["g_grp__group.c"]["blocked"] == []       # its copy of leaf is stripped


def test_new_lock_members_default_to_keep_and_nothing_assumes_a_count():
    lock, specs, files, defs, addresses = _fixture()
    for k in range(40):
        name = f"func_8009{k:04X}"
        lock[name] = {"source": f"src/blob/{name}.c", "flagset": "-O3"}
        addresses[name] = 0x80095000 + 16 * k
    files, _ = bu.unit_files(lock, specs)
    for f in files:
        defs.setdefault(f["id"], _g(f["owner"]))
    m = bu.derive(lock, specs, files, defs, addresses)
    assert m["counts"]["locked"] == 44 and len(m["members"]) == 44
    assert all(n in m["keep"] for n in lock if n.startswith("func_8009"))
    assert m["internal"] == ["ctx", "leaf"]


def test_link_order_is_descending_address_of_the_lowest_locked_function():
    lock, specs, files, defs, addresses = _fixture()
    m = bu.derive(lock, specs, files, defs, addresses)
    # grp's private copy of locked `leaf` is stripped, so `member` places the file
    assert [f["id"] for f in m["files"]] == ["g_grp2__a.c", "g_grp__group.c", "s_caller.c",
                                             "s_leaf.c"]
    assert [f["address"] for f in m["files"]] == sorted(
        (f["address"] for f in m["files"]), reverse=True)


def test_prefer_definition_override_and_stale_override():
    lock, specs, files, defs, addresses = _fixture()
    over = bu.validate_overrides({"prefer_definition": [
        {"name": "leaf", "file": "src/blob/groups/grp/group.c", "reason": "callers pass an arg"}]})
    m = bu.derive(lock, specs, files, defs, addresses, over)
    assert m["functions"]["leaf"] == "g_grp__group.c"
    assert m["duplicates"]["leaf"]["stripped_from"] == ["s_leaf.c"]
    assert m["duplicates"]["leaf"]["reason"].startswith("override:")
    assert "leaf" in m["members"]
    stale = bu.validate_overrides({"prefer_definition": [
        {"name": "leaf", "file": "src/blob/groups/grp2/a.c", "reason": "x"}]})
    with pytest.raises(bu.UnitError, match="stale override"):
        bu.derive(lock, specs, files, defs, addresses, stale)


def test_candidate_source_wins_and_is_kept_unless_made_internal():
    lock, specs, _, defs, addresses = _fixture()
    files, _ = bu.unit_files(lock, specs, extras=["/tmp/work/new-fn.c"])
    assert files[-1]["id"] == "c_new_fn.c" and files[-1]["kind"] == "candidate"
    defs["c_new_fn.c"] = _g("ctx", "leaf")
    m = bu.derive(lock, specs, files, defs, addresses)
    assert m["functions"]["ctx"] == "c_new_fn.c" and m["functions"]["leaf"] == "c_new_fn.c"
    assert "ctx" in m["internal"]                 # grp still says internal
    m = bu.derive(lock, specs, files, defs, addresses, keep=["ctx"], internal=["caller"],
                  block=["ctx"])
    assert "ctx" in m["keep"] and "caller" in m["internal"] and m["blockers"] == ["ctx"]
    # placed by its lowest image function
    ids = [f["id"] for f in m["files"]]
    assert ids[-1] == "c_new_fn.c"          # leaf (lowest address) is now defined here
    assert ids[0] == "s_leaf.c"             # ... and its locked file defines nothing


def test_locked_function_without_a_definition_is_a_problem_not_a_crash():
    lock, specs, files, defs, addresses = _fixture()
    defs["s_caller.c"] = _g("renamed_caller")
    m = bu.derive(lock, specs, files, defs, addresses)
    assert m["problems"] == {"caller": "no definition of it in its locked source"}
    assert "caller" not in m["members"]


def test_alignment_pad_is_linked_after_its_file_and_kept():
    lock, specs, files, defs, addresses = _fixture()
    m = bu.derive(lock, specs, files, defs, addresses, pads={"member": 5, "leaf": 0})
    ids = [f["id"] for f in m["files"]]
    assert ids[ids.index("g_grp__group.c") + 1] == "p_member.c"
    assert "p_leaf.c" not in ids
    pad = m["files"][ids.index("p_member.c")]
    text, names = bu.pad_source("member", 5)
    assert pad["defines"] == names and set(names) <= set(m["keep"])
    assert len(bu.scan_defs(text)) == len(names) == 2


def test_pad_shapes_cover_every_residue():
    for words, (empty, store) in bu.PAD_SHAPE.items():
        assert (2 * empty + 3 * store) % (bu.ALIGN // 4) == words
        assert len(bu.pad_source("f", words)[1]) == empty + store


def test_alignment_pads_account_for_earlier_pads():
    base = 0x80000000
    extents = {"a": (base + 0x104, 64), "b": (base + 0x228, 64)}
    funcs = {"a": 0x100, "b": 0x224}
    pads = bu.alignment_pads(funcs, extents, base, ["a", "b", "absent"], {})
    # a needs one word (emitted as 9); that moves b by 36 bytes = 4 mod 32, so
    # b, one word short before, needs nothing
    assert pads == {"a": 1}
    assert bu.alignment_pads({"a": 0x124, "b": 0x248}, extents, base, ["a", "b"], pads) == pads


def test_stage_writes_only_changes_and_removes_stale_files(tmp_path):
    lock, specs, files, defs, addresses = _fixture()
    over = bu.validate_overrides({"inline_blockers": [{"name": "leaf", "reason": "r"}]})
    m = bu.derive(lock, specs, files, defs, addresses, over, pads={"member": 2})
    texts = {
        "s_leaf.c": "int leaf(int a) { return a; }\n",
        "s_caller.c": "static int local(void) { return 1; }\nint caller(void) { return local(); }\n",
        "g_grp__group.c": ("int leaf(int a) { return a; }\nvoid ctx(void) {}\n"
                           "int member(void) { return leaf(1); }\n"
                           "void __standin(void) { member(); }\n"),
        "g_grp2__a.c": "void ctx(void) {}\nvoid other(void) { ctx(); }\nvoid __standin(void) { other(); }\n",
    }
    out = tmp_path / "stage"
    out.mkdir()
    (out / "s_stale.c").write_text("old")
    bu.stage(m, texts, out)
    names = sorted(p.name for p in out.iterdir())
    assert names == sorted([f["id"] for f in m["files"]]
                           + ["keep.txt", "files.txt", "units.txt", "build.sh"])
    assert "if (0) {" in (out / "s_leaf.c").read_text()
    group = (out / "g_grp__group.c").read_text()
    assert "int leaf(int a) ;" in group and "__standin__u1" in group and "if (0)" not in group
    lines = (out / "files.txt").read_text().splitlines()
    assert lines == [ln.rstrip() for ln in lines]          # xargs -L joins on trailing blanks
    assert "s_caller.c -Xcpluscomm" in lines
    assert (out / "units.txt").read_text().splitlines() == [
        f["id"][:-2] + ".u" for f in m["files"]]
    assert (out / "keep.txt").read_text().split() == m["keep"]
    stamp = (out / "s_leaf.c").stat().st_mtime_ns
    bu.stage(m, texts, out)
    assert (out / "s_leaf.c").stat().st_mtime_ns == stamp
    script = (out / "build.sh").read_text()
    assert "-kp keep.txt" in script and "-r4300_mul" in script
    assert "/tmp/blobgroup" not in script and "/tmp/blobsplice" not in script


def test_remote_work_directory_is_not_the_splice_pipelines():
    assert bu.REMOTE_DIR == "rush2049/scratch/frontier/unit"
    assert "repo" not in bu.REMOTE_DIR and not bu.REMOTE_DIR.startswith("/tmp")
    with pytest.raises(bu.UnitError):
        bu.Run(tag="../repo")


def test_inlined_reads_umerge_verbose_log():
    log = "  leaf\n  caller\n    inlining leaf\n    inlining tiny\n  other\n"
    assert bu.inlined(log) == {"caller": ["leaf", "tiny"]}


# --- comparison against the image ---------------------------------------------

BASE = 0x80086A50
F_ADDR, G_ADDR = 0x80090000, 0x80090040
EXT_FN, D_GLOBAL = 0x80001234, 0x80150010
JR_RA, NOP = 0x03E00008, 0


def _words(*ws):
    return b"".join(struct.pack(">I", w) for w in ws)


def _hi(addr):
    return ((addr + 0x8000) >> 16) & 0xFFFF


def _unit(f_words, g_words, relocs, rodata=b"", rodata_relocs=(), extra_syms=(),
          tail=b""):
    """A synthetic unit object: f then g in .text, .rodata, .bss."""
    text = _words(*f_words) + tail + _words(*g_words)
    sections = [dict(name="", type=0, offset=0, size=0, link=0, info=0),
                dict(name=".text", type=bu.SHT_PROGBITS, offset=0, size=len(text), link=0, info=0),
                dict(name=".rodata", type=bu.SHT_PROGBITS, offset=0, size=len(rodata), link=0, info=0),
                dict(name=".bss", type=bu.SHT_NOBITS, offset=0, size=64, link=0, info=0)]
    g_off = len(f_words) * 4 + len(tail)
    symbols = [
        dict(name="", value=0, size=0, type=0, bind=0, shndx=0),
        dict(name="f", value=0, size=0, type=bu.STT_FUNC, bind=1, shndx=1),          # 1
        dict(name="g", value=g_off, size=0, type=bu.STT_FUNC, bind=1, shndx=1),      # 2
        dict(name="ext_fn", value=0, size=0, type=bu.STT_FUNC, bind=1, shndx=0),     # 3
        dict(name="D_global", value=0, size=0, type=1, bind=1, shndx=0),             # 4
        dict(name=".text", value=0, size=0, type=bu.STT_SECTION, bind=0, shndx=1),   # 5
        dict(name=".rodata", value=0, size=0, type=bu.STT_SECTION, bind=0, shndx=2),  # 6
        dict(name=".bss", value=0, size=0, type=bu.STT_SECTION, bind=0, shndx=3),    # 7
        dict(name="mystery", value=0, size=0, type=1, bind=1, shndx=0),              # 8
        *extra_syms]
    rel = {".text": list(relocs)}
    if rodata_relocs:
        rel[".rodata"] = list(rodata_relocs)
    return bu.Obj(sections, symbols, rel, {".text": text, ".rodata": rodata})


F_UNIT = [0x0C000000,            # jal g              R_26 g
          NOP,
          0x3C040000,            # lui a0,%hi(D_global+4)
          0x24840004,            # addiu a0,a0,%lo(D_global+4)
          0x0C000000,            # jal ext_fn
          NOP, JR_RA, NOP]
F_RELOCS = [(0, bu.R_MIPS_26, 2), (8, bu.R_MIPS_HI16, 4), (12, bu.R_MIPS_LO16, 4),
            (16, bu.R_MIPS_26, 3)]
G_UNIT = [JR_RA, 0x24020001]


def _f_image(callee=G_ADDR, data=D_GLOBAL + 4, ext=EXT_FN):
    return [0x0C000000 | ((callee >> 2) & 0x03FFFFFF), NOP,
            0x3C040000 | _hi(data), 0x24840000 | (data & 0xFFFF),
            0x0C000000 | ((ext >> 2) & 0x03FFFFFF), NOP, JR_RA, NOP]


def _image(placements):
    """Image bytes with {address: bytes} placed."""
    size = max(a - BASE + len(b) for a, b in placements.items()) + 64
    buf = bytearray(size)
    for addr, data in placements.items():
        buf[addr - BASE:addr - BASE + len(data)] = data
    return bytes(buf)


EXTENTS = {"f": (F_ADDR, 32), "g": (G_ADDR, 8)}
EXTERN = {"ext_fn": EXT_FN, "D_global": D_GLOBAL}.get


def _compare(obj, f_image, names=("f", "g"), extents=EXTENTS, more=None):
    image = _image({F_ADDR: _words(*f_image), G_ADDR: _words(*G_UNIT), **(more or {})})
    return bu.compare_unit(obj, names, extents, EXTERN, image, BASE)


def test_relocated_slices_equal_to_the_image_pass():
    res = _compare(_unit(F_UNIT, G_UNIT, F_RELOCS), _f_image())
    assert res["f"]["status"] == "ok" and res["g"]["status"] == "ok"
    assert res["f"]["differing"] == 0 and res["f"]["words"] == 8 and not res["f"]["notes"]


@pytest.mark.parametrize("image,word", [
    (_f_image(callee=G_ADDR + 8), 0),          # wrong callee
    (_f_image(data=D_GLOBAL + 8), 3),          # wrong addend
    (_f_image(data=D_GLOBAL + 0x10004), 2),    # wrong global (high half)
    (_f_image(ext=EXT_FN + 4), 4),             # wrong external callee
])
def test_a_wrong_reference_fails_at_that_word(image, word):
    res = _compare(_unit(F_UNIT, G_UNIT, F_RELOCS), image)
    assert res["f"]["status"] == "fail" and res["g"]["status"] == "ok"
    assert [w for w, _, _ in res["f"]["first"]] == [word]
    assert "1 of 8 words differ" in res["f"]["error"]


def test_plain_word_difference_and_missing_function():
    image = _f_image()
    image[5] = 0x24020007
    res = _compare(_unit(F_UNIT, G_UNIT, F_RELOCS), image, names=("f", "g", "nowhere", "h"),
                   extents={**EXTENTS, "h": (0x80090100, 8)})
    assert res["f"]["first"] == [(5, 0x24020007, NOP)]
    assert res["nowhere"]["error"] == "no extent in the layout"
    assert res["h"]["status"] == "fail" and "absent from the unit object" in res["h"]["error"]


def test_unresolved_symbol_is_a_failure_never_a_mask():
    relocs = [(0, bu.R_MIPS_26, 2), (8, bu.R_MIPS_HI16, 8), (12, bu.R_MIPS_LO16, 8),
              (16, bu.R_MIPS_26, 3)]
    res = _compare(_unit(F_UNIT, G_UNIT, relocs), _f_image())
    assert res["f"]["status"] == "fail" and "unresolved symbol mystery" in res["f"]["error"]


def test_call_into_a_function_without_an_image_address_fails():
    res = _compare(_unit(F_UNIT, G_UNIT, F_RELOCS), _f_image(), names=("f",),
                   extents={"f": EXTENTS["f"]})
    assert res["f"]["status"] == "fail"
    assert "refers to g, which has no image address" in res["f"]["error"]


def test_shorter_and_longer_bodies():
    obj = _unit(F_UNIT, G_UNIT, F_RELOCS)
    res = _compare(obj, _f_image() + [NOP, NOP], extents={"f": (F_ADDR, 40), "g": EXTENTS["g"]})
    assert "compiled body is 8 words, target 10" in res["f"]["error"]
    res = _compare(obj, _f_image()[:4], extents={"f": (F_ADDR, 16), "g": EXTENTS["g"]})
    assert res["f"]["status"] == "fail" and "compiled body is 8 words, target 4" in res["f"]["error"]


def test_zero_padding_and_deleted_procedure_stubs_after_a_body():
    padded = _unit(F_UNIT, G_UNIT, F_RELOCS, tail=_words(NOP, NOP))
    res = _compare(padded, _f_image())
    assert res["f"]["status"] == "ok" and not res["f"]["notes"]
    stubs = _unit(F_UNIT, G_UNIT, F_RELOCS, tail=_words(JR_RA, NOP))
    res = _compare(stubs, _f_image())
    assert res["f"]["status"] == "ok"
    assert res["f"]["notes"][0].startswith(bu.NOTE_STUBTAIL)
    junk = _unit(F_UNIT, G_UNIT, F_RELOCS, tail=_words(0x24020001, NOP))
    assert "compiled body is 10 words, target 8" in _compare(junk, _f_image())["f"]["error"]


# own .rodata: a float literal, then a two-entry jump table into f
LIT_ADDR = 0x80123870
F_LIT = [0x3C010000,             # lui at,%hi(.rodata+0)
         0xC4240000,             # lwc1 f4,%lo(.rodata+0)(at)
         0x3C010000,             # lui at,%hi(.rodata+8)
         0x8C280008,             # lw t0,%lo(.rodata+8)(at)
         JR_RA, NOP, JR_RA, NOP]
F_LIT_RELOCS = [(0, bu.R_MIPS_HI16, 6), (4, bu.R_MIPS_LO16, 6),
                (8, bu.R_MIPS_HI16, 6), (12, bu.R_MIPS_LO16, 6)]
RODATA = struct.pack(">f", 1.5) + b"\0\0\0\0" + _words(16, 24)     # table: .text+16, .text+24
RODATA_RELOCS = [(8, bu.R_MIPS_32, 5), (12, bu.R_MIPS_32, 5)]
TABLE_ADDR = LIT_ADDR + 0x40


def _lit_image():
    return [0x3C010000 | _hi(LIT_ADDR), 0xC4240000 | (LIT_ADDR & 0xFFFF),
            0x3C010000 | _hi(TABLE_ADDR), 0x8C280000 | (TABLE_ADDR & 0xFFFF),
            JR_RA, NOP, JR_RA, NOP]


def _lit_data(literal=1.5, entries=(F_ADDR + 16, F_ADDR + 24)):
    return {LIT_ADDR: struct.pack(">f", literal) + b"\xAA\xBB\xCC\xDD",   # next unit's data
            TABLE_ADDR: _words(*entries)}


def test_own_rodata_is_verified_against_image_bytes():
    obj = _unit(F_LIT, G_UNIT, F_LIT_RELOCS, RODATA, RODATA_RELOCS)
    res = _compare(obj, _lit_image(), more=_lit_data())
    assert res["f"]["status"] == "ok" and res["f"]["own_data"] == 2 and not res["f"]["notes"]


def test_a_wrong_literal_fails():
    obj = _unit(F_LIT, G_UNIT, F_LIT_RELOCS, RODATA, RODATA_RELOCS)
    res = _compare(obj, _lit_image(), more=_lit_data(literal=2.5))
    assert res["f"]["status"] == "fail" and res["f"]["differing"] == 0
    assert "own .rodata+0x0 (3fc00000) differs from the image" in res["f"]["error"]


def test_a_wrong_jump_table_entry_fails():
    obj = _unit(F_LIT, G_UNIT, F_LIT_RELOCS, RODATA, RODATA_RELOCS)
    res = _compare(obj, _lit_image(), more=_lit_data(entries=(F_ADDR + 16, F_ADDR + 28)))
    # the whole table must equal the image, not just the referenced first entry
    assert res["f"]["status"] == "fail" and "own .rodata+0xc" in res["f"]["error"]
    res = _compare(obj, _lit_image(), more=_lit_data(entries=(F_ADDR + 20, F_ADDR + 24)))
    assert res["f"]["status"] == "fail" and "own .rodata+0x8" in res["f"]["error"]


def test_jump_table_into_unplaced_text_is_not_verified():
    obj = _unit(F_LIT, G_UNIT, F_LIT_RELOCS, RODATA[:8] + _words(16, 32), RODATA_RELOCS)
    res = _compare(obj, _lit_image(), names=("f",), extents={"f": EXTENTS["f"]},
                   more=_lit_data())
    assert res["f"]["status"] == "fail" and "has no image address" in res["f"]["error"]


def test_one_own_data_offset_cannot_sit_at_two_image_addresses():
    g_lit = [0x3C010000, 0xC4240000, JR_RA, NOP]
    relocs = F_LIT_RELOCS + [(32, bu.R_MIPS_HI16, 6), (36, bu.R_MIPS_LO16, 6)]
    other = LIT_ADDR + 0x100
    obj = _unit(F_LIT, g_lit, relocs, RODATA, RODATA_RELOCS)
    g_image = [0x3C010000 | _hi(other), 0xC4240000 | (other & 0xFFFF), JR_RA, NOP]
    image = _image({F_ADDR: _words(*_lit_image()), G_ADDR: _words(*g_image),
                    other: struct.pack(">f", 1.5), **_lit_data()})
    res = bu.compare_unit(obj, ("f", "g"), {"f": EXTENTS["f"], "g": (G_ADDR, 16)}, EXTERN,
                          image, BASE)
    assert res["f"]["status"] == res["g"]["status"] == "fail"
    assert "different image addresses" in res["g"]["error"]


def test_own_bss_reference_is_noted_not_failed():
    relocs = [(0, bu.R_MIPS_HI16, 7), (4, bu.R_MIPS_LO16, 7)]
    f = [0x3C010000, 0x8C280010, JR_RA, NOP, JR_RA, NOP, JR_RA, NOP]
    addr = 0x80160010
    image = [0x3C010000 | _hi(addr), 0x8C280000 | (addr & 0xFFFF), JR_RA, NOP, JR_RA, NOP,
             JR_RA, NOP]
    res = _compare(_unit(f, G_UNIT, relocs), image)
    assert res["f"]["status"] == "ok" and res["f"]["notes"] == [bu.NOTE_BSS]


def test_emission_summary():
    obj = _unit(F_UNIT, G_UNIT, F_RELOCS)
    info = bu.emission(obj, {"f": (F_ADDR, 32), "g": (F_ADDR + 32, 8)})
    assert info["functions"] == 2 and info["adjacent_inversions"] == 0
    assert info["image_adjacent_pairs"] == info["pairs_adjacent_in_unit"] == 1
    swapped = bu.emission(obj, {"g": (F_ADDR, 8), "f": (F_ADDR + 8, 32)})
    assert swapped["adjacent_inversions"] == 1 and swapped["pairs_adjacent_in_unit"] == 0


@pytest.mark.skipif(shutil.which("mips-linux-gnu-as") is None, reason="needs the mips binutils")
def test_parse_a_real_object(tmp_path):
    src = tmp_path / "u.s"
    src.write_text("""
    .set noreorder
    .text
    .globl f
    .type f, @function
f:
    jal g
    nop
    lui $a0, %hi(D_global+4)
    addiu $a0, $a0, %lo(D_global+4)
    jal ext_fn
    nop
    jr $ra
    nop
    .globl g
    .type g, @function
g:
    jr $ra
    addiu $v0, $zero, 1
""")
    obj_path = tmp_path / "u.o"
    subprocess.run(["mips-linux-gnu-as", "-EB", "-mips2", "-32", "-o", str(obj_path), str(src)],
                   check=True)
    obj = bu.Obj.parse(obj_path.read_bytes())
    assert obj.functions() == {"f": 0, "g": 32}
    assert [(o, t, obj.symbols[s]["name"]) for o, t, s in obj.relocs[".text"]] == [
        (0, bu.R_MIPS_26, "g"), (8, bu.R_MIPS_HI16, "D_global"),
        (12, bu.R_MIPS_LO16, "D_global"), (16, bu.R_MIPS_26, "ext_fn")]
    res = _compare(obj, _f_image())
    assert res["f"]["status"] == "ok" and res["g"]["status"] == "ok"
    assert _compare(obj, _f_image(callee=G_ADDR + 4))["f"]["status"] == "fail"


def test_manifest_round_trips_as_json():
    lock, specs, files, defs, addresses = _fixture()
    m = bu.derive(lock, specs, files, defs, addresses)
    assert json.loads(json.dumps(m)) == m
