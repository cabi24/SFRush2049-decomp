"""tools/recomp/gen_config.py and check_output.py on small synthetic fixtures.

No ROM data: the fixture "ROM" and images are made-up bytes. The one
end-to-end test needs the private ROM and skips without it.
"""
import hashlib
import importlib.util
import json
import os
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


gc = _load("recomp_gen_config", "tools/recomp/gen_config.py")
co = _load("recomp_check_output", "tools/recomp/check_output.py")

JR_RA, NOP = 0x03E00008, 0x00000000
LISTS = {"reimplemented_funcs": ["osCreateThread"], "ignored_funcs": ["__osDisableInt", "osCreateThread"],
         "renamed_funcs": ["memcpy"]}


def words(*ws):
    return b"".join(struct.pack(">I", w) for w in ws)


def leaf(n_words):
    """n_words of code ending in jr ra; nop."""
    return words(*([0x24020001] * (n_words - 2) + [JR_RA, NOP]))


def fixture():
    """(images, functions, symbols, rom) for a tiny four-population program."""
    static = leaf(4) + leaf(2) + leaf(2) + leaf(4) + b"\0" * 16            # 0x40 code + data
    game = leaf(2) + words(0x8F000000, NOP, JR_RA, NOP) + leaf(4) + b"\0" * 8
    ovl_a = leaf(2) + leaf(4)
    ovl_b = leaf(2) + leaf(2) + b"\0" * 4
    rom = bytearray(0x2000)
    rom[0x1000:0x1000 + len(static)] = static
    images = {"static": (gc.BOOT_VRAM, bytes(static)), "game": (gc.GAME_VRAM, game),
              "ovl_a": (gc.OVL_VRAM, ovl_a), "ovl_b": (gc.OVL_VRAM, ovl_b)}
    functions = {
        "static": [gc._fn("entrypoint", 0x80000400, 16), gc._fn("osCreateThread", 0x80000410, 8),
                   gc._fn("memcpy", 0x80000418, 8), gc._fn("main", 0x80000420, 16)],
        "game": [gc._fn("game_a", 0x80086A50, 8), gc._fn("hud_render", 0x80086A58, 16),
                 gc._fn("game_c", 0x80086A68, 16)],
        "ovl_a": [gc._fn("func_8038A400", 0x8038A400, 8), gc._fn("func_8038A408", 0x8038A408, 16)],
        "ovl_b": [gc._fn("func_8038A400", 0x8038A400, 8), gc._fn("func_8038A408", 0x8038A408, 8)],
    }
    symbols = {
        "static": {"D_80000430": 0x80000430, "main": 0x80000420, "gstate": 0x80086A90},
        "game": {"D_80086A78": 0x80086A78, "gstate": 0x80086A94, "D_8038A410": 0x8038A410},
        "ovl_a": {"D_8038A414": 0x8038A414},
        "ovl_b": {"D_8038A410": 0x8038A410, "D_8038A418": 0x8038A418},
    }
    return images, functions, symbols, bytes(rom)


def run(tmp_path, sub="out", **kw):
    images, functions, symbols, rom = fixture()
    args = dict(lists=LISTS, expected_counts=None, extras=None,
                patches={"stubs": {}, "ignored": {}, "renamed": {"main": "host main"}, "instruction": []})
    args.update(kw)
    out = tmp_path / sub
    return gc.generate(images, functions, symbols, rom, out, **args), out


def tree(path):
    return {str(p.relative_to(path)): p.read_bytes() for p in sorted(path.rglob("*")) if p.is_file()}


def symbol_names(path):
    return re.findall(r'\{ name = "(\w+)", vram', (path / "rush2049.syms.toml").read_text())


# ---------------------------------------------------------------------- ROM gate

def test_missing_rom_refused(tmp_path):
    with pytest.raises(gc.GenError, match="ROM not found"):
        gc.read_rom(tmp_path / "absent.z64", expected_sha1="0" * 40)
    with pytest.raises(gc.GenError, match="ROM not found"):
        gc.read_rom(None, expected_sha1="0" * 40)


def test_wrong_hash_rom_refused(tmp_path):
    rom = tmp_path / "rom.z64"
    rom.write_bytes(b"not the game" * 10)
    with pytest.raises(gc.GenError, match="SHA-1"):
        gc.read_rom(rom, expected_sha1=hashlib.sha1(b"other").hexdigest())
    assert gc.read_rom(rom, expected_sha1=hashlib.sha1(rom.read_bytes()).hexdigest()) == rom.read_bytes()


def test_cli_refuses_wrong_rom_before_writing(tmp_path, capsys):
    rom = tmp_path / "rom.z64"
    rom.write_bytes(b"\0" * 64)
    lists_dir = tmp_path / "n64r" / "src"
    lists_dir.mkdir(parents=True)
    (lists_dir / "symbol_lists.cpp").write_text(SYMBOL_LISTS_CPP)
    out = tmp_path / "out"
    assert gc.main(["--rom", str(rom), "--out", str(out), "--n64recomp-src", str(tmp_path / "n64r")]) == 2
    assert "SHA-1" in capsys.readouterr().err
    assert not out.exists()


def test_committed_rom_hash_is_the_retail_one():
    assert gc.expected_rom_sha1() == "3f99351d7bb61656614bdb2aa1a90cfe55d1922c"


# ------------------------------------------------------------------ generation

def test_deterministic_output(tmp_path):
    m1, out1 = run(tmp_path, "a")
    m2, out2 = run(tmp_path, "b")
    assert m1 == m2
    assert tree(out1) == tree(out2)


def test_every_function_once(tmp_path):
    manifest, out = run(tmp_path)
    images, functions, _, _ = fixture()
    names = symbol_names(out)
    expected = sorted(gc.NAME_PREFIX[p] + f["name"] for p in gc.SECTIONS for f in functions[p])
    assert sorted(names) == expected
    assert len(names) == len(set(names)) == manifest["total_functions"] == 11
    # both images keep their own func_8038A400, under a per-image prefix
    assert "ovl_a_func_8038A400" in names and "ovl_b_func_8038A400" in names


def test_sections_and_synthetic_rom(tmp_path):
    manifest, out = run(tmp_path)
    images, _, _, rom = fixture()
    synthetic = (out / "rom" / "rush2049.recomp.z64").read_bytes()
    assert synthetic[:len(rom)] == rom
    secs = {s["name"]: s for s in manifest["sections"]}
    assert secs[".static"]["rom"] == "0x00001000"
    for pop in ("game", "ovl_a", "ovl_b"):
        rom_off = int(secs["." + pop]["rom"], 16)
        assert rom_off % gc.APPEND_ALIGN == 0 and rom_off >= len(rom)
        data = images[pop][1]
        assert synthetic[rom_off:rom_off + len(data)] == data
        assert (out / "images" / (pop + ".bin")).read_bytes() == data
    assert secs[".ovl_a"]["vram"] == secs[".ovl_b"]["vram"] == "0x8038A400"
    cfg = (out / "rush2049.recomp.toml").read_text()
    assert 'symbols_file_path = "rush2049.syms.toml"' in cfg
    assert "entrypoint = 0x80000400" in cfg
    # sections are not relocatable: no relocs arrays (see README, two images at one vram)
    assert "relocs" not in (out / "rush2049.syms.toml").read_text()


def test_patches_rendered_and_zero_load_rule(tmp_path):
    manifest, out = run(tmp_path, patches={
        "stubs": {"game_c": "x"}, "ignored": {}, "renamed": {"main": "host main"},
        "instruction": [{"func": "game_a", "vram": 0x80086A50, "original": JR_RA,
                         "value": 0, "now": "nop"}]})
    cfg = (out / "rush2049.recomp.toml").read_text()
    assert re.search(r'stubs = \[\s*"game_c",\s*\]', cfg)
    assert re.search(r'renamed = \[\s*"main",\s*\]', cfg)
    # lw $zero in hud_render is replaced automatically
    assert 'func = "hud_render"\nvram = 0x80086A58\nvalue = 0x00000000' in cfg
    assert [e["vram"] for e in manifest["patches"]["instruction"]] == ["0x80086A50", "0x80086A58"]
    builtin = manifest["n64recomp_builtin_lists"]["static"]
    assert builtin == {"reimplemented": ["osCreateThread"], "ignored": [], "renamed": ["memcpy"]}


def test_instruction_patch_checks_original_word(tmp_path):
    with pytest.raises(gc.GenError, match="expects"):
        run(tmp_path, patches={"instruction": [{"func": "game_a", "vram": 0x80086A50,
                                                 "original": 0x12345678, "value": 0, "now": ""}]})
    with pytest.raises(gc.GenError, match="not inside"):
        run(tmp_path, patches={"instruction": [{"func": "game_a", "vram": 0x80086A58,
                                                 "original": 0x8F000000, "value": 0, "now": ""}]})


def test_builtin_names_cannot_be_patched(tmp_path):
    with pytest.raises(gc.GenError, match="built-in"):
        run(tmp_path, patches={"stubs": {"osCreateThread": "x"}})
    with pytest.raises(gc.GenError, match="not a function"):
        run(tmp_path, patches={"ignored": {"nope": "x"}})


def test_inventory_errors(tmp_path):
    images, functions, symbols, rom = fixture()
    bad = json.loads(json.dumps(functions))
    bad["game"][1]["vram"] = 0x80086A54                       # overlaps game_a
    with pytest.raises(gc.GenError, match="overlaps"):
        gc.generate(images, bad, symbols, rom, tmp_path / "o", lists=LISTS, expected_counts=None)
    bad = json.loads(json.dumps(functions))
    bad["ovl_b"][1]["size"] = 0x100                           # runs past image B
    with pytest.raises(gc.GenError, match="outside"):
        gc.generate(images, bad, symbols, rom, tmp_path / "o", lists=LISTS, expected_counts=None)
    bad = json.loads(json.dumps(functions))
    bad["game"][0]["name"] = "entrypoint"                     # same name as a static function
    with pytest.raises(gc.GenError, match="in both"):
        gc.generate(images, bad, symbols, rom, tmp_path / "o", lists=LISTS, expected_counts=None)
    with pytest.raises(gc.GenError, match="expected"):
        gc.generate(images, functions, symbols, rom, tmp_path / "o", lists=LISTS,
                    expected_counts={"game": 4})


def test_extras_merge_after_count_check(tmp_path):
    extras = {"static": [gc._fn("func_80000430", 0x80000430, 8)]}
    manifest, out = run(tmp_path, extras=extras, expected_counts={"static": 4})
    assert "func_80000430" in symbol_names(out)
    assert manifest["inventory_functions"]["static"] == 4
    assert manifest["provisional_functions"] == {"static": ["func_80000430"]}


def test_data_symbols(tmp_path):
    manifest, out = run(tmp_path)
    text = (out / "rush2049.datasyms.toml").read_text()
    assert "main" not in re.findall(r'name = "(\w+)", vram', text)     # a function
    # overlay-range symbols come only from that overlay's own symbols.json
    ovl_a = text.split('name = ".ovl_a"')[1].split("[[section]]")[0]
    ovl_b = text.split('name = ".ovl_b"')[1].split("[[section]]")[0]
    absolute = text.split('name = ".absolute"')[1]
    assert "D_8038A414" in ovl_a and "D_8038A410" not in ovl_a      # game's copy is not A's
    assert "D_8038A410" in ovl_b and "D_8038A418" not in ovl_b      # 0x8038A418 is past image B
    assert "D_8038A418" in absolute and "gstate" in absolute
    assert manifest["data_symbol_conflicts"] == [
        {"dropped": 0x80086A94, "from": "game", "kept": 0x80086A90, "name": "gstate"}]


# ------------------------------------------------------------------- inventories

def test_game_asm_parser(tmp_path):
    asm = tmp_path / "blob"
    asm.mkdir()
    (asm / "blob_80086a50.s").write_text(
        "/* region blob_80086a50: 0x80086A50-0x80086A68 */\n.set noreorder\n\n"
        '.section .text.alpha, "ax", @progbits\n.globl alpha\nalpha:\n    .word 0x1\n    .word 0x2\n\n'
        '.section .blobdata.op_80086a58, "a", @progbits\n    .incbin "build/game_code.bin", 8, 8\n\n'
        '.section .text.beta, "ax", @progbits\n.globl beta\nbeta:\n    .word 0x3, 0x4\n')
    assert gc.load_game_functions(asm) == [gc._fn("alpha", 0x80086A50, 8), gc._fn("beta", 0x80086A60, 8)]
    (asm / "blob_80086a50.s").write_text(
        "/* region blob_80086a50: 0x80086A50-0x80086A60 */\n"
        '.section .text.alpha, "ax", @progbits\n    .word 0x1\n')
    with pytest.raises(gc.GenError, match="region says"):
        gc.load_game_functions(asm)


SYMBOL_LISTS_CPP = """#include "recompiler/context.h"

const std::unordered_set<std::string> N64Recomp::reimplemented_funcs {
    // comment
    "osCreateThread",
};

const std::unordered_set<std::string> N64Recomp::ignored_funcs {
    "osCreateThread",
    "__osDisableInt",
};

const std::unordered_set<std::string> N64Recomp::renamed_funcs {
    "memcpy",
};
"""


def test_builtin_lists_parser(tmp_path):
    (tmp_path / "src").mkdir()
    (tmp_path / "src" / "symbol_lists.cpp").write_text(SYMBOL_LISTS_CPP)
    assert gc.builtin_lists(tmp_path) == LISTS
    with pytest.raises(gc.GenError, match="cannot read"):
        gc.builtin_lists(tmp_path / "absent")


def test_committed_patch_inputs_are_well_formed():
    patches = gc.load_patches()
    extras = gc.load_extra_functions()
    names = [e["name"] for e in extras["static"]]
    assert len(names) == len(set(names))
    for e in extras["static"]:
        assert e["vram"] % 4 == 0 and e["size"] % 4 == 0 and e["evidence"]
    assert not set(patches["stubs"]) & set(patches["ignored"])
    for e in patches["instruction"]:
        assert e["vram"] % 4 == 0


def test_site_arg_hooks(tmp_path):
    """Phase 5: `"site_arg": true` passes before_vram as a third argument; the
    committed busy-wait hooks (rush_hook_spin) all use it, one per site."""
    p = tmp_path / "patches.json"
    p.write_text(json.dumps({"hook": [
        {"func": "f", "before_vram": "0x800FD500", "call": "rush_hook_spin", "site_arg": True},
        {"func": "g", "call": "plain_hook"}]}))
    spin, plain = gc.load_patches(p)["hook"]
    assert spin["text"] == ("{ void rush_hook_spin(uint8_t*, recomp_context*, uint32_t); "
                            "rush_hook_spin(rdram, ctx, 0x800FD500U); }")
    assert spin["contract"] == ["rush_hook_spin"] and "text" not in plain
    p.write_text(json.dumps({"hook": [{"func": "f", "call": "h", "site_arg": True}]}))
    with pytest.raises(gc.GenError):
        gc.load_patches(p)
    spins = [e for e in gc.load_patches()["hook"] if e["call"] == "rush_hook_spin"]
    assert len(spins) == 14 and all("text" in e for e in spins)
    sites = [(e["func"], e["before_vram"]) for e in spins]
    assert len(set(sites)) == len(sites) and ("game_loop", 0x800FD500) in sites


# --------------------------------------------------------------- check_output

def test_check_output_summary(tmp_path):
    _, out = run(tmp_path)
    funcs = out / "RecompiledFuncs"
    funcs.mkdir()
    (funcs / "funcs.h").write_text("void osCreateThread_recomp(uint8_t* rdram, recomp_context* ctx);\n")
    body = ("RECOMP_FUNC void {0}(uint8_t* rdram, recomp_context* ctx) {{\n"
            "    {1}(rdram, ctx);\n;}}\n")
    (funcs / "funcs_0.c").write_text(
        body.format("recomp_entrypoint", "osCreateThread_recomp")
        + body.format("memcpy_recomp", "__osDisableInt_recomp")
        + body.format("main_recomp", "game_a")
        + "".join(body.format(n, "game_a") for n in
                  ("game_a", "hud_render", "game_c", "ovl_a_func_8038A400", "ovl_a_func_8038A408",
                   "ovl_b_func_8038A400", "ovl_b_func_8038A408", "static_1_80086A70")))
    report = co.summarise(out)
    assert report["sections"][".static"] == {"functions": 4, "emitted": 3, "not_emitted": ["osCreateThread"]}
    assert all(report["sections"][s]["emitted"] == report["sections"][s]["functions"]
               for s in (".game", ".ovl_a", ".ovl_b"))
    assert report["statics"] == ["static_1_80086A70"]
    assert report["called_not_defined"] == {
        "__osDisableInt_recomp": {"declared_in_funcs_h": False, "callers": 1},
        "osCreateThread_recomp": {"declared_in_funcs_h": True, "callers": 1}}


# ------------------------------------------------------------------ end to end

def _private_rom():
    for path in (os.environ.get("RUSH2049_ROM"), REPO / "baserom.us.z64",
                 REPO.parents[2] / "baserom.us.z64" if len(REPO.parents) > 2 else None):
        if path and Path(path).is_file():
            return Path(path)
    return None


@pytest.mark.skipif(_private_rom() is None, reason="private input: baserom.us.z64 absent "
                    "(set RUSH2049_ROM to run the end-to-end generation)")
def test_end_to_end_with_rom(tmp_path):
    lists = tmp_path / "n64r" / "src"
    lists.mkdir(parents=True)
    (lists / "symbol_lists.cpp").write_text(SYMBOL_LISTS_CPP)
    out = tmp_path / "recomp"
    assert gc.main(["--rom", str(_private_rom()), "--out", str(out),
                    "--n64recomp-src", str(tmp_path / "n64r")]) == 0
    manifest = json.loads((out / "manifest.json").read_text())
    assert manifest["inventory_functions"] == gc.EXPECTED_COUNTS
    names = symbol_names(out)
    assert len(names) == len(set(names)) == manifest["total_functions"]
    first = tree(out)
    assert gc.main(["--rom", str(_private_rom()), "--out", str(out),
                    "--n64recomp-src", str(tmp_path / "n64r")]) == 0
    assert tree(out) == first
