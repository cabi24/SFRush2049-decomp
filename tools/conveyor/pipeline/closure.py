"""Callee closure: register the in-blob call targets the work inventory missed.

    python3 -m tools.conveyor.pipeline.closure run [--data DIR] [--report PATH]

Contract: specs/007-population-closure/contracts/closure-and-datasyms.md §1-§6.

Discovery decodes ``j``/``jal`` straight from the raw words of every
gate-passed extracted extent (no objdump, no m2c — total even for targets
whose derivation fails).  An unknown, aligned, in-blob target is a candidate;
each candidate passes through the 005 extent scanner and, when it survives,
registers through the same carve/assemble/store path as 005 registration with
``gate_reason='discovered'``.  Newly registered targets seed the next
iteration until an iteration registers nothing (caps: 10 iterations, 2000
registrations — a cap is an explicit ``cap_hit`` outcome, never silent).

Registration only ever INSERTs new rows, so the 003 supersession rule can
never fire as a side effect (contract §5).  The one value-level change to
existing rows is the 005 rule applied in the other direction (amendment
recorded in quickstart §1): an inventory row whose address lies strictly
inside a newly registered extent is a function *suffix* (the inventory's
prologue scan started late), and is marked ``extent_conflict:<func_id>`` —
object and evidence untouched.  ``targets._extent_plan`` honours discovered
extents the same way, so a later ``matrix extract`` agrees.
"""
import argparse
import datetime
import hashlib
import json
import struct
import subprocess
import sys
import tempfile
from collections import Counter
from pathlib import Path

from ..client import DEFAULT_DATA
from . import targets as targetsmod

REPO = Path(__file__).resolve().parents[3]
GAME_CODE_BASE = targetsmod.GAME_CODE_BASE
GAME_CODE_BIN = targetsmod.GAME_CODE_BIN
REPORT = REPO / "build" / "closure_report.json"

MAX_ITERATIONS = 10
MAX_REGISTRATIONS = 2000
GATE_REASON = "discovered"

OUTCOMES = ("registered", "inside_existing_extent", "scan_failure",
            "invalid", "cap_hit")


# --- discovery ---------------------------------------------------------------

def decode_jumps(words, address):
    """``[(insn_vaddr, target)]`` for every ``j`` (opcode 2) / ``jal`` (opcode
    3) in ``words`` (ints) laid out from ``address``.  Absolute target per
    contract §1: ``((word & 0x3FFFFFF) << 2) | (pc & 0xF0000000)``."""
    found = []
    for index, word in enumerate(words):
        if (word >> 26) in (2, 3):
            pc = address + 4 * index
            found.append((pc, ((word & 0x3FFFFFF) << 2) | (pc & 0xF0000000)))
    return found


def _words_at(image, address, insn_count):
    offset = address - GAME_CODE_BASE
    chunk = image[offset: offset + 4 * insn_count]
    return [word for (word,) in struct.iter_unpack(">I", chunk)]


def gate_passed_rows(conn):
    """Extracted rows whose extent passed the 005 gate (contract §1)."""
    rows = conn.execute(
        "SELECT target_id,address,insn_count,gate_reason FROM n64_target"
        " WHERE population='extracted' AND address IS NOT NULL"
        " AND insn_count IS NOT NULL ORDER BY address, target_id"
    ).fetchall()
    return [row for row in rows if _gate_passed(row["gate_reason"])]


def _gate_passed(gate_reason):
    reason = gate_reason or ""
    return not (reason.startswith("extent_conflict")
                or reason.startswith("scan_overrun"))


def discover(sources, image, known_addresses, blob_end):
    """Candidates called from ``sources`` (rows with address/insn_count/
    target_id): ``{target_address: provenance}`` where provenance is the first
    discovering call site in (source address, call vaddr) order.  Static-range
    targets (below the blob) are never candidates; anything else unknown is,
    including misaligned/out-of-image addresses, which classify as
    ``invalid`` downstream so the report stays total."""
    candidates = {}
    for row in sorted(sources, key=lambda r: (r["address"], r["target_id"])):
        words = _words_at(image, row["address"], row["insn_count"])
        for vaddr, target in decode_jumps(words, row["address"]):
            if target < GAME_CODE_BASE or target in known_addresses:
                continue
            if target not in candidates:
                candidates[target] = {"target_id": row["target_id"],
                                      "vaddr": vaddr}
    return dict(sorted(candidates.items()))


# --- gate + register ---------------------------------------------------------

def classify(address, image, extents):
    """One outcome per candidate (contract §2).  ``extents`` is the list of
    registered ``(start, end, target_id)`` triples.  Returns
    ``(outcome, detail)`` where detail carries ``insn_count`` for a
    registrable candidate or the reason otherwise."""
    blob_end = GAME_CODE_BASE + len(image)
    if address % 4 or not (GAME_CODE_BASE <= address < blob_end):
        return "invalid", {"reason": "misaligned" if address % 4
                           else "outside_image"}
    container = [t for (start, end, t) in extents if start < address < end]
    if container:
        return "inside_existing_extent", {"container": sorted(container)[0]}
    try:
        scanned = targetsmod.scan_extent(image, address)
    except ValueError as exc:
        return "invalid", {"reason": str(exc)}
    if not isinstance(scanned, int):
        return "scan_failure", {"reason": scanned}
    end = address + 4 * scanned
    overlaps = sorted(t for (start, _e, t) in extents if address < start < end)
    detail = {"insn_count": scanned}
    if overlaps:
        # Same precedent as 005: the outer extent keeps its scan; overlap is
        # recorded, not hidden.
        detail["overlaps"] = overlaps
    return "registered", detail


def register(conn, store, image, address, insn_count, tmpdir):
    """Insert one discovered target through the 005 raw-word path.  Returns
    the new target_id.  INSERT only — an existing row is a caller bug."""
    from ..coordinator import db as dbmod

    target_id = f"func_{address:08X}"
    words = [f"{w:08X}" for w in _words_at(image, address, insn_count)]
    if len(words) != insn_count:
        raise ValueError(f"{target_id}: extent runs past the image")
    o_path = Path(tmpdir) / f"{target_id}.o"
    targetsmod.assemble_words(words, o_path, target_id)
    o_sha = store.put_file(o_path)
    asm_sha = hashlib.sha256("\n".join(words).encode()).hexdigest()
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO n64_target (target_id, address, population,"
            " insn_count, target_asm_sha, target_o_sha, tier, gate_reason)"
            " VALUES (?, ?, 'extracted', ?, ?, ?, 'raw_word', ?)",
            (target_id, address, insn_count, asm_sha, o_sha, GATE_REASON),
        )
        conn.execute(
            "INSERT INTO function_status (target_id, status, updated_at)"
            " VALUES (?, 'unmatched', strftime('%Y-%m-%dT%H:%M:%fZ','now'))"
            " ON CONFLICT(target_id) DO NOTHING",
            (target_id,),
        )
        conn.execute(
            "INSERT OR IGNORE INTO blob (sha256, kind, size_bytes, created_at)"
            " VALUES (?, 'target', ?, strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
            (o_sha, store.size(o_sha) or 0),
        )
    return target_id


def reclassify_contained(conn, discovered):
    """Mark existing extracted rows that start strictly inside a discovered
    extent as ``extent_conflict:<discovered id>`` (tightest container).  Rows
    already carrying an extent_conflict, and discovered rows themselves, are
    left alone.  Returns ``{target_id: {container, previous_gate_reason}}``;
    idempotent (a second run finds nothing to change)."""
    from ..coordinator import db as dbmod

    if not discovered:
        return {}
    rows = conn.execute(
        "SELECT target_id,address,gate_reason FROM n64_target"
        " WHERE population='extracted' AND address IS NOT NULL"
        " AND (gate_reason IS NULL OR gate_reason NOT LIKE 'extent_conflict%')"
        " AND (gate_reason IS NULL OR gate_reason != ?) ORDER BY address, target_id",
        (GATE_REASON,)).fetchall()
    changed = {}
    with dbmod.tx(conn):
        for row in rows:
            containers = [(end - start, start, t) for (start, end, t) in discovered
                          if start < row["address"] < end]
            if not containers:
                continue
            container = min(containers)[2]
            conn.execute("UPDATE n64_target SET gate_reason=? WHERE target_id=?",
                         (f"extent_conflict:{container}", row["target_id"]))
            changed[row["target_id"]] = {
                "container": container, "address": f"{row['address']:08X}",
                "previous_gate_reason": row["gate_reason"]}
    return changed


# --- fixpoint ----------------------------------------------------------------

def run(conn, store, image_path=GAME_CODE_BIN, report_path=REPORT,
        max_iterations=MAX_ITERATIONS, max_registrations=MAX_REGISTRATIONS):
    """Closure to fixpoint; writes and returns the report (contract §3-§4)."""
    image_path = Path(image_path)
    image = image_path.read_bytes()
    blob_end = GAME_CODE_BASE + len(image)

    known = {row["address"] for row in conn.execute(
        "SELECT address FROM n64_target WHERE address IS NOT NULL")}
    extents = [(row["address"], row["address"] + 4 * row["insn_count"],
                row["target_id"]) for row in gate_passed_rows(conn)]
    sources = list(gate_passed_rows(conn))

    candidates = {}      # addr -> report entry, in discovery order per addr
    iterations = []
    total_registered = 0
    caps = {"iterations": {"limit": max_iterations, "hit": False},
            "registrations": {"limit": max_registrations, "hit": False}}

    with tempfile.TemporaryDirectory(prefix="closure-") as tmpdir:
        iteration = 0
        while True:
            iteration += 1
            discovered = discover(sources, image, known, blob_end)
            counts = Counter()
            registered_rows = []
            for address, provenance in discovered.items():
                entry = {"discovered_by": provenance, "iteration": iteration}
                if total_registered >= max_registrations:
                    caps["registrations"]["hit"] = True
                    outcome, detail = "cap_hit", {"reason": "registrations"}
                else:
                    outcome, detail = classify(address, image, extents)
                if outcome == "registered":
                    target_id = register(conn, store, image, address,
                                         detail["insn_count"], tmpdir)
                    known.add(address)
                    extents.append((address, address + 4 * detail["insn_count"],
                                    target_id))
                    registered_rows.append({
                        "target_id": target_id, "address": address,
                        "insn_count": detail["insn_count"]})
                    detail = dict(detail, target_id=target_id)
                    total_registered += 1
                entry["outcome"] = outcome
                entry.update(detail)
                candidates[address] = entry
                counts[outcome] += 1
            iterations.append({
                "iteration": iteration, "sources": len(sources),
                "discovered": len(discovered), "outcomes": dict(sorted(counts.items())),
            })
            if not registered_rows:
                break
            if iteration >= max_iterations:
                # Explicit: what the next iteration would have examined.
                caps["iterations"]["hit"] = True
                pending = discover(registered_rows, image, known, blob_end)
                for address, provenance in pending.items():
                    candidates[address] = {
                        "discovered_by": provenance, "iteration": iteration + 1,
                        "outcome": "cap_hit", "reason": "iterations"}
                if pending:
                    iterations.append({
                        "iteration": iteration + 1, "sources": len(registered_rows),
                        "discovered": len(pending),
                        "outcomes": {"cap_hit": len(pending)}})
                break
            sources = registered_rows

    # Every closure-registered extent (this run and earlier ones) is a
    # container for the suffix rule; the step is a no-op when nothing changed.
    discovered_extents = [
        (row["address"], row["address"] + 4 * row["insn_count"], row["target_id"])
        for row in conn.execute(
            "SELECT target_id,address,insn_count FROM n64_target"
            " WHERE population='extracted' AND gate_reason=?", (GATE_REASON,))]
    reclassified = reclassify_contained(conn, discovered_extents)

    ordered = {f"{addr:08X}": candidates[addr] for addr in sorted(candidates)}
    for entry in ordered.values():
        entry["discovered_by"] = {
            "target_id": entry["discovered_by"]["target_id"],
            "vaddr": f"{entry['discovered_by']['vaddr']:08X}"}
    totals = Counter(entry["outcome"] for entry in ordered.values())
    report = {
        "stamp": {
            "image_sha": hashlib.sha256(image).hexdigest(),
            "image_size": len(image),
            "timestamp": datetime.datetime.now(datetime.timezone.utc).isoformat(),
        },
        "caps": caps,
        "iterations": iterations,
        "totals": {name: totals.get(name, 0) for name in OUTCOMES},
        "reclassified": dict(sorted(reclassified.items())),
        "candidates": ordered,
    }
    report_path = Path(report_path)
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    return report


# --- orphan functions ----------------------------------------------------------

ORPHAN_RUN_CAP = 4096       # bytes; larger opaque runs are data (the image tail)
_VALID_OPCODE_CHECK = None


def _decodes_as_code(image, address, count):
    """Every word disassembles to a real instruction (no .word/(bad))."""
    import re
    off = address - GAME_CODE_BASE
    with tempfile.NamedTemporaryFile(suffix=".bin") as f:
        f.write(image[off:off + 4 * count])
        f.flush()
        out = subprocess.run(
            ["mips-linux-gnu-objdump", "-D", "-b", "binary", "-m", "mips:4300",
             "-EB", f.name], capture_output=True, text=True).stdout
    return not re.search(r"\.word|\(bad\)|unknown", out)


def _branch_lands_in(image, lo, hi, reach=8192):
    """A PC-relative branch in the `reach` bytes before `lo` targets [lo, hi)."""
    for addr in range(max(GAME_CODE_BASE, lo - reach), lo, 4):
        word = struct.unpack_from(">I", image, addr - GAME_CODE_BASE)[0]
        op = word >> 26
        branch = op in {0x01, 0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17} or (
            op in {0x10, 0x11, 0x12} and ((word >> 21) & 0x1F) == 0x08)
        if not branch:
            continue
        imm = word & 0xFFFF
        imm -= 0x10000 if imm & 0x8000 else 0
        if lo <= addr + 4 + imm * 4 < hi:
            return True
    return False


def find_orphans(image, rows, run_cap=ORPHAN_RUN_CAP):
    """Complete functions sitting in the gaps between registered extents.

    Nothing calls them (they are not jal targets, so the callee closure never
    finds them), but they are real code: most are empty functions
    (`jr $ra; nop`) and small setters/clearers. A gap of at most `run_cap`
    bytes is walked from its start: skip zero padding; the next word must not
    be reachable by fall-through (targets.falls_through_into); the 005 extent
    scanner must end inside the gap; every word must decode as an
    instruction; and no branch in the preceding code may land inside. Each
    accepted function continues the walk after its end. Returns
    [(address, insn_count)]."""
    image_end = GAME_CODE_BASE + len(image)
    extents = sorted((r["address"], r["address"] + 4 * r["insn_count"]) for r in rows)
    gaps, cursor = [], GAME_CODE_BASE
    for lo, hi in extents:
        if lo > cursor:
            gaps.append((cursor, lo))
        cursor = max(cursor, hi)
    if cursor < image_end:
        gaps.append((cursor, image_end))
    word_at = lambda a: struct.unpack_from(">I", image, a - GAME_CODE_BASE)[0]
    found = []
    for start, end in gaps:
        if end - start > run_cap:
            continue
        p = start
        while p < end:
            while p < end and word_at(p) == 0:
                p += 4
            if p >= end or targetsmod.falls_through_into(image, p):
                break
            try:
                count = targetsmod.scan_extent(image, p)
            except ValueError:
                break
            if not isinstance(count, int) or p + 4 * count > end:
                break
            if not _decodes_as_code(image, p, count):
                break
            if not _branch_lands_in(image, p, p + 4 * count):
                found.append((p, count))
            p += 4 * count
    return found


def main():
    from ..coordinator import db as dbmod
    from ..coordinator.store import BlobStore

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("run")
    p.add_argument("--image", default=str(GAME_CODE_BIN))
    p.add_argument("--report", default=str(REPORT))
    o = sub.add_parser("orphans", help="register complete uncalled functions in "
                                       "the gaps between extents")
    o.add_argument("--image", default=str(GAME_CODE_BIN))
    o.add_argument("--apply", action="store_true")
    args = parser.parse_args()

    data = Path(args.data)
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    if args.command == "orphans":
        image = Path(args.image).read_bytes()
        found = find_orphans(image, gate_passed_rows(conn))
        known = {r[0] for r in conn.execute("SELECT address FROM n64_target")}
        new = [(a, n) for a, n in found if a not in known]
        print(f"orphan functions: {len(new)} ({sum(n for _, n in new)} insns); "
              f"{sum(1 for _, n in new if n == 2)} are 2-instruction bodies")
        if not args.apply:
            print("dry run; pass --apply to register them")
            return
        with tempfile.TemporaryDirectory() as tmp:
            names = [register(conn, store, image, a, n, tmp) for a, n in new]
        print(f"registered {len(names)} targets (gate_reason={GATE_REASON!r})")
        return
    try:
        report = run(conn, store, args.image, args.report)
    except subprocess.CalledProcessError as exc:
        sys.exit(f"closure: assembler failed: {exc}")
    for item in report["iterations"]:
        print(f"iteration {item['iteration']}: sources={item['sources']} "
              f"discovered={item['discovered']} {item['outcomes']}")
    print("totals: " + "  ".join(f"{k}={v}" for k, v in report["totals"].items()))
    print(f"reclassified as extent_conflict (inventory rows inside a discovered"
          f" extent): {len(report['reclassified'])}")
    hit = [name for name, cap in report["caps"].items() if cap["hit"]]
    print(f"caps hit: {', '.join(hit) if hit else 'none'}; report -> {args.report}")


if __name__ == "__main__":
    main()
