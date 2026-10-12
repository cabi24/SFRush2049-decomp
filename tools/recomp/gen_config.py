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
  manifest.json             counts, hashes, layout, name map, builtin-list hits,
                            runtime_contract (shims, hooks, images)
  rush2049_images.h         stream -> section table for the runtime inflate hook
  rush2049_repl.h           replaced functions (Phase 2; empty table without
                            --replace-set/--replace) and the .repl image location

Phase 2 (function replacement, tools/recomp/replace.py):
  --replace-set SET / --replace ID,...  build the replacements.json entries
  into a fifth section `.repl` (IDO-compiled bodies) or host wrappers
  (native), each delegated to by a hook at the original function's entry.

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
NAME_PREFIX = {"static": "", "game": "", "ovl_a": "ovl_a_", "ovl_b": "ovl_b_",
               # Phase 2 replacement image (tools/recomp/replace.py): names are final
               "repl": ""}

EXTRA_FUNCTIONS = REPO / "tools" / "recomp" / "extra_functions.json"
IDENTITY = REPO / "tools" / "recomp" / "identity.json"
PATCHES = REPO / "tools" / "recomp" / "patches.json"

EXPECTED_COUNTS = {"static": 668, "game": 1216, "ovl_a": 193, "ovl_b": 49}


_C_IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


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


def stream_offsets():
    """{population: ROM offset of its compressed stream} (game, ovl_a, ovl_b)."""
    from tools.conveyor.pipeline import blob_rom, ovl_image
    return {"game": blob_rom.ROM_OFFSET, "ovl_a": ovl_image.STREAMS["a"].rom_offset,
            "ovl_b": ovl_image.STREAMS["b"].rom_offset}


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


def _toml_list(names):
    if not names:
        return "[]"
    return "[\n" + "".join("    %s,\n" % _toml_str(n) for n in names) + "]"


def hook_text(call):
    """The C statement a [[patches.hook]] inserts: a block-scope prototype and
    the call (the emitted files are C; the runtime defines `call` extern "C")."""
    return "{ void %s(uint8_t*, recomp_context*); %s(rdram, ctx); }" % (call, call)


def entry_hook_text(e):
    return e.get("text") or hook_text(e["call"])


def render_config_toml(rom_rel, syms_rel, stubs=(), ignored=(), renamed=(), instruction=(),
                       output_rel="RecompiledFuncs", hooks=()):
    patches = []
    for e in sorted(hooks, key=lambda e: (e["func"], e["before_vram"] or 0)):
        patches += ["", "[[patches.hook]]", "func = %s" % _toml_str(e["func"])]
        if e["before_vram"] is not None:
            patches.append("before_vram = %s" % _hex(e["before_vram"]))
        patches.append("text = %s" % _toml_str(entry_hook_text(e)))
    for e in sorted(instruction, key=lambda e: e["vram"]):
        patches += ["", "[[patches.instruction]]",
                    "func = %s" % _toml_str(e["func"]),
                    "vram = %s" % _hex(e["vram"]),
                    "value = %s # was 0x%08X; %s" % (_hex(e["value"]), e["original"], e["now"])]
    return "\n".join([
        "# N64Recomp configuration for Rush 2049 (US). Generated by",
        "# tools/recomp/gen_config.py. Paths are relative to this file.",
        "",
        "[input]",
        "entrypoint = %s" % _hex(ENTRYPOINT),
        "output_func_path = %s" % _toml_str(output_rel),
        "symbols_file_path = %s" % _toml_str(syms_rel),
        "rom_file_path = %s" % _toml_str(rom_rel),
        "# 50 per file (upstream default; the one-per-file mode omits funcs.h)",
        "functions_per_output_file = 50",
        "",
        "[patches]",
        "# N64Recomp applies its own reimplemented/ignored/renamed lists by name in",
        "# symbol-file mode; listing those names here would fail (they are renamed",
        "# to NAME_recomp before [patches] is applied).",
        "# Ours: tools/recomp/patches.json (with the reason for each).",
        "stubs = %s" % _toml_list(sorted(stubs)),
        "ignored = %s" % _toml_list(sorted(ignored)),
        "renamed = %s" % _toml_list(sorted(renamed)),
    ] + patches + [""])


def load_extra_functions(path=EXTRA_FUNCTIONS):
    """{population: [function]} from the committed provisional-extent list."""
    path = Path(path)
    if not path.is_file():
        return {}
    doc = json.loads(path.read_text())
    out = {}
    for pop in SECTIONS:
        out[pop] = [dict(_fn(e["name"], int(e["vram"], 16), int(e["size"], 16)),
                         evidence=e.get("evidence", ""))
                    for e in doc.get(pop, [])]
    return out


def load_identity(path=IDENTITY):
    """[{vram, inventory, recomp}] from the committed recompiler-name map
    (static libultra functions whose inventory name is not their identity)."""
    path = Path(path)
    if not path.is_file():
        return []
    doc = json.loads(path.read_text())
    return [{"vram": int(e["vram"], 16), "inventory": e["inventory"], "recomp": e["recomp"]}
            for e in doc.get("static", [])]


def apply_identity(functions, identity):
    """Rename static functions for the recompiler only (N64Recomp replaces
    libultra functions BY NAME, so a wrong inventory name either replaces the
    wrong function with a host version or hides a real libultra function from
    the replacement). Each entry must name the function at its vram exactly;
    the renamed set must stay unique. Returns (functions, applied)."""
    if not identity:
        return functions, []
    by_vram = {f["vram"]: f for f in functions["static"]}
    renames = {}
    for e in identity:
        f = by_vram.get(e["vram"])
        if f is None:
            raise GenError(f"identity.json: no static function at 0x{e['vram']:08X}")
        if f["name"] != e["inventory"]:
            raise GenError(f"identity.json: 0x{e['vram']:08X} is {f['name']}, not {e['inventory']}")
        if not _C_IDENT.match(e["recomp"]):
            raise GenError(f"identity.json: {e['recomp']!r} is not a C identifier")
        if e["vram"] in renames:
            raise GenError(f"identity.json: 0x{e['vram']:08X} listed twice")
        renames[e["vram"]] = e["recomp"]
    static = [dict(f, name=renames.get(f["vram"], f["name"])) for f in functions["static"]]
    seen = {}
    for f in static:
        if f["name"] in seen:
            raise GenError(f"identity.json: {f['name']} would name both 0x{seen[f['name']]:08X} "
                           f"and 0x{f['vram']:08X} (rename the displaced function too)")
        seen[f["name"]] = f["vram"]
    out = dict(functions)
    out["static"] = static
    applied = [{"vram": _hex(e["vram"]), "inventory": e["inventory"], "recomp": e["recomp"]}
               for e in sorted(identity, key=lambda e: e["vram"])]
    return out, applied


def load_patches(path=PATCHES):
    path = Path(path)
    if not path.is_file():
        return {"stubs": {}, "ignored": {}, "renamed": {}, "instruction": [], "hook": []}
    doc = json.loads(path.read_text())
    return {"stubs": dict(doc.get("stubs", {})), "ignored": dict(doc.get("ignored", {})),
            "renamed": dict(doc.get("renamed", {})),
            "hook": [{"func": e["func"], "call": e["call"],
                      "before_vram": int(e["before_vram"], 16) if e.get("before_vram") else None}
                     for e in doc.get("hook", [])],
            "instruction": [{"func": e["func"], "vram": int(e["vram"], 16),
                             "original": int(e["original"], 16), "value": int(e["value"], 16),
                             "now": e.get("now", "")}
                            for e in doc.get("instruction", [])]}


# Loads (lb lh lwl lw lbu lhu lwr lwu ld ldl ldr) whose destination is $zero:
# N64Recomp 1.x emits `0 = MEM_W(...)`, which does not compile. The load has
# no architectural effect, so it is replaced by a nop.
_LOAD_OPCODES = frozenset((0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x37, 0x1A, 0x1B))


def zero_load_patches(functions, images, skip=()):
    out = []
    for pop in SECTIONS:
        vram, data = images[pop]
        for f in sorted(functions[pop], key=lambda f: f["vram"]):
            name = NAME_PREFIX[pop] + f["name"]
            if name in skip:
                continue
            for addr in range(f["vram"], f["vram"] + f["size"], 4):
                word = int.from_bytes(data[addr - vram:addr - vram + 4], "big")
                if (word >> 26) in _LOAD_OPCODES and (word >> 16) & 31 == 0:
                    out.append({"func": name, "vram": addr, "original": word, "value": 0,
                                "now": "nop (load into $zero; auto)"})
    return out


def merge_extras(functions, extras, expected_counts=None):
    """Inventory functions plus the provisional extras. The inventory counts
    are checked before merging so the extras never mask a lost function."""
    merged = {}
    for pop in SECTIONS:
        funcs = list(functions.get(pop, []))
        if expected_counts and pop in expected_counts and len(funcs) != expected_counts[pop]:
            raise GenError(f"{pop}: {len(funcs)} inventory functions, expected {expected_counts[pop]}")
        merged[pop] = funcs + [{k: e[k] for k in ("name", "vram", "size")}
                               for e in (extras or {}).get(pop, [])]
    return merged


def check_hooks(hooks, funcs_by_name, dropped, renamed_builtin):
    """Validated [[patches.hook]] entries (patches.json "hook"): the function is
    emitted, before_vram (optional; default = function entry) is an
    instruction of it, the call is a C identifier, one hook per point."""
    out, seen = [], set()
    for e in hooks:
        if e["func"] not in funcs_by_name:
            raise GenError(f"hook for unknown function {e['func']}")
        if e["func"] in dropped:
            raise GenError(f"hook for {e['func']}, which is not emitted (stub/ignored/built-in)")
        if not _C_IDENT.match(e["call"]):
            raise GenError(f"hook call {e['call']!r} is not a C identifier")
        f = funcs_by_name[e["func"]][1]
        bv = e["before_vram"]
        if bv is not None and (not f["vram"] <= bv < f["vram"] + f["size"] or bv % 4):
            raise GenError(f"hook before_vram 0x{bv:08X} is not an instruction of {e['func']}")
        key = (e["func"], bv)
        if key in seen:
            raise GenError(f"two hooks at {e['func']} {bv}")
        seen.add(key)
        func = e["func"] + "_recomp" if e["func"] in renamed_builtin else e["func"]
        hook = {"func": func, "call": e["call"], "before_vram": bv}
        if e.get("text"):
            hook["text"] = e["text"]
            hook["contract"] = list(e.get("contract", []))
        out.append(hook)
    return sorted(out, key=lambda e: (e["func"], e["before_vram"] or 0))


def jal_targets(functions, images, skip=()):
    """{target vram: sorted caller names} for every `jal` in the emitted
    functions (static and game code; overlay calls are in-image)."""
    out = {}
    for pop in SECTIONS:
        vram, data = images[pop]
        for f in functions[pop]:
            name = NAME_PREFIX[pop] + f["name"]
            if name in skip:
                continue
            for addr in range(f["vram"], f["vram"] + f["size"], 4):
                word = int.from_bytes(data[addr - vram:addr - vram + 4], "big")
                if word >> 26 == 3:
                    target = ((addr + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
                    out.setdefault(target, set()).add(name)
    return {t: sorted(c) for t, c in out.items()}


def runtime_contract(functions, images, sections, patches, lists, hooks, streams=None):
    """What the host runtime (tools/recomp/runtime) must provide for this run:
    shims = every name the emitted code or the section table references
    but N64Recomp does not emit: built-in reimplemented names in the run and
    called built-in ignored names (both as NAME_recomp; N64ModernRuntime
    implements most), and our patches.json "ignored" (called by NAME), each
    with its callers;
    hooks = the C functions [[patches.hook]] calls; images = the compressed
    stream -> recompiled section map the inflate hook loads (streams:
    {population: ROM offset of its compressed stream})."""
    # N64Recomp's precedence: a reimplemented name is never "ignored".
    builtin_ignored = (set(lists["ignored_funcs"]) - set(lists["reimplemented_funcs"])
                       if lists is not None else set())
    dropped = set(patches["stubs"]) | set(patches["ignored"])
    if lists is not None:
        dropped |= set(lists["reimplemented_funcs"]) | builtin_ignored
    by_vram = {}
    for pop in ("static", "game"):
        for f in functions[pop]:
            by_vram.setdefault(f["vram"], f["name"])
    shims = {}
    # Reimplemented names are not emitted but stay in N64Recomp's section
    # table (recomp_overlays.inl) as NAME_recomp: the runtime must define each.
    if lists is not None:
        reimpl = set(lists["reimplemented_funcs"])
        for pop in SECTIONS:
            for f in functions[pop]:
                if NAME_PREFIX[pop] + f["name"] in reimpl:
                    shims.setdefault(NAME_PREFIX[pop] + f["name"] + "_recomp", [])
    for target, callers in jal_targets(functions, images, skip=dropped).items():
        name = by_vram.get(target)
        if name in patches["ignored"]:
            shims[name] = callers
        elif name in builtin_ignored or (lists is not None and name in lists["reimplemented_funcs"]):
            shims[name + "_recomp"] = callers
    imgs = []
    for sec in sections:
        if sec["name"] == "static" or not streams or sec["name"] not in streams:
            continue
        imgs.append({"name": sec["name"], "stream_rom": _hex(streams[sec["name"]]),
                     "section_rom": _hex(sec["rom"]), "vram": _hex(sec["vram"]),
                     "size": sec["size"]})
    return {"shims": dict(sorted(shims.items())),
            "hooks": sorted({e["call"] for e in hooks if "text" not in e}
                            | {n for e in hooks for n in e.get("contract", [])}),
            "images": imgs}


def render_images_header(images):
    lines = ["/* Generated by tools/recomp/gen_config.py: compressed stream -> recompiled",
             " * section (synthetic ROM offset) for the runtime's inflate hook. */",
             "#pragma once",
             "#include <stdint.h>",
             "typedef struct { const char* name; uint32_t stream_rom; uint32_t section_rom;",
             "                 uint32_t vram; uint32_t size; } RushImage;",
             "static const RushImage rush_images[] = {"]
    for i in images:
        lines.append('    { "%s", %s, %s, %s, 0x%X },' % (i["name"], i["stream_rom"], i["section_rom"],
                                                         i["vram"], i["size"]))
    if not images:
        lines.append("    { 0, 0, 0, 0, 0 },")
    lines += ["};", "#define RUSH_NUM_IMAGES %d" % len(images), ""]
    return "\n".join(lines)


def replacement_hooks(replaced):
    """Entry hooks that delegate each replaced function to its replacement
    (tools/recomp/replace.py hook_text): counted, then called, then return."""
    out = []
    for k, r in enumerate(replaced):
        func = NAME_PREFIX[r["population"]] + r["function"]
        text = ("{ void rush_repl_count(uint8_t*, int); void %s(uint8_t*, recomp_context*); "
                "rush_repl_count(rdram, %d); %s(rdram, ctx); return; }" % (r["call"], k, r["call"]))
        contract = ["rush_repl_count"] + ([r["call"]] if r["mode"] == "native" else [])
        out.append({"func": func, "call": r["call"], "before_vram": None, "text": text,
                    "contract": contract})
    return out


def render_repl_header(replaced, image=None):
    lines = ["/* Generated by tools/recomp/gen_config.py: replaced functions (tools/recomp/replace.py). */",
             "#pragma once", "#include <stdint.h>",
             "#define RUSH_REPL_COUNT %d" % len(replaced),
             "static const char* const rush_repl_names[] = {"]
    for r in replaced:
        lines.append('    "%s=%s:%s",' % (r["id"], r["mode"], r["function"]))
    lines += ["    0", "};"]
    rom, vram, size = (image["rom"], image["vram"], image["size"]) if image else (0, 0, 0)
    lines += ["#define RUSH_REPL_ROM 0x%08Xu" % rom, "#define RUSH_REPL_VRAM 0x%08Xu" % vram,
              "#define RUSH_REPL_SIZE 0x%Xu" % size, ""]
    return "\n".join(lines)


def generate(images, functions, symbols, rom_bytes, out_dir, lists=None,
             expected_counts=EXPECTED_COUNTS, rom_sha1=None, extras=None, patches=None,
             streams=None, identity=None, repl=None):
    """Write every output under out_dir. `images` is {pop: (vram, bytes)}.
    `repl` (optional): tools/recomp/replace.py build() result.
    Pure function of its arguments; returns the manifest dict."""
    out_dir = Path(out_dir)
    for pop in SECTIONS:
        if pop not in images:
            raise GenError(f"missing image for {pop}")
    patches = dict(patches or {})
    for key, empty in (("stubs", {}), ("ignored", {}), ("renamed", {}), ("instruction", []),
                       ("hook", [])):
        patches.setdefault(key, empty)
    inventory_counts = {pop: len(functions.get(pop, [])) for pop in SECTIONS}
    functions, identity_applied = apply_identity(functions, identity)
    functions = merge_extras(functions, extras, expected_counts)
    sections, synthetic_size = plan_layout(len(rom_bytes), images)
    repl = repl or {"image": None, "functions": [], "replaced": [], "native": []}
    repl_image = None
    if repl["image"]:
        data = repl["image"]["bytes"]
        repl_image = {"rom": synthetic_size, "vram": repl["image"]["vram"], "size": len(data)}
        sections = sections + [{"name": "repl", "rom": synthetic_size, "vram": repl_image["vram"],
                                "size": len(data)}]
        synthetic_size = _align(synthetic_size + len(data), APPEND_ALIGN)
        images = dict(images, repl=(repl_image["vram"], data))
        functions = dict(functions, repl=list(repl["functions"]))
    elif repl["functions"]:
        raise GenError("replacement functions without an image")
    names = check_functions(functions, sections)
    builtin = set()
    if lists is not None:
        builtin = set(lists["reimplemented_funcs"]) | set(lists["ignored_funcs"]) | set(lists["renamed_funcs"])
    for kind in ("stubs", "ignored", "renamed"):
        for name in patches[kind]:
            if name not in names:
                raise GenError(f"patches.json {kind}: {name} is not a function of this run")
            if name in builtin:
                raise GenError(f"patches.json {kind}: {name} is on N64Recomp's built-in lists "
                               "(it is renamed NAME_recomp first; listing it would fail)")
    funcs_by_name = {NAME_PREFIX[pop] + f["name"]: (pop, f) for pop in SECTIONS for f in functions[pop]}
    # Functions N64Recomp drops by name, and ones we stub/ignore, take no patches;
    # its built-in "renamed" functions are looked up as NAME_recomp.
    dropped = set(patches["stubs"]) | set(patches["ignored"])
    renamed_builtin = set()
    if lists is not None:
        dropped |= set(lists["reimplemented_funcs"]) | set(lists["ignored_funcs"])
        renamed_builtin = set(lists["renamed_funcs"])
    manual_vrams = {e["vram"] for e in patches["instruction"]}
    auto = [e for e in zero_load_patches(functions, images, skip=dropped)
            if e["vram"] not in manual_vrams]
    patches["instruction"] = list(patches["instruction"]) + auto
    for e in patches["instruction"]:
        if e["func"] not in funcs_by_name:
            raise GenError(f"instruction patch for unknown function {e['func']}")
        pop, f = funcs_by_name[e["func"]]
        if not f["vram"] <= e["vram"] < f["vram"] + f["size"] or e["vram"] % 4:
            raise GenError(f"instruction patch 0x{e['vram']:08X} is not inside {e['func']}")
        vram, data = images[pop]
        word = int.from_bytes(data[e["vram"] - vram:e["vram"] - vram + 4], "big")
        if word != e["original"]:
            raise GenError(f"instruction patch 0x{e['vram']:08X}: image has 0x{word:08X}, "
                           f"patches.json expects 0x{e['original']:08X}")
    patches["instruction"] = [dict(e, func=e["func"] + "_recomp") if e["func"] in renamed_builtin
                              else dict(e) for e in patches["instruction"]]
    if set(patches["stubs"]) & set(patches["ignored"]) or set(patches["renamed"]) & set(patches["ignored"]):
        raise GenError("a function is in two of stubs/ignored/renamed in patches.json")
    hooks = check_hooks(list(patches["hook"]) + replacement_hooks(repl["replaced"]),
                        funcs_by_name, dropped, renamed_builtin)
    contract = runtime_contract(functions, images, sections, patches, lists, hooks, streams)
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
    for pop in list(SECTIONS) + (["repl"] if repl_image else []):
        (out_dir / "images" / (pop + ".bin")).write_bytes(images[pop][1])
    rom_name = "rush2049.recomp.z64"
    (out_dir / "rom" / rom_name).write_bytes(bytes(synthetic))
    (out_dir / "rush2049.syms.toml").write_text(render_symbols_toml(sections, functions))
    (out_dir / "rush2049.datasyms.toml").write_text(render_datasyms_toml(sections, placed))
    (out_dir / "rush2049.recomp.toml").write_text(
        render_config_toml("rom/" + rom_name, "rush2049.syms.toml",
                           stubs=patches["stubs"], ignored=patches["ignored"],
                           renamed=patches["renamed"],
                           instruction=patches["instruction"], hooks=hooks))
    (out_dir / "rush2049_images.h").write_text(render_images_header(contract["images"]))
    (out_dir / "rush2049_repl.h").write_text(render_repl_header(repl["replaced"], repl_image))

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
        "inventory_functions": inventory_counts,
        "provisional_functions": {pop: [e["name"] for e in (extras or {}).get(pop, [])]
                                  for pop in SECTIONS if (extras or {}).get(pop)},
        "patches": {"stubs": dict(sorted(patches["stubs"].items())),
                    "ignored": dict(sorted(patches["ignored"].items())),
                    "renamed": dict(sorted(patches["renamed"].items())),
                    "instruction": [{"func": e["func"], "vram": _hex(e["vram"]),
                                     "original": _hex(e["original"]), "value": _hex(e["value"])}
                                    for e in sorted(patches["instruction"], key=lambda e: e["vram"])],
                    "hook": [{"func": e["func"], "call": e["call"],
                              "before_vram": None if e["before_vram"] is None else _hex(e["before_vram"])}
                             for e in hooks if "text" not in e]},
        "runtime_contract": contract,
        "identity_renames": identity_applied,
        "replacements": {"replaced": repl["replaced"], "native": repl["native"],
                         "image": None if not repl_image else
                         {"rom": _hex(repl_image["rom"]), "vram": _hex(repl_image["vram"]),
                          "size": repl_image["size"], "sha256": sha256(images["repl"][1])}},
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
    parser.add_argument("--n64recomp-src", required=True,
                        help="N64Recomp checkout: its built-in reimplemented/ignored/renamed "
                        "lists (src/symbol_lists.cpp) decide which names the config may patch")
    parser.add_argument("--replacements", default=None,
                        help="replacement manifest (default tools/recomp/replacements.json)")
    parser.add_argument("--replace-set", default=None, help="build this set of replacements.json")
    parser.add_argument("--replace", default="", help="comma-separated replacement ids to build")
    parser.add_argument("--ido53", default=None, help="IDO 5.3 toolkit dir (contains ido/cc)")
    parser.add_argument("--ido71", default=None, help="IDO 7.1 toolkit dir (contains ido/cc)")
    parser.add_argument("--ld", default="ld.lld", help="ld.lld for the replacement image")
    args = parser.parse_args(argv)
    try:
        lists = builtin_lists(args.n64recomp_src)
        rom = read_rom(args.rom)
        images = extract_images(rom)
        out = Path(args.out)
        (out / "images").mkdir(parents=True, exist_ok=True)
        game_path = out / "images" / "game.bin"
        game_path.write_bytes(images["game"][1])          # layout.derive reads it
        functions, symbols = load_inventories(args.rom, game_path)
        repl = None
        ids = [i for i in args.replace.split(",") if i]
        if args.replace_set or ids:
            sys.path.insert(0, str(Path(__file__).resolve().parent))
            import replace as replmod
            doc = replmod.load_manifest(args.replacements or replmod.MANIFEST)
            chosen = replmod.select(doc, args.replace_set, ids)
            toolkits = {k: v for k, v in (("ido5.3", args.ido53), ("ido7.1", args.ido71)) if v}
            repl = replmod.build(chosen, functions, symbols, out / "replace", toolkits, ld=args.ld)
        else:
            import shutil
            shutil.rmtree(out / "replace", ignore_errors=True)
        manifest = generate(images, functions, symbols, rom, out, lists=lists,
                            extras=load_extra_functions(), patches=load_patches(),
                            streams=stream_offsets(), identity=load_identity(), repl=repl)
    except GenError as exc:
        print(f"gen_config: {exc}", file=sys.stderr)
        return 2
    except Exception as exc:  # replace.ReplaceError
        if type(exc).__name__ != "ReplaceError":
            raise
        print(f"gen_config: replacement: {exc}", file=sys.stderr)
        return 2
    for s in manifest["sections"]:
        print(f"{s['name']:7s} rom {s['rom']} vram {s['vram']} size {s['size']:7d}  "
              f"{s['functions']:5d} functions  {s['data_symbols']:5d} data symbols")
    extra = sum(len(v) for v in manifest["provisional_functions"].values())
    print(f"inventory {sum(manifest['inventory_functions'].values())} + provisional {extra}; "
          f"stubs {len(manifest['patches']['stubs'])}, ignored {len(manifest['patches']['ignored'])}")
    for r in manifest["replacements"]["replaced"]:
        print(f"replaced {r['function']} ({r['population']}) <- {r['id']} [{r['mode']}] {r['source']}")
    print(f"total {manifest['total_functions']} functions -> {Path(args.out) / 'rush2049.recomp.toml'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
