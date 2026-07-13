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
M2C = REPO / "tools" / "mips_to_c" / "m2c.py"
SHIM = REPO / "tools" / "conveyor" / "seeds" / "shim" / "conveyor_shim.h"
_GLABEL_RE = re.compile(r"^\s*glabel\s+(\S+)")

# Preprocessed type context for m2c: the OS headers parse cleanly (the game
# headers make m2c's parser assert). It gives m2c real struct layouts so
# struct-using functions type correctly and compile; it also carries the
# scalar typedefs, so a seed built from it is self-contained for the permuter's
# `cpp -nostdinc` preprocess (no separate shim needed).
_CTX_HEADERS = ["types.h"] + [f"PR/{p.name}" for p in
                              sorted((REPO / "include" / "PR").glob("*.h"))]
_context_cache = None


def _context():
    """(preprocessed_context_path, context_text), built once."""
    global _context_cache
    if _context_cache is not None:
        return _context_cache
    import tempfile
    head = "\n".join(f'#include "{h}"' for h in _CTX_HEADERS) + "\n"
    src = Path(tempfile.mkdtemp(prefix="m2cctx-")) / "ctx.h"
    src.write_text(head)
    out = src.with_name("ctx.c")
    proc = subprocess.run(
        ["cpp", "-P", "-nostdinc", "-I", str(REPO / "include"),
         "-I", str(REPO / "include" / "PR"), "-D_LANGUAGE_C",
         str(src), "-o", str(out)],
        capture_output=True, text=True)
    text = out.read_text() if out.exists() and proc.returncode == 0 else ""
    _context_cache = (str(out) if text else None, text)
    return _context_cache


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


# Macros the preprocessed context drops (cpp consumed them) but m2c output uses.
_PRELUDE = "#define NULL ((void *)0)\n#define TRUE 1\n#define FALSE 0\n"


def _clean_m2c(body):
    """Strip m2c's mechanical output warts so the seed compiles: diagnostic
    lines that leak into stdout, and `?` placeholders m2c emits for parameter
    types it couldn't infer (default them to s32 — the permuter fixes the rest;
    ternary `?` is left alone)."""
    lines = [l for l in body.splitlines()
             if not l.lstrip().startswith(("Warning:", "Error:", "GLOBAL_ASM"))]
    text = "\n".join(lines)
    # `(? arg` / `, ? arg` (an unknown parameter type) -> s32
    text = re.sub(r"([(,]\s*)\?(\s+\w)", r"\1s32\2", text)
    return text


def m2c_seed(target_id, vaddr, asm_idx):
    """Self-contained C seed for a target from its own asm, or None if m2c
    can't decompile it (missing asm / failure)."""
    asm_file = asm_idx.get(target_id) or asm_idx.get(f"func_{vaddr:08X}")
    if asm_file is None:
        return None
    ctx_path, ctx_text = _context()
    cmd = [sys.executable, str(M2C), str(asm_file), "-f", target_id]
    if ctx_path:
        cmd += ["--context", ctx_path]
    proc = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
    body = _clean_m2c(proc.stdout.strip())
    if proc.returncode != 0 or not body or "def " in body[:20]:
        return None
    if ctx_text:
        # Self-contained via the context (has scalar types + OS structs). Drop
        # the target's own prototype from the context so it doesn't conflict
        # with m2c's definition; C89 needs no prototypes for the rest.
        proto = re.compile(rf"^[^\n]*\b{re.escape(target_id)}\s*\([^;{{]*\)\s*;",
                           re.M)
        prelude = proto.sub("", ctx_text)
        return _PRELUDE + prelude + "\n" + body + "\n"
    # Fallback: minimal shim (scalar types only).
    return _PRELUDE + SHIM.read_text() + "\n" + body + "\n"


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


AUTO_WORK = REPO / "work" / "auto"


def cmd_harvest(args):
    """Direct, robust replacement for the farm's flaky harvest: scan finished
    permuter searches, and for every score-0 hit save its best.c and mark the
    function matched. No verify_promote / cluster fan-out (the parts that
    stalled the farm) — just bank the win so nothing is stranded."""
    conn, store = _conn(args.data)
    seen, banked, degenerate = set(), [], []
    rows = conn.execute(
        "SELECT target_id, result_sha, created_at FROM work_unit"
        " WHERE job_type='permuter_search' AND state='DONE'"
        " AND result_sha IS NOT NULL ORDER BY created_at DESC").fetchall()
    for r in rows:
        tid = r["target_id"]
        if not tid or tid in seen:
            continue
        seen.add(tid)
        result, artifacts = farmmod._read_result(store, r["result_sha"])
        if not result or result.get("exit") != "ok":
            continue
        pl = result.get("payload", {})
        best_c = artifacts.get("best.c")
        if pl.get("final_best_score") != 0 or not best_c:
            continue
        st = conn.execute("SELECT status FROM function_status WHERE target_id=?",
                          (tid,)).fetchone()
        if st and st["status"] in ("matched", "verified"):
            continue
        d = AUTO_WORK / tid
        d.mkdir(parents=True, exist_ok=True)
        (d / "matched.c").write_bytes(best_c)
        with dbmod.tx(conn):
            conn.execute(
                "UPDATE function_status SET status='matched', best_score=0,"
                " seed_kind='m2c', updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now')"
                " WHERE target_id=?", (tid,))
        (degenerate if pl.get("base_score") == 0 else banked).append(tid)
    print(f"harvested {len(banked) + len(degenerate)} score-0 -> {AUTO_WORK}")
    if banked:
        print(f"  permuter wins ({len(banked)}): {sorted(banked)}")
    if degenerate:
        print(f"  trivial/base-0 ({len(degenerate)}): {sorted(degenerate)}")


def cmd_seed(args):
    conn, store = _conn(args.data)
    http = Http(args.coordinator, load_token(args.token, args.data))
    toolkit_sha = http.pinned_toolkit()
    asm_idx = _asm_index()
    # unmatched static functions with a target object, most-tractable first
    rows = conn.execute(
        "SELECT t.target_id, t.address FROM n64_target t"
        " JOIN function_status f USING (target_id)"
        " WHERE f.status IN ('unmatched','seeded') AND t.target_o_sha IS NOT NULL"
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
    s = sub.add_parser("harvest")
    s.set_defaults(func=cmd_harvest)
    args = p.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
