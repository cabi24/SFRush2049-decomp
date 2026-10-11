#!/usr/bin/env python3
"""Generate an N64Recomp configuration for Rush 2049 (recomp Phase 0).

    python3 tools/recomp/gen_config.py --rom PATH [--out build/recomp]
                                       [--n64recomp-src DIR]

Inputs (committed): the four code populations' inventories
  static  668 functions  layout map (tools.conveyor.pipeline.layout.derive:
                         splat.us.yaml + symbol_addrs.us.txt + asm/us/*.s +
                         asm/us/boot_tail/extents.json), boot segment
                         ROM 0x1000-0x2F4E0 -> 0x80000400
  game   1216 functions  asm/us/blob/*.s (one `.section .text.NAME` per
                         function; region headers give the addresses),
                         image inflated from ROM 0xB0CB10 -> 0x80086A50
  ovl_a   193 functions  asm/us/ovl_a/extents.json, image A -> 0x8038A400
  ovl_b    49 functions  asm/us/ovl_b/extents.json, image B -> 0x8038A400
plus the data symbols in each population's symbols.json, and the ROM
(path argument; refused unless its SHA-1 is the one in us.sha1).

Outputs (all under --out, git-ignored, PRIVATE: derived from the ROM):
  images/static.bin, images/game.bin, images/ovl_a.bin, images/ovl_b.bin
  rom/rush2049.recomp.z64   synthetic ROM = retail ROM + the three inflated
                            images appended at fixed aligned offsets (the
                            adapter: N64Recomp's symbol-file mode reads every
                            function's words at section.rom + offset of ONE
                            ROM file, so the inflated images need ROM offsets)
  rush2049.syms.toml        N64Recomp symbol file: 4 sections, every function
  rush2049.datasyms.toml    data symbols in N64Recomp's data-reference format
                            (documentation/Phase 1 input; the symbol-file mode
                            does not read data symbols)
  rush2049.recomp.toml      the recompiler configuration
  manifest.json             counts, hashes, layout, name map, builtin-list hits

The output is a pure function of the inputs (no timestamps, sorted keys).
See tools/recomp/README.md for the format notes and the design decisions.
Stdlib only, Python 3.9+.
"""
import argparse
import glob
import hashlib
import json
import os
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
if str(REPO) not in sys.path:
    sys.path.insert(0, str(REPO))

ROM_SHA1_FILE = REPO / "us.sha1"
DEFAULT_OUT = REPO / "build" / "recomp"

# Facts (each one is checked against the committed inventories or the ROM).
BOOT_ROM = (0x1000, 0x2F4E0)          # boot segment, = asm/us/boot_tail rom_range
BOOT_VRAM = 0x80000400
GAME_VRAM = 0x80086A50
OVL_VRAM = 0x8038A400
ENTRYPOINT = 0x80000400                # ROM header entry; the function at ROM 0x1000
APPEND_ALIGN = 0x10000                 # images appended to the synthetic ROM here

# Section names in the symbol file (N64Recomp trims leading dots for C names).
SECTIONS = ("static", "game", "ovl_a", "ovl_b")
# Overlay functions share address-derived names (both images start with
# func_8038A400): the recompiler emits one C function per name, so overlay
# functions get a per-image prefix. Static and game names are kept.
NAME_PREFIX = {"static": "", "game": "", "ovl_a": "ovl_a_", "ovl_b": "ovl_b_"}

EXPECTED_COUNTS = {"static": 668, "game": 1216, "ovl_a": 193, "ovl_b": 49}


class GenError(RuntimeError):
    """The configuration cannot be produced without guessing."""


# --------------------------------------------------------------------------- ROM

def expected_rom_sha1(path=ROM_SHA1_FILE):
    return Path(path).read_text().split()[0].strip().lower()


def read_rom(path, expected_sha1=None):
    """The ROM bytes; refuses a missing file or a wrong SHA-1."""
    path = Path(path) if path else None
    if path is None or not path.is_file():
        raise GenError(f"ROM not found: {path} (the ROM is private and never committed; "
                       "pass --rom /path/to/baserom.us.z64)")
    data = path.read_bytes()
    want = (expected_sha1 or expected_rom_sha1()).lower()
    got = hashlib.sha1(data).hexdigest()
    if got != want:
        raise GenError(f"ROM {path} has SHA-1 {got}, expected {want} (us.sha1); refusing")
    return data


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def extract_images(rom, repo=REPO):
    """{population: (vram, bytes)} from the retail ROM, each hash-checked
    against the committed inventories. Inflation reuses the pipeline."""
    from tools.conveyor.pipeline import blob_rom, ovl_image

    images = {}
    boot_ext = json.loads((repo / "asm/us/boot_tail/extents.json").read_text())
    lo, hi = (int(x, 16) for x in boot_ext["rom_range"].split("-"))
    if (lo, hi) != BOOT_ROM or int(boot_ext["base"], 16) != BOOT_VRAM:
        raise GenError(f"boot segment inventory {boot_ext['rom_range']} @ {boot_ext['base']} "
                       f"is not ROM 0x{BOOT_ROM[0]:X}-0x{BOOT_ROM[1]:X} @ 0x{BOOT_VRAM:08X}")
    boot = rom[lo:hi]
    if sha256(boot) != boot_ext["image_sha256"]:
        raise GenError("boot segment bytes do not match asm/us/boot_tail image_sha256")
    images["static"] = (BOOT_VRAM, boot)

    game, consumed = ovl_image.inflate_stream(rom, blob_rom.ROM_OFFSET)
    game_sha = json.loads((repo / "asm/us/blob/symbols.json").read_text())["image_sha256"]
    if consumed != blob_rom.LENGTH or sha256(game) != game_sha:
        raise GenError("game image inflated from ROM 0x%X is not the inventoried image "
                       "(%d B consumed, sha256 %s)" % (blob_rom.ROM_OFFSET, consumed,
                                                       sha256(game)[:12]))
    if blob_rom.targetsmod.GAME_CODE_BASE != GAME_VRAM:
        raise GenError("pipeline game base disagrees with GAME_VRAM")
    images["game"] = (GAME_VRAM, game)

    for key in ("a", "b"):
        data = ovl_image.inflate(key, rom)           # checks length and sha256
        if ovl_image.BASE != OVL_VRAM:
            raise GenError("pipeline overlay base disagrees with OVL_VRAM")
        images["ovl_" + key] = (OVL_VRAM, data)
    return images


# -------------------------------------------------------------------- inventories

def _fn(name, vram, size):
    return {"name": name, "vram": int(vram), "size": int(size)}


def load_static_functions(rom_path, game_image_path, repo=REPO):
    """The 668 static functions from the layout map. layout.derive reads ROM
    bytes through targets.BASEROM / targets.GAME_CODE_BIN; both are pointed at
    the verified inputs for the call and restored afterwards."""
    from tools.conveyor.pipeline import layout
    from tools.conveyor.pipeline import targets as targetsmod

    saved = (targetsmod.BASEROM, targetsmod.GAME_CODE_BIN)
    targetsmod.BASEROM, targetsmod.GAME_CODE_BIN = Path(rom_path), Path(game_image_path)
    targetsmod._image_cache.clear()
    try:
        mapping = layout.derive()
    finally:
        targetsmod.BASEROM, targetsmod.GAME_CODE_BIN = saved
        targetsmod._image_cache.clear()
    out = []
    for seg in mapping["segments"]:
        if seg["refusal"]:
            if seg["yaml_name"] == "dynamic":
                continue
            raise GenError(f"static segment {seg['yaml_name']} refused: {seg['refusal']}")
        for f in seg["functions"]:
            out.append(_fn(f["name"], int(f["vaddr"], 16), f["size"]))
    return out


_REGION_RE = re.compile(r"/\* region (\S+): 0x([0-9A-Fa-f]+)-0x([0-9A-Fa-f]+) \*/")
_SECTION_RE = re.compile(r"\.section \.(text|blobdata)\.([A-Za-z0-9_.$]+?),")
_INCBIN_RE = re.compile(r'\.incbin "[^"]+", (\d+), (\d+)')


def load_game_functions(asm_dir, base=GAME_VRAM):
    """Functions from the committed game-image assembly: each region file is a
    tiling of `.text.NAME` (function) and `.blobdata.*` (opaque) sections."""
    out = []
    paths = sorted(glob.glob(os.path.join(str(asm_dir), "*.s")))
    if not paths:
        raise GenError(f"no game assembly in {asm_dir}")
    for path in paths:
        addr = region_end = None
        current = None
        for line in Path(path).read_text().splitlines():
            stripped = line.strip()
            m = _REGION_RE.match(stripped)
            if m:
                addr, region_end = int(m.group(2), 16), int(m.group(3), 16)
                continue
            m = _SECTION_RE.match(stripped)
            if m:
                if addr is None:
                    raise GenError(f"{path}: section before region header")
                if current is not None:
                    out.append(current)
                current = (_fn(m.group(2), addr, 0) if m.group(1) == "text"
                           else {"opaque": True, "size": 0})
                continue
            if stripped.startswith(".word"):
                current["size"] += 4 * len(stripped[5:].split(","))
                addr += 4 * len(stripped[5:].split(","))
                continue
            m = _INCBIN_RE.match(stripped)
            if m:
                if base + int(m.group(1)) != addr:
                    raise GenError(f"{path}: .incbin offset {m.group(1)} is not at 0x{addr:08X}")
                current["size"] += int(m.group(2))
                addr += int(m.group(2))
        if current is not None:
            out.append(current)
        if addr != region_end:
            raise GenError(f"{path}: sections end at 0x{addr:08X}, region says 0x{region_end:08X}")
    return [f for f in out if not f.get("opaque")]


def load_extents_functions(extents_path):
    doc = json.loads(Path(extents_path).read_text())
    return [_fn(f["name"], int(f["address"], 16), f["size"]) for f in doc["functions"]]


def load_symbols(symbols_path):
    doc = json.loads(Path(symbols_path).read_text())
    return {name: int(addr, 16) for name, addr in doc["symbols"].items()}


def load_inventories(rom_path, game_image_path, repo=REPO):
    """({population: [function]}, {population: {symbol: vram}})."""
    functions = {
        "static": load_static_functions(rom_path, game_image_path, repo),
        "game": load_game_functions(repo / "asm/us/blob"),
        "ovl_a": load_extents_functions(repo / "asm/us/ovl_a/extents.json"),
        "ovl_b": load_extents_functions(repo / "asm/us/ovl_b/extents.json"),
    }
    symbols = {
        "static": load_symbols(repo / "asm/us/boot_tail/symbols.json"),
        "game": load_symbols(repo / "asm/us/blob/symbols.json"),
        "ovl_a": load_symbols(repo / "asm/us/ovl_a/symbols.json"),
        "ovl_b": load_symbols(repo / "asm/us/ovl_b/symbols.json"),
    }
    return functions, symbols


# ---------------------------------------------------------------- N64Recomp lists

def builtin_lists(n64recomp_src):
    """The name sets N64Recomp applies by name in symbol-file mode
    (src/symbol_lists.cpp): reimplemented_funcs, ignored_funcs, renamed_funcs."""
    path = Path(n64recomp_src) / "src" / "symbol_lists.cpp"
    try:
        text = path.read_text()
    except OSError as exc:
        raise GenError(f"cannot read {path}: {exc}") from exc
    out = {}
    for key in ("reimplemented_funcs", "ignored_funcs", "renamed_funcs"):
        m = re.search(r"N64Recomp::" + key + r"\s*\{(.*?)\n\};", text, re.S)
        if not m:
            raise GenError(f"{key} not found in symbol_lists.cpp")
        out[key] = sorted(set(re.findall(r'"([^"]+)"', m.group(1))))
    return out


def classify_builtin(names, lists):
    """How N64Recomp's main.cpp treats each name (same precedence)."""
    reimpl = set(lists["reimplemented_funcs"])
    ignored = set(lists["ignored_funcs"])
    renamed = set(lists["renamed_funcs"])
    out = {"reimplemented": [], "ignored": [], "renamed": []}
    for name in names:
        if name in reimpl:
            out["reimplemented"].append(name)
        elif name in ignored:
            out["ignored"].append(name)
        elif name in renamed:
            out["renamed"].append(name)
    return {k: sorted(v) for k, v in out.items()}


# ---------------------------------------------------------------------- generate

def _align(value, alignment):
    return (value + alignment - 1) // alignment * alignment


def plan_layout(rom_size, images):
    """Section table: static keeps its retail ROM offset; the inflated images
    are appended to the synthetic ROM at aligned offsets in SECTIONS order."""
    sections = []
    cursor = _align(rom_size, APPEND_ALIGN)
    for pop in SECTIONS:
        vram, data = images[pop]
        if pop == "static":
            rom = BOOT_ROM[0]
        else:
            rom = cursor
            cursor = _align(cursor + len(data), APPEND_ALIGN)
        sections.append({"name": pop, "rom": rom, "vram": vram, "size": len(data)})
    return sections, cursor


def check_functions(functions, sections, expected_counts=None):
    """Every function inside its section, word aligned, no overlaps, names
    unique across the whole run after prefixing. Returns the recomp-name map."""
    by_name = {}
    for sec in sections:
        pop = sec["name"]
        funcs = sorted(functions[pop], key=lambda f: (f["vram"], f["name"]))
        if expected_counts and pop in expected_counts and len(funcs) != expected_counts[pop]:
            raise GenError(f"{pop}: {len(funcs)} functions, expected {expected_counts[pop]}")
        end_prev = None
        for f in funcs:
            if f["vram"] % 4 or f["size"] % 4 or f["size"] <= 0:
                raise GenError(f"{pop}:{f['name']} misaligned or empty "
                               f"(0x{f['vram']:08X}+{f['size']})")
            if f["vram"] < sec["vram"] or f["vram"] + f["size"] > sec["vram"] + sec["size"]:
                raise GenError(f"{pop}:{f['name']} 0x{f['vram']:08X}+{f['size']} outside its section")
            if end_prev is not None and f["vram"] < end_prev:
                raise GenError(f"{pop}:{f['name']} overlaps the previous function")
            end_prev = f["vram"] + f["size"]
            recomp_name = NAME_PREFIX[pop] + f["name"]
            if not re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", recomp_name):
                raise GenError(f"{pop}:{f['name']} is not a C identifier")
            if recomp_name in by_name:
                raise GenError(f"function name {recomp_name} in both {by_name[recomp_name][0]} and {pop}")
            by_name[recomp_name] = (pop, f["name"])
    return by_name


def data_symbols(functions, symbols, sections):
    """Data symbols per section (or absolute), deduplicated by name.

    A symbol inside an overlay's vram range is taken only from that overlay's
    own symbols.json (both images share the range); others from the union.
    Function names/addresses are excluded. Conflicts are reported, first
    population (SECTIONS order) wins."""
    func_names = {f["name"] for fl in functions.values() for f in fl}
    func_addrs = {(pop, f["vram"]) for pop, fl in functions.items() for f in fl}
    ovl_lo = OVL_VRAM
    ovl_hi = {s["name"]: s["vram"] + s["size"] for s in sections if s["name"].startswith("ovl_")}
    placed = {s["name"]: {} for s in sections}
    placed["absolute"] = {}
    seen = {}
    conflicts = []
    for pop in SECTIONS:
        for name, vram in sorted(symbols.get(pop, {}).items()):
            if name in func_names:
                continue
            in_ovl = ovl_lo <= vram < max(ovl_hi.values() or [ovl_lo])
            if in_ovl:
                if not pop.startswith("ovl_") or vram >= ovl_hi[pop]:
                    continue                     # another image's range; skip
                target = pop
                key = (pop, name)
            else:
                target = None
                for sec in sections:
                    if sec["name"] in ("static", "game") and sec["vram"] <= vram < sec["vram"] + sec["size"]:
                        target = sec["name"]
                target = target or "absolute"
                key = ("*", name)
            if (target, vram) in func_addrs:
                continue
            if key in seen:
                if seen[key] != vram:
                    conflicts.append({"name": name, "kept": seen[key], "dropped": vram, "from": pop})
                continue
            seen[key] = vram
            placed[target][name] = vram
    return placed, conflicts


def _hex(v):
    return "0x%08X" % v


def _toml_str(s):
    return '"' + s.replace("\\", "\\\\").replace('"', '\\"') + '"'


def render_symbols_toml(sections, functions):
    lines = ["# N64Recomp symbol file for Rush 2049 (US). Generated by",
             "# tools/recomp/gen_config.py; private (ROM-derived offsets), do not commit.", ""]
    for sec in sections:
        pop = sec["name"]
        lines += ["[[section]]",
                  "name = %s" % _toml_str("." + pop),
                  "rom = %s" % _hex(sec["rom"]),
                  "vram = %s" % _hex(sec["vram"]),
                  "size = 0x%X" % sec["size"],
                  "functions = ["]
        for f in sorted(functions[pop], key=lambda f: (f["vram"], f["name"])):
            lines.append("    { name = %s, vram = %s, size = 0x%X },"
                         % (_toml_str(NAME_PREFIX[pop] + f["name"]), _hex(f["vram"]), f["size"]))
        lines += ["]", ""]
    return "\n".join(lines)


def render_datasyms_toml(sections, placed):
    lines = ["# Data symbols (N64Recomp data-reference-symbol format). Generated by",
             "# tools/recomp/gen_config.py. Not read by the symbol-file mode; kept for",
             "# Phase 1 (runtime debugging) and Phase 3 (capture/diff naming).", ""]
    for sec in sections + [{"name": "absolute"}]:
        syms = placed.get(sec["name"], {})
        if not syms:
            continue
        lines.append("[[section]]")
        lines.append("name = %s" % _toml_str("." + sec["name"]))
        if sec["name"] != "absolute":
            lines += ["rom = %s" % _hex(sec["rom"]), "vram = %s" % _hex(sec["vram"]),
                      "size = 0x%X" % sec["size"]]
        else:
            lines += ["vram = 0x00000000", "size = 0x0"]
        lines.append("symbols = [")
        for name, vram in sorted(syms.items(), key=lambda kv: (kv[1], kv[0])):
            lines.append("    { name = %s, vram = %s }," % (_toml_str(name), _hex(vram)))
        lines += ["]", ""]
    return "\n".join(lines)


def render_config_toml(rom_rel, syms_rel, output_rel="RecompiledFuncs"):
    return "\n".join([
        "# N64Recomp configuration for Rush 2049 (US). Generated by",
        "# tools/recomp/gen_config.py. Paths are relative to this file.",
        "",
        "[input]",
        "entrypoint = %s" % _hex(ENTRYPOINT),
        "output_func_path = %s" % _toml_str(output_rel),
        "symbols_file_path = %s" % _toml_str(syms_rel),
        "rom_file_path = %s" % _toml_str(rom_rel),
        "# one .c file per function (emitted-function counts are file counts)",
        "functions_per_output_file = 1",
        "",
        "[patches]",
        "# N64Recomp applies its own reimplemented/ignored/renamed lists by name in",
        "# symbol-file mode; listing those names here would fail (they are renamed",
        "# to NAME_recomp before [patches] is applied).",
        "stubs = []",
        "ignored = []",
        "",
    ])


def generate(images, functions, symbols, rom_bytes, out_dir, lists=None,
             expected_counts=EXPECTED_COUNTS, rom_sha1=None):
    """Write every output under out_dir. `images` is {pop: (vram, bytes)}.
    Pure function of its arguments; returns the manifest dict."""
    out_dir = Path(out_dir)
    for pop in SECTIONS:
        if pop not in images:
            raise GenError(f"missing image for {pop}")
    sections, synthetic_size = plan_layout(len(rom_bytes), images)
    names = check_functions(functions, sections, expected_counts)
    placed, conflicts = data_symbols(functions, symbols, sections)

    synthetic = bytearray(rom_bytes)
    synthetic.extend(b"\0" * (synthetic_size - len(synthetic)))
    for sec in sections:
        data = images[sec["name"]][1]
        if sec["name"] == "static":
            if bytes(synthetic[sec["rom"]:sec["rom"] + sec["size"]]) != data:
                raise GenError("static image is not the ROM at its offset")
            continue
        synthetic[sec["rom"]:sec["rom"] + sec["size"]] = data

    (out_dir / "images").mkdir(parents=True, exist_ok=True)
    (out_dir / "rom").mkdir(parents=True, exist_ok=True)
    for pop in SECTIONS:
        (out_dir / "images" / (pop + ".bin")).write_bytes(images[pop][1])
    rom_name = "rush2049.recomp.z64"
    (out_dir / "rom" / rom_name).write_bytes(bytes(synthetic))
    (out_dir / "rush2049.syms.toml").write_text(render_symbols_toml(sections, functions))
    (out_dir / "rush2049.datasyms.toml").write_text(render_datasyms_toml(sections, placed))
    (out_dir / "rush2049.recomp.toml").write_text(
        render_config_toml("rom/" + rom_name, "rush2049.syms.toml"))

    manifest = {
        "generator": "tools/recomp/gen_config.py",
        "rom_sha1": rom_sha1 or hashlib.sha1(rom_bytes).hexdigest(),
        "synthetic_rom": {"path": "rom/" + rom_name, "size": len(synthetic),
                          "sha256": sha256(bytes(synthetic))},
        "entrypoint": _hex(ENTRYPOINT),
        "sections": [{
            "name": "." + s["name"], "rom": _hex(s["rom"]), "vram": _hex(s["vram"]),
            "size": s["size"], "image_sha256": sha256(images[s["name"]][1]),
            "functions": len(functions[s["name"]]),
            "function_bytes": sum(f["size"] for f in functions[s["name"]]),
            "data_symbols": len(placed[s["name"]]),
            "name_prefix": NAME_PREFIX[s["name"]],
        } for s in sections],
        "absolute_data_symbols": len(placed["absolute"]),
        "data_symbol_conflicts": conflicts,
        "total_functions": len(names),
    }
    if lists is not None:
        manifest["n64recomp_builtin_lists"] = {
            pop: classify_builtin([NAME_PREFIX[pop] + f["name"] for f in functions[pop]], lists)
            for pop in SECTIONS}
    (out_dir / "manifest.json").write_text(json.dumps(manifest, indent=1, sort_keys=True) + "\n")
    return manifest


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--rom", required=True, help="retail US ROM (z64, big-endian)")
    parser.add_argument("--out", default=str(DEFAULT_OUT), help="output dir (default build/recomp)")
    parser.add_argument("--n64recomp-src", help="N64Recomp checkout: report which functions "
                        "its built-in lists reimplement/ignore/rename (manifest.json)")
    args = parser.parse_args(argv)
    try:
        lists = builtin_lists(args.n64recomp_src) if args.n64recomp_src else None
        rom = read_rom(args.rom)
        images = extract_images(rom)
        out = Path(args.out)
        (out / "images").mkdir(parents=True, exist_ok=True)
        game_path = out / "images" / "game.bin"
        game_path.write_bytes(images["game"][1])          # layout.derive reads it
        functions, symbols = load_inventories(args.rom, game_path)
        manifest = generate(images, functions, symbols, rom, out, lists=lists)
    except GenError as exc:
        print(f"gen_config: {exc}", file=sys.stderr)
        return 2
    for s in manifest["sections"]:
        print(f"{s['name']:7s} rom {s['rom']} vram {s['vram']} size {s['size']:7d}  "
              f"{s['functions']:5d} functions  {s['data_symbols']:5d} data symbols")
    print(f"total {manifest['total_functions']} functions -> {Path(args.out) / 'rush2049.recomp.toml'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
