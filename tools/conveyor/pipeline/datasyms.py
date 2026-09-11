"""Generated data symbols: type every formed data address the symbolizer sees.

    python3 -m tools.conveyor.pipeline.datasyms generate [--data DIR]

Contract: specs/007-population-closure/contracts/closure-and-datasyms.md §7-§11.

Scans the derived assembly of every gate-passed extracted target with the
HAND table only (so the scan is independent of its own previous output and
regenerable), collects each formed effective data address that no hand entry
names — direct ``lui``+access, ``lui``+``addiu`` formation, and the indexed
``lui``+``addu``+access idiom — and types it by the widest observed access.
Emits ``build/m2c_datasyms.json``: sorted, byte-stable, never hand-edited.
``disasm.symbol_table()`` merges it under the hand table; ``protos generate``
emits its externs (§11).
"""
import argparse
import hashlib
import json
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

from ..client import DEFAULT_DATA
from . import disasm

REPO = Path(__file__).resolve().parents[3]
OUTPUT = disasm.DATASYMS_JSON
DERIVATION_VERSION = 1

# mnemonic -> (width in bytes, kind, signed)
_ACCESS = {
    "lb": (1, "int", True), "lbu": (1, "int", False), "sb": (1, "int", None),
    "lh": (2, "int", True), "lhu": (2, "int", False), "sh": (2, "int", None),
    "lw": (4, "int", True), "sw": (4, "int", None),
    "lwl": (4, "int", True), "lwr": (4, "int", True),
    "swl": (4, "int", None), "swr": (4, "int", None),
    "ll": (4, "int", True), "sc": (4, "int", None),
    "ld": (8, "int", True), "sd": (8, "int", None),
    "ldl": (8, "int", True), "ldr": (8, "int", True),
    "lwc1": (4, "fp", None), "swc1": (4, "fp", None),
    "ldc1": (8, "fp", None), "sdc1": (8, "fp", None),
}
_INT_TYPE = {1: ("s8", "u8"), 2: ("s16", "u16"), 4: ("s32", "u32"),
             8: ("s64", "u64")}
_FP_TYPE = {4: "f32", 8: "f64"}
FORMATION_ONLY_TYPE = "s32"
# Only RDRAM (kseg0) addresses can be data symbols: lui+addiu also forms plain
# large integer constants (0x39B40) and lui+lwc1 pairs form float immediates
# (0x3F00xxxx) — naming those would rewrite constants into `&D_xxx`.
RAM_LOW, RAM_HIGH = 0x80000000, 0x80800000


def choose_type(accesses):
    """Contract §8. ``accesses`` are mnemonics; returns ``(type, conflicts)``.

    Widest access wins (word > half > byte); FP-only types f32/f64; a
    same-width integer/FP disagreement records the conflict and types
    integer; formation-only entries (no access) type s32 (recorded)."""
    widths = defaultdict(set)      # kind -> set of widths
    signed = defaultdict(set)      # width -> {True, False}
    for mnemonic in accesses:
        spec = _ACCESS.get(mnemonic)
        if spec is None:
            continue
        width, kind, is_signed = spec
        widths[kind].add(width)
        if kind == "int" and is_signed is not None:
            signed[width].add(is_signed)
    if not widths:
        return FORMATION_ONLY_TYPE, ["formation_only"]
    int_w = max(widths["int"]) if widths["int"] else 0
    fp_w = max(widths["fp"]) if widths["fp"] else 0
    conflicts = []
    if int_w and fp_w:
        conflicts.append(f"int{int_w * 8}_vs_fp{fp_w * 8}")
    if fp_w > int_w:
        return _FP_TYPE[fp_w], conflicts
    # integer wins on a same-width conflict, or when it is the widest
    signs = signed.get(int_w, set())
    unsigned = bool(signs) and True not in signs
    return _INT_TYPE[int_w][1 if unsigned else 0], conflicts


def scan_rows(conn, image_path=disasm.GAME_CODE_BIN,
              objdump="mips-linux-gnu-objdump"):
    """Gate-passed extracted rows, disassembled with the hand table only.
    Yields ``(target_id, observations)``; targets objdump cannot cover are
    skipped (they are `no_disasm` in the histogram anyway)."""
    rows = conn.execute(
        "SELECT target_id,address,insn_count,gate_reason FROM n64_target"
        " WHERE population='extracted' AND address IS NOT NULL"
        " AND insn_count IS NOT NULL ORDER BY address, target_id").fetchall()
    known = {row["address"]: row["target_id"] for row in conn.execute(
        "SELECT target_id,address FROM n64_target WHERE address IS NOT NULL")}
    hand = disasm.symbol_table(include_generated=False)
    for row in rows:
        reason = row["gate_reason"] or ""
        if reason.startswith("extent_conflict") or reason.startswith("scan_overrun"):
            continue
        start = row["address"]
        stop = start + 4 * row["insn_count"]
        command = [
            objdump, "-D", "-b", "binary", "-m", "mips:4300", "-EB",
            f"--adjust-vma=0x{disasm.GAME_CODE_BASE:08X}",
            "--start-address", f"0x{start:08X}",
            "--stop-address", f"0x{stop:08X}", str(image_path),
        ]
        proc = subprocess.run(command, capture_output=True, text=True)
        if proc.returncode != 0:
            continue
        observations = []
        disasm.normalize_objdump(proc.stdout, row["target_id"], known,
                                 symbols=hand, observations=observations)
        yield row["target_id"], observations


def _citations(items):
    """Each deriving instruction once, in (target, label, mnemonic) order."""
    unique = sorted({(c["target"], c["label"], c["mnemonic"]) for c in items})
    return [{"target": t, "label": l, "mnemonic": m} for t, l, m in unique]


def build_table(scans, hand_table, function_addresses=frozenset()):
    """``(symbols, omitted)`` per contract §8-§9 from ``(target_id,
    observations)`` pairs.  Deterministic regardless of scan order.  Omitted
    (with reason): hand-table collisions, non-RAM constants, and formed
    addresses that are known function entries (function pointers, not
    data)."""
    entries = defaultdict(lambda: {"accesses": [], "formations": []})
    for target_id, observations in scans:
        for item in observations:
            if item["symbol"] is not None:
                continue                      # already named by the hand table
            citation = {"target": target_id, "label": f".L{item['vaddr']:08X}",
                        "mnemonic": item["mnemonic"]}
            bucket = "accesses" if item["kind"] == "access" else "formations"
            entries[item["address"]][bucket].append(citation)
    symbols, omitted = {}, {}
    for address in sorted(entries):
        entry = entries[address]
        key = f"{address:08X}"
        if address in hand_table:
            omitted[key] = {"reason": "hand_table", "name": hand_table[address]}
            continue
        if not (RAM_LOW <= address < RAM_HIGH):
            omitted[key] = {"reason": "not_ram"}
            continue
        if address in function_addresses:
            omitted[key] = {"reason": "function_address"}
            continue
        accesses = _citations(entry["accesses"])
        formations = _citations(entry["formations"])
        symbol_type, conflicts = choose_type(c["mnemonic"] for c in accesses)
        symbols[key] = {
            "name": f"D_{key}",
            "type": symbol_type,
            "accesses": accesses,
            "formations": formations,
            "conflicts": conflicts,
        }
    return symbols, omitted


def render(symbols, omitted, stamp):
    data = {"stamp": stamp, "symbols": symbols, "omitted": omitted,
            "counts": {"symbols": len(symbols), "omitted": len(omitted)}}
    return json.dumps(data, indent=2, sort_keys=True) + "\n"


def generate(data=DEFAULT_DATA, output=OUTPUT, image_path=disasm.GAME_CODE_BIN):
    from ..coordinator import db as dbmod

    conn = dbmod.connect(Path(data) / "conveyor.db")
    hand = disasm.symbol_table(include_generated=False)
    functions = frozenset(row[0] for row in conn.execute(
        "SELECT address FROM n64_target WHERE address IS NOT NULL"))
    symbols, omitted = build_table(scan_rows(conn, image_path), hand, functions)
    stamp = {
        "image_sha": disasm._sha256(image_path),
        "hand_table_sha": hashlib.sha256(json.dumps(
            sorted(hand.items()), separators=(",", ":")).encode()).hexdigest(),
        "derivation_version": DERIVATION_VERSION,
        "disasm_derivation_version": disasm.DERIVATION_VERSION,
    }
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(render(symbols, omitted, stamp))
    types = defaultdict(int)
    for entry in symbols.values():
        types[entry["type"]] += 1
    print(f"datasyms: {len(symbols)} symbols "
          + " ".join(f"{k}={v}" for k, v in sorted(types.items()))
          + f"; conflicts={sum(1 for e in symbols.values() if e['conflicts'] and e['conflicts'] != ['formation_only'])}"
          + f" formation_only={sum(1 for e in symbols.values() if e['conflicts'] == ['formation_only'])}"
          + f" omitted={len(omitted)}; -> {output}")
    return symbols, omitted


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("generate")
    p.add_argument("--output", default=str(OUTPUT))
    p.add_argument("--image", default=str(disasm.GAME_CODE_BIN))
    args = parser.parse_args()
    try:
        generate(args.data, args.output, args.image)
    except FileNotFoundError as exc:
        sys.exit(f"datasyms: {exc}")


if __name__ == "__main__":
    main()
