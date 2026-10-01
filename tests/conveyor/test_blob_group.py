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


@pytest.mark.parametrize("include_next_context", [True, False])
def test_short_context_cannot_capture_the_next_function(tmp_path, include_next_context):
    obj = _asm_object(tmp_path, """
    .set noreorder
    .text
    .globl ctx
    .type ctx, @function
ctx:
    jr $ra
    nop
    .globl g
    .type g, @function
g:
    jr $ra
    nop
    .globl f
    .type f, @function
f:
    jal g
    nop
    jr $ra
    nop
""")
    extents = {"ctx": {"vaddr": 0x80100000, "size": 16},
               "g": {"vaddr": 0x80200000, "size": 8},
               "f": {"vaddr": 0x80300000, "size": 16}}
    names = ["ctx", "f"] + (["g"] if include_next_context else [])
    slices, ndx = blob_group.member_slices(obj, names, extents)
    if include_next_context:
        body = blob_group.relocate(obj, slices, ndx, {}, members=["f"])["f"]
        assert _words(body)[0] == 0x0C000000 | ((0x80200000 >> 2) & 0x03FFFFFF)
    else:
        with pytest.raises(blob_group.GroupError, match="no member slice"):
            blob_group.relocate(obj, slices, ndx, {}, members=["f"])


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


_LOCAL_ASM = """
    .set noreorder
    .text
    .globl f
    .type f, @function
f:
    lui $a0, %hi(done)
    lw $a0, %lo(done)($a0)
    lui $a1, %hi(done)
    sw $zero, %lo(done)($a1)
    jr $ra
    nop
    .data
done:
    .word DONE_VALUE
"""


def _local_object(tmp_path, value=0):
    src = tmp_path / "l.s"
    src.write_text(_LOCAL_ASM.replace("DONE_VALUE", str(value)))
    obj = tmp_path / "l.o"
    subprocess.run(["mips-linux-gnu-as", "-EB", "-mips2", "-32", "-o", str(obj), str(src)],
                   check=True)
    return obj


def _retail(data_addr, lo_second=None, data_word=0):
    """A 0x40-byte image at 0x80100000: f at +0, its static at data_addr."""
    hi = ((data_addr + 0x8000) >> 16) & 0xFFFF
    lo = data_addr & 0xFFFF
    lo2 = lo if lo_second is None else lo_second
    words = [0x3C040000 | hi, 0x8C840000 | lo, 0x3C050000 | hi, 0xACA00000 | lo2,
             0x03E00008, 0] + [0] * 10
    off = (data_addr - 0x80100000) // 4
    words[off] = data_word
    words[off + 1] = 0x28282800          # the next unit's data, past our padding
    return b"".join(struct.pack(">I", w) for w in words), 0x80100000


def _local_bodies(obj, image):
    extents = {"f": {"vaddr": 0x80100000, "size": 24}}
    slices, text_ndx = blob_group.member_slices(obj, ["f"], extents)
    return blob_group.relocate(obj, slices, text_ndx, {}, image=image)


def test_local_static_resolves_to_its_verified_image_address(tmp_path):
    image = _retail(0x80100020)
    body = _local_bodies(_local_object(tmp_path), image)["f"]
    assert body == image[0][:24]


def test_local_static_sites_must_agree(tmp_path):
    with pytest.raises(blob_group.GroupError, match="disagree"):
        _local_bodies(_local_object(tmp_path), _retail(0x80100020, lo_second=0x0030))


def test_local_static_contents_must_match_the_image(tmp_path):
    with pytest.raises(blob_group.GroupError, match="differ from the image"):
        _local_bodies(_local_object(tmp_path, value=5), _retail(0x80100020))


def test_local_static_needs_the_image(tmp_path):
    with pytest.raises(blob_group.GroupError, match="need the image"):
        _local_bodies(_local_object(tmp_path), None)


_TABLE_ASM = """
    .set noreorder
    .text
    .globl f
    .type f, @function
f:
    jr $ra
    nop
    .globl ctx
    .type ctx, @function
ctx:
    lui $t0, %hi(table)
    lw $t0, %lo(table)($t0)
L1:
    jr $t0
    nop
    .rdata
table:
    .word L1
"""


def _asm_object(tmp_path, text, name="t"):
    src = tmp_path / f"{name}.s"
    src.write_text(text)
    obj = tmp_path / f"{name}.o"
    subprocess.run(["mips-linux-gnu-as", "-EB", "-mips2", "-32", "-o", str(obj), str(src)],
                   check=True)
    return obj


def test_jump_table_used_only_by_context_is_ignored(tmp_path):
    obj = _asm_object(tmp_path, _TABLE_ASM)
    extents = {"f": {"vaddr": 0x80100000, "size": 8},
               "ctx": {"vaddr": 0x80100008, "size": 16}}
    slices, text_ndx = blob_group.member_slices(obj, ["f", "ctx"], extents)
    bodies = blob_group.relocate(obj, slices, text_ndx, {}, members=["f"])
    assert _words(bodies["f"]) == [0x03E00008, 0]


def _table_image(entry=0x80100010):
    """f (2 words), ctx (4 words), padding, then the jump table at 0x80100020."""
    words = [0x03E00008, 0, 0x3C088010, 0x8D080020, 0x01000008, 0, 0, 0, entry]
    return b"".join(struct.pack(">I", w) for w in words), 0x80100000


def _table_bodies(tmp_path, image):
    obj = _asm_object(tmp_path, _TABLE_ASM)
    extents = {"f": {"vaddr": 0x80100000, "size": 8},
               "ctx": {"vaddr": 0x80100008, "size": 16}}
    slices, text_ndx = blob_group.member_slices(obj, ["f", "ctx"], extents)
    return blob_group.relocate(obj, slices, text_ndx, {}, members=["ctx"], image=image)


def test_jump_table_used_by_a_member_is_placed_and_checked_against_the_image(tmp_path):
    bodies = _table_bodies(tmp_path, _table_image())
    assert _words(bodies["ctx"]) == [0x3C088010, 0x8D080020, 0x01000008, 0]


def test_jump_table_entry_that_differs_from_the_image_is_refused(tmp_path):
    with pytest.raises(blob_group.GroupError, match="differ from the image"):
        _table_bodies(tmp_path, _table_image(entry=0x80100014))


def test_unit_defined_global_with_a_known_address_resolves_by_name(tmp_path):
    obj = _asm_object(tmp_path, """
    .set noreorder
    .text
    .globl f
    .type f, @function
f:
    lui $a0, %hi(D_80149B64)
    lw $a0, %lo(D_80149B64)($a0)
    jr $ra
    nop
    .globl D_80149B64
    .lcomm D_80149B64, 4
""")
    extents = {"f": {"vaddr": 0x80100000, "size": 16}}
    slices, text_ndx = blob_group.member_slices(obj, ["f"], extents)
    body = blob_group.relocate(obj, slices, text_ndx, {"D_80149B64": 0x80149B64})["f"]
    assert _words(body)[:2] == [0x3C048015, 0x8C849B64]
