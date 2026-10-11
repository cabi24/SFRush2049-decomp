"""Phase 1 generator features (hooks, runtime contract, images header,
identity renames) and the runtime's contract checker, on ROM-free fixtures."""
import importlib.util
import json
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


gc = _load("recomp_gen_config_p1", "tools/recomp/gen_config.py")
cc = _load("recomp_check_contract", "tools/recomp/runtime/check_contract.py")

JR_RA, NOP = 0x03E00008, 0x00000000
LISTS = {"reimplemented_funcs": ["osCreateThread", "osSetEventMesg"],
         "ignored_funcs": ["__osDisableInt", "osCreateThread"], "renamed_funcs": ["memcpy"]}


def words(*ws):
    return b"".join(struct.pack(">I", w) for w in ws)


def jal(target):
    return 0x0C000000 | ((target & 0x0FFFFFFF) >> 2)


def fixture():
    # static: entrypoint (jal __osDisableInt, jal osCreateThread, jal osCauseGet),
    # osCreateThread, __osDisableInt, osCauseGet, memcpy, osSetEventMesgAlt
    static = (words(jal(0x80000428), NOP, jal(0x80000420), NOP, jal(0x80000430), NOP, JR_RA, NOP)
              + words(JR_RA, NOP) + words(JR_RA, NOP) + words(JR_RA, NOP) + words(JR_RA, NOP)
              + words(JR_RA, NOP))
    game = words(jal(0x80000438), NOP, JR_RA, NOP)
    rom = bytearray(0x2000)
    rom[0x1000:0x1000 + len(static)] = static
    images = {"static": (gc.BOOT_VRAM, bytes(static)), "game": (gc.GAME_VRAM, game),
              "ovl_a": (gc.OVL_VRAM, words(JR_RA, NOP)), "ovl_b": (gc.OVL_VRAM, words(JR_RA, NOP))}
    functions = {
        "static": [gc._fn("entrypoint", 0x80000400, 32), gc._fn("osCreateThread", 0x80000420, 8),
                   gc._fn("__osDisableInt", 0x80000428, 8), gc._fn("osCauseGet", 0x80000430, 8),
                   gc._fn("memcpy", 0x80000438, 8), gc._fn("osSetEventMesgAlt", 0x80000440, 8)],
        "game": [gc._fn("game_a", 0x80086A50, 16)],
        "ovl_a": [gc._fn("func_8038A400", 0x8038A400, 8)],
        "ovl_b": [gc._fn("func_8038A400", 0x8038A400, 8)],
    }
    return images, functions, {p: {} for p in gc.SECTIONS}, bytes(rom)


def run(tmp_path, patches=None, **kw):
    images, functions, symbols, rom = fixture()
    p = {"stubs": {}, "ignored": {"osCauseGet": "mfc0"}, "renamed": {}, "instruction": [], "hook": []}
    p.update(patches or {})
    out = tmp_path / "out"
    args = dict(lists=LISTS, expected_counts=None, extras=None, patches=p,
                streams={"game": 0xB0CB10, "ovl_a": 0xB5C534, "ovl_b": 0xB6FEC4})
    args.update(kw)
    return gc.generate(images, functions, symbols, rom, out, **args), out


def test_hook_rendered(tmp_path):
    manifest, out = run(tmp_path, patches={"hook": [
        {"func": "game_a", "call": "rush_hook_x", "before_vram": None},
        {"func": "memcpy", "call": "rush_hook_y", "before_vram": 0x8000043C}]})
    toml = (out / "rush2049.recomp.toml").read_text()
    assert ('[[patches.hook]]\nfunc = "game_a"\ntext = "{ void rush_hook_x(uint8_t*, recomp_context*); '
            'rush_hook_x(rdram, ctx); }"') in toml
    # a built-in renamed function is hooked under NAME_recomp
    assert '[[patches.hook]]\nfunc = "memcpy_recomp"\nbefore_vram = 0x8000043C\n' in toml
    assert manifest["runtime_contract"]["hooks"] == ["rush_hook_x", "rush_hook_y"]


@pytest.mark.parametrize("hook,msg", [
    ({"func": "nope", "call": "h", "before_vram": None}, "unknown function"),
    ({"func": "osCauseGet", "call": "h", "before_vram": None}, "not emitted"),
    ({"func": "osCreateThread", "call": "h", "before_vram": None}, "not emitted"),
    ({"func": "game_a", "call": "1h", "before_vram": None}, "C identifier"),
    ({"func": "game_a", "call": "h", "before_vram": 0x80086A60}, "not an instruction"),
])
def test_hook_refusals(tmp_path, hook, msg):
    with pytest.raises(gc.GenError, match=msg):
        run(tmp_path, patches={"hook": [hook]})


def test_runtime_contract_shims(tmp_path):
    manifest, _ = run(tmp_path)
    shims = manifest["runtime_contract"]["shims"]
    # reimplemented (wins over ignored): section-table entry + caller;
    # built-in ignored and called: NAME_recomp; ours ignored: plain NAME
    assert shims == {"__osDisableInt_recomp": ["entrypoint"], "osCauseGet": ["entrypoint"],
                     "osCreateThread_recomp": ["entrypoint"]}


def test_images_header(tmp_path):
    manifest, out = run(tmp_path)
    h = (out / "rush2049_images.h").read_text()
    assert '{ "game", 0x00B0CB10, 0x00010000, 0x80086A50, 0x10 },' in h
    assert '{ "ovl_b", 0x00B6FEC4, 0x00030000, 0x8038A400, 0x8 },' in h
    assert "#define RUSH_NUM_IMAGES 3" in h
    assert [i["name"] for i in manifest["runtime_contract"]["images"]] == ["game", "ovl_a", "ovl_b"]


def test_identity_renames(tmp_path):
    ident = [{"vram": 0x80000440, "inventory": "osSetEventMesgAlt", "recomp": "osSetEventMesg"}]
    manifest, out = run(tmp_path, identity=ident)
    syms = (out / "rush2049.syms.toml").read_text()
    assert 'name = "osSetEventMesg", vram = 0x80000440' in syms and "osSetEventMesgAlt" not in syms
    assert manifest["identity_renames"] == [{"vram": "0x80000440", "inventory": "osSetEventMesgAlt",
                                             "recomp": "osSetEventMesg"}]
    # the renamed function is now a reimplemented name: the runtime must define it
    assert "osSetEventMesg_recomp" in manifest["runtime_contract"]["shims"]


@pytest.mark.parametrize("ident,msg", [
    ([{"vram": 0x80000500, "inventory": "x", "recomp": "y"}], "no static function"),
    ([{"vram": 0x80000440, "inventory": "wrong", "recomp": "y"}], "not wrong"),
    ([{"vram": 0x80000440, "inventory": "osSetEventMesgAlt", "recomp": "memcpy"}], "would name both"),
    ([{"vram": 0x80000440, "inventory": "osSetEventMesgAlt", "recomp": "a-b"}], "C identifier"),
])
def test_identity_refusals(tmp_path, ident, msg):
    with pytest.raises(gc.GenError, match=msg):
        run(tmp_path, identity=ident)


def test_committed_identity_and_hooks_well_formed():
    ident = gc.load_identity()
    vrams = [e["vram"] for e in ident]
    assert len(vrams) == len(set(vrams))
    recomp = [e["recomp"] for e in ident]
    assert len(recomp) == len(set(recomp))
    doc = json.loads(gc.IDENTITY.read_text()) if gc.IDENTITY.is_file() else {"static": []}
    for e in doc["static"]:
        assert e.get("evidence") and e.get("confidence") in ("high", "medium", "low")
    for h in gc.load_patches()["hook"]:
        assert gc._C_IDENT.match(h["call"])


# ------------------------------------------------------------ check_contract

def test_check_contract(tmp_path):
    src = tmp_path / "src"
    src.mkdir()
    (src / "a.cpp").write_text(
        'SHIM(foo_recomp) {}\nINERT(bar_recomp)\n// SHIM(commented_out)\n'
        'extern "C" void rush_hook_x(uint8_t* rdram, recomp_context* ctx) {}\n')
    lib = tmp_path / "rt" / "librecomp" / "src"
    lib.mkdir(parents=True)
    (lib / "b.cpp").write_text('extern "C" void osYieldThread_recomp(RDRAM_ARG recomp_context* ctx) {}\n')
    manifest = tmp_path / "manifest.json"
    contract = {"shims": {"foo_recomp": ["x"], "bar_recomp": [], "osYieldThread_recomp": ["y"]},
                "hooks": ["rush_hook_x"]}
    manifest.write_text(json.dumps({"runtime_contract": contract}))
    assert cc.main(["--manifest", str(manifest), "--runtime", str(src),
                    "--n64modernruntime", str(tmp_path / "rt")]) == 0
    assert cc.main(["--manifest", str(manifest), "--runtime", str(src)]) == 1
    contract["shims"]["commented_out"] = []
    manifest.write_text(json.dumps({"runtime_contract": contract}))
    assert cc.main(["--manifest", str(manifest), "--runtime", str(src),
                    "--n64modernruntime", str(tmp_path / "rt")]) == 1


def test_committed_runtime_defines_its_hooks():
    names = cc.defined_names(REPO / "tools" / "recomp" / "runtime" / "src")
    for h in gc.load_patches()["hook"]:
        assert h["call"] in names
    for n in ("__fixdfdi", "__fixsfdi", "osCauseGet", "__osSendInterrupt"):
        assert n in names
