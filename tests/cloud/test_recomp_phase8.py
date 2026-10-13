"""Recomp Phase 8 (lane w28r): decomp-feedback tooling.

Fast unit tests (no ROM, no recomp host): the committed ipa_callees.json
entries, interprocedural register-parameter derivation (regmap_verify
--derive / draft_check --probe-callees), draft_check --unit plumbing
(replace.py unit entries, gen_config unit hooks, member --params, census
script choice), the dc cleanup policy (--gc), script_probe's text log, the
runtime contract names. End to end on the recomp host only (rocky: ROM,
N64Recomp and the IDO toolkits present; skipped elsewhere): script_probe on a
short script and draft_check --unit on the w27e controller-pak cluster.
"""
import importlib.util
import json
import os
import struct
import subprocess
import sys
import time
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]
RECOMP = REPO / "tools" / "recomp"


def _load(name, rel):
    spec = importlib.util.spec_from_file_location(name, REPO / rel)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


dc = _load("recomp_p8_draft_check", "tools/recomp/draft_check.py")
rp = dc.rp
rv = _load("recomp_p8_regmap_verify", "tools/recomp/regmap_verify.py")
ipd = rv.ipa_detect
sp = _load("recomp_p8_script_probe", "tools/recomp/script_probe.py")
gc = _load("recomp_p8_gen_config", "tools/recomp/gen_config.py")


# ------------------------------------------------------------------ ipa_callees.json

def test_committed_table_has_the_lane_conventions():
    t = rp.load_ipa_table()
    assert t["slot_state_setup"]["params"] == ["s2"] and t["slot_state_setup"]["clobbers"] == ["s0", "s1", "s3"]
    assert t["sound_update_channel"]["params"] == ["t0"]
    assert t["func_800B6748"]["params"] == ["a0", "a1", "a2", "a3", "t0", "t1", "t2"]
    assert t["func_80096CA8"]["params"] == ["s8", "t0"]
    for name, e in t.items():
        assert e.get("evidence"), name
        if "params" in e:     # every shim is buildable
            words = rp.ipa_shim(0x800B4200, e["prototype"], e["params"], e.get("clobbers", ()), "out")
            assert words[-1] >> 16 == 0x27BD          # addiu sp, sp, +frame (delay slot of jr ra)


def test_func_800B6748_shim_moves_stack_args_into_t0_t2():
    t = rp.load_ipa_table()["func_800B6748"]
    words = rp.ipa_shim(0x800B6748, t["prototype"], t["params"], (), "out")
    loads = [(w >> 16) & 31 for w in words if w >> 26 == 0x23 and (w >> 21) & 31 == 29]   # lw rt, N(sp)
    assert {8, 9, 10} <= set(loads)


# ------------------------------------------------------------------ interprocedural derivation

def _enc_jal(target):
    return (3 << 26) | ((target >> 2) & 0x03FFFFFF)


def _image(funcs):
    """{vram: [words]} -> (base, bytes) image + function list."""
    base = min(funcs)
    end = max(v + 4 * len(w) for v, w in funcs.items())
    data = bytearray(end - base)
    for v, ws in funcs.items():
        for i, w in enumerate(ws):
            struct.pack_into(">I", data, v - base + 4 * i, w)
    flist = [{"name": "f_%08X" % v, "vram": v, "size": 4 * len(w)} for v, w in funcs.items()]
    return (base, bytes(data)), flist


JR_RA, NOP = 0x03E00008, 0
BNEZ_T0 = 0x15000002          # bnez t0, +2
MOVE_T0_ZERO = 0x00004025     # or t0, zero, zero
LUI_AT = 0x3C018011


def test_call_preserving_parameter_is_found_only_interprocedurally():
    leaf, callee, caller = 0x80002000, 0x80001000, 0x80003000
    img, funcs = _image({
        leaf: [JR_RA, NOP],
        callee: [_enc_jal(leaf), NOP, BNEZ_T0, NOP, JR_RA, NOP],
        caller: [_enc_jal(callee), MOVE_T0_ZERO, JR_RA, NOP]})
    words = ipd.body_words(img, callee, 24)
    assert "t0" not in ipd.register_params(words, include_o32=True)          # w24t default: calls clobber t0
    ip = rv.Interproc(img, funcs)
    assert "t0" in ipd.register_params(words, include_o32=True, call_effects=ip.call_effects)
    d = rv.derive("f_%08X" % callee, img, funcs)
    assert d["params"] == [("t0", "int")] and d["sites"] == 1 and d["clobbers"] == []
    assert rv.callee_spec(d) == "void f_80001000(s32 p0)=t0"
    name, entry = dc.parse_callee(rv.callee_spec(d))
    assert name == "f_80001000" and entry["params"] == ["t0"]


def test_unsaved_callee_saved_write_is_a_clobber_and_propagates():
    inner, outer = 0x80001000, 0x80002000
    or_s1 = (0 << 21) | (0 << 16) | (17 << 11) | 0x25          # or s1, zero, zero
    img, funcs = _image({inner: [or_s1, JR_RA, NOP], outer: [_enc_jal(inner), NOP, JR_RA, NOP]})
    ip = rv.Interproc(img, funcs)
    assert ip.unsaved(inner) == ["s1"] and ip.unsaved(outer) == ["s1"]


def test_static_kind_and_spec_shapes():
    assert rv.static_kind("lui a0, 0x8015") == "ptr"
    assert rv.static_kind("addiu a1, a1, 16256") == "ptr"
    assert rv.static_kind("addiu t0, zero, 1   (delay slot)") == "int"
    assert rv.static_kind("lwc1 f22, 360(sp)") == "float"
    d = {"function": "g", "params": [("a0", "ptr"), ("f22", "float")], "clobbers": ["f20"], "ret": "void",
         "order_evidence": {}}
    assert rv.callee_spec(d) == "void g(void *p0, f32 p1)=a0,f22/f20"
    lines = []
    assert rv.derive_report(dict(d, params=[("a0", "int"), ("f12", "float")], clobbers=[]), lines.append) is None
    assert any("plain o32" in x for x in lines)


# ------------------------------------------------------------------ draft_check --unit plumbing

def test_member_params_and_census_union(tmp_path):
    assert dc.member_params(["a:x=a0", "b:y=t3"], ["a", "b"], True) == {"a": "x=a0", "b": "y=t3"}
    with pytest.raises(dc.DCError, match="must start with one of the members"):
        dc.member_params(["x=a0"], ["a", "b"], True)
    assert dc.member_params(["x=a0"], ["a"], False) == {"a": "x=a0"}
    (tmp_path / "census.json").write_text(json.dumps({"functions": {
        "a": {"scripts": {"race": 3, "saves": 1}}, "b": {"scripts": {"saves": 9}}, "c": {"scripts": {}}}}))
    assert dc.census_scripts_for(["a", "b"], tmp_path) == (["saves", "race"], True)
    assert dc.census_scripts_for(["c"], tmp_path) == ([], True)
    assert dc.census_scripts_for(["x", "y"], tmp_path) == (None, False)


def test_replace_unit_entry_ids_and_capture_records(tmp_path):
    src = tmp_path / "u.c"
    src.write_text("void f(int a) {}\nint g(int b, int c) { return b; }\n")
    e = {"function": "f", "unit": ["f", "g"], "population": "game", "source": str(src), "mode": "ido",
         "capture_args": ["a=s8"], "unit_capture": {"g": {"capture_args": ["b=a0", "c=t3"], "capture_ret": "v0"}}}
    assert rp.unit_members(e) == ["f", "g"]
    with pytest.raises(rp.ReplaceError):
        rp.unit_members(dict(e, unit=["g", "f"]))
    recs = rp.capture_entries([("draft", e)], tmp_path)
    assert [r["id"] for r in recs] == ["draft__f", "draft__g"] and all(r["unit"] == "draft" for r in recs)
    assert [r["function"] for r in recs] == ["f", "g"]
    named = rp.name_functions("draft", e, [{"name": "f", "vram": 0x80800000, "size": 8},
                                           {"name": "g", "vram": 0x80800010, "size": 8},
                                           {"name": "helper", "vram": 0x80800020, "size": 8}])
    assert [n["replaces"] for n in named] == [True, True, False]
    with pytest.raises(rp.ReplaceError, match="does not define g"):
        rp.name_functions("draft", e, [{"name": "f", "vram": 0x80800000, "size": 8}])
    single = rp.capture_entries([("draft", dict(e, unit=None))], tmp_path)
    assert [r["id"] for r in single] == ["draft"] and "unit" not in single[0]


def test_gen_config_unit_hooks_record_only_the_outermost_call():
    recs = [{"id": "draft__f", "function": "f", "population": "game", "mode": "capture", "call": None,
             "unit": "draft"},
            {"id": "draft__g", "function": "g", "population": "game", "mode": "ido", "call": "repl__draft__g",
             "unit": "draft"},
            {"id": "x", "function": "h", "population": "game", "mode": "ido", "call": "repl__x__h"}]
    hooks = gc.replacement_hooks(recs)
    by = {h["func"]: h for h in hooks}
    f, g, h = by[gc.NAME_PREFIX["game"] + "f"], by[gc.NAME_PREFIX["game"] + "g"], by[gc.NAME_PREFIX["game"] + "h"]
    assert "rush_unit_enter()" in f["text"] and "rush_cap_arm(0)" in f["text"] and "rush_cap_bypass(0)" in f["text"]
    assert "rush_unit_enter()" in g["text"] and "repl__draft__g(rdram, ctx); } rush_unit_exit()" in g["text"]
    assert "rush_unit_enter" not in h["text"]
    assert {"rush_unit_enter", "rush_unit_exit"} <= set(f["contract"]) and "rush_cap_arm" in f["contract"]


def test_runtime_defines_the_new_contract_names():
    sys.path.insert(0, str(RECOMP / "runtime"))
    try:
        cc = _load("recomp_p8_check_contract", "tools/recomp/runtime/check_contract.py")
    finally:
        sys.path.pop(0)
    names = cc.defined_names(RECOMP / "runtime" / "src")
    assert {"rush_unit_enter", "rush_unit_exit", "rush_cap_arm", "rush_text_log", "rush_cap_enter"} <= names


def test_divergence_names_the_member():
    res = {"script": "saves", "first": {"name": "func_800A1C6C (draft__func_800A1C6C)", "frame": 720, "fn_call": 1,
                                         "seq": 667, "what": "effect", "fields": [], "words": []}}
    lines = dc.divergence_lines(res, "slot_state_lookup", ["slot_state_lookup", "func_800A1C6C"])
    assert lines[0] == "FIRST DIVERGING MEMBER: func_800A1C6C (unit slot_state_lookup, func_800A1C6C)"
    assert lines[1].startswith("FIRST DIVERGENCE: script saves, frame 720")


# ------------------------------------------------------------------ --gc policy

def _tree(root, checkout, name, age_s, now):
    d = root / "trees" / checkout / name
    for k in ("cap", "repl"):
        (d / k).mkdir(parents=True)
        (d / k / "lock").write_text("")
        os.utime(d / k / "lock", (now - age_s, now - age_s))
    return d


def test_gc_plan_keeps_recent_trees_and_young_runs(tmp_path):
    now = time.time()
    t_new = _tree(tmp_path, "repo-1", "fresh", 10, now)
    t_mid = _tree(tmp_path, "repo-1", "mid", 1000, now)
    t_old = _tree(tmp_path, "w27r-2", "stale", 10 ** 6, now)
    for n in ("fresh", "stale"):
        (tmp_path / "cache" / n / "k1").mkdir(parents=True)
    k2 = tmp_path / "cache" / "fresh" / "k2"
    k2.mkdir()
    os.utime(tmp_path / "cache" / "fresh" / "k1", (now - 500, now - 500))
    runs = tmp_path / "runs" / "fresh"
    for i in range(8):
        r = runs / ("r%d" % i)
        r.mkdir(parents=True)
        os.utime(r, (now - 10 ** 6 + i, now - 10 ** 6 + i))
    young = runs / "young"
    young.mkdir()
    inbox_old = tmp_path / "inbox" / "old"
    inbox_old.mkdir(parents=True)
    os.utime(inbox_old, (now - 10 ** 6, now - 10 ** 6))
    (tmp_path / "inbox" / "new").mkdir()
    plan = {p: why for p, why in dc.gc_plan(tmp_path, keep=2, days=3, now=now)}
    assert t_old in plan and t_new not in plan and t_mid not in plan
    assert tmp_path / "cache" / "stale" in plan
    assert tmp_path / "cache" / "fresh" / "k1" in plan and k2 not in plan
    deleted_runs = sorted(p.name for p in plan if p.parent == runs)
    assert deleted_runs == ["r0", "r1", "r2", "r3"]          # newest 5 kept (young, r7..r4), older ones go
    assert inbox_old in plan and tmp_path / "inbox" / "new" not in plan


# ------------------------------------------------------------------ script_probe

def test_script_probe_text_log_and_hook(tmp_path):
    log = tmp_path / "t.log"
    log.write_text("2\tLicensed by Nintendo\n92\tPRESS START\n900\t3\n901\t1:36.533\n950\tGAME OVER\nbad\n")
    assert sp.read_text(log) == [(2, "Licensed by Nintendo"), (92, "PRESS START"), (950, "GAME OVER")]
    assert sp.read_text(log, 90, 940, numeric=True) == [(92, "PRESS START"), (900, "3"), (901, "1:36.533")]
    hook = sp.text_hook([{"name": "menu_input_process", "vram": 0x800B6994, "size": 1280}])
    assert hook == [{"func": "menu_input_process", "before_vram": "0x800B6998",
                     "text": "{ void rush_text_log(uint8_t*, recomp_context*); rush_text_log(rdram, ctx); }"}]
    calls = tmp_path / "c.log"
    calls.write_text("# fn 0 c_f=capture:f args=a0 ret=v0\n# fn 1 c_g=capture:g args=a0 ret=v0\n"
                     "c 1 5 0 00000001\nC 1 5 0 r=0 w=0:0\nc 2 9 0 00000002\nc 3 12 1 00000003\n")
    assert sp.reached(calls) == {"f": {"calls": 2, "first": 5, "last": 9}, "g": {"calls": 1, "first": 12,
                                                                                "last": 12}}


def test_new_scripts_parse():
    for p in sorted((RECOMP / "scripts").glob("*.press")):
        sc = dc.oracle.parse_script(p.read_text(), p.name)      # raises on a malformed line
        assert sc["frames"] and sc["pak"] is not None, p.name   # every script says its length and pak mode


# ------------------------------------------------------------------ end to end (recomp host only)

def _on_recomp_host():
    p = dc.paths()
    return all(Path(p[k]).exists() for k in ("rom", "n64recomp", "rsprecomp", "runtime", "ido53"))


rocky = pytest.mark.skipif(not _on_recomp_host(), reason="recomp host only (ROM, N64Recomp, IDO toolkits)")


@rocky
def test_e2e_script_probe_title_text(tmp_path):
    press = tmp_path / "title.press"
    press.write_text("frames 200\npak on\n")
    out = tmp_path / "s.json"
    rc = sp.main([str(press), "--functions", "menu_input_process", "--text", "--json", str(out), "--jobs", "1"])
    assert rc == 0
    doc = json.loads(out.read_text())["title"]
    assert "menu_input_process" in doc["reached"]
    assert any(t == "PRESS START" for _f, t in doc["text"])


@rocky
def test_e2e_unit_equivalent_and_member_divergence(tmp_path):
    src = REPO / "cloud" / "work" / "frontier" / "w27e" / "pak_write_cluster" / "cluster.c"
    members = "func_800A1BB4,func_800A1DD4"
    good = subprocess.run([sys.executable, "-m", "tools.recomp.draft_check", "--unit", members, str(src),
                           "--scripts", "saves"], cwd=str(REPO), capture_output=True, text=True)
    assert good.returncode == 0, good.stdout + good.stderr
    assert "VERDICT: EQUIVALENT" in good.stdout and "func_800A1DD4 9" in good.stdout
    bad_src = tmp_path / "cluster_bad.c"
    text = src.read_text()
    # func_800A1BB4 (clears the pak record's modified flag): store 1 instead of 0
    head, sep, rest = text.partition("void func_800A1BB4(s32 index) {")
    assert sep and "if (p == 0) D_80144030[index].modified = 0;" in rest
    bad_src.write_text(head + sep + rest.replace("if (p == 0) D_80144030[index].modified = 0;",
                                                 "if (p == 0) D_80144030[index].modified = 1;", 1))
    bad = subprocess.run([sys.executable, "-m", "tools.recomp.draft_check", "--unit", members, str(bad_src),
                          "--scripts", "saves"], cwd=str(REPO), capture_output=True, text=True)
    assert bad.returncode == 1, bad.stdout + bad.stderr
    assert "FIRST DIVERGING MEMBER: func_800A1BB4" in bad.stdout
