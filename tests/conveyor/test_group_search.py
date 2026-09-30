"""group_search: the ELF reader, the objdump stand-in, the node script, the bundle."""
import json
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]
JOBS = REPO / "tools" / "conveyor" / "jobs"
sys.path.insert(0, str(JOBS))

import group_elf  # noqa: E402
import group_search  # noqa: E402
import groupdump  # noqa: E402

from tools.conveyor.pipeline import blob_group, group_jobs  # noqa: E402

needs_as = pytest.mark.skipif(shutil.which("mips-linux-gnu-as") is None,
                              reason="needs the mips binutils")

_ASM = """
    .set noreorder
    .text
    .globl f
    .type f, @function
f:
    jal g
    nop
    lui $a0, %hi(D_far)
    addiu $a0, $a0, %lo(D_far)
    lui $a1, %hi(local_word)
    lw $a1, %lo(local_word)($a1)
    jr $ra
    nop
    .balign 64
    .globl g
    .type g, @function
g:
    jal ext_fn
    nop
    jr $ra
    nop
    .data
local_word:
    .word 5
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


@needs_as
def test_target_words_are_linked_at_image_addresses(tmp_path):
    obj = group_elf.Obj(_object(tmp_path).read_bytes())
    retail = struct.pack(">8I", 0, 0, 0, 0, 0x3C050001, 0x8CA50234, 0, 0)
    got = _words(group_elf.target_words(
        obj, "f", 32, {"f": 0x80100000, "g": 0x80200000},
        {"D_far": 0x80138000}, retail))
    assert got[0] == 0x0C000000 | (0x80200000 >> 2) & 0x03FFFFFF   # jal g
    assert got[2] == 0x3C040000 | 0x8014                           # %hi with the carry
    assert got[3] == 0x24840000 | 0x8000                           # %lo
    # local_word lives in this unit's .data: its fields come from the retail words
    assert got[4] == 0x3C050001 and got[5] & 0xFFFF == 0x0234


@needs_as
def test_a_call_to_an_unknown_function_takes_the_retail_field(tmp_path):
    obj = group_elf.Obj(_object(tmp_path).read_bytes())
    retail = struct.pack(">4I", 0x0C000123, 0, 0, 0)
    got = _words(group_elf.target_words(obj, "g", 16, {"g": 0x80200000}, {}, retail))
    assert got[0] == 0x0C000123


@needs_as
def test_trailing_padding_beyond_the_retail_size_is_dropped(tmp_path):
    obj = group_elf.Obj(_object(tmp_path).read_bytes())
    got = group_elf.target_words(obj, "f", 32, {"f": 0x80100000, "g": 0x80200000},
                                 {"D_far": 0x80138000}, bytes(32))
    assert len(got) == 32                 # the 32 bytes of zero padding before g are dropped


def test_the_target_stub_round_trips():
    words = struct.pack(">3I", 1, 2, 3)
    data = groupdump.stub_bytes(words)
    assert data[:4] == b"\x7fELF" and int.from_bytes(data[16:18], "big") == 0
    assert groupdump.words_of({}, data) == words


@needs_as
def test_the_permuter_scorer_reads_the_stub_and_a_candidate(tmp_path):
    from src.scorer import Scorer  # vendored permuter, put on the path by _permuter
    obj = _object(tmp_path)
    spec = {"target": "g", "size": 16, "vaddr": 0x80200000, "slices": {"g": 0x80200000},
            "symbols": {"ext_fn": 0x80000400},
            "retail": struct.pack(">4I", 0x0C000100, 0, 0x03E00008, 0).hex()}
    spec_path = tmp_path / "spec.json"
    spec_path.write_text(json.dumps(spec))
    target = tmp_path / "target.o"
    target.write_bytes(groupdump.stub_bytes(bytes.fromhex(spec["retail"])))
    scorer = Scorer(target_o=str(target), stack_differences=True, algorithm="difflib",
                    debug_mode=False, ign_branch_targets=False,
                    objdump_command=f"{sys.executable} {JOBS / 'groupdump.py'} {spec_path}")
    assert scorer.score(str(obj))[0] == 0
    spec["symbols"]["ext_fn"] = 0x80000800          # a different callee
    spec_path.write_text(json.dumps(spec))
    scorer = Scorer(target_o=str(target), stack_differences=True, algorithm="difflib",
                    debug_mode=False, ign_branch_targets=False,
                    objdump_command=f"{sys.executable} {JOBS / 'groupdump.py'} {spec_path}")
    assert scorer.score(str(obj))[0] > 0


def _stages(script):
    """The `$T/<tool> ...` commands of a build script, without paths or redirects."""
    out = []
    for part in script.split(";"):
        m = re.search(r"\$T/(\w+) (.*)", part)
        if m and m.group(1) != "cc":
            out.append((m.group(1), re.sub(r"\s*>.*$", "", m.group(2)).strip()))
    return out


def test_node_pipeline_is_the_same_as_the_builder_pipeline():
    spec = {"flags": "-g0 -O3 -mips2 -G 0 -non_shared", "files": ["group.c"]}
    node = group_search.pipeline("/tk", spec["flags"], spec["files"])
    builder = blob_group.builder_script(spec)
    assert _stages(node) == _stages(builder)
    assert "cc -j -g0 -O3 -mips2 -G 0 -non_shared group.c" in node
    assert "-r4300_mul" in node and "-kp keep.txt" in node


def _group_dir(tmp_path):
    gdir = tmp_path / "groups" / "demo"
    gdir.mkdir(parents=True)
    (gdir / "group.c").write_text("int f(int a) {\n    return a;\n}\n\nint g(void) {\n    return 1;\n}\n")
    (gdir / "group.json").write_text(json.dumps(
        {"members": ["f"], "context": ["g"], "files": ["group.c"], "keep": ["f", "g"],
         "flags": "-g0 -O3 -mips2 -G 0 -non_shared"}))
    return gdir.parent


def test_bundle_carries_the_source_the_keep_list_and_the_retail_words(tmp_path):
    from tools.conveyor.jobs import runner  # noqa: F401  (import check only)
    root = _group_dir(tmp_path)
    image = tmp_path / "image.bin"
    image.write_bytes(struct.pack(">8I", 0x03E00008, 0, 0x24020001, 0x03E00008, 0, 0, 0, 0))
    document = {"image": {"path": str(image), "base": "0x80100000"},
                "regions": [{"entries": [
                    {"kind": "function", "target_id": "f", "vaddr": 0x80100000, "size": 8},
                    {"kind": "function", "target_id": "g", "vaddr": 0x80100008, "size": 12}]}]}
    spec = blob_group.load("demo", root)
    bundle, sha, job = group_jobs.build_bundle(spec, "g", "toolkit", document=document,
                                               out_dir=tmp_path)
    assert job["job_type"] == "group_search" and job["target_id"] == "g"
    assert job["batch"] is False and job["max_attempts"] is None
    import tarfile
    with tarfile.open(bundle) as tar:
        names = tar.getnames()
        manifest = json.loads(tar.extractfile("manifest.json").read())
        gspec = json.loads(tar.extractfile("inputs/spec.json").read())
    assert "inputs/base.c" in names
    assert manifest["keep"] == ["f", "g"] and manifest["seed_name"] == "group.c"
    assert gspec["retail"] == struct.pack(">3I", 0x24020001, 0x03E00008, 0).hex()
    assert gspec["slices"] == {"f": 0x80100000, "g": 0x80100008}


def test_candidates_skip_locked_and_unlisted_functions(tmp_path):
    root = _group_dir(tmp_path)
    document = {"regions": [{"entries": [
        {"kind": "function", "target_id": "f", "vaddr": 0x80100000, "size": 8},
        {"kind": "function", "target_id": "g", "vaddr": 0x80100008, "size": 12}]}]}
    assert group_jobs.candidates(root, document, lock={"f": {}}) == [("demo", "g", 12)]
    assert group_jobs.candidates(root, document, lock={}) == [("demo", "f", 8), ("demo", "g", 12)]


_INLINE_SRC = """static __inline s32 helper(s32 a) {
    return a + 1;
}

static __inline void proto(void);
s32 plain(s32 a) {
    return helper(a);
}
__inline s32 loose(s32 a) {
    return a;
}
"""


def test_strip_inline_round_trips():
    stripped, names = group_search.strip_inline(_INLINE_SRC)
    assert "__inline" not in stripped
    assert names == {"helper": "static", "proto": "static", "loose": ""}
    assert stripped.count("\n") == _INLINE_SRC.count("\n")
    assert group_search.restore_inline(stripped, names) == _INLINE_SRC


def test_restore_inline_survives_a_permuted_body_and_skips_calls():
    stripped, names = group_search.strip_inline(_INLINE_SRC)
    permuted = stripped.replace("return a + 1;", "s32 t = a;\n    return t + 1;")
    permuted += "s32 use(s32 a) {\n    return helper(a);\n}\n"
    back = group_search.restore_inline(permuted, names)
    assert back.count("__inline") == 3
    assert "    return helper(a);" in back and "static __inline s32 helper(s32 a) {" in back


def test_source_without_inline_is_untouched():
    src = "s32 f(s32 a) {\n    return a;\n}\n"
    assert group_search.strip_inline(src) == (src, {})
    assert group_search.inline_sed({}) == ""


def test_compile_sh_restores_inline_before_the_build(tmp_path):
    manifest = {"seed_name": "group.c", "extra_files": [], "compile_flags": "-O3"}
    group_search.write_compile_sh(tmp_path, "/tk", manifest, tmp_path / "static",
                                  {"helper": "static", "loose": ""})
    text = (tmp_path / "compile.sh").read_text()
    assert text.startswith("#!/bin/sh\n")
    assert "sed -e '/__inline/b'" in text and '"$SRC" > "$W/group.c"' in text
    assert "helper" in text and "loose" in text
    assert text.index("sed -e") < text.index("$T/cc -j")
    group_search.write_compile_sh(tmp_path, "/tk", manifest, tmp_path / "static")
    assert 'cp "$SRC" "$W/group.c"' in (tmp_path / "compile.sh").read_text()


@pytest.mark.skipif(shutil.which("sed") is None, reason="needs sed")
def test_generated_sed_matches_restore_inline(tmp_path):
    stripped, names = group_search.strip_inline(_INLINE_SRC)
    src = tmp_path / "in.c"
    src.write_text(stripped)
    out = subprocess.run(f"sed {group_search.inline_sed(names)} {src}", shell=True,
                         capture_output=True, text=True, check=True).stdout
    assert out == _INLINE_SRC


def test_progress_checkpoints_get_inline_back():
    import base64
    import gzip
    seen = {}

    class P:
        def update(self, **kw):
            seen.update(kw)

    stripped, names = group_search.strip_inline(_INLINE_SRC)
    wrapped = group_search._RestoringProgress(P(), names)
    wrapped.update(best_score=5, best_source=base64.b64encode(
        gzip.compress(stripped.encode())).decode())
    assert seen["best_score"] == 5
    assert gzip.decompress(base64.b64decode(seen["best_source"])).decode() == _INLINE_SRC
