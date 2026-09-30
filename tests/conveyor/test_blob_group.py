"""010 Phase 4: IPA call groups spliced member by member."""
import json
import shutil
import struct
import subprocess

import pytest

from tools.conveyor.pipeline import blob_group

pytestmark = pytest.mark.skipif(shutil.which("mips-linux-gnu-as") is None,
                                reason="needs the mips binutils")

_ASM = """
    .set noreorder
    .text
    .globl f
    .type f, @function
f:
    jal g
    nop
    lui $a0, %hi(D_data)
    addiu $a0, $a0, %lo(D_data)
    jr $ra
    nop
    .globl g
    .type g, @function
g:
    jal ext_fn
    nop
    jr $ra
    nop
"""


def _object(tmp_path):
    src = tmp_path / "g.s"
    src.write_text(_ASM)
    obj = tmp_path / "g.o"
    subprocess.run(["mips-linux-gnu-as", "-EB", "-mips2", "-32", "-o", str(obj), str(src)],
                   check=True)
    return obj


def _words(data):
    return list(struct.unpack(f">{len(data) // 4}I", data))


def test_members_relocate_to_their_own_image_addresses(tmp_path):
    obj = _object(tmp_path)
    extents = {"f": {"vaddr": 0x80100000, "size": 24},
               "g": {"vaddr": 0x80200000, "size": 16}}
    slices, text_ndx = blob_group.member_slices(obj, ["f", "g"], extents)
    bodies = blob_group.relocate(obj, slices, text_ndx,
                                 {"D_data": 0x8012FFF0, "ext_fn": 0x80000400})
    f, g = _words(bodies["f"]), _words(bodies["g"])
    assert f[0] == 0x0C000000 | (0x80200000 >> 2) & 0x03FFFFFF   # jal g at g's address
    assert f[2] & 0xFFFF == 0x8013                                # %hi, carry from %lo
    assert f[3] & 0xFFFF == 0xFFF0                                # %lo
    assert g[0] == 0x0C000000 | (0x80000400 >> 2) & 0x03FFFFFF   # external call


def test_unresolved_symbol_is_refused(tmp_path):
    obj = _object(tmp_path)
    extents = {"f": {"vaddr": 0x80100000, "size": 24},
               "g": {"vaddr": 0x80200000, "size": 16}}
    slices, text_ndx = blob_group.member_slices(obj, ["f", "g"], extents)
    with pytest.raises(blob_group.GroupError, match="unresolved symbol"):
        blob_group.relocate(obj, slices, text_ndx, {"ext_fn": 0x80000400})


def test_call_into_a_non_member_is_refused(tmp_path):
    # f calls g, but only f is a member: its jal would point at bytes that are
    # not placed anywhere in the image.
    obj = _object(tmp_path)
    slices, text_ndx = blob_group.member_slices(
        obj, ["f"], {"f": {"vaddr": 0x80100000, "size": 24}})
    with pytest.raises(blob_group.GroupError, match="no member slice"):
        blob_group.relocate(obj, slices, text_ndx, {"D_data": 0x80130000})


def _group(root, flags="-g0 -O3 -mips2 -G 0 -non_shared"):
    d = root / "grp"
    d.mkdir(parents=True)
    (d / "a.c").write_text("int f(void) { return 1; }\n")
    (d / "group.json").write_text(json.dumps(
        {"members": ["f"], "files": ["a.c"], "keep": ["f"], "flags": flags}))
    return d


def test_group_must_be_compiled_at_O3(tmp_path):
    _group(tmp_path, flags="-g0 -O2 -mips2 -G 0 -non_shared")
    with pytest.raises(blob_group.GroupError, match="-O3"):
        blob_group.load("grp", tmp_path)


def test_check_reports_drifted_group_sources(tmp_path):
    d = _group(tmp_path)
    spec = blob_group.load("grp", tmp_path)
    lock = tmp_path / "lock.json"
    lock.write_text(json.dumps({"f": {"group": "grp",
                                      "source_sha256": blob_group.source_sha(spec)}}))
    assert blob_group.check(lock, tmp_path) == []
    (d / "a.c").write_text("int f(void) { return 2; }\n")
    assert blob_group.check(lock, tmp_path) == [("grp", "group sources drifted from the lock")]


def test_builder_script_uses_kp_not_preserve_dead_code():
    spec = {"flags": "-g0 -O3 -mips2 -G 0 -non_shared", "files": ["a.c", "b.c"]}
    script = blob_group.builder_script(spec)
    assert "-kp keep.txt a.u b.u" in script
    assert "-preserve_dead_code" not in script.split("cc.log")[1]


def test_a_function_cannot_be_both_member_and_context(tmp_path):
    d = _group(tmp_path)
    spec = json.loads((d / "group.json").read_text())
    spec["context"] = ["f"]
    (d / "group.json").write_text(json.dumps(spec))
    with pytest.raises(blob_group.GroupError, match="both member and context"):
        blob_group.load("grp", tmp_path)


def test_context_functions_resolve_calls_but_are_not_spliced(tmp_path):
    obj = _object(tmp_path)
    extents = {"f": {"vaddr": 0x80100000, "size": 24},
               "g": {"vaddr": 0x80200000, "size": 16}}
    slices, text_ndx = blob_group.member_slices(obj, ["f", "g"], extents)
    bodies = blob_group.relocate(obj, slices, text_ndx,
                                 {"D_data": 0x8012FFF0, "ext_fn": 0x80000400})
    # g as context: f's call into it still resolves to g's image address
    assert _words(bodies["f"])[0] == 0x0C000000 | (0x80200000 >> 2) & 0x03FFFFFF


def test_mismatched_callee_prototypes_are_unprototyped():
    prelude = ("void a(void);\ns32 b(s32 x, s32 y);\nvoid c(s32 x, ...);\n"
               "void member(s32 x);\n")
    new, names = blob_group.unprototype_mismatched(
        prelude, ["a(1); b(1, 2); c(1, 2, 3); member(1, 2);"], exclude=["member"])
    assert names == ["a"]
    assert "void a();" in new and "s32 b(s32 x, s32 y);" in new
    assert "void member(s32 x);" in new          # members are left alone


def test_context_relocations_are_not_applied_even_when_cut(tmp_path):
    """A context function longer than its extent can have a HI16/LO16 pair
    straddling the cut; only member slices are relocated."""
    src = tmp_path / "ctx.s"
    src.write_text("""
    .set noreorder
    .text
    .globl f
    .type f, @function
f:
    jal g
    nop
    jr $ra
    nop
    .globl g
    .type g, @function
g:
    nop
    lui $a0, %hi(D_data)
    nop
    addiu $a0, $a0, %lo(D_data)
    jr $ra
    nop
""")
    obj = tmp_path / "ctx.o"
    subprocess.run(["mips-linux-gnu-as", "-EB", "-mips2", "-32", "-o", str(obj), str(src)],
                   check=True)
    extents = {"f": {"vaddr": 0x80100000, "size": 16},
               "g": {"vaddr": 0x80200000, "size": 12}}      # cut after the lui
    slices, text_ndx = blob_group.member_slices(obj, ["f", "g"], extents)
    bodies = blob_group.relocate(obj, slices, text_ndx, {"D_data": 0x80130000},
                                 members=["f"])
    assert _words(bodies["f"])[0] == 0x0C000000 | (0x80200000 >> 2) & 0x03FFFFFF
