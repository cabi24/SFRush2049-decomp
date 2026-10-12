"""Recomp Phase 3 on fixtures (no ROM, no compiler, no host): per-call capture
layouts and hooks, the call-log diff, input scripts, static binding."""
import importlib.util
import json
import re
import struct
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


rp = _load("recomp_replace_p3", "tools/recomp/replace.py")
oracle = _load("recomp_oracle_p3", "tools/recomp/oracle.py")
tg = _load("recomp_test_gen_config_p3", "tests/recomp/test_gen_config.py")
gc = tg.gc


# ------------------------------------------------------------------ capture layouts

def test_capture_layout_from_prototype():
    text = "typedef float f32;\nvoid steer(s32 i, f32 *p, f32 r, u8 m, s32 extra) {\n}\n"
    lay = rp.capture_layout(text, "steer")
    assert lay["known"] and lay["ret"] == 0
    assert lay["args"] == [["a0:i", 4], ["a1:p", 5], ["a2:r", 6], ["a3:m", 7], ["sp+16:extra", 1016]]


def test_capture_layout_floats_returns_and_unknown_types():
    lay = rp.capture_layout("f32 f(f32 a, f32 b, Thing c) {}", "f")
    assert lay["args"] == [["f12:a", 112], ["f14:b", 114], ["a2:c", 6]] and lay["ret"] == 100
    assert rp.capture_layout("f64 g(f64 a) {}", "g")["args"] == [["f12:a", 212]]
    assert rp.capture_layout("f64 g(f64 a) {}", "g")["ret"] == 200
    assert rp.capture_layout("s64 h(void) {}", "h")["ret"] == 3
    assert rp.capture_layout("Effect *k(s32 x) {}", "k")["ret"] == 2


def test_capture_layout_without_prototype_records_a0_to_a3():
    lay = rp.capture_layout("/* nothing */", "absent")
    assert not lay["known"] and [c for _l, c in lay["args"]] == [4, 5, 6, 7] and lay["ret"] == 2


def test_explicit_capture_registers():
    lay = rp.capture_layout("", "x", explicit=["handle=a0", "f22", "f24:d", "s0", "sp+20"], explicit_ret="void")
    assert lay["args"] == [["a0:handle", 4], ["f22", 122], ["f24:d", 224], ["s0", 16], ["sp+20", 1020]]
    assert lay["ret"] == 0
    with pytest.raises(rp.ReplaceError, match="unknown register"):
        rp.explicit_capture(["q9"])
    with pytest.raises(rp.ReplaceError, match="capture_ret"):
        rp.explicit_capture(["a0"], "f7")


def test_manifest_probes_and_sets():
    doc = rp.load_manifest(rp.MANIFEST)
    e = doc["entries"]["probe_entity_transform_calc"]
    assert e["mode"] == "capture" and e["capture_args"][1:] == ["f22", "f24", "f26", "f28"]
    chosen = rp.select(doc, "draft_engine_sound_sync_probe")
    entries = rp.capture_entries(chosen)
    assert [r["mode"] for r in entries] == ["capture"] * 5
    assert [r["probe"] for r in entries] == [False, True, True, True, True]
    assert entries[1]["capture"]["args"][1] == ["f22", 122]


def test_manifest_refuses_a_probe_without_prototype(tmp_path):
    p = tmp_path / "m.json"
    p.write_text(json.dumps({"entries": {"x": {"function": "f", "population": "game", "mode": "capture"}}}))
    with pytest.raises(rp.ReplaceError, match="prototype"):
        rp.load_manifest(p)


# ------------------------------------------------------------------ hooks and header

def _cap(rid, func, mode="ido", call=None, probe=False, args=(), ret=0):
    return {"id": rid, "function": func, "population": "game", "mode": mode, "call": call,
            "probe": probe, "capture": {"args": [list(a) for a in args], "ret": ret, "known": True}}


def test_capture_hook_reenters_the_original_with_a_bypass():
    hooks = gc.replacement_hooks([_cap("s", "game_c", mode="capture")])
    text = hooks[0]["text"]
    assert hooks[0]["func"] == "game_c" and hooks[0]["call"] == "game_c"
    assert "if (!rush_cap_bypass(0)) {" in text and "rush_cap_enter(rdram, ctx, 0); game_c(rdram, ctx); " \
        "rush_cap_exit(rdram, ctx, 0); return; } }" in text
    assert set(hooks[0]["contract"]) == {"rush_repl_count", "rush_cap_enter", "rush_cap_exit", "rush_cap_bypass"}


def test_probe_on_a_function_the_replacement_defines_hooks_the_copy_too():
    replaced = [_cap("d", "draft_f", call="repl__d__draft_f"), _cap("p", "helper", mode="capture", probe=True)]
    hooks = gc.replacement_hooks(replaced, (), ["repl__d__draft_f", "repl__d__helper"])
    funcs = [h["func"] for h in hooks]
    assert funcs == ["draft_f", "repl__d__helper", "helper"]
    assert "rush_cap_enter(rdram, ctx, 1); repl__d__helper(rdram, ctx);" in hooks[1]["text"]


def test_repl_header_capture_tables():
    replaced = [_cap("a", "f", call="repl__a__f", args=[("a0:x", 4), ("f14:y", 114)], ret=2),
                _cap("b", "g", mode="capture", probe=True)]
    header = gc.render_repl_header(replaced)
    assert "{2, 4, 114}," in header and "{0}," in header
    assert re.search(r"rush_cap_ret\[\] = \{\n    2,\n    0,", header)
    assert re.search(r"rush_cap_probe\[\] = \{\n    0,\n    1,", header)
    assert '"args=a0:x,f14:y ret=v0 proto=known",' in header and '"args=- ret=void' in header


def test_callee_probes_follow_retail_jals():
    # caller at 0x80100000: jal 0x80100010 (callee), jal 0x80100000 (itself), jal 0x80100010 again
    def jal(t):
        return (3 << 26) | ((t >> 2) & 0x03FFFFFF)
    words = [jal(0x80100010), 0, jal(0x80100000), jal(0x80100010), 0x03E00008, 0]
    data = b"".join(struct.pack(">I", w) for w in words)
    functions = {"game": [{"name": "caller", "vram": 0x80100000, "size": 16},
                          {"name": "callee", "vram": 0x80100010, "size": 8}], "static": []}
    images = {"game": (0x80100000, data)}
    probes = gc.callee_probes([_cap("c", "caller", call="repl__c__caller")], functions, images)
    assert [(p["id"], p["probe"], p["probe_of"]) for p in probes] == [("callee_callee", True, "caller")]
    assert probes[0]["capture"]["args"] == [] and probes[0]["capture"]["ret"] == 0


def test_generate_drops_probes_on_functions_the_runtime_reimplements(tmp_path):
    r = {"image": None, "functions": [], "native": [],
         "replaced": [_cap("s", "game_c", mode="capture"),
                      dict(_cap("p", "osCreateThread", mode="capture", probe=True), population="static")]}
    manifest, out = tg.run(tmp_path, repl=r)
    assert manifest["replacements"]["probes_dropped"] == ["osCreateThread"]
    assert "#define RUSH_REPL_COUNT 1" in (out / "rush2049_repl.h").read_text()


def test_self_calls_and_zero_loads_in_the_image():
    def jal(t):
        return (3 << 26) | ((t >> 2) & 0x03FFFFFF)
    raw = struct.pack(">IIII", jal(0x80800000), 0x8C200010, 0x3F800000, jal(0x80800000))  # 4th word = data
    out, n = rp.retarget_self_calls(raw, 0x80800000, 0x80800000, 0x80123450, [(0x80800000, 8)])
    assert n == 1 and struct.unpack(">I", out[:4])[0] == jal(0x80123450) and out[12:] == raw[12:]
    out, n = rp.nop_zero_loads(raw, 0x80800000, [(0x80800000, 8)])   # lw $zero, 0x10($at)
    assert n == 1 and out[4:8] == b"\0\0\0\0"


def test_legacy_names_win_for_game_bodies(tmp_path):
    leg = tmp_path / "legacy.txt"
    leg.write_text("// c\nosJamMesg = 0x800075E0; // static name osSendMesg\n")
    legacy = rp.game_legacy_names(leg)
    assert legacy == {"osJamMesg": 0x800075E0}
    functions = {"static": [{"name": "osJamMesg", "vram": 0x80007440}], "game": []}
    assert rp.address_table(functions, {}, "game", legacy)["osJamMesg"] == 0x800075E0
    assert rp.address_table(functions, {}, "static", legacy)["osJamMesg"] == 0x80007440
    assert rp.game_legacy_names(tmp_path / "gone.txt") == {}


# ------------------------------------------------------------------ call-log diff

HEAD = ["# rush2049 call log v2: effect = words changed",
        "# fn 0 d=ido:outer args=a0:x ret=v0 proto=known probe=0",
        "# fn 1 p=capture:inner args=f22 ret=void proto=known probe=1"]


def calls(*records, head=HEAD):
    return "\n".join(list(head) + list(records) + ["END 2 calls"]) + "\n"


STOCK = ["c 1 5 0 00000001", "c 2 5 1 3F800000", "C 2 5 1 r=- w=1:aa", "C 1 5 0 r=00000002 w=3:bb"]


def cmp(a, b):
    return oracle.compare_calls(a.splitlines(True), b.splitlines(True))


def test_identical_call_logs():
    r = cmp(calls(*STOCK), calls(*STOCK))
    assert r == {"verdict": "SAME", "calls": 2}
    assert oracle.call_report(r) == ["  per-call: all 2 captured calls identical (arguments, returns, effects)"]


def test_first_differing_argument_names_the_call_and_its_caller():
    repl = list(STOCK)
    repl[1] = "c 2 5 1 00000000"
    r = cmp(calls(*STOCK), calls(*repl))
    assert r["what"] == "args" and r["seq"] == 2 and r["frame"] == 5 and r["fn_call"] == 1
    assert r["name"] == "inner (p)" and r["fields"] == [("f22", "3F800000", "00000000")]
    assert r["inside"] == [{"seq": 1, "name": "outer (d)", "fn_call": 1}]
    lines = oracle.call_report(r)
    assert lines[0] == "FIRST DIFFERING CALL: seq 2 = inner (p) call 1, frame 5: args"
    assert lines[1] == "  inside: outer (d) call 1 (seq 1)"


def test_return_effect_sequence_and_length_differences():
    repl = list(STOCK)
    repl[3] = "C 1 5 0 r=00000003 w=3:bb"
    assert cmp(calls(*STOCK), calls(*repl))["what"] == "return"
    repl[3] = "C 1 5 0 r=00000002 w=4:cc"
    r = cmp(calls(*STOCK), calls(*repl))
    assert r["what"] == "effect" and r["fields"] == [("effect words:hash", "3:bb", "4:cc")]
    r = cmp(calls(*STOCK), calls(STOCK[0], STOCK[3]))           # replaced body never calls inner
    assert r["what"] == "sequence" and "entry of inner" in r["fields"][0][1] and "exit of outer" in r["fields"][0][2]
    r = cmp(calls(*STOCK), calls(*STOCK[:2]))
    assert r["verdict"] == "DIFFERENT"


def test_call_logs_of_different_layouts_or_versions_are_refused():
    other = [HEAD[0], HEAD[1].replace("a0:x", "a0:y"), HEAD[2]]
    with pytest.raises(oracle.OracleError, match="different functions or layouts"):
        cmp(calls(*STOCK), calls(*STOCK, head=other))
    old = [HEAD[0].replace("v2", "v1")] + HEAD[1:]
    with pytest.raises(oracle.OracleError, match="capture versions"):
        cmp(calls(*STOCK), calls(*STOCK, head=old))
    assert oracle.compare_calls(["# rush2049 call log v2\n", "END 0 calls\n"], calls(*STOCK).splitlines(True)) is None


def test_effect_words_and_diff():
    a = ["W 7 80142DDC 3F800000 3F800000\n", "W 7 80142DF4 00000000 3F800000\n", "W 8 80000000 0 1\n"]
    b = ["W 7 80142DDC 3F800000 C0000000\n", "W 7 80142DF4 00000000 3F800000\n"]
    wa, wb = oracle.effect_words(a, 7), oracle.effect_words(b, 7)
    assert wa[0x80142DF4] == ("00000000", "3F800000") and 0x80000000 not in wa
    d = oracle.effect_diff(wa, wb, [(0x80142DD8, "D_80142DD8")])
    assert d == [(0x80142DDC, ("3F800000", "3F800000"), ("3F800000", "C0000000"), "D_80142DD8+0x4")]
    assert oracle.effect_diff({1: ("0", "1")}, {}, None) == [(1, ("0", "1"), None, "")]


def test_cli_calls(tmp_path, capsys):
    (tmp_path / "s").write_text(calls(*STOCK))
    repl = list(STOCK)
    repl[2] = "C 2 5 1 r=- w=2:ee"
    (tmp_path / "r").write_text(calls(*repl))
    assert oracle.main(["calls", str(tmp_path / "s"), str(tmp_path / "r")]) == 1
    out = capsys.readouterr().out
    assert out.startswith("FIRST DIFFERING CALL: seq 2 = inner (p) call 1, frame 5: effect")


def test_compare_with_calls_turns_equivalent_into_different(tmp_path, capsys):
    log = "# rush2049 frame oracle v1\nF 1 dl=a dls=1 dlb=8 ram=0 stk=0\nP 0:x\nR d=ido:outer 1\nEND 1 frames\n"
    (tmp_path / "a.log").write_text(log)
    (tmp_path / "b.log").write_text(log)
    (tmp_path / "s").write_text(calls(*STOCK))
    repl = list(STOCK)
    repl[0] = "c 1 5 0 00000009"
    (tmp_path / "r").write_text(calls(*repl))
    rc = oracle.main(["compare", str(tmp_path / "a.log"), str(tmp_path / "b.log"),
                      "--calls", str(tmp_path / "s"), str(tmp_path / "r")])
    out = capsys.readouterr().out
    assert rc == 1 and out.startswith("FIRST DIFFERING CALL: seq 1 = outer (d) call 1, frame 5: args")
    assert "  a0:x: stock 00000001, replaced 00000009" in out and "  frames: no judged frame difference" in out


def test_thread_host_words_are_skipped_in_word_diffs(tmp_path):
    log = ("# rush2049 frame oracle v1\n# thread_host_words 0x20,0x24\nT 0 0x80034BA0\n"
           "F 1 dl=a dls=1 dlb=8 ram=0 stk=0\nP 0:x\nEND 1 frames\n")
    (tmp_path / "a.log").write_text(log)
    res = oracle.compare(oracle.Run(tmp_path / "a.log"), [oracle.Run(tmp_path / "a.log")])
    assert res["threads"] == [0x80034BA0] and res["thread_host_words"] == [0x20, 0x24]
    assert oracle.thread_word_ranges(res) == [(0x80034BC0, 0x80034BC4), (0x80034BC4, 0x80034BC8)]


# ------------------------------------------------------------------ input scripts

def test_parse_script():
    sc = oracle.parse_script("# demo\nframes 900\n11 A\n5 START 0.5\n6.5 RIGHT@2   # p2\n6.5 A@1\n")
    assert sc == {"press": "5:START:0.5,6.5:RIGHT@2,6.5:A,11:A", "frames": 900, "controllers": 2}


@pytest.mark.parametrize("text,msg", [
    ("5 JUMP", "unknown button"), ("5 A@5", "port must be 1-4"), ("x A", "bad number"),
    ("5", "expected TIME"), ("frames many", "frames takes"), ("-1 A", "times must be"),
    ("# only a comment", "no presses"), ("5 A 0", "durations"),
])
def test_parse_script_errors(text, msg):
    with pytest.raises(oracle.OracleError, match=msg):
        oracle.parse_script(text)


def test_committed_scripts_parse_and_have_a_readme_line():
    readme = (REPO / "tools/recomp/README.md").read_text()
    names = sorted(p.stem for p in oracle.SCRIPT_DIR.glob("*.press"))
    assert {"race", "race_track2", "two_player", "audio_options"} <= set(names)
    for name in names:
        sc = oracle.load_script(name)
        assert sc["frames"] and sc["press"]
        assert "`%s.press`" % name in readme, name
    assert oracle.load_script("race")["press"] == oracle.DEFAULT_PRESS
    assert oracle.load_script("two_player")["controllers"] == 2
    with pytest.raises(oracle.OracleError, match="no input script"):
        oracle.load_script("no_such_script")


# ------------------------------------------------------------------ static binding

def make_object(text_words, symbols, rels, bss_size=8, data=b""):
    """Big-endian ELF32 relocatable: .text, .data, .bss, .symtab, .strtab,
    .shstrtab, .rel.text. symbols: [(name, value, size, info, shndx)] after the
    null symbol (shndx: 1 .text, 2 .data, 3 .bss); rels: [(offset, type, symndx)]."""
    text = b"".join(struct.pack(">I", w) for w in text_words)
    strtab = b"\0"
    sym = struct.pack(">IIIBBH", 0, 0, 0, 0, 0, 0)
    for name, value, size, info, shndx in symbols:
        off = len(strtab) if name else 0
        if name:
            strtab += name.encode() + b"\0"
        sym += struct.pack(">IIIBBH", off, value, size, info, 0, shndx)
    rel = b"".join(struct.pack(">II", o, (n << 8) | t) for o, t, n in rels)
    names = [".text", ".data", ".bss", ".symtab", ".strtab", ".shstrtab", ".rel.text"]
    shstr = b"\0" + b"".join(n.encode() + b"\0" for n in names)
    noff = {n: 1 + sum(len(m) + 1 for m in names[:i]) for i, n in enumerate(names)}
    body = [text, data, b"", sym, strtab, shstr, rel]
    offs, off = [], 52
    for b in body:
        offs.append(off)
        off += len(b)

    def sh(name, typ, o, size, link=0, info=0, entsize=0):
        return struct.pack(">IIIIIIIIII", noff[name] if name else 0, typ, 0, 0, o, size, link, info, 4, entsize)
    shdrs = sh(None, 0, 0, 0)
    shdrs += sh(".text", 1, offs[0], len(text))
    shdrs += sh(".data", 1, offs[1], len(data))
    shdrs += sh(".bss", 8, offs[2], bss_size)
    shdrs += sh(".symtab", 2, offs[3], len(sym), 5, 1, 16)
    shdrs += sh(".strtab", 3, offs[4], len(strtab))
    shdrs += sh(".shstrtab", 3, offs[5], len(shstr))
    shdrs += sh(".rel.text", 9, offs[6], len(rel), 4, 1, 8)
    ident = b"\x7fELF" + bytes([1, 2, 1]) + b"\0" * 9
    hdr = ident + struct.pack(">HHIIIIIHHHHHH", 1, 8, 1, 0, 0, off, 0, 52, 0, 0, 40, 8, 6)
    data_all = hdr + b"".join(body) + shdrs
    elf = rp.read_elf(data_all)
    elf["_data"] = data_all
    return elf


# lui $at,%hi(sym+2); sh $t6,%lo(sym+2)($at); jr $ra; nop  (func_800E7710's SI message store)
CODE = [0x3C010000, 0xA42E0002, 0x03E00008, 0x00000000]
FUNC = ("f", 0, 16, 0x12, 1)
RETAIL = {0x800E7710: 0x3C018015, 0x800E7714: 0xA42E0F5A}


def retail_word(addr):
    return RETAIL.get(addr)


def bind(elf, named=None, retail=0x800E7710):
    func = next(s for s in elf["symbols"] if s["name"] == "f")
    return rp.static_bindings(elf, "f", func, retail, retail_word, named)


def test_static_bound_from_the_retail_instructions():
    # relocation against the .bss SECTION symbol (STT_SECTION, local), addend 2 in the instructions
    elf = make_object(CODE, [(None, 0, 0, 0x03, 3), FUNC], [(0, 5, 1), (4, 6, 1)])
    bound, missing = bind(elf)
    assert bound == {(".bss", 2): (0x80150F5A, "retail")} and missing == []
    raw = struct.pack(">IIII", *CODE)
    out, n = rp.apply_static_bindings(raw, 0x80800000, 0x80800000, elf, bound)
    assert n == 1 and struct.unpack(">II", out[:8]) == (0x3C018015, 0xA42E0F5A)


def test_static_left_private_when_the_retail_code_differs():
    elf = make_object(CODE, [(None, 0, 0, 0x03, 3), FUNC], [(0, 5, 1), (4, 6, 1)])
    func = next(s for s in elf["symbols"] if s["name"] == "f")
    bound, missing = rp.static_bindings(elf, "f", func, 0x800E7710,
                                        lambda a: {0x800E7710: 0x3C018015, 0x800E7714: 0xAC2E0F5A}.get(a))
    assert bound == {} and missing == [((".bss", 2), None)]       # sw, not sh: not this pair
    assert bind(elf, retail=None) == ({}, [((".bss", 2), None)])


def test_static_bound_by_address_name_or_manifest():
    syms = [("D_80150F58", 0, 4, 0x01, 3), FUNC]                  # LOCAL OBJECT named by address
    elf = make_object(CODE, syms, [(0, 5, 1), (4, 6, 1)])
    assert bind(elf, retail=None)[0] == {(".bss", 2): (0x80150F5A, "name")}
    syms = [("si_message", 0, 4, 0x01, 3), FUNC]
    elf = make_object(CODE, syms, [(0, 5, 1), (4, 6, 1)])
    assert bind(elf, {"si_message": "0x80150F58"}, retail=None)[0] == {(".bss", 2): (0x80150F5A, "manifest")}


def test_globals_and_text_references_are_not_statics():
    syms = [("D_80111111", 0, 4, 0x11, 3), FUNC]                  # GLOBAL: bound by the linker script
    elf = make_object(CODE, syms, [(0, 5, 1), (4, 6, 1)])
    assert bind(elf) == ({}, [])


def test_one_hi16_serving_two_lo16():
    code = [0x3C010000, 0xA42E0000, 0xA42F0004, 0x03E00008]
    elf = make_object(code, [(None, 0, 0, 0x03, 3), FUNC], [(0, 5, 1), (4, 6, 1), (8, 6, 1)])
    text = elf["_data"][52:52 + 16]
    pairs = rp.hilo_pairs(elf, text)
    assert [(p[1], p[2], p[3]) for p in pairs] == [([0], 4, 0), ([0], 8, 4)]
