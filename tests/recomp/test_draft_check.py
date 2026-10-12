"""draft_check (recomp Phase 7): argument parsing, census lookup, clobber shims,
remote file collection, divergence wording; end to end on the recomp host
(rocky: ROM, N64Recomp, IDO toolkits present) with a known-good draft (the
matched render_helper) and a deliberately broken one (`>` flipped to `<`)."""
import importlib.util
import json
import subprocess
import sys
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]


def _load(name, rel):
    spec = importlib.util.spec_from_file_location(name, REPO / rel)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


dc = _load("recomp_draft_check", "tools/recomp/draft_check.py")


def test_parse_params_by_name_either_order():
    proto = "void f(s32 handle, s32 mark, void *p)"
    cap, regs = dc.parse_params("handle=a0, t3=mark, p=s0", proto)
    assert cap == ["handle=a0", "mark=t3", "p=s0"] and regs == ["a0", "t3", "s0"]
    with pytest.raises(dc.DCError, match="no register for p"):
        dc.parse_params("handle=a0,mark=t3", proto)
    with pytest.raises(dc.DCError, match="not a parameter"):
        dc.parse_params("handle=a0,mark=t3,p=s0,q=s1", proto)
    with pytest.raises(dc.DCError, match="not a register"):
        dc.parse_params("handle=a0,mark=x9,p=s0", proto)


def test_parse_callee():
    name, e = dc.parse_callee("void entity_transform_calc(s32 h, f32 x, f32 y)=a0,f22,f24/f20")
    assert name == "entity_transform_calc" and e["params"] == ["a0", "f22", "f24"] and e["clobbers"] == ["f20"]
    with pytest.raises(dc.DCError, match="2 parameters, 1 registers"):
        dc.parse_callee("void g(s32 a, s32 b)=a0")


def test_census_lookup(tmp_path):
    (tmp_path / "census.json").write_text(json.dumps({"functions": {
        "f": {"scripts": {"race": 3, "battle": 10}}, "g": {"scripts": {}}}}))
    assert dc.census_scripts("f", tmp_path) == (["battle", "race"], True)
    assert dc.census_scripts("g", tmp_path) == ([], True)
    assert dc.census_scripts("h", tmp_path) == (None, False)
    assert dc.census_scripts("f", tmp_path / "none") == (None, False)


def test_all_scripts_skip_unrunnable():
    s = dc.all_scripts()
    assert "race" in s and "saves_reload" not in s


def test_clobber_shims(tmp_path):
    fr = tmp_path / "frontier.json"
    fr.write_text(json.dumps({"facts": {
        "clob": {"unsaved_callee_regs": ["s0", "s3"], "register_params": []},
        "regp": {"unsaved_callee_regs": ["s1"], "register_params": ["t3"]},
        "plain": {"unsaved_callee_regs": [], "register_params": []},
        "known": {"unsaved_callee_regs": ["s2"]}}}))
    out = dc.clobber_shims({"known": {}}, fr)
    assert set(out) == {"clob"}
    e = out["clob"]
    assert e["clobbers"] == ["s0", "s3"] and e["params"][:4] == ["a0", "a1", "a2", "a3"]
    assert len(e["params"]) == len(dc.rp.parse_prototype(e["prototype"])[2])
    words = dc.rp.ipa_shim(0x80091CC8, e["prototype"], e["params"], e["clobbers"], "out")
    assert words[-2] == (31 << 21) | 8                    # jr ra; addiu sp in the delay slot


def test_local_files_follow_quoted_includes(tmp_path):
    (tmp_path / "sub").mkdir()
    (tmp_path / "sub" / "a.h").write_text('#include "../b.h"\n')
    (tmp_path / "b.h").write_text("/* b */\n")
    d = tmp_path / "sub" / "draft.c"
    d.write_text('#include "a.h"\n#include "types.h"\nvoid f(void) {}\n')
    got = {p.name for p in dc.local_files(d)}
    assert got == {"draft.c", "a.h", "b.h"}


def test_divergence_lines_wording():
    res = {"script": "race", "first": {"what": "effect", "frame": 2, "name": "render_helper (draft)", "fn_call": 2,
                                       "seq": 2, "fields": [["effect words:hash", "1:aa", "13:bb"]],
                                       "words": [["0x80149B88", None, ["00000000", "00000010"], "D_80149B88+0x0"]]}}
    lines = dc.divergence_lines(res, "render_helper")
    assert lines[0].startswith("FIRST DIVERGENCE: script race, frame 2, render_helper (draft) call #2")
    assert "words written: retail 1 | draft 13" in lines[1]
    assert "retail unchanged | draft 00000000->00000010" in lines[2]


def _on_recomp_host():
    p = dc.paths()
    return all(Path(p[k]).exists() for k in ("rom", "n64recomp", "rsprecomp", "runtime", "ido53"))


@pytest.mark.skipif(not _on_recomp_host(), reason="private input: recomp host (ROM, N64Recomp, IDO) not here")
@pytest.mark.parametrize("draft,rc,word", [
    ("src/blob/render_helper.c", 0, "VERDICT: EQUIVALENT"),
    ("cloud/work/frontier/w22r/bug/render_helper_flipped.c", 1, "VERDICT: DIVERGES"),
])
def test_end_to_end(draft, rc, word):
    p = subprocess.run([sys.executable, "-m", "tools.recomp.draft_check", "render_helper", draft,
                        "--scripts", "race"], cwd=str(REPO), capture_output=True, text=True, timeout=1200)
    assert p.returncode == rc, p.stdout + p.stderr
    assert word in p.stdout
    if rc == 0:
        assert "NOT a match" in p.stdout
    else:
        assert "FIRST DIVERGENCE: script race, frame 2, render_helper (draft) call #2" in p.stdout


# ------------------------------------------------------------------ regmap_verify

rv = _load("recomp_regmap_verify", "tools/recomp/regmap_verify.py")


def test_parse_mapping():
    m = rv.parse_mapping("a0=handle:int, mark=t3, s0=buf:ptr, f22=x:float")
    assert m == [("a0", "handle", "int"), ("t3", "mark", "any"), ("s0", "buf", "ptr"), ("f22", "x", "float")]
    with pytest.raises(rv.VerifyError, match="even FP"):
        rv.parse_mapping("f23=x")
    with pytest.raises(rv.VerifyError, match="kind"):
        rv.parse_mapping("a0=x:struct")


def test_kind_of():
    assert rv.kind_of({0x80144D5C, 0}, "a0") == "ptr"
    assert rv.kind_of({0, 1}, "t3") == "int"
    assert rv.kind_of({0}, "t3") == "zero"
    assert rv.kind_of({0x3F800000, 0xC0000000}, "f22") == "float"
    assert rv.kind_of({0x180, 0x80143FF8}, "a0") == "mixed"
    assert rv.kind_ok("int", "zero") and not rv.kind_ok("ptr", "mixed") and rv.kind_ok("any", "mixed")


def test_static_sites_and_setters():
    # caller at 0x80000000: addiu t3,zero,1 ; lw a0,8(sp) ; jal 0x80000020 ; move a1,s0 ; jr ra ; nop
    # callee at 0x80000020: addu v0,a0,t3 ; jr ra ; nop
    import struct
    words = [0x240B0001, 0x8FA40008, 0x0C000008, 0x02002825, 0x03E00008, 0, 0, 0,
             0x008B1021, 0x03E00008, 0]
    image = (0x80000000, struct.pack(">%dI" % len(words), *words))
    funcs = [{"name": "caller", "vram": 0x80000000, "size": 24}, {"name": "callee", "vram": 0x80000020, "size": 12}]
    sites = rv.call_sites(image, funcs, 0x80000020)
    assert sites == [(0x80000008, "caller", 0x80000000, 24)]
    assert rv.setter(image, 0x80000008, 0x80000000, "t3") == (0x80000000, "addiu t3, zero, 1")
    assert rv.setter(image, 0x80000008, 0x80000000, "a0") == (0x80000004, "lw a0, 8(sp)")
    assert rv.setter(image, 0x80000008, 0x80000000, "a1")[1].endswith("(delay slot)")
    assert rv.setter(image, 0x80000008, 0x80000000, "a2") is None
    live = rv.ipa_detect.register_params(words[8:], include_o32=True)
    assert live == ["a0", "t3"]
