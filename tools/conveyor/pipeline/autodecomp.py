"""Automated decompilation seeding (no human in the loop).

For each unmatched function: decompile its own asm with mips_to_c into a
structurally-correct C seed (far better than a distant arcade candidate),
make it self-contained (inline the shim so the permuter's `cpp -nostdinc`
preprocess works), and submit a permuter_search. The farm then harvests:
score 0 -> matched -> lock/promote. This is the standard shipped-decomp loop
(m2c seed -> permuter -> match) wired into the conveyor pool.

    python3 -m tools.conveyor.pipeline.autodecomp seed [--limit N] [--budget S]
    python3 -m tools.conveyor.pipeline.autodecomp one <target_id>

Yield is partial by nature: scalar/clean functions match readily; struct-heavy
ones need better type context; some never converge. But it turns the entire
unmatched backlog into real, autonomous pool work.
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

from ..client import DEFAULT_DATA, Http, load_token
from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from . import seeds as seedsmod
from . import farm as farmmod

REPO = Path(__file__).resolve().parents[3]
ASM_DIR = REPO / "asm" / "us"
M2C = REPO / "tools" / "m2c.py"
SHIM = REPO / "tools" / "conveyor" / "seeds" / "shim" / "conveyor_shim.h"
_GLABEL_RE = re.compile(r"^\s*glabel\s+(\S+)")


def _asm_index():
    """{glabel_name: asm_file} across asm/us/*.s (unconverted segments). Splat
    now emits canonical glabels (e.g. `glabel viGetTimeToDeadline`), so the
    target_id keys directly."""
    idx = {}
    for f in sorted(ASM_DIR.glob("*.s")):
        for line in f.read_text(errors="replace").splitlines():
            m = _GLABEL_RE.match(line)
            if m:
                idx.setdefault(m.group(1), f)
    return idx


def m2c_seed(target_id, vaddr, asm_idx):
    """Self-contained C seed for a target from its own asm, or None if m2c
    can't decompile it (missing asm / failure)."""
    asm_file = asm_idx.get(target_id) or asm_idx.get(f"func_{vaddr:08X}")
    if asm_file is None:
        return None
    proc = subprocess.run(
        [sys.executable, str(M2C), str(asm_file), "-f", target_id],
        capture_output=True, text=True, timeout=120,
    )
    body = proc.stdout.strip()
    if proc.returncode != 0 or not body or "def " in body[:20]:
        return None
    # Inline the shim (not #include) so `cpp -nostdinc` in the permuter works.
    return SHIM.read_text() + "\n" + body + "\n"


def submit_one(conn, store, http, toolkit_sha, target_id, vaddr, asm_idx,
               budget_seconds):
    seed = m2c_seed(target_id, vaddr, asm_idx)
    if seed is None:
        return "no_seed"
    try:
        bundle, m_sha, job = seedsmod.build_search_bundle(
            conn, store, target_id, seed,
            farmmod._flagset_for(conn, target_id), toolkit_sha,
            budget={"wall_seconds": budget_seconds, "iterations": None})
    except KeyError:
        return "no_target_o"
    _, out = http.call("POST", "/api/v1/blobs", raw=bundle.read_bytes())
    job.update(bundle_sha=out["sha256"], target_id=target_id, priority=30)
    http.call("POST", "/api/v1/work", body=[job])
    with dbmod.tx(conn):
        conn.execute(
            "UPDATE function_status SET seed_kind='m2c',"
            " updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now') WHERE target_id=?",
            (target_id,))
        farmmod._set_status(conn, target_id, "in_search")
    return "seeded"


def _conn(data):
    d = Path(data)
    return dbmod.connect(d / "conveyor.db"), BlobStore(d / "blobs")


def cmd_seed(args):
    conn, store = _conn(args.data)
    http = Http(args.coordinator, load_token(args.token, args.data))
    toolkit_sha = http.pinned_toolkit()
    asm_idx = _asm_index()
    # unmatched static functions with a target object, most-tractable first
    rows = conn.execute(
        "SELECT t.target_id, t.address FROM n64_target t"
        " JOIN function_status f USING (target_id)"
        " WHERE f.status='unmatched' AND t.target_o_sha IS NOT NULL"
        " AND t.population='static' AND t.insn_count IS NOT NULL"
        " ORDER BY t.insn_count LIMIT ?", (args.limit,)).fetchall()
    counts = {}
    for r in rows:
        outcome = submit_one(conn, store, http, toolkit_sha, r["target_id"],
                             r["address"], asm_idx, args.budget)
        counts[outcome] = counts.get(outcome, 0) + 1
        if outcome == "seeded" and args.verbose:
            print(f"  seeded {r['target_id']}")
    print("autodecomp seed: " + "  ".join(f"{k}={v}" for k, v in sorted(counts.items())))


def cmd_one(args):
    conn, store = _conn(args.data)
    http = Http(args.coordinator, load_token(args.token, args.data))
    row = conn.execute("SELECT address FROM n64_target WHERE target_id=?",
                       (args.target,)).fetchone()
    if row is None:
        sys.exit(f"no target {args.target}")
    outcome = submit_one(conn, store, http, http.pinned_toolkit(), args.target,
                        row["address"], _asm_index(), args.budget)
    print(f"{args.target}: {outcome}")


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.add_argument("--coordinator", default="http://127.0.0.1:8323")
    p.add_argument("--token", default=None)
    sub = p.add_subparsers(dest="command", required=True)
    s = sub.add_parser("seed")
    s.add_argument("--limit", type=int, default=200)
    s.add_argument("--budget", type=int, default=1200)
    s.add_argument("--verbose", action="store_true")
    s.set_defaults(func=cmd_seed)
    s = sub.add_parser("one")
    s.add_argument("target")
    s.add_argument("--budget", type=int, default=1200)
    s.set_defaults(func=cmd_one)
    args = p.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
