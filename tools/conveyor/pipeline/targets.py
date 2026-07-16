"""N64 target inventory: populate n64_target rows and produce per-function
target objects (.o) for the Scorer.

Sources:
- work/**/info.txt          — name, address, category, flags (1,319 functions)
- build/game_code.bin       — decompressed game code (RAM 0x80086A50+), the
                              752-function "extracted" population
- baserom.us.z64            — static population; ROM offset = vaddr - 0x7FFFF400

Target .o files come in two tiers (003, specs/003-reloc-aware-targets):

- `reloc_aware`: static targets assembled from their splat asm region, which
  carries `%hi/%lo/jal` symbol operands, so the object carries real
  relocations. Gated by a per-target round-trip check (masked-word equality
  against the ROM) before it may replace the raw-word object.
- `raw_word`: everything else — dynamic game-code targets, and any static
  target whose region is missing, won't assemble, or fails the gate. Built by
  assembling the raw instruction words (`.word`) so no disassembler round-trip
  can distort them (V1 conservatism, preserved as the fallback).

`n64_target.target_o_sha` points at the chosen object; `tier`/`gate_reason`
record which path it took. When the object bytes change, the target's derived
`matrix_entry` evidence is superseded (purged) in the same transaction.
"""
import dataclasses
import hashlib
import os
import re
import struct
import subprocess
import sys
import tempfile
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
GAME_CODE_BASE = 0x80086A50
GAME_CODE_BIN = REPO / "build" / "game_code.bin"
BASEROM = REPO / "baserom.us.z64"
STATIC_ROM_DELTA = 0x7FFFF400  # vaddr - delta = ROM offset (verified: strlen)
ASM_DIR = REPO / "asm" / "us"

EXTENT_REPORT_TARGETS = (
    "game_loop", "game_mode_handler", "attract_or_transition", "process_inputs",
    "sound_control", "playgame_state_change", "RaceStateMachine_Update",
    "countdown", "countdown_handler", "Input_ProcessGameplayPad",
)

_SIZE_RE = re.compile(r"\((\d+)\s*bytes\)")

# A splat instruction line: `/* <off> <vaddr> <word> */  <mnemonic ...>`.
# We key regions by <vaddr> and gate against <word>.
_REGION_LINE_RE = re.compile(
    r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/"
)
_GLABEL_RE = re.compile(r"^\s*glabel\s+(\S+)")
_ENDLABEL_RE = re.compile(r"^\s*endlabel\s+(\S+)")


def load_work_inventory(work_dir=None):
    """Parse every work/**/info.txt into {name, address, category, flags, size?}."""
    work_dir = Path(work_dir or REPO / "work")
    entries = []
    for info in sorted(work_dir.rglob("info.txt")):
        fields = {}
        for line in info.read_text().splitlines():
            if ":" in line:
                key, _, value = line.partition(":")
                fields[key.strip()] = value.strip()
        if "address" not in fields or "name" not in fields:
            continue
        size = None
        m = _SIZE_RE.search(fields.get("comment", ""))
        if m:
            size = int(m.group(1))
        entries.append(
            {
                "name": fields["name"],
                "address": int(fields["address"], 16),
                "category": fields.get("category", ""),
                "flags": fields.get("compiler_flags", ""),
                "size": size,
            }
        )
    # Dedup by address (first name wins), then infer missing sizes from the
    # gap to the next function.
    by_addr = {}
    for e in entries:
        by_addr.setdefault(e["address"], e)
    ordered = sorted(by_addr.values(), key=lambda e: e["address"])
    for i, e in enumerate(ordered):
        gap = ordered[i + 1]["address"] - e["address"] if i + 1 < len(ordered) else None
        if e["size"] is None:
            e["size"] = gap if gap is not None else 256
        elif gap is not None:
            e["size"] = min(e["size"], gap)
    # target_id (name) is the n64_target primary key, but a handful of game-code
    # functions share a heuristic name at different addresses. Collapse to one
    # entry per name (lowest address wins, deterministically) so populate is
    # idempotent — otherwise two entries upsert the same row and ping-pong its
    # object every run, faking a supersession churn (breaks FR-010/SC-007).
    by_name = {}
    for e in ordered:
        by_name.setdefault(e["name"], e)
    return list(by_name.values())


_image_cache = {}


def scan_extent(image_bytes, address):
    """Return the instruction count ending at the first eligible ``jr $ra``.

    ``image_bytes`` is the complete game-code image mapped at
    :data:`GAME_CODE_BASE`.  A forward direct branch keeps the scan alive
    through its target; jumps and backward branches do not.  The string
    ``"scan_overrun"`` denotes reaching the image boundary or 16 KiB cap
    without a terminating return and its delay slot.
    """
    offset = address - GAME_CODE_BASE
    if offset < 0 or offset % 4 or offset >= len(image_bytes):
        raise ValueError(f"address {address:#x} outside game-code image")

    image_end = GAME_CODE_BASE + len(image_bytes)
    bound = min(address + 16 * 1024, image_end)
    furthest = address - 4
    pc = address
    while pc + 4 <= bound:
        word = struct.unpack_from(">I", image_bytes, pc - GAME_CODE_BASE)[0]
        opcode = word >> 26

        # Direct PC-relative branch encodings: REGIMM, beq/bne/blez/bgtz,
        # their likely forms, and bc0/bc1/bc2.  J/JAL are deliberately absent.
        is_branch = opcode in {0x01, 0x04, 0x05, 0x06, 0x07,
                               0x14, 0x15, 0x16, 0x17}
        if opcode in {0x10, 0x11, 0x12} and ((word >> 21) & 0x1F) == 0x08:
            is_branch = True
        if is_branch:
            immediate = word & 0xFFFF
            if immediate & 0x8000:
                immediate -= 0x10000
            target = pc + 4 + immediate * 4
            if target > pc:
                furthest = max(furthest, target)

        # SPECIAL / JR / rs=$ra.  The delay slot must fit inside both bounds.
        # >= not >: a shared-return leaf branches directly to its jr, so
        # furthest == pc at the true end (contract §3 amendment).
        if (word & 0xFFE0003F) == 0x03E00008 and pc >= furthest:
            return (pc - address) // 4 + 2 if pc + 8 <= bound else "scan_overrun"
        pc += 4

    return "scan_overrun"


def _image(path):
    if path not in _image_cache:
        _image_cache[path] = path.read_bytes()
    return _image_cache[path]


def function_words(address, size):
    """Raw big-endian instruction words for a function, from whichever image
    holds that address. Returns list of 8-hex-digit strings."""
    if GAME_CODE_BASE <= address < GAME_CODE_BASE + GAME_CODE_BIN.stat().st_size:
        data = _image(GAME_CODE_BIN)
        off = address - GAME_CODE_BASE
    else:
        data = _image(BASEROM)
        off = address - STATIC_ROM_DELTA
        if off < 0 or off >= len(data):
            raise ValueError(f"address {address:#x} outside known images")
    chunk = data[off : off + (size // 4) * 4]
    return [f"{w:08X}" for (w,) in struct.iter_unpack(">I", chunk)]


def assemble_words(words, out_o, func_name="func"):
    asm = [".set noreorder", ".text", f".globl {func_name}", f"{func_name}:"]
    asm += [f"    .word 0x{w}" for w in words]
    with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as f:
        f.write("\n".join(asm) + "\n")
        src = f.name
    subprocess.run(
        ["mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-o", str(out_o), src],
        check=True,
    )


# --- reloc-aware assembly from splat asm regions (003) -----------------------


@dataclasses.dataclass
class Region:
    """One `glabel`..`endlabel` function region from a splat `.s` file."""
    name: str          # the asm glabel (func_XXXXXXXX), not the target_id
    vaddr: int         # vaddr of the first instruction (the region key)
    lines: list        # assembler-ready text (comments stripped from insns)
    words: list        # ROM instruction words, 8-hex strings, in order


# KSEG1 de-symbolization (003 review fix). splat symbolizes MMIO addresses
# (DPC_CLOCK_REG = 0xA4100010, …), but IDO compiles #define'd KSEG1 addresses
# to literal immediates with NO relocation — so a relocation against a KSEG1
# symbol in a target is a disassembly artifact, not ROM truth, and it penalizes
# every correctly-matched candidate (SC-004 regression: 4 locked functions).
# Rule: an instruction whose %hi/%lo symbol resolves into KSEG1
# (0xA0000000..0xBFFFFFFF) is emitted as its raw `.word` — the ROM word IS the
# literal IDO produced. RAM symbols keep their relocations.
_HILO_SYM_RE = re.compile(r"%(?:hi|lo)\(([A-Za-z_][A-Za-z0-9_]*)")
_SYM_DEF_RE = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*0x([0-9A-Fa-f]+)\s*;")
_ADDR_NAME_RE = re.compile(r"^(?:D|func|jtbl)_([0-9A-Fa-f]{8})$")
KSEG1_BASE, KSEG1_END = 0xA0000000, 0xC0000000

_symbol_map_cache = None


def _symbol_map():
    """{name: address} from symbol_addrs.us.txt + hardware_regs.ld (both are
    `NAME = 0xADDR;` lines). Cached; missing files contribute nothing."""
    global _symbol_map_cache
    if _symbol_map_cache is None:
        m = {}
        for fname in ("symbol_addrs.us.txt", "hardware_regs.ld"):
            path = REPO / fname
            if not path.is_file():
                continue
            for line in path.read_text(errors="replace").splitlines():
                d = _SYM_DEF_RE.match(line)
                if d:
                    m.setdefault(d.group(1), int(d.group(2), 16))
        _symbol_map_cache = m
    return _symbol_map_cache


def _resolve_symbol(name):
    """Best-effort address for an asm symbol: the symbol tables first, then
    splat's address-bearing name patterns (D_/func_/jtbl_XXXXXXXX)."""
    addr = _symbol_map().get(name)
    if addr is not None:
        return addr
    m = _ADDR_NAME_RE.match(name)
    return int(m.group(1), 16) if m else None


def _is_kseg1_ref(mnemonic_text):
    """True iff the instruction references a %hi/%lo symbol that resolves into
    KSEG1 (uncached MMIO space — never link-time relocated in a real build)."""
    m = _HILO_SYM_RE.search(mnemonic_text)
    if not m:
        return False
    addr = _resolve_symbol(m.group(1))
    return addr is not None and KSEG1_BASE <= addr < KSEG1_END


def index_asm_regions(asm_dir=None):
    """{first_instruction_vaddr: Region} over every `asm/us/*.s`.

    A region runs from `glabel <name>` to `endlabel <name>`. Instruction lines
    (`/* off vaddr word */  mnemonic`) contribute their word to the gate and,
    comment-stripped, their mnemonic to the assembler input; interior label
    lines (`.L…:`) and any blanks/directives are kept verbatim — the assembler
    tolerates them and the gate judges the result. Instructions referencing
    KSEG1 (MMIO) symbols are emitted as their raw `.word` instead (see
    _is_kseg1_ref) so the object matches IDO's literal-immediate codegen.
    """
    asm_dir = Path(asm_dir) if asm_dir else ASM_DIR
    regions = {}
    for path in sorted(asm_dir.glob("*.s")):
        cur = None
        for line in path.read_text(errors="replace").splitlines():
            g = _GLABEL_RE.match(line)
            if g:
                cur = {"name": g.group(1), "vaddr": None, "lines": [], "words": []}
                continue
            e = _ENDLABEL_RE.match(line)
            if e:
                if cur is not None and cur["vaddr"] is not None:
                    regions[cur["vaddr"]] = Region(
                        cur["name"], cur["vaddr"], cur["lines"], cur["words"])
                cur = None
                continue
            if cur is None:
                continue
            m = _REGION_LINE_RE.search(line)
            if m:
                if cur["vaddr"] is None:
                    cur["vaddr"] = int(m.group(1), 16)
                word = m.group(2).upper()
                cur["words"].append(word)
                # Everything after the closing `*/` is the mnemonic; the
                # comment (the only `*/` on the line in MIPS asm) is dropped.
                mnemonic = line.split("*/", 1)[1].rstrip()
                if _is_kseg1_ref(mnemonic):
                    cur["lines"].append(f"    .word 0x{word}")
                else:
                    cur["lines"].append(mnemonic)
            else:
                cur["lines"].append(line.rstrip())
    return regions


class AssembleError(Exception):
    """Assembler rejected a region; carries the first stderr line as the
    gate_reason detail."""


def assemble_region(region, target_id, out_o):
    """Assemble a region into a relocatable object named `target_id`. Raises
    AssembleError (first stderr line preserved) on assembler failure."""
    asm = [
        ".set noreorder",
        ".set noat",
        ".section .text",
        f".globl {target_id}",
        f"{target_id}:",
    ]
    asm += region.lines
    with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as f:
        f.write("\n".join(asm) + "\n")
        src = f.name
    try:
        proc = subprocess.run(
            ["mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-o", str(out_o), src],
            capture_output=True, text=True,
        )
    finally:
        os.unlink(src)
    if proc.returncode != 0:
        # Preserve the first real error, stripped of the temp-file path so the
        # gate_reason is deterministic across runs (FR-010). GNU as format:
        # `<path>:<line>: Error: <msg>` — keep from `<line>:` onward.
        detail = "assembler failed"
        for line in (proc.stderr or "").splitlines():
            if ": Error:" in line:
                detail = line.split(".s:", 1)[-1].strip() if ".s:" in line \
                    else line.strip()
                break
        raise AssembleError(detail)


def _strip_trailing_nops(words):
    """Drop trailing 0x00000000 (nop) padding words. The assembler pads `.text`
    to a 16-byte boundary while splat's `endlabel` excludes the ROM's own
    alignment nops, so a byte-identical region can differ only in trailing
    padding — bookkeeping the linker owns, not function content (research D3)."""
    n = len(words)
    while n > 0 and words[n - 1] == 0:
        n -= 1
    return words[:n]


def _gate_decide(rom_words, new_words, sites):
    """Pure gate decision over two int-word lists + the new object's reloc
    sites: (ok, reason). Trailing nop padding on either side is ignored, then
    lengths must match and the masked words must be equal. Reuses
    jobs/scoring's mask helpers — one mask logic in the codebase (research D3).
    Factored out so tests can drive it without an assembler."""
    from ..jobs import scoring

    rom = _strip_trailing_nops(list(rom_words))
    new = _strip_trailing_nops(list(new_words))
    if len(new) != len(rom):
        return False, f"length_mismatch {len(new)} != {len(rom)}"
    if scoring._masked_diff(rom, new, sites) != 0:
        masked_t, masked_c = list(rom), list(new)
        for i, mask in sites:
            if i < len(masked_c):
                masked_c[i] &= mask
            if i < len(masked_t):
                masked_t[i] &= mask
        idx = next((i for i in range(len(masked_t)) if masked_t[i] != masked_c[i]), 0)
        return False, f"word_mismatch@{idx}"
    return True, None


def gate_target(rom_words, new_o):
    """Round-trip gate: (ok, reason). The reassembled object's instruction
    words, masked at its own relocation sites, must equal the original ROM
    words masked identically. `rom_words` are 8-hex strings (region ROM words)."""
    from ..jobs import scoring

    binary = scoring._objdump_path()
    # -dz (disassemble-zeroes): without it objdump collapses runs of zero words
    # to `...`, which _parse_text_words skips — undercounting interior/trailing
    # nops against the raw ROM words and faking a length_mismatch. The permuter
    # Scorer disassembles with -z for the same reason.
    new_words = scoring._parse_text_words(scoring._objdump(binary, "-dz", str(new_o)))
    sites = scoring._parse_relocs(scoring._objdump(binary, "-r", str(new_o)))
    return _gate_decide([int(w, 16) for w in rom_words], new_words, sites)


def _fallback_category(reason):
    """Coarse bucket of a gate_reason for the coverage histogram."""
    if reason is None:
        return None
    for prefix in ("assemble_error", "word_mismatch", "length_mismatch"):
        if reason.startswith(prefix):
            return prefix
    return reason  # no_asm_region


def _supersede_target(conn, target_id, previous_sha, new_sha):
    """Apply feature 003's object-identity supersession contract.

    The caller must invoke this in the same transaction as its n64_target
    update.  Returns ``(changed, purged_matrix_rows)``.
    """
    if previous_sha == new_sha:
        return False, 0
    cur = conn.execute("DELETE FROM matrix_entry WHERE target_id=?", (target_id,))
    return True, max(cur.rowcount, 0)


def _extent_plan(conn, inventory):
    """Scan extracted inventory and return per-name repair metadata."""
    image = _image(GAME_CODE_BIN)
    previous = {
        row["target_id"]: row
        for row in conn.execute(
            "SELECT target_id, insn_count, gate_reason FROM n64_target"
            " WHERE population='extracted'"
        )
    }
    plan = {}
    for entry in inventory:
        address = entry["address"]
        if not (GAME_CODE_BASE <= address < GAME_CODE_BASE + len(image)):
            continue
        try:
            scanned = scan_extent(image, address)
        except ValueError:
            scanned = "scan_overrun"
        plan[entry["name"]] = {
            "address": address,
            "scanned": scanned,
            "previous": previous.get(entry["name"]),
            "container": None,
        }

    extents = [
        (name, item["address"], item["address"] + item["scanned"] * 4)
        for name, item in plan.items() if isinstance(item["scanned"], int)
    ]
    for name, item in plan.items():
        containers = [
            extent for extent in extents
            if extent[0] != name and extent[1] < item["address"] < extent[2]
        ]
        if containers:
            # Name the tightest containing function, deterministically.
            item["container"] = min(
                containers, key=lambda extent: (extent[2] - extent[1], extent[1], extent[0])
            )[0]
    return plan


def populate(conn, store, work_dir=None, limit=None):
    """Fill n64_target rows and build target .o blobs. Static targets attempt
    reloc-aware assembly (region → assemble → gate) with a raw-word fallback;
    dynamic targets stay raw-word. When a target's object bytes change, its
    matrix_entry evidence is superseded in the same transaction. Returns a
    summary dict and prints the FR-009 coverage report."""
    from ..coordinator import db as dbmod

    inventory = load_work_inventory(work_dir)
    if limit:
        inventory = inventory[:limit]
    regions = index_asm_regions()
    extent_plan = _extent_plan(conn, inventory)

    built, skipped = 0, 0
    tiers = {"reloc_aware": 0, "raw_word_static": 0, "raw_word_dynamic": 0}
    fallbacks = Counter()
    superseded_targets, purged_rows = 0, 0
    extent_counts = Counter()

    with tempfile.TemporaryDirectory() as tmp:
        tmpdir = Path(tmp)
        for e in inventory:
            population = (
                "extracted"
                if GAME_CODE_BASE <= e["address"] < GAME_CODE_BASE + 0x9E0A0
                else "static"
            )
            extent = extent_plan.get(e["name"])
            if extent is not None:
                scanned = extent["scanned"]
                if isinstance(scanned, int):
                    e = dict(e, size=scanned * 4)
            try:
                words = function_words(e["address"], e["size"])
                if not words:
                    raise ValueError("empty function body")
            except (ValueError, subprocess.CalledProcessError) as exc:
                print(f"  skip {e['name']}: {exc}", file=sys.stderr)
                skipped += 1
                continue

            tier = "raw_word"
            gate_reason = None
            o_path = None
            # Static targets try the reloc-aware path (contract §populate 1-4).
            if population == "static":
                region = regions.get(e["address"])
                if region is None:
                    gate_reason = "no_asm_region"
                else:
                    reloc_o = tmpdir / "reloc.o"
                    try:
                        assemble_region(region, e["name"], reloc_o)
                    except AssembleError as exc:
                        gate_reason = f"assemble_error: {exc}"
                    else:
                        ok, reason = gate_target(region.words, reloc_o)
                        if ok:
                            tier, gate_reason, o_path = "reloc_aware", None, reloc_o
                        else:
                            gate_reason = reason

            if o_path is None:
                # Raw-word object: gate fallback (static) or dynamic population.
                raw_o = tmpdir / "raw.o"
                try:
                    assemble_words(words, raw_o, e["name"])
                except subprocess.CalledProcessError as exc:
                    print(f"  skip {e['name']}: {exc}", file=sys.stderr)
                    skipped += 1
                    continue
                o_path = raw_o
                if population != "static":
                    previous = extent["previous"] if extent else None
                    if extent and extent["container"]:
                        gate_reason = f"extent_conflict:{extent['container']}"
                    elif extent and extent["scanned"] == "scan_overrun":
                        gate_reason = "scan_overrun"
                    elif previous and previous["insn_count"] == len(words):
                        prior_reason = previous["gate_reason"] or ""
                        gate_reason = ("extent_repaired"
                                       if prior_reason == "extent_repaired" else None)
                    else:
                        gate_reason = "extent_repaired"

            o_sha = store.put_file(o_path)
            asm_sha = hashlib.sha256("\n".join(words).encode()).hexdigest()

            if tier == "reloc_aware":
                tiers["reloc_aware"] += 1
            elif population == "static":
                tiers["raw_word_static"] += 1
                fallbacks[_fallback_category(gate_reason)] += 1
            else:
                tiers["raw_word_dynamic"] += 1

            if extent is not None:
                if extent["container"]:
                    extent_counts["conflict"] += 1
                elif extent["scanned"] == "scan_overrun":
                    extent_counts["scan_overrun"] += 1
                elif extent["previous"] and extent["previous"]["insn_count"] == len(words):
                    extent_counts["agree"] += 1
                else:
                    extent_counts["repaired"] += 1

            with dbmod.tx(conn):
                prev = conn.execute(
                    "SELECT target_o_sha FROM n64_target WHERE target_id=?",
                    (e["name"],),
                ).fetchone()
                prev_sha = prev["target_o_sha"] if prev else None
                changed, purged = _supersede_target(
                    conn, e["name"], prev_sha, o_sha
                )
                if changed:
                    superseded_targets += 1
                    purged_rows += purged
                conn.execute(
                    "INSERT INTO n64_target (target_id, address, population,"
                    " insn_count, target_asm_sha, target_o_sha, tier, gate_reason)"
                    " VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
                    " ON CONFLICT(target_id) DO UPDATE SET address=excluded.address,"
                    " population=excluded.population, insn_count=excluded.insn_count,"
                    " target_asm_sha=excluded.target_asm_sha,"
                    " target_o_sha=excluded.target_o_sha, tier=excluded.tier,"
                    " gate_reason=excluded.gate_reason",
                    (e["name"], e["address"], population, len(words), asm_sha,
                     o_sha, tier, gate_reason),
                )
                conn.execute(
                    "INSERT INTO function_status (target_id, status, updated_at)"
                    " VALUES (?, 'unmatched', strftime('%Y-%m-%dT%H:%M:%fZ','now'))"
                    " ON CONFLICT(target_id) DO NOTHING",
                    (e["name"],),
                )
                conn.execute(
                    "INSERT OR IGNORE INTO blob (sha256, kind, size_bytes, created_at)"
                    " VALUES (?, 'target', ?, strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
                    (o_sha, store.size(o_sha) or 0),
                )
            built += 1

    # FR-009 coverage report.
    print(f"target tiers: reloc_aware={tiers['reloc_aware']} "
          f"raw_word_static={tiers['raw_word_static']} "
          f"raw_word_dynamic={tiers['raw_word_dynamic']}")
    top = "  ".join(f"{cat}={n}" for cat, n in fallbacks.most_common())
    print(f"gate fallbacks (static): {tiers['raw_word_static']}"
          + (f" — top reasons: {top}" if top else ""))
    print(f"superseded: {superseded_targets} targets, "
          f"{purged_rows} evidence rows purged")
    print(f"extents: {extent_counts['agree']} agree, "
          f"{extent_counts['repaired']} repaired, "
          f"{extent_counts['conflict']} conflict")
    for target_id in EXTENT_REPORT_TARGETS:
        extent = extent_plan.get(target_id)
        if extent is None:
            continue
        before = extent["previous"]["insn_count"] if extent["previous"] else None
        after = extent["scanned"]
        before_end = (extent["address"] + before * 4) if before is not None else None
        after_end = (extent["address"] + after * 4) if isinstance(after, int) else after
        before_text = f"{before_end:#010x}" if before_end is not None else "missing"
        after_text = f"{after_end:#010x}" if isinstance(after_end, int) else after_end
        print(f"  {target_id}: {before_text} -> {after_text}")

    return {
        "built": built, "skipped": skipped, "total": len(inventory),
        "tiers": tiers, "fallbacks": dict(fallbacks),
        "superseded_targets": superseded_targets, "purged_rows": purged_rows,
        "extents": dict(extent_counts),
    }


def main():
    import argparse

    from ..coordinator import db as dbmod
    from ..coordinator.store import BlobStore

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(Path("~/.conveyor").expanduser()))
    parser.add_argument("--limit", type=int, default=None)
    args = parser.parse_args()
    conn = dbmod.connect(Path(args.data) / "conveyor.db")
    store = BlobStore(Path(args.data) / "blobs")
    summary = populate(conn, store, limit=args.limit)
    print(f"targets: {summary['built']} built, {summary['skipped']} skipped, "
          f"{summary['total']} in inventory")


if __name__ == "__main__":
    main()
