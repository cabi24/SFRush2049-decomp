"""Re-register static inventory targets under their current layout names.

    python3 -m tools.conveyor.pipeline.targets_rename audit
    python3 -m tools.conveyor.pipeline.targets_rename apply NAME [NAME...] \
        [--retire TARGET_ID ...] [--absorbed TARGET_ID ...] [--data DIR] [--lockfile PATH] [--dry-run]
    python3 -m tools.conveyor.pipeline.targets_rename rename MAP.json \
        [--data DIR] [--lockfile PATH] [--dry-run]

The conveyor inventory (`n64_target`) was populated from the historical
`work/**/info.txt` labels; the S12 census renamed boot functions in
symbol_addrs and the layout, but the inventory still carries some old names
(and two rows that are not functions at all). `lock add --target` scores
against an inventory row, and promotion keys the segment flagset by the lock's
target_id, so a promotion needs a row under the slot's current name.

`audit` (read-only) lists every static row whose name, address or extent
disagrees with `layout.derive()`.

`apply NAME...` takes each NAME's address and tiled extent from
`layout.derive()` (no info.txt), builds its reloc-aware target object the way
`targets.populate` does for static targets (asm region -> assemble -> 003
round-trip gate; a gate failure refuses, it never falls back to raw words),
and upserts the row. A row registered under another name at NAME's address is
retired (the rename); a row named NAME at another address is re-addressed.
`--retire ID` additionally retires a row that is not a function start in the
layout (a fragment or a mid-function label). Retiring deletes the n64_target
row, its matrix_entry evidence and its function_status row.

Refusals (nothing is written): NAME absent or ambiguous in the layout; a row to
retire that a matched.lock.json entry targets, that has a promotion_record, an
open work_unit, a non-static population, or whose name is itself a current
layout function; a `--retire` row that starts a layout function; no asm region
or a failed gate for NAME. All writes happen in one transaction, and every row
not named in the plan is left untouched. `--dry-run` builds the objects in a
temporary store and prints the exact plan without writing anything.

`--absorbed ID` (w24s) retires a row whose slot the layout has folded into a
function named in the same plan: a bogus split (the "viStub" slot at
0x800015DC was viDeadlinePassed's trailing `jr ra; nop` pairs at -g1 -O1). The
row may carry promotion_record history (it was promoted as its own slot); the
records are kept with outcome 'promoted' -> 'retired', so the history stays
and nothing counts it as promoted. Refusals as for `--retire` otherwise: the
row's address must lie strictly inside a layout function that this plan
registers or refreshes (so that function's object is rebuilt over the absorbed
bytes), no lock entry may target it (remove it from matched.lock.json first),
no open work unit, static population, not itself a layout function.

`rename MAP.json` renames rows IN PLACE (static_rename's table: accepted rows
`{vram, old, new}`), keeping their history: every table keyed by target_id
(n64_target, function_status, matrix_entry, work_unit, promotion_record) moves
from the old to the new id, through temporary ids in one transaction, so the
names may form cycles (osSendMesg <-> osJamMesg) that `apply` refuses. Each
renamed row's reloc-aware object is rebuilt under its new name (the object
carries the symbol), and a changed object supersedes its matrix evidence as in
`apply`. Refusals (nothing is written): a new name that is not the layout's
name at the row's vram; an old row missing, not static, or at another address;
a new name already registered and not itself renamed away by the table; an open
work unit on a renamed row; a lock entry still targeting an old name that is
not also a new name; a failed object gate.
"""
import argparse
import hashlib
import json
import sys
import tempfile
from pathlib import Path

from . import targets as targetsmod

REPO = targetsmod.REPO
LOCKFILE = REPO / "matched.lock.json"
OPEN_WORK_STATES = ("PENDING", "LEASED", "RUNNING")


class RenameRefusal(Exception):
    """The plan cannot be applied safely; nothing was written."""


# --- inputs -----------------------------------------------------------------

def layout_functions(mapping):
    """{name: [(vaddr, size, segment)]} over every derivable static segment."""
    out = {}
    for seg in mapping["segments"]:
        if seg.get("refusal"):
            continue
        for f in seg["functions"]:
            out.setdefault(f["name"], []).append(
                (int(f["vaddr"], 16), int(f["size"]), seg["yaml_name"]))
    return out


def layout_starts(functions):
    """{vaddr: (name, size)} from layout_functions()."""
    return {v: (name, size) for name, places in functions.items()
            for v, size, _seg in places}


def lock_targets(path=None):
    """{target_id: [lock spec, ...]} from matched.lock.json."""
    path = Path(path or LOCKFILE)
    out = {}
    if path.is_file():
        for spec, entry in json.loads(path.read_text()).items():
            tid = entry.get("target_id") or spec.rpartition(":")[2]
            out.setdefault(tid, []).append(spec)
    return out


def _row(conn, target_id):
    r = conn.execute("SELECT * FROM n64_target WHERE target_id=?", (target_id,)).fetchone()
    return dict(r) if r else None


# --- audit ------------------------------------------------------------------

def audit(conn, functions, regions=None):
    """[(target_id, address, problem)] for static rows disagreeing with the layout.

    An insn_count is accepted when it equals either the tiled layout extent or
    the asm region's instruction count (the extent less alignment padding)."""
    starts = layout_starts(functions)
    regions = regions or {}
    problems = []
    for r in conn.execute("SELECT target_id, address, insn_count FROM n64_target"
                          " WHERE population='static' ORDER BY address"):
        tid, addr, n = r["target_id"], r["address"], r["insn_count"]
        here = starts.get(addr)
        if here is None:
            inside = [(name, v) for v, (name, size) in starts.items() if v < addr < v + size]
            where = (f"inside {inside[0][0]} @0x{inside[0][1]:08X}" if inside
                     else "no layout function there")
            problems.append((tid, addr, f"not a layout function start ({where})"))
        elif here[0] != tid:
            problems.append((tid, addr, f"layout name is {here[0]}"))
        elif n is None or (n * 4 != here[1] and (addr not in regions
                                                   or n != len(regions[addr].words))):
            problems.append((tid, addr, f"insn_count {n} != layout extent {here[1] // 4}"))
    return problems


# --- plan -------------------------------------------------------------------

def _check_retirable(conn, row, locks, functions, why, allow_promoted=False):
    tid = row["target_id"]
    if row["population"] != "static":
        raise RenameRefusal(f"{tid}: population {row['population']!r}, only static rows are handled")
    if tid in locks:
        raise RenameRefusal(f"{tid}: referenced by lock(s) {', '.join(sorted(locks[tid]))}; "
                            f"would retire it ({why})")
    if tid in functions:
        raise RenameRefusal(f"{tid}: is a current layout function name; would retire it ({why})")
    if not allow_promoted and conn.execute(
            "SELECT 1 FROM promotion_record WHERE target_id=?", (tid,)).fetchone():
        raise RenameRefusal(f"{tid}: has a promotion_record; would retire it ({why})")
    q = ",".join("?" * len(OPEN_WORK_STATES))
    if conn.execute(f"SELECT 1 FROM work_unit WHERE target_id=? AND state IN ({q})",
                    (tid,) + OPEN_WORK_STATES).fetchone():
        raise RenameRefusal(f"{tid}: has open work units; would retire it ({why})")


def plan(conn, functions, names, retire=(), locks=None, absorbed=()):
    """The ordered list of actions; raises RenameRefusal on any unsafe step.

    Each action is a dict with `op` in register | readdress | refresh | keep |
    retire, `target_id`, and for the first four `address` / `size`. A retire
    of an `absorbed` row also carries `absorbed_by` (the planned function
    whose extent now covers it)."""
    locks = {} if locks is None else locks
    starts = layout_starts(functions)
    actions, retiring = [], {}
    names = list(dict.fromkeys(names))
    both = sorted(set(retire) & set(absorbed))
    if both:
        raise RenameRefusal(f"{both[0]}: listed as both retired and absorbed")
    for name in names:
        places = functions.get(name)
        if not places:
            raise RenameRefusal(f"{name}: not a function in the derived layout")
        if len(places) > 1:
            raise RenameRefusal(f"{name}: {len(places)} layout functions carry this name")
        vaddr, size, seg = places[0]
        # Rows registered at this address under another name: the rename.
        for r in conn.execute("SELECT * FROM n64_target WHERE address=? AND target_id<>?",
                              (vaddr, name)):
            r = dict(r)
            why = f"renamed to {name} @0x{vaddr:08X}"
            _check_retirable(conn, r, locks, functions, why)
            retiring[r["target_id"]] = (r, why)
        row = _row(conn, name)
        if row is not None and row["population"] != "static":
            raise RenameRefusal(f"{name}: existing row has population {row['population']!r}")
        if row is None:
            op, note = "register", "new row"
        elif row["address"] != vaddr:
            old = starts.get(row["address"])
            if old is not None and old[0] != name:
                raise RenameRefusal(
                    f"{name}: existing row sits at 0x{row['address']:08X}, the start of "
                    f"layout function {old[0]}; register {old[0]} first")
            op, note = "readdress", f"was 0x{row['address']:08X} ({row['insn_count']} insns)"
        else:
            op, note = "refresh", f"was {row['insn_count']} insns"
        actions.append({"op": op, "target_id": name, "address": vaddr, "size": size,
                        "segment": seg, "note": note})
    for tid in dict.fromkeys(retire):
        if tid in retiring:
            continue
        row = _row(conn, tid)
        if row is None:
            raise RenameRefusal(f"{tid}: no such inventory row to retire")
        if row["address"] in starts:
            raise RenameRefusal(f"{tid}: 0x{row['address']:08X} starts layout function "
                                f"{starts[row['address']][0]}; rename it instead of retiring")
        inside = [(n, v) for v, (n, s) in starts.items() if v < row["address"] < v + s]
        why = (f"mid-function: inside {inside[0][0]} @0x{inside[0][1]:08X}" if inside
               else "no layout function at this address")
        _check_retirable(conn, row, locks, functions, why)
        retiring[tid] = (row, why)
    absorbed_by = {}
    for tid in dict.fromkeys(absorbed):
        row = _row(conn, tid)
        if row is None:
            raise RenameRefusal(f"{tid}: no such inventory row to absorb")
        if row["address"] in starts:
            raise RenameRefusal(f"{tid}: 0x{row['address']:08X} starts layout function "
                                f"{starts[row['address']][0]}; it is not absorbed")
        inside = [(n, v) for v, (n, s) in starts.items() if v < row["address"] < v + s]
        if not inside or inside[0][0] not in names:
            raise RenameRefusal(
                f"{tid}: 0x{row['address']:08X} is not inside a layout function this plan "
                f"registers or refreshes" + (f" (inside {inside[0][0]}: name it too)" if inside else ""))
        owner, start = inside[0]
        why = f"absorbed into {owner} @0x{start:08X}"
        _check_retirable(conn, row, locks, functions, why, allow_promoted=True)
        retiring[tid] = (row, why)
        absorbed_by[tid] = owner
    renamed = {a["target_id"] for a in actions}
    for tid, (row, why) in sorted(retiring.items()):
        if tid in renamed:
            raise RenameRefusal(f"{tid}: both registered and retired by this plan")
        action = {"op": "retire", "target_id": tid, "address": row["address"],
                  "size": (row["insn_count"] or 0) * 4, "note": why}
        if tid in absorbed_by:
            action["absorbed_by"] = absorbed_by[tid]
        actions.append(action)
    return actions


# --- objects ----------------------------------------------------------------

def build_object(region, name, out_o):
    """Reloc-aware target object for one region (the static populate path).
    Returns the out path; raises RenameRefusal if it does not gate."""
    try:
        targetsmod.assemble_region(region, name, out_o)
    except targetsmod.AssembleError as exc:
        raise RenameRefusal(f"{name}: assemble_error: {exc}")
    ok, reason = targetsmod.gate_target(region.words, out_o)
    if not ok:
        raise RenameRefusal(f"{name}: round-trip gate failed: {reason}")
    return out_o


def build_objects(actions, regions, store, builder=None, words_of=None):
    """Attach `o_sha`, `asm_sha`, `insn_count` to every non-retire action."""
    builder = builder or build_object
    words_of = words_of or targetsmod.function_words
    with tempfile.TemporaryDirectory() as tmp:
        for i, a in enumerate(actions):
            if a["op"] == "retire":
                continue
            region = regions.get(a["address"])
            if region is None:
                raise RenameRefusal(f"{a['target_id']}: no asm region at 0x{a['address']:08X}")
            if len(region.words) * 4 > a["size"]:
                raise RenameRefusal(f"{a['target_id']}: asm region exceeds the layout extent")
            out_o = Path(tmp) / f"{i}.o"
            builder(region, a["target_id"], out_o)
            # The instruction body (the region, without the slot's alignment
            # padding) is the extent the target object and its ROM words cover.
            nbytes = len(region.words) * 4
            words = words_of(a["address"], nbytes)
            if words != [w.upper() for w in region.words]:
                raise RenameRefusal(f"{a['target_id']}: ROM words differ from the asm region")
            if len(words) * 4 != nbytes:
                raise RenameRefusal(f"{a['target_id']}: ROM image short at 0x{a['address']:08X}")
            a["o_sha"] = store.put_file(out_o)
            a["o_size"] = out_o.stat().st_size
            a["asm_sha"] = hashlib.sha256("\n".join(words).encode()).hexdigest()
            a["insn_count"] = len(words)
    return actions


# --- apply ------------------------------------------------------------------

def apply(conn, actions):
    """Write the planned actions in one transaction. Returns purged evidence rows."""
    from ..coordinator import db as dbmod

    purged = 0
    with dbmod.tx(conn):
        for a in actions:
            tid = a["target_id"]
            if a["op"] == "retire":
                cur = conn.execute("DELETE FROM matrix_entry WHERE target_id=?", (tid,))
                purged += max(cur.rowcount, 0)
                conn.execute("DELETE FROM function_status WHERE target_id=?", (tid,))
                conn.execute("DELETE FROM n64_target WHERE target_id=?", (tid,))
                if a.get("absorbed_by"):
                    conn.execute("UPDATE promotion_record SET outcome='retired'"
                                 " WHERE target_id=? AND outcome='promoted'", (tid,))
                continue
            prev = _row(conn, tid)
            changed, n = targetsmod._supersede_target(
                conn, tid, prev["target_o_sha"] if prev else None, a["o_sha"])
            purged += n
            if prev is None:
                conn.execute(
                    "INSERT INTO n64_target (target_id, address, population, insn_count,"
                    " target_asm_sha, target_o_sha, tier, gate_reason)"
                    " VALUES (?, ?, 'static', ?, ?, ?, 'reloc_aware', NULL)",
                    (tid, a["address"], a["insn_count"], a["asm_sha"], a["o_sha"]))
            else:
                # A re-addressed row described another body: its similarity
                # fingerprint and cluster no longer apply.
                stale = prev["address"] != a["address"]
                conn.execute(
                    "UPDATE n64_target SET address=?, insn_count=?, target_asm_sha=?,"
                    " target_o_sha=?, tier='reloc_aware', gate_reason=NULL"
                    + (", fingerprint=NULL, cluster_id=NULL" if stale else "")
                    + " WHERE target_id=?",
                    (a["address"], a["insn_count"], a["asm_sha"], a["o_sha"], tid))
            conn.execute(
                "INSERT INTO function_status (target_id, status, updated_at)"
                " VALUES (?, 'unmatched', strftime('%Y-%m-%dT%H:%M:%fZ','now'))"
                " ON CONFLICT(target_id) DO NOTHING", (tid,))
            conn.execute(
                "INSERT OR IGNORE INTO blob (sha256, kind, size_bytes, created_at)"
                " VALUES (?, 'target', ?, strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
                (a["o_sha"], a["o_size"]))
    return purged


def format_plan(conn, actions):
    lines = []
    for a in actions:
        tid = a["target_id"]
        if a["op"] == "retire":
            n = {t: conn.execute(f"SELECT COUNT(*) FROM {t} WHERE target_id=?", (tid,)).fetchone()[0]
                 for t in ("matrix_entry", "function_status")}
            line = (f"retire    {tid:24} @0x{a['address']:08X}  {a['note']}; deletes n64_target 1,"
                    f" matrix_entry {n['matrix_entry']}, function_status {n['function_status']}")
            if a.get("absorbed_by"):
                k = conn.execute("SELECT COUNT(*) FROM promotion_record WHERE target_id=?"
                                 " AND outcome='promoted'", (tid,)).fetchone()[0]
                line += f"; promotion_record {k} promoted -> retired (kept)"
            lines.append(line)
            continue
        prev = _row(conn, tid)
        sup = ""
        if prev and prev["target_o_sha"] != a.get("o_sha"):
            n = conn.execute("SELECT COUNT(*) FROM matrix_entry WHERE target_id=?",
                             (tid,)).fetchone()[0]
            sup = f"; object changes, supersedes {n} matrix_entry rows"
        elif prev:
            sup = "; object unchanged"
        lines.append(f"{a['op']:9} {tid:24} @0x{a['address']:08X} {a.get('insn_count', a['size'] // 4):4} insns"
                     f" seg {a['segment']:8} reloc_aware o={a.get('o_sha', '?')[:12]}  ({a['note']}{sup})")
    return lines


# --- in-place rename ---------------------------------------------------------

_KEYED_TABLES = ("n64_target", "function_status", "matrix_entry", "work_unit",
                 "promotion_record")


def rename_plan(conn, functions, rows, locks=None):
    """[{op: 'rename', old, target_id, address, size, segment}] or RenameRefusal."""
    locks = {} if locks is None else locks
    olds = {o for _v, o, _n in rows}
    news = {n for _v, _o, n in rows}
    actions = []
    for vram, old, new in rows:
        places = functions.get(new) or []
        if len(places) != 1 or places[0][0] != vram:
            raise RenameRefusal(f"{new}: not the layout's single function at 0x{vram:08X}")
        row = _row(conn, old)
        if row is None:
            raise RenameRefusal(f"{old}: no inventory row to rename")
        if row["population"] != "static" or row["address"] != vram:
            raise RenameRefusal(f"{old}: not a static row at 0x{vram:08X}")
        if _row(conn, new) is not None and new not in olds:
            raise RenameRefusal(f"{new}: already registered and not renamed away by this table")
        q = ",".join("?" * len(OPEN_WORK_STATES))
        if conn.execute(f"SELECT 1 FROM work_unit WHERE target_id=? AND state IN ({q})",
                        (old,) + OPEN_WORK_STATES).fetchone():
            raise RenameRefusal(f"{old}: has open work units")
        if old in locks and old not in news:
            raise RenameRefusal(f"{old}: still targeted by lock(s) {', '.join(sorted(locks[old]))};"
                                " migrate the lockfile first (static_rename lock)")
        _vaddr, size, seg = places[0]
        actions.append({"op": "rename", "old": old, "target_id": new, "address": vram,
                        "size": size, "segment": seg, "note": f"was {old}"})
    return actions


def build_rename_objects(conn, actions, regions, store, builder=None, words_of=None):
    """build_objects for a rename, with one allowance: a row whose object had
    already failed the round-trip gate keeps that recorded failure (and its
    columns) under the new name when the rebuild fails the gate too. A rename
    must neither hide a gate failure nor invent a pass; any other refusal, and
    any gate failure of a row that used to pass, still refuses."""
    for a in actions:
        prev = _row(conn, a["old"])
        try:
            build_objects([a], regions, store, builder, words_of)
        except RenameRefusal as exc:
            if not (prev is not None and prev["gate_reason"]
                    and "round-trip gate failed" in str(exc)):
                raise
            a.update(carry=prev["gate_reason"], o_sha=prev["target_o_sha"],
                     insn_count=prev["insn_count"])
    return actions


def apply_rename(conn, actions):
    """Move every keyed row old -> new (via temporary ids), then refresh objects
    (a carried gate failure keeps its row's object columns)."""
    from ..coordinator import db as dbmod

    purged = 0
    with dbmod.tx(conn):
        for a in actions:
            for table in _KEYED_TABLES:
                conn.execute(f"UPDATE {table} SET target_id=? WHERE target_id=?",
                             ("\x00rename\x00" + a["target_id"], a["old"]))
        for a in actions:
            for table in _KEYED_TABLES:
                conn.execute(f"UPDATE {table} SET target_id=? WHERE target_id=?",
                             (a["target_id"], "\x00rename\x00" + a["target_id"]))
        for a in actions:
            if a.get("carry"):
                continue
            tid = a["target_id"]
            prev = _row(conn, tid)
            _changed, n = targetsmod._supersede_target(conn, tid, prev["target_o_sha"], a["o_sha"])
            purged += n
            conn.execute("UPDATE n64_target SET insn_count=?, target_asm_sha=?, target_o_sha=?,"
                         " tier='reloc_aware', gate_reason=NULL WHERE target_id=?",
                         (a["insn_count"], a["asm_sha"], a["o_sha"], tid))
            conn.execute(
                "INSERT OR IGNORE INTO blob (sha256, kind, size_bytes, created_at)"
                " VALUES (?, 'target', ?, strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
                (a["o_sha"], a["o_size"]))
    return purged


def format_rename(conn, actions):
    lines = []
    for a in actions:
        n = {t: conn.execute(f"SELECT COUNT(*) FROM {t} WHERE target_id=?", (a["old"],)).fetchone()[0]
             for t in _KEYED_TABLES}
        prev = _row(conn, a["old"])
        sup = (f"gate failure carried: {a['carry']}" if a.get("carry")
               else "object unchanged" if prev["target_o_sha"] == a["o_sha"]
               else f"object changes, supersedes {n['matrix_entry']} matrix_entry rows")
        lines.append(f"rename    {a['old']:24} -> {a['target_id']:24} @0x{a['address']:08X}"
                     f" {a['insn_count'] or 0:4} insns o={(a['o_sha'] or '-')[:12]}  (function_status"
                     f" {n['function_status']}, work_unit {n['work_unit']}, promotion_record"
                     f" {n['promotion_record']}; {sup})")
    return lines


# --- command ----------------------------------------------------------------

def main(argv=None):
    from ..client import DEFAULT_DATA
    from ..coordinator import db as dbmod
    from ..coordinator.store import BlobStore
    from . import layout as layoutmod

    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--data", default=str(DEFAULT_DATA),
                        help="conveyor data dir holding conveyor.db and blobs/")
    parser.add_argument("--lockfile", default=str(LOCKFILE))
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("audit")
    p = sub.add_parser("apply")
    p.add_argument("names", nargs="+", help="current layout function names to register")
    p.add_argument("--retire", nargs="*", default=[], metavar="TARGET_ID",
                   help="non-function rows to retire (fragments, mid-function labels)")
    p.add_argument("--absorbed", nargs="*", default=[], metavar="TARGET_ID",
                   help="rows whose slot a planned function now covers (may carry promotion history)")
    p.add_argument("--dry-run", action="store_true")
    r = sub.add_parser("rename")
    r.add_argument("map", help="rename table (static_rename format)")
    r.add_argument("--dry-run", action="store_true")
    args = parser.parse_args(argv)

    data = Path(args.data)
    if not (data / "conveyor.db").is_file():
        sys.exit(f"no conveyor.db under {data}")
    conn = dbmod.connect(data / "conveyor.db")
    functions = layout_functions(layoutmod.derive())
    if args.command == "audit":
        for tid, addr, problem in audit(conn, functions, layoutmod.collect_regions()):
            print(f"  {tid:28} @0x{addr:08X}  {problem}")
        return
    if args.command == "rename":
        return _main_rename(args, conn, functions, data, layoutmod, BlobStore)
    try:
        actions = plan(conn, functions, args.names, args.retire, lock_targets(args.lockfile),
                       absorbed=args.absorbed)
        with tempfile.TemporaryDirectory() as tmp:
            store = BlobStore(Path(tmp) if args.dry_run else data / "blobs")
            build_objects(actions, layoutmod.collect_regions(), store)
    except RenameRefusal as exc:
        sys.exit(f"refusing: {exc}")
    for line in format_plan(conn, actions):
        print(line)
    total = conn.execute("SELECT COUNT(*) FROM n64_target").fetchone()[0]
    touched = len({a["target_id"] for a in actions})
    if args.dry_run:
        print(f"dry run: nothing written ({len(actions)} actions; "
              f"{total - touched} other n64_target rows untouched)")
        return
    purged = apply(conn, actions)
    after = conn.execute("SELECT COUNT(*) FROM n64_target").fetchone()[0]
    print(f"applied {len(actions)} actions to {data / 'conveyor.db'}: n64_target {total} -> {after},"
          f" {purged} matrix_entry rows purged")


def _main_rename(args, conn, functions, data, layoutmod, BlobStore):
    from .static_rename import RenameError, load_map
    try:
        rows = load_map(args.map)
        actions = rename_plan(conn, functions, rows, lock_targets(args.lockfile))
        with tempfile.TemporaryDirectory() as tmp:
            store = BlobStore(Path(tmp) if args.dry_run else data / "blobs")
            build_rename_objects(conn, actions, layoutmod.collect_regions(), store)
    except (RenameRefusal, RenameError) as exc:
        sys.exit(f"refusing: {exc}")
    for line in format_rename(conn, actions):
        print(line)
    if args.dry_run:
        print(f"dry run: nothing written ({len(actions)} renames)")
        return
    purged = apply_rename(conn, actions)
    print(f"renamed {len(actions)} rows in {data / 'conveyor.db'} ({', '.join(_KEYED_TABLES)}),"
          f" {purged} matrix_entry rows purged")


if __name__ == "__main__":
    main()
