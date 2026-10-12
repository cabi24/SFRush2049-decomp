"""tools/recomp/replace.py (Phase 2 function replacement) and its gen_config
integration, on fixtures. No ROM and no compiler: the IDO/ld.lld steps are
exercised on the recomp host (tools/recomp/README.md "Phase 2")."""
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


rp = _load("recomp_replace", "tools/recomp/replace.py")
tg = _load("recomp_test_gen_config", "tests/recomp/test_gen_config.py")


# ------------------------------------------------------------------ o32 argument layout

def kinds(proto):
    _ret, _name, params = rp.parse_prototype(proto)
    out = []
    for loc in rp.o32_layout(params):
        if loc["kind"] == "gpr":
            out.append("r%d" % loc["reg"])
        elif loc["kind"] == "fpr":
            out.append("f%d" % loc["reg"])
        elif loc["kind"] == "gpr64":
            out.append("r%d:r%d" % (loc["hi"], loc["lo"]))
        else:
            out.append("sp+%d" % loc["offset"])
    return out


def test_o32_integer_and_stack_slots():
    assert kinds("void f(s32 a, u8 b, s16 c, void *d, u32 e, int g)") == \
        ["r4", "r5", "r6", "r7", "sp+16", "sp+20"]


def test_o32_float_registers_only_for_leading_floats():
    assert kinds("f32 f(f32 a, f32 b, f32 c)") == ["f12", "f14", "r6"]
    assert kinds("void f(f32 a, s32 b, f32 c)") == ["f12", "r5", "r6"]
    # first argument not floating point: floats travel in integer registers
    assert kinds("void f(float *src, float *dst, float scale)") == ["r4", "r5", "r6"]
    assert kinds("void f(s32 a, f32 b)") == ["r4", "r5"]


def test_o32_doubles_take_aligned_pairs():
    assert kinds("void f(f64 a, f64 b)") == ["f12", "f14"]
    assert kinds("void f(s32 a, f64 b)") == ["r4", "r6:r7"]
    assert kinds("void f(s32 a, s64 b, s32 c)") == ["r4", "r6:r7", "sp+16"]
    assert kinds("void f(s32 a, s32 b, s32 c, f64 d)") == ["r4", "r5", "r6", "sp+16"]


def test_array_parameters_are_pointers():
    ret, name, params = rp.parse_prototype("void func_8008B32C(float src[3][3], float dst[3][3], float scale)")
    assert name == "func_8008B32C" and ret == "void"
    assert [p for p, _ in params] == ["float *", "float *", "float"]


def test_find_prototype_from_source():
    text = "typedef float f32;\n/* c */\nf32 dot3(f32 *a,f32 *b) {\n return a[0]*b[0];\n}\n"
    assert rp.find_prototype(text, "dot3") == "f32 dot3(f32 *a,f32 *b)"
    with pytest.raises(rp.ReplaceError):
        rp.find_prototype(text, "absent")


def test_native_fit_refuses_byte_order_unsafe_pointers():
    rp.check_native_fit("f32", rp.parse_prototype("f32 f(f32 *a, s32 *b)")[2])
    with pytest.raises(rp.ReplaceError, match="32-bit scalars"):
        rp.check_native_fit("void", rp.parse_prototype("void f(s16 *a)")[2])
    with pytest.raises(rp.ReplaceError, match="32-bit scalars"):
        rp.check_native_fit("void", rp.parse_prototype("void f(Header *h)")[2])
    with pytest.raises(rp.ReplaceError, match="pointer to pointer"):
        rp.check_native_fit("void", rp.parse_prototype("void f(s32 **h)")[2])
    with pytest.raises(rp.ReplaceError, match="unsupported"):
        rp.parse_prototype("void f(s32 a, ...)")
        rp.check_native_fit("void", rp.parse_prototype("void f(s32 a, ...)")[2])


def test_render_wrapper_unpacks_and_returns():
    ret, name, params = rp.parse_prototype("f32 dot(f32 *a, f32 *b, f32 w, s32 n, s16 m)")
    text = rp.render_wrapper("dot_id", ret, name, params, "body_dot")
    assert "void native__dot_id(uint8_t* rdram, recomp_context* ctx) {" in text
    assert "float body_dot(void*, void*, float, int32_t, int16_t);" in text
    assert "uint32_t a0_w = (uint32_t)ctx->r4;" in text
    assert "void* a1 = rush_host_ptr(rdram, a1_w);" in text
    assert "float a2; memcpy(&a2, &a2_w, 4);" in text          # float in a2 (bits)
    assert "uint32_t a4_w = (uint32_t)MEM_W(16, ctx->r29);" in text  # 5th slot on the stack
    assert "int16_t a4 = (int16_t)a4_w;" in text
    assert "ctx->f0.fl = body_dot(a0, a1, a2, a3, a4);" in text
    ret, name, params = rp.parse_prototype("f64 g(f64 x, f32 y)")
    text = rp.render_wrapper("g", ret, name, params, "b")
    assert "double a0 = ctx->f12.d;" in text and "float a1 = ctx->f14.fl;" in text
    assert "ctx->f0.d = b(a0, a1);" in text
    ret, name, params = rp.parse_prototype("s64 h(s32 a, s64 b)")
    text = rp.render_wrapper("h", ret, name, params, "b")
    assert "((uint64_t)(uint32_t)ctx->r6 << 32) | (uint32_t)ctx->r7" in text
    assert "ctx->r2 = (gpr)(int32_t)(uint32_t)(r >> 32);" in text and "ctx->r3 = (gpr)(int32_t)(uint32_t)r;" in text
    ret, name, params = rp.parse_prototype("void *k(void *p)")
    text = rp.render_wrapper("k", ret, name, params, "b")
    assert "0x80000000u + (uint32_t)(r - rdram)" in text
    ret, name, params = rp.parse_prototype("u16 m(void)")
    assert "ctx->r2 = (gpr)(int32_t)(uint16_t)b();" in rp.render_wrapper("m", ret, name, params, "b")


def test_native_body_includes_source_unchanged(tmp_path):
    src = tmp_path / "f.c"
    src.write_text("int f(int a) { return a; }\n")
    text = rp.render_native_body(src, "f", "rush_native_body__x")
    assert "#define f rush_native_body__x" in text and '#include "%s"' % src.resolve() in text


# ------------------------------------------------------------------ manifest, flags, symbols

def test_committed_manifest_is_well_formed():
    doc = rp.load_manifest(rp.MANIFEST)
    assert len(doc["sets"]["matched10"]) == 10
    for rid, e in doc["entries"].items():
        if e.get("mode") == "capture":                    # Phase 3 probe: prototype, no body
            assert e.get("state") == "probe" and (e.get("prototype") or e.get("source")), rid
            continue
        assert (REPO / e["source"]).is_file(), e["source"]
        assert e.get("state") in ("matched", "draft", "injected-bug")
    chosen = rp.select(doc, "matched10")
    assert len({e["function"] for _r, e in chosen}) == 10
    assert sum(1 for _r, e in chosen if e["mode"] == "native") == 2


def test_select_refuses_two_replacements_of_one_function():
    doc = {"entries": {"a": {"function": "f", "population": "game", "source": "x.c"},
                       "b": {"function": "f", "population": "game", "source": "y.c"}},
           "sets": {"s": ["a", "b"]}}
    with pytest.raises(rp.ReplaceError, match="replaced twice"):
        rp.select(doc, "s")
    assert [r for r, _e in rp.select(doc, None, ["a"])] == ["a"]


def test_manifest_validation(tmp_path):
    p = tmp_path / "m.json"
    p.write_text(json.dumps({"entries": {"x": {"function": "f", "population": "moon", "source": "a.c"}}}))
    with pytest.raises(rp.ReplaceError, match="population"):
        rp.load_manifest(p)
    p.write_text(json.dumps({"entries": {}, "sets": {"s": ["missing"]}}))
    with pytest.raises(rp.ReplaceError, match="unknown replacement"):
        rp.load_manifest(p)


def test_source_flags():
    assert rp.source_flags("/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (also MATCH at -O2) */\n", "game") == \
        "-g0 -O3 -mips2 -G 0 -non_shared"
    assert rp.source_flags("/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */", "game").endswith(
        "-Wab,-r4300_mul")
    assert rp.source_flags("void f(void) {}", "static") == rp.DEFAULT_FLAGS["static"]


def test_address_table_and_resolve():
    functions = {"static": [{"name": "osRecvMesg", "vram": 0x80001000, "size": 8}],
                 "game": [{"name": "audio_dsp_process", "vram": 0x80090000, "size": 8}],
                 "ovl_a": [{"name": "func_8038A400", "vram": 0x8038A400, "size": 8}],
                 "ovl_b": [{"name": "func_8038A400", "vram": 0x8038A400, "size": 8}]}
    symbols = {"game": {"gThing": 0x80123450}, "ovl_a": {"D_8038B000": 0x8038B000}}
    table = rp.address_table(functions, symbols, "game")
    assert "func_8038A400" not in table and "D_8038B000" not in table
    assert rp.address_table(functions, symbols, "ovl_a")["D_8038B000"] == 0x8038B000
    res, missing = rp.resolve(["audio_dsp_process", "gThing", "D_80114744", "func_8008A644", "strcpy"], table)
    assert res == {"audio_dsp_process": 0x80090000, "gThing": 0x80123450, "D_80114744": 0x80114744,
                   "func_8008A644": 0x8008A644}
    assert missing == ["strcpy"]


def test_linker_script():
    text = rp.render_linker_script({"D_80114744": 0x80114744}, 0x80800100)
    assert ". = 0x80800100;" in text and "D_80114744 = 0x80114744;" in text
    assert ".repl : {" in text and "/DISCARD/" in text


# ------------------------------------------------------------------ ELF reader

def tiny_elf():
    """Big-endian ELF32 relocatable: .text (2 words), symbols f (FUNC) and an
    undefined g referenced by a relocation, and an undefined h no relocation uses."""
    text = struct.pack(">II", 0x0C000000, 0x03E00008)
    strtab = b"\0f\0g\0h\0"
    shstr = b"\0.text\0.symtab\0.strtab\0.shstrtab\0.rel.text\0"
    sym = struct.pack(">IIIBBH", 0, 0, 0, 0, 0, 0)
    sym += struct.pack(">IIIBBH", 1, 0, 8, 0x12, 0, 1)   # f: GLOBAL FUNC in .text
    sym += struct.pack(">IIIBBH", 3, 0, 0, 0x10, 0, 0)   # g: GLOBAL NOTYPE UND
    sym += struct.pack(">IIIBBH", 5, 0, 0, 0x10, 0, 0)   # h: GLOBAL NOTYPE UND
    rel = struct.pack(">II", 0, (2 << 8) | 4)             # R_MIPS_26 against g
    body = [text, sym, strtab, shstr, rel]
    offs, off = [], 52
    for b in body:
        offs.append(off)
        off += len(b)
    shoff = off
    def sh(name, typ, addr, o, size, link=0, info=0, entsize=0):
        return struct.pack(">IIIIIIIIII", name, typ, 0, addr, o, size, link, info, 4, entsize)
    shdrs = sh(0, 0, 0, 0, 0)
    shdrs += sh(1, 1, 0x80800000, offs[0], len(text))          # 1 .text
    shdrs += sh(7, 2, 0, offs[1], len(sym), 3, 1, 16)          # 2 .symtab
    shdrs += sh(15, 3, 0, offs[2], len(strtab))                # 3 .strtab
    shdrs += sh(23, 3, 0, offs[3], len(shstr))                 # 4 .shstrtab
    shdrs += sh(33, 9, 0, offs[4], len(rel), 2, 1, 8)          # 5 .rel.text
    ident = b"\x7fELF" + bytes([1, 2, 1]) + b"\0" * 9
    hdr = ident + struct.pack(">HHIIIIIHHHHHH", 1, 8, 1, 0, 0, shoff, 0, 52, 0, 0, 40, 6, 4)
    return hdr + b"".join(body) + shdrs


def test_read_elf():
    data = tiny_elf()
    elf = rp.read_elf(data)
    assert [s["name"] for s in elf["sections"]][1:] == [".text", ".symtab", ".strtab", ".shstrtab", ".rel.text"]
    assert rp.undefined_symbols(elf) == ["g"]                 # h: no relocation uses it
    funcs = rp.defined_functions(elf, ".text")
    assert funcs == [{"name": "f", "vram": 0, "size": 8, "local": False}]
    vram, raw = rp.section_bytes(data, elf, ".text")
    assert vram == 0x80800000 and raw == struct.pack(">II", 0x0C000000, 0x03E00008)
    with pytest.raises(rp.ReplaceError):
        rp.read_elf(b"\x7fELF" + bytes([2, 1]) + b"\0" * 60)


def test_name_functions():
    named = rp.name_functions("rh", {"function": "render_helper"},
                              [{"name": "render_helper", "vram": 0x80800000, "size": 0xE8, "local": False},
                               {"name": "helper", "vram": 0x808000F0, "size": 8, "local": True}])
    assert [f["recomp_name"] for f in named] == ["repl__rh__render_helper", "repl__rh__helper"]
    with pytest.raises(rp.ReplaceError, match="does not define"):
        rp.name_functions("x", {"function": "absent"}, [])


# ------------------------------------------------------------------ gen_config integration

def repl_fixture():
    code = struct.pack(">IIII", 0x24020001, 0x03E00008, 0x00000000, 0x3F800000)  # li v0,1; jr ra; nop; 1.0f
    return {"image": {"vram": 0x80800000, "bytes": code},
            "functions": [{"name": "repl__g__game_c", "vram": 0x80800000, "size": 12}],
            "replaced": [{"id": "g", "function": "game_c", "population": "game", "mode": "ido",
                          "call": "repl__g__game_c", "source": "src/blob/game_c.c", "private_data": []},
                         {"id": "n", "function": "func_8038A408", "population": "ovl_b", "mode": "native",
                          "call": "native__n", "source": "src/ovl_b/x.c"}],
            "native": [{"id": "n", "wrapper": "native__n"}]}


def test_generate_with_replacements(tmp_path):
    manifest, out = tg.run(tmp_path, repl=repl_fixture())
    syms = (out / "rush2049.syms.toml").read_text()
    assert 'name = ".repl"' in syms and 'name = "repl__g__game_c", vram = 0x80800000' in syms
    cfg = (out / "rush2049.recomp.toml").read_text()
    hooks = re.findall(r'func = "(\w+)"\ntext = "([^\n]*)"', cfg)
    assert ("game_c", "{ void rush_repl_count(uint8_t*, int); void rush_cap_enter(uint8_t*, recomp_context*, int); "
            "void rush_cap_exit(uint8_t*, recomp_context*, int); void repl__g__game_c(uint8_t*, recomp_context*); "
            "rush_repl_count(rdram, 0); rush_cap_enter(rdram, ctx, 0); repl__g__game_c(rdram, ctx); "
            "rush_cap_exit(rdram, ctx, 0); return; }") in hooks
    assert any(f == "ovl_b_func_8038A408" and "native__n(rdram, ctx); rush_cap_exit(rdram, ctx, 1); return;" in t
               for f, t in hooks)
    header = (out / "rush2049_repl.h").read_text()
    assert "#define RUSH_REPL_COUNT 2" in header and '"g=ido:game_c",' in header
    assert "#define RUSH_REPL_VRAM 0x80800000u" in header
    rom = (out / "rom" / "rush2049.recomp.z64").read_bytes()
    img = manifest["replacements"]["image"]
    off = int(img["rom"], 16)
    assert rom[off:off + 16] == repl_fixture()["image"]["bytes"] and img["size"] == 16
    contract = manifest["runtime_contract"]
    assert "rush_repl_count" in contract["hooks"] and "native__n" in contract["hooks"]
    assert "repl__g__game_c" not in contract["hooks"]       # recompiled, not a runtime function
    assert all("text" not in h for h in manifest["patches"]["hook"])


def test_generate_without_replacements_writes_empty_header(tmp_path):
    manifest, out = tg.run(tmp_path)
    header = (out / "rush2049_repl.h").read_text()
    assert "#define RUSH_REPL_COUNT 0" in header and "#define RUSH_REPL_SIZE 0x0u" in header
    assert manifest["replacements"] == {"image": None, "native": [], "replaced": [], "probes_dropped": []}
    assert 'name = ".repl"' not in (out / "rush2049.syms.toml").read_text()


def test_replacing_a_dropped_function_is_refused(tmp_path):
    r = repl_fixture()
    r["replaced"][0]["function"] = "osCreateThread"       # reimplemented by the runtime: not emitted
    r["replaced"][0]["population"] = "static"
    with pytest.raises(tg.gc.GenError, match="not emitted"):
        tg.run(tmp_path, repl=r)


def test_contract_checker_reads_native_wrappers(tmp_path):
    cc = _load("recomp_check_contract_r", "tools/recomp/runtime/check_contract.py")
    d = tmp_path / "native"
    d.mkdir()
    (d / "n_wrap.c").write_text("void native__n(uint8_t* rdram, recomp_context* ctx) {\n}\n")
    assert "native__n" in cc.defined_names(d, patterns=("*.c",))
