"""Corpus candidates (002): search-don't-synthesize matching for library code.

A corpus root is a registered local git clone (V1: decompals/ultralib) whose
functions become candidates alongside the arcade tree. Targets are paired to
candidates by *exact function name* (not the size-window matrix), compiled
through the pool with the candidate's own repository context, and scored with
both the true score and the relocation-blind score.

    python3 -m tools.conveyor.pipeline.corpus register <origin> <path> ...
    python3 -m tools.conveyor.pipeline.corpus ingest [<origin>] [--allow-dirty]
    python3 -m tools.conveyor.pipeline.corpus submit [--dry-run]
    python3 -m tools.conveyor.pipeline.corpus ingest-results
    python3 -m tools.conveyor.pipeline.corpus report [--target X]

Corpus jobs are ordinary `compile_score` manifests, so dedupe, caching,
ingest, and failure clustering all apply unchanged (FR-012). Nothing here
promotes or locks a target that lacks a true score of 0 (SC-007); a
relocation-blind 0 with nonzero true score is a review flag, not a match.
"""
import argparse
import datetime
import hashlib
import json
import subprocess
import sys
import tempfile
from collections import defaultdict
from pathlib import Path

from ..bundles.build_job import build_job_bundle
from ..client import DEFAULT_DATA, Http, load_token
from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from ..seeds import context as contextmod
from ..seeds import extract_candidates as extractmod
from . import matrix as matrixmod

FLAGSETS = matrixmod.FLAGSETS
REPO = extractmod.REPO
CANDIDATES_PER_JOB = 20
PRIORITY = 50  # above bulk matrix (100), below verify/smoke (1)

# Canonical-name aliases: ultralib candidate function name -> our target_id.
# A few N64 targets were historically given generic/wrong labels in
# symbol_addrs.us.txt, so name-pairing (targets.get(candidate_name)) misses the
# real libultra source. Verified against ultralib by asm audit (2026-07-14):
#   dll_remove        @ 0x8000C050 is byte-for-byte __osDequeueThread
#   dll_get_priority  @ 0x8000C490 is byte-for-byte osGetThreadPri
# The proper fix is renaming the linker symbol (touches every cross-file `jal`
# site + 2 locked regression anchors, so it is gated on a full SHA-1 rebuild);
# until then this lets corpus pair the candidate against the right target .o
# with zero build/link/lock risk. See docs/SYMBOL_MISATTRIBUTION.md.
CANONICAL_ALIASES = {
    "__osDequeueThread": "dll_remove",
    "osGetThreadPri": "dll_get_priority",
}


def _conn_store(data):
    data = Path(data)
    return dbmod.connect(data / "conveyor.db"), BlobStore(data / "blobs")


# --- git provenance ----------------------------------------------------------

def _run_git(path, *args):
    return subprocess.run(
        ["git", "-C", str(path), *args], capture_output=True, text=True
    )


def _git_head(path):
    r = _run_git(path, "rev-parse", "HEAD")
    return r.stdout.strip() if r.returncode == 0 else None


def _short(commit):
    """Short display form, preserving a `-dirty` provenance suffix."""
    if commit.endswith("-dirty"):
        return commit[:8] + "-dirty"
    return commit[:8]


# --- register ----------------------------------------------------------------

def cmd_register(args):
    origin = args.origin
    if origin == "arcade" or "/" in origin or ".c" in origin:
        sys.exit(f"refusing: invalid origin {origin!r} "
                 "('arcade' is reserved; must not contain '/' or '.c')")
    include_dirs = [d for d in (args.include_dirs or "").split(",") if d]
    abspath = REPO / args.path
    head = _git_head(abspath)
    if head is None:
        sys.exit(f"refusing: {args.path} is not a git clone")

    conn, _ = _conn_store(args.data)
    existing = conn.execute(
        "SELECT path FROM corpus_root WHERE origin=?", (origin,)
    ).fetchone()
    if existing and existing["path"] != args.path:
        sys.exit(f"refusing: {origin} already registered at {existing['path']} "
                 f"(not {args.path})")
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO corpus_root (origin, path, repo_url, commit_sha, include_dirs)"
            " VALUES (?, ?, ?, ?, ?)"
            " ON CONFLICT(origin) DO UPDATE SET path=excluded.path,"
            " repo_url=excluded.repo_url, commit_sha=excluded.commit_sha,"
            " include_dirs=excluded.include_dirs",
            (origin, args.path, args.repo_url or "", head, json.dumps(include_dirs)),
        )
    print(f"registered {origin} at {args.path} @ {head[:8]} "
          f"(include_dirs={include_dirs})")


# --- ingest ------------------------------------------------------------------

def _ingest_root(conn, root, allow_dirty):
    origin = root["origin"]
    abspath = REPO / root["path"]
    head = _git_head(abspath)
    if head is None:
        sys.exit(f"refusing {origin}: clone missing at {root['path']} "
                 "(provenance would be a lie)")
    dirty = bool(_run_git(abspath, "status", "--porcelain").stdout.strip())
    moved = head != root["commit_sha"] and not root["commit_sha"].endswith("-dirty")
    if (dirty or moved) and not allow_dirty:
        why = ("uncommitted local edits" if dirty
               else f"HEAD is {head[:8]}, registered {root['commit_sha'][:8]}")
        sys.exit(f"refusing {origin}: {why}; nothing recorded "
                 "(commit/clean the clone, or pass --allow-dirty)")
    commit = f"{head}-dirty" if allow_dirty else head

    count, new, upd, files = 0, 0, 0, set()
    # A few ultralib files carry two #ifdef-guarded definitions of one function
    # (e.g. debug/BBPlayer variants); the extractor yields both, but the 3-part
    # candidate_id grammar admits only one. Keep the first (deterministic: the
    # walk and extraction are both ordered), so ingest counts stay idempotent.
    seen = set()
    with dbmod.tx(conn):
        for path in sorted(abspath.rglob("*.c")):
            rel = str(path.relative_to(abspath))
            try:
                text = path.read_text(errors="replace")
            except OSError:
                continue
            for name, start, end in extractmod.extract_functions(text):
                cand_id = f"{origin}:{rel}:{name}"
                if cand_id in seen:
                    continue
                seen.add(cand_id)
                body = text[start:end]
                bsha = hashlib.sha256(body.encode()).hexdigest()
                prov = json.dumps(
                    {"repo_url": root["repo_url"], "rel": rel, "commit": commit},
                    sort_keys=True,
                )
                prev = conn.execute(
                    "SELECT body_sha FROM arcade_candidate WHERE candidate_id=?",
                    (cand_id,),
                ).fetchone()
                conn.execute(
                    "INSERT INTO arcade_candidate"
                    " (candidate_id, name, src_file, body_sha, origin, provenance)"
                    " VALUES (?, ?, ?, ?, ?, ?)"
                    " ON CONFLICT(candidate_id) DO UPDATE SET"
                    " body_sha=excluded.body_sha, name=excluded.name,"
                    " src_file=excluded.src_file, origin=excluded.origin,"
                    " provenance=excluded.provenance",
                    (cand_id, name, rel, bsha, origin, prov),
                )
                if prev is None:
                    new += 1
                elif prev["body_sha"] != bsha:
                    upd += 1
                count += 1
                files.add(rel)
        conn.execute(
            "UPDATE corpus_root SET commit_sha=?,"
            " ingested_at=strftime('%Y-%m-%dT%H:%M:%fZ','now') WHERE origin=?",
            (commit, origin),
        )
    print(f"{origin}: {count} functions from {len(files)} files "
          f"({new} new, {upd} updated) @ {_short(commit)}")


def cmd_ingest(args):
    conn, _ = _conn_store(args.data)
    if args.origin:
        roots = conn.execute(
            "SELECT * FROM corpus_root WHERE origin=?", (args.origin,)
        ).fetchall()
        if not roots:
            sys.exit(f"no corpus root registered for {args.origin!r}")
    else:
        roots = conn.execute("SELECT * FROM corpus_root ORDER BY origin").fetchall()
        if not roots:
            sys.exit("no corpus roots registered (run `corpus register` first)")
    for root in roots:
        _ingest_root(conn, root, args.allow_dirty)


# --- submit ------------------------------------------------------------------

def _existing_cells(conn, toolkit_sha):
    rows = conn.execute(
        "SELECT target_id, candidate_id, flagset FROM matrix_entry"
        " WHERE toolkit_sha = ?", (toolkit_sha,),
    )
    return {(r["target_id"], r["candidate_id"], r["flagset"]) for r in rows}


def cmd_submit(args):
    conn, store = _conn_store(args.data)
    http = Http(args.coordinator, load_token(args.token, args.data))
    toolkit_sha = http.pinned_toolkit()

    roots = {r["origin"]: r for r in
             conn.execute("SELECT * FROM corpus_root").fetchall()}
    q = ("SELECT candidate_id, name, src_file, origin FROM arcade_candidate"
         " WHERE origin != 'arcade'")
    params = ()
    if args.origin:
        q += " AND origin = ?"
        params = (args.origin,)
    candidates = conn.execute(q + " ORDER BY candidate_id", params).fetchall()

    targets = {r["target_id"]: r for r in conn.execute(
        "SELECT target_id, target_o_sha FROM n64_target").fetchall()}
    done = _existing_cells(conn, toolkit_sha)

    by_origin = defaultdict(list)
    for c in candidates:
        by_origin[c["origin"]].append(c)

    jobs, cells_planned = [], 0
    pairings = skip_no_o = skip_unextractable = 0
    target_bytes = {}

    def submit_batch(cells, files, include_dirs):
        nonlocal cells_planned
        if not cells:
            return
        m = {"job_type": "compile_score", "toolkit_sha": toolkit_sha,
             "include_dirs": include_dirs, "cells": cells}
        with tempfile.TemporaryDirectory() as tmp:
            bundle, m_sha = build_job_bundle(m, files, Path(tmp) / "job.tar.gz")
            if args.dry_run:
                jobs.append({"manifest_sha": m_sha})
                return
            _, out = http.call("POST", "/api/v1/blobs", raw=bundle.read_bytes())
        jobs.append({
            "job_type": "compile_score", "manifest_sha": m_sha,
            "bundle_sha": out["sha256"], "toolkit_sha": toolkit_sha,
            "priority": PRIORITY,
        })

    for origin, cands in by_origin.items():
        root = roots.get(origin)
        if root is None:
            continue  # candidate whose root was unregistered; skip quietly
        abspath = REPO / root["path"]
        include_dirs = json.loads(root["include_dirs"])
        cells, files, n_cands = [], {}, 0

        for c in cands:
            # tid is the target identity used everywhere downstream (matrix_entry
            # key, target .o, work dir); c["name"] stays the candidate's own
            # function name for extraction/reduction. They differ only for the
            # handful of misattributed symbols in CANONICAL_ALIASES.
            tid = CANONICAL_ALIASES.get(c["name"], c["name"])
            target = targets.get(tid)
            if target is None:
                continue  # not a name pairing
            pairings += 1
            if not target["target_o_sha"]:
                skip_no_o += 1
                continue
            tu_path = abspath / c["src_file"]
            try:
                text = tu_path.read_text(errors="replace")
            except OSError:
                skip_unextractable += 1
                continue
            names = {n for n, _, _ in extractmod.extract_functions(text)}
            if c["name"] not in names:
                skip_unextractable += 1
                continue
            wanted = [fs for fs in args.flagsets
                      if (tid, c["candidate_id"], fs) not in done]
            if not wanted:
                continue

            # Place the reduced source at its real repo-relative directory
            # (unique basename so two functions from one file don't collide)
            # so the compiler's "search the includer's own dir first" rule
            # finds the file's private headers — e.g. gu/*.c's `guint.h`,
            # bundled by resolve_headers at its own path. Include-dir headers
            # (PR/...) still resolve via the registered -I dirs.
            sha16 = hashlib.sha256(c["candidate_id"].encode()).hexdigest()[:16]
            src_dir = str(Path(c["src_file"]).parent)
            source_name = (f"{src_dir}/{sha16}.c" if src_dir not in (".", "")
                           else f"{sha16}.c")
            files[source_name] = (contextmod.strip_comments(
                contextmod.reduced_tu(text, c["name"])) + "\n").encode()
            for hdr_rel, hdr_text in contextmod.resolve_headers(
                    tu_path, abspath, include_dirs).items():
                files.setdefault(
                    hdr_rel, contextmod.strip_comments(hdr_text).encode())
            o_name = tid + ".o"
            if o_name not in files:
                sha = target["target_o_sha"]
                if sha not in target_bytes:
                    target_bytes[sha] = store.get(sha).read_bytes()
                files[o_name] = target_bytes[sha]
            for fs in wanted:
                cells.append({
                    "candidate_id": c["candidate_id"], "source": source_name,
                    "flagset": fs,
                    "targets": [{"target_id": tid, "file": o_name,
                                 "target_o_sha": target["target_o_sha"]}],
                })
                cells_planned += 1
            n_cands += 1
            if n_cands >= CANDIDATES_PER_JOB:
                submit_batch(cells, files, include_dirs)
                cells, files, n_cands = [], {}, 0
        submit_batch(cells, files, include_dirs)

    skips = f"skipped: no_target_o={skip_no_o} unextractable={skip_unextractable}"
    if args.dry_run:
        print(f"{pairings} name pairings; dry run: {len(jobs)} jobs "
              f"({cells_planned} cells), {skips}")
        return
    submitted = cached = 0
    for i in range(0, len(jobs), 100):
        _, out = http.call("POST", "/api/v1/work", body=jobs[i:i + 100])
        for entry in out:
            if "cached_result" in entry:
                cached += 1
            else:
                submitted += 1
    print(f"{pairings} name pairings; {submitted} jobs submitted "
          f"({cells_planned} cells), {cached} cache hits, {skips}")


# --- ingest-results ----------------------------------------------------------

def _find_work_dir(target_id, work_root=None):
    """The target's work directory by the verify_promote rglob rule, else the
    corpus fallback dir (created)."""
    work_root = Path(work_root) if work_root else REPO / "work"
    matches = [p for p in work_root.rglob(target_id) if p.is_dir()]
    if matches:
        return matches[0]
    fallback = work_root / "corpus" / target_id
    fallback.mkdir(parents=True, exist_ok=True)
    return fallback


def _artifact_text(conn, target_id, best):
    """corpus_match.c contents for a reloc_only_diff target (data-model.md)."""
    cand_id = best["candidate_id"]
    row = conn.execute(
        "SELECT origin, src_file, provenance FROM arcade_candidate WHERE candidate_id=?",
        (cand_id,)).fetchone()
    origin = row["origin"]
    rel = row["src_file"]
    name = cand_id.rpartition(":")[2]
    root = conn.execute(
        "SELECT path, repo_url, commit_sha FROM corpus_root WHERE origin=?",
        (origin,)).fetchone()
    commit = _short(root["commit_sha"])
    try:
        body = extractmod.extract_named_function(REPO / root["path"] / rel, name)
    except (KeyError, OSError):
        body = f"/* candidate {name} no longer extractable from {rel} */"
    header = (
        "/*\n"
        " * CORPUS MATCH (reloc_only_diff) — NOT byte-verified; do not commit as matched.\n"
        " * Generated by pipeline.corpus; regenerated on every ingest — do not hand-edit.\n"
        f" * Origin:      {origin} ({root['repo_url']})\n"
        f" * Source:      {rel} @ {commit}\n"
        f" * Flags:       {best['flagset']}\n"
        f" * Scores:      true={best['best_true']}  reloc_blind={best['best_reloc_blind']}\n"
        " * Upgrade:     re-scores automatically when relocation-aware targets land\n"
        " */\n"
    )
    return header + body + "\n"


def _recompute_reloc_flags(conn, toolkit_sha, work_root=None):
    """Set human_flag='reloc_only_diff' on targets whose best corpus evidence is
    reloc_blind 0 with true score > 0 (and no true-0 anywhere), clear it where
    it no longer holds, and write/refresh their corpus_match.c artifacts. Runs
    after update_rankings, so it re-stamps the advisory flag that promotion may
    have cleared. Scoped to not-yet-matched, non-override targets — never a
    promotion (SC-007)."""
    best = _corpus_best(conn, toolkit_sha)
    qualifying = {}
    for tid, b in best.items():
        if b["best_true"] is None or b["best_reloc_blind"] is None:
            continue
        if not (b["best_reloc_blind"] == 0 and b["best_true"] > 0):
            continue
        st = conn.execute(
            "SELECT status, override FROM function_status WHERE target_id=?",
            (tid,)).fetchone()
        if st is None or st["override"] or st["status"] in ("matched", "verified"):
            continue
        qualifying[tid] = b

    previously = {r["target_id"] for r in conn.execute(
        "SELECT target_id FROM function_status WHERE human_flag='reloc_only_diff'")}
    newly = set(qualifying) - previously
    artifacts = 0
    with dbmod.tx(conn):
        for tid in qualifying:
            conn.execute(
                "UPDATE function_status SET human_flag='reloc_only_diff',"
                " updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now') WHERE target_id=?",
                (tid,))
        # Clear the flag where evidence stopped satisfying it (FR-011).
        for tid in previously - set(qualifying):
            conn.execute(
                "UPDATE function_status SET human_flag=NULL,"
                " updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now')"
                " WHERE target_id=? AND human_flag='reloc_only_diff'",
                (tid,))
    for tid, b in qualifying.items():
        work_dir = _find_work_dir(tid, work_root)
        (work_dir / "corpus_match.c").write_text(_artifact_text(conn, tid, b))
        artifacts += 1
    return len(qualifying), len(newly), artifacts


def cmd_ingest_results(args):
    # 1-2. Shared matrix ingest (both scores into matrix_entry) + update_rankings
    # (true-0 corpus targets take the normal candidate_identified path, FR-009).
    matrixmod.cmd_ingest(args)
    # 3-4. Corpus post-processing: reloc_only_diff flags + artifacts.
    conn, _ = _conn_store(args.data)
    toolkit_sha = dbmod.get_meta(conn, "toolkit_sha")
    flagged, newly, artifacts = _recompute_reloc_flags(conn, toolkit_sha)
    print(f"reloc_only_diff: {flagged} flagged ({newly} newly), "
          f"{artifacts} artifacts written")


# --- report ------------------------------------------------------------------

def _corpus_best(conn, toolkit_sha):
    """{target_id: {best_true, best_reloc_blind, candidate_id, flagset}} over
    corpus-origin cells under the pinned toolkit. best_reloc_blind ignores
    NULLs; the shown candidate/flagset is the cell minimizing (reloc_blind
    (NULL last), true)."""
    rows = conn.execute(
        "SELECT m.target_id, m.candidate_id, m.flagset, m.score,"
        " m.score_reloc_blind FROM matrix_entry m"
        " JOIN arcade_candidate a ON a.candidate_id = m.candidate_id"
        " WHERE a.origin != 'arcade' AND m.toolkit_sha = ?", (toolkit_sha,),
    ).fetchall()
    best = {}
    for r in rows:
        b = best.setdefault(r["target_id"], {
            "best_true": None, "best_reloc_blind": None,
            "candidate_id": None, "flagset": None})
        if b["best_true"] is None or r["score"] < b["best_true"]:
            b["best_true"] = r["score"]
        rb = r["score_reloc_blind"]
        if rb is not None and (b["best_reloc_blind"] is None
                               or rb < b["best_reloc_blind"]):
            b["best_reloc_blind"] = rb
        key = (rb if rb is not None else 1 << 30, r["score"])
        cur = ((b["_rbk"] if "_rbk" in b else (1 << 30, 1 << 30)))
        if key < cur:
            b["_rbk"] = key
            b["candidate_id"] = r["candidate_id"]
            b["flagset"] = r["flagset"]
    return best


def cmd_report(args):
    conn, _ = _conn_store(args.data)
    toolkit_sha = dbmod.get_meta(conn, "toolkit_sha")

    # 1. Roots.
    roots = conn.execute("SELECT * FROM corpus_root ORDER BY origin").fetchall()
    if not roots:
        print("no corpus roots registered")
        return
    print("corpus roots:")
    for r in roots:
        n = conn.execute(
            "SELECT COUNT(*) AS n FROM arcade_candidate WHERE origin=?",
            (r["origin"],)).fetchone()["n"]
        print(f"  {r['origin']:12} @ {_short(r['commit_sha'])}  "
              f"{n} candidates  ingested={r['ingested_at'] or 'never'}")

    best = _corpus_best(conn, toolkit_sha) if toolkit_sha else {}

    # --target detail mode.
    if args.target:
        print(f"\ncorpus candidates for {args.target} (under pinned toolkit):")
        rows = conn.execute(
            "SELECT m.candidate_id, m.flagset, m.score, m.score_reloc_blind"
            " FROM matrix_entry m JOIN arcade_candidate a"
            " ON a.candidate_id = m.candidate_id"
            " WHERE m.target_id=? AND a.origin!='arcade' AND m.toolkit_sha=?"
            " ORDER BY m.score_reloc_blind IS NULL, m.score_reloc_blind,"
            " m.score, m.candidate_id",
            (args.target, toolkit_sha)).fetchall()
        if not rows:
            print("  (no corpus evidence)")
            return
        for r in rows:
            print(f"  true={r['score']:>6}  reloc_blind="
                  f"{r['score_reloc_blind'] if r['score_reloc_blind'] is not None else '  -':>4}"
                  f"  {r['candidate_id']}  [{r['flagset']}]")
        return

    # 2. Pairing coverage.
    corpus_names = {r["name"] for r in conn.execute(
        "SELECT DISTINCT name FROM arcade_candidate WHERE origin!='arcade'")}
    all_targets = {r["target_id"] for r in conn.execute(
        "SELECT target_id FROM n64_target")}
    paired = corpus_names & all_targets
    print(f"\npairing coverage: {len(paired)} targets name-match a corpus "
          f"candidate; {len(best)} have scored evidence under the pinned toolkit")

    # 3. Compile rate per origin.
    print("compile coverage per origin:")
    for r in roots:
        rows = conn.execute(
            "SELECT compile_status FROM arcade_candidate WHERE origin=?",
            (r["origin"],)).fetchall()
        ok = fail = untried = 0
        for row in rows:
            cs = json.loads(row["compile_status"])
            if not cs:
                untried += 1
            elif any(v == "ok" for v in cs.values()):
                ok += 1
            else:
                fail += 1
        if ok + fail:
            print(f"  {r['origin']:12} {ok} ok, {fail} fail, {untried} untried "
                  f"({100 * ok // max(ok + fail, 1)}% of tried)")
        else:
            print(f"  {r['origin']:12} {untried} untried (no results ingested yet)")

    # 4. Per-target best evidence.
    if best:
        ordered = sorted(
            best.items(),
            key=lambda kv: (kv[1]["best_reloc_blind"] if kv[1]["best_reloc_blind"]
                            is not None else 1 << 30, kv[1]["best_true"]))
        print(f"\nper-target best evidence ({len(ordered)} targets):")
        print(f"  {'target':28} {'true':>6} {'reloc_blind':>11}  candidate  [flagset]")
        for tid, b in ordered:
            rb = b["best_reloc_blind"]
            print(f"  {tid:28} {b['best_true']:>6} "
                  f"{rb if rb is not None else '-':>11}  "
                  f"{b['candidate_id']}  [{b['flagset']}]")

    # 5. Flag summary.
    true0 = sum(1 for b in best.values() if b["best_true"] == 0)
    reloc = sum(1 for b in best.values()
                if b["best_true"] and b["best_true"] > 0
                and b["best_reloc_blind"] == 0)
    neither = len(best) - true0 - reloc
    print(f"\nflag summary: {true0} true-0 (promotion path), "
          f"{reloc} reloc_only_diff, {neither} neither")

    # SC-006 attribution invariant: every surviving matrix_entry row must have
    # been scored against the target's current object. NULLs are pre-003 legacy
    # cells (excluded from the mismatch count, reported separately). Nonzero
    # mismatched is a loud warning, not a crash.
    arow = conn.execute(
        "SELECT COUNT(*) AS n,"
        " SUM(CASE WHEN m.target_o_sha IS NULL THEN 1 ELSE 0 END) AS legacy,"
        " SUM(CASE WHEN m.target_o_sha IS NOT NULL"
        "     AND m.target_o_sha != t.target_o_sha THEN 1 ELSE 0 END) AS mismatched"
        " FROM matrix_entry m JOIN n64_target t USING (target_id)"
    ).fetchone()
    print(f"attribution: {arow['n']} cells checked, "
          f"{arow['mismatched'] or 0} mismatched (expect 0), "
          f"legacy={arow['legacy'] or 0}")


def _add_common(p):
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.add_argument("--coordinator", default="http://127.0.0.1:8323")
    p.add_argument("--token", default=None)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)

    p = sub.add_parser("register")
    _add_common(p)
    p.add_argument("origin")
    p.add_argument("path", help="repo-root-relative clone path")
    p.add_argument("--repo-url", default=None)
    p.add_argument("--include-dirs", default=None,
                   help="comma-separated, repo-root-relative include dirs")
    p.set_defaults(func=cmd_register)

    p = sub.add_parser("ingest")
    _add_common(p)
    p.add_argument("origin", nargs="?", default=None)
    p.add_argument("--allow-dirty", action="store_true")
    p.set_defaults(func=cmd_ingest)

    p = sub.add_parser("submit")
    _add_common(p)
    p.add_argument("--origin", default=None)
    p.add_argument("--flagsets", nargs="*", default=list(FLAGSETS))
    p.add_argument("--dry-run", action="store_true")
    p.set_defaults(func=cmd_submit)

    p = sub.add_parser("ingest-results")
    _add_common(p)
    p.set_defaults(func=cmd_ingest_results)

    p = sub.add_parser("report")
    _add_common(p)
    p.add_argument("--target", default=None)
    p.set_defaults(func=cmd_report)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
