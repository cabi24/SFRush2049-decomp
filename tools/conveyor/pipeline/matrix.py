"""Match-matrix orchestration (US1): candidates × flagsets × targets.

    python3 -m tools.conveyor.pipeline.matrix extract   # inventories (local)
    python3 -m tools.conveyor.pipeline.matrix submit    # batch jobs -> pool
    python3 -m tools.conveyor.pipeline.matrix ingest    # results -> matrix_entry
    python3 -m tools.conveyor.pipeline.matrix report    # coverage + rankings
    python3 -m tools.conveyor.pipeline.matrix failures  # compile-fail clusters

Pruning: scoring every pair is ~5.8M cells; a size window (candidate body
length vs target instruction count) cuts this ~10x with negligible recall
loss — a 10-instruction target never matches a 500-line candidate. Cells are
grouped so each job compiles a candidate once and scores it against every
size-compatible target (bundled targets are shared across the job's
candidates). Every batch is a pure function of its manifest: identical
re-submissions are answered from the result cache (FR-006).
"""
import argparse
import hashlib
import json
import re
import tarfile
import tempfile
from collections import Counter
from pathlib import Path

from ..bundles.build_job import build_job_bundle
from ..client import DEFAULT_DATA, Http, load_token
from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from ..seeds import extract_candidates as extractmod
from . import targets as targetsmod

# Confirmed baseline flag sets (docs/COMPILER_SETTINGS.md).
FLAGSETS = (
    "-g0 -O1 -mips2 -G 0 -non_shared",
    "-g0 -O2 -mips2 -G 0 -non_shared",
)

CHARS_PER_INSN = 11        # crude proxy: candidate body chars -> insn estimate
SIZE_WINDOW = (0.25, 4.0)  # target insns must be within this factor of proxy
CANDIDATES_PER_JOB = 20
NO_ANCESTRY_FACTOR = 3.0   # best score > factor*insn_count => no arcade ancestry
HIGH_CONF_FACTOR = 0.5     # best score <= factor*insn_count ...
HIGH_CONF_MARGIN = 1.25    # ...and runner-up >= margin*best => high confidence


def _conn_store(data):
    data = Path(data)
    return dbmod.connect(data / "conveyor.db"), BlobStore(data / "blobs")


# --- extract ----------------------------------------------------------------

def cmd_extract(args):
    conn, store = _conn_store(args.data)
    t = targetsmod.populate(conn, store)
    c = extractmod.populate(conn)
    print(f"targets:    {t['built']} built / {t['total']} inventory "
          f"({t['skipped']} skipped)")
    print(f"candidates: {c['candidates']} functions from {c['files']} files")


# --- submit -----------------------------------------------------------------

def _size_estimate(body):
    code = "\n".join(
        line for line in body.splitlines() if line.strip() and not
        line.strip().startswith(("/*", "*", "//"))
    )
    return max(len(code) // CHARS_PER_INSN, 8)


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
    target_bytes_cache = {}  # target_o_sha -> bytes, read once per run
    body_cache = {}          # src_file -> {name: body}, parse each file once

    targets = conn.execute(
        "SELECT target_id, insn_count, target_o_sha FROM n64_target"
        " WHERE target_o_sha IS NOT NULL ORDER BY insn_count"
    ).fetchall()
    candidates = conn.execute(
        "SELECT candidate_id FROM arcade_candidate ORDER BY candidate_id"
    ).fetchall()
    done = _existing_cells(conn, toolkit_sha)
    print(f"{len(targets)} targets × {len(candidates)} candidates × "
          f"{len(args.flagsets)} flagsets; {len(done)} cells already scored")

    jobs, cells_planned = [], 0
    batch = []

    def flush_batch(flagset):
        nonlocal cells_planned
        if not batch:
            return
        cells, files = [], {}
        for cand_id, body, matched_targets in batch:
            source_name = hashlib.sha256(cand_id.encode()).hexdigest()[:16] + ".c"
            files[source_name] = (
                '#include "conveyor_shim.h"\n\n' + body + "\n"
            ).encode()
            cell_targets = []
            for t in matched_targets:
                o_name = t["target_id"] + ".o"
                if o_name not in files:
                    sha = t["target_o_sha"]
                    if sha not in target_bytes_cache:
                        target_bytes_cache[sha] = store.get(sha).read_bytes()
                    files[o_name] = target_bytes_cache[sha]
                # Echo the exact target object identity for attribution (003):
                # it rides the manifest → result → matrix_entry.target_o_sha.
                cell_targets.append({"target_id": t["target_id"], "file": o_name,
                                     "target_o_sha": t["target_o_sha"]})
                cells_planned += 1
            cells.append({
                "candidate_id": cand_id, "source": source_name,
                "flagset": flagset, "targets": cell_targets,
            })
        m = {"job_type": "compile_score", "toolkit_sha": toolkit_sha, "cells": cells}
        with tempfile.TemporaryDirectory() as tmp:
            bundle, m_sha = build_job_bundle(m, files, Path(tmp) / "job.tar.gz")
            if args.dry_run:
                # A dry run must not upload anything or grow the blob store.
                jobs.append({"manifest_sha": m_sha})
                batch.clear()
                return
            _, out = http.call("POST", "/api/v1/blobs", raw=bundle.read_bytes())
        jobs.append({
            "job_type": "compile_score", "manifest_sha": m_sha,
            "bundle_sha": out["sha256"], "toolkit_sha": toolkit_sha,
            "priority": 100,
        })
        batch.clear()

    def cached_body(cand_id):
        rel, _, name = cand_id.partition(":")
        if rel not in body_cache:
            try:
                text = (extractmod.ARCADE / rel).read_text(errors="replace")
            except OSError:
                body_cache[rel] = {}
                return None
            body_cache[rel] = {
                fn_name: text[start:end]
                for fn_name, start, end in extractmod.extract_functions(text)
            }
        return body_cache[rel].get(name)

    for flagset in args.flagsets:
        for (cand_id,) in candidates:
            body = cached_body(cand_id)
            if body is None:
                continue
            proxy = _size_estimate(body)
            lo, hi = proxy * SIZE_WINDOW[0], proxy * SIZE_WINDOW[1]
            matched = [
                t for t in targets
                if t["insn_count"] and lo <= t["insn_count"] <= hi
                and (t["target_id"], cand_id, flagset) not in done
            ]
            if not matched:
                continue
            batch.append((cand_id, body, matched))
            if len(batch) >= CANDIDATES_PER_JOB:
                flush_batch(flagset)
        flush_batch(flagset)

    if args.dry_run:
        print(f"dry run: {len(jobs)} jobs, {cells_planned} cells")
        return
    submitted = cached = 0
    for i in range(0, len(jobs), 100):
        _, out = http.call("POST", "/api/v1/work", body=jobs[i : i + 100])
        for entry in out:
            if "cached_result" in entry:
                cached += 1
            else:
                submitted += 1
    print(f"submitted {submitted} jobs ({cells_planned} cells), {cached} cache hits")


# --- ingest -----------------------------------------------------------------

def cmd_ingest(args):
    conn, store = _conn_store(args.data)
    pinned = dbmod.get_meta(conn, "toolkit_sha")
    rows = conn.execute(
        "SELECT job_id, result_sha, toolkit_sha FROM work_unit"
        " WHERE job_type='compile_score' AND state='DONE'"
        " AND result_sha IS NOT NULL AND ingested_at IS NULL"
    ).fetchall()
    new_cells, stale, stale_target, compile_fail = 0, 0, 0, {}
    current_o = {r["target_id"]: r["target_o_sha"] for r in conn.execute(
        "SELECT target_id, target_o_sha FROM n64_target")}
    for row in rows:
        # Data-model rule: results from a non-pinned toolkit are never merged
        # (scores are only comparable within one toolkit, FR-005).
        if pinned and row["toolkit_sha"] != pinned:
            with dbmod.tx(conn):
                conn.execute(
                    "UPDATE work_unit SET ingested_at=strftime"
                    "('%Y-%m-%dT%H:%M:%fZ','now') WHERE job_id=?",
                    (row["job_id"],),
                )
            stale += 1
            continue
        path = store.get(row["result_sha"])
        if path is None:
            continue  # blob missing: retry next run, don't mark ingested
        with tarfile.open(path) as tar:
            result = json.loads(tar.extractfile("result.json").read())
        with dbmod.tx(conn):
            conn.execute(
                "UPDATE work_unit SET ingested_at=strftime"
                "('%Y-%m-%dT%H:%M:%fZ','now') WHERE job_id=?",
                (row["job_id"],),
            )
            if result["exit"] != "ok":
                continue
            for cell in result["payload"]["cells"]:
                key = (cell["candidate_id"], cell["flagset"])
                if cell["compile"] != "ok":
                    # Keep the first fail message, but never let a fail from one
                    # job override an "ok" recorded from another (a candidate that
                    # compiles under some source/toolkit is ok — ok always wins).
                    compile_fail.setdefault(key, cell["compile"])
                    continue
                compile_fail[key] = "ok"
                # Supersession guard (003): a result computed against a target
                # object that has since been replaced must not re-introduce
                # stale evidence after the extract-time purge (late-arriving
                # results otherwise sneak past it). Attributed cells whose
                # object is no longer current are dropped, not stored.
                cell_o = cell.get("target_o_sha")
                if cell_o is not None and cell_o != current_o.get(cell["target_id"]):
                    stale_target += 1
                    continue
                # score_reloc_blind (002) and target_o_sha (003) are present only
                # for newer toolkit results; old result blobs lack them and
                # ingest as NULL (data-model.md). Old blobs are never rejected.
                conn.execute(
                    "INSERT OR IGNORE INTO matrix_entry"
                    " (target_id, candidate_id, flagset, toolkit_sha, score,"
                    " score_reloc_blind, target_o_sha)"
                    " VALUES (?, ?, ?, ?, ?, ?, ?)",
                    (cell["target_id"], cell["candidate_id"], cell["flagset"],
                     row["toolkit_sha"], cell["score"],
                     cell.get("score_reloc_blind"), cell.get("target_o_sha")),
                )
                new_cells += 1
    with dbmod.tx(conn):
        for (cand_id, flagset), status in compile_fail.items():
            row = conn.execute(
                "SELECT compile_status FROM arcade_candidate WHERE candidate_id=?",
                (cand_id,),
            ).fetchone()
            if row is None:
                continue
            cs = json.loads(row["compile_status"])
            cs[flagset] = status
            conn.execute(
                "UPDATE arcade_candidate SET compile_status=? WHERE candidate_id=?",
                (json.dumps(cs, sort_keys=True), cand_id),
            )
    print(f"ingested {new_cells} new cells from {len(rows)} done jobs"
          + (f" ({stale} stale-toolkit jobs discarded)" if stale else "")
          + (f" ({stale_target} stale-target cells dropped)" if stale_target
             else ""))
    update_rankings(conn, pinned)


# --- rankings / no-ancestry (T024) -------------------------------------------

def rankings_for(conn, target_id, limit=10, toolkit_sha=None):
    """Ranked candidates for one target — stable (score, candidate_id) order,
    within a single toolkit (scores across toolkits are not comparable)."""
    toolkit_sha = toolkit_sha or dbmod.get_meta(conn, "toolkit_sha")
    return conn.execute(
        "SELECT candidate_id, flagset, MIN(score) AS score FROM matrix_entry"
        " WHERE target_id = ? AND (? IS NULL OR toolkit_sha = ?)"
        " GROUP BY candidate_id ORDER BY score, candidate_id LIMIT ?",
        (target_id, toolkit_sha, toolkit_sha, limit),
    ).fetchall()


def update_rankings(conn, toolkit_sha=None):
    """Refresh function_status from matrix state (unmatched->candidate_identified,
    no-ancestry flagging, SC-001 high-confidence marking). Status changes go
    through the transition engine; rows with a manual override keep their
    pairing fields untouched (FR-015)."""
    from . import status as statusmod

    toolkit_sha = toolkit_sha or dbmod.get_meta(conn, "toolkit_sha")
    targets = conn.execute(
        "SELECT t.target_id, t.insn_count, f.status, f.override FROM n64_target t"
        " JOIN function_status f USING (target_id)"
    ).fetchall()
    promoted = flagged = high_conf = 0
    with dbmod.tx(conn):
        for t in targets:
            if t["override"]:
                continue  # manual pairing/seed: never touched by the matrix
            top = rankings_for(conn, t["target_id"], limit=2,
                               toolkit_sha=toolkit_sha)
            if not top or top[0]["score"] is None:
                continue
            best = top[0]
            insns = t["insn_count"] or 1
            if best["score"] > NO_ANCESTRY_FACTOR * insns:
                if t["status"] == "unmatched":
                    conn.execute(
                        "UPDATE function_status SET human_flag='no_ancestry',"
                        " best_score=?, updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now')"
                        " WHERE target_id=?",
                        (best["score"], t["target_id"]),
                    )
                    flagged += 1
                continue
            is_high_conf = best["score"] <= HIGH_CONF_FACTOR * insns and (
                len(top) < 2 or top[1]["score"] >= HIGH_CONF_MARGIN * max(best["score"], 1)
            )
            if is_high_conf:
                high_conf += 1
            if t["status"] == "unmatched":
                statusmod.transition(
                    conn, t["target_id"], "candidate_identified",
                    best_score=best["score"],
                    best_candidate_id=best["candidate_id"],
                )
                promoted += 1
    print(f"rankings: {promoted} targets -> candidate_identified, "
          f"{flagged} flagged no_ancestry, {high_conf} high-confidence (SC-001)")


# --- failures (compile-failure clustering) ------------------------------------

_UNDEF_RE = re.compile(r"'([A-Za-z_][A-Za-z0-9_]*)' undefined")
_CFE_RE = re.compile(r"cfe: (?:Error|Fatal)[^:]*: \S+ line \d+: ([^\n]+)")
_TYPEISH_RE = re.compile(r"\b([A-Z][A-Z0-9_]{2,})\b")


def failure_signatures(message):
    """Cluster one captured compile-failure message into stable signatures.

    The message is IDO cfe stderr, possibly truncated. `'X' undefined`
    errors (the dominant class — cfe reports each identifier once per
    file) become ``undefined: X``. Syntax errors are usually an unknown
    typedef name, and every error after the first is cascade noise, so
    the first error's context line is mined for a type-like token — but
    only when the capture starts at the true first error (head capture;
    tail-truncated messages stay generic). Anything unrecognized (e.g.
    ``timeout``) falls back to the raw text."""
    sigs = set()
    for ident in _UNDEF_RE.findall(message):
        sigs.add(f"undefined: {ident}")
    for i, m in enumerate(_CFE_RE.finditer(message)):
        text = m.group(1).strip().rstrip(".")
        if "' undefined" in text:
            continue
        if text in ("Syntax Error", "Empty declaration specifiers"):
            if i > 0 or not message.startswith("cfe:"):
                sigs.add(text)  # cascade or truncated: keep it generic
                continue
            context = message[m.end():].split("\n", 2)[1:2]
            tokens = _TYPEISH_RE.findall(context[0]) if context else []
            sigs.add(f"unknown type? {tokens[0]}" if tokens else text)
        else:
            sigs.add(text[:120])
    if not sigs:
        sigs.add(message.strip()[:120] or "empty error output")
    return sigs


def aggregate_failures(conn, origin=None):
    """(blocked, ok) where blocked maps candidate_id -> signature set for
    every candidate that failed to compile under all tried flagsets, and ok
    counts candidates that compiled under at least one (same semantics as
    the coverage line in `report`). With origin set, restrict to that origin so
    corpus failures don't pollute arcade shim-gap analysis (002 edge case)."""
    blocked, ok = {}, 0
    if origin is None:
        rows = conn.execute(
            "SELECT candidate_id, compile_status FROM arcade_candidate"
        ).fetchall()
    else:
        rows = conn.execute(
            "SELECT candidate_id, compile_status FROM arcade_candidate"
            " WHERE origin = ?", (origin,)
        ).fetchall()
    for r in rows:
        cs = json.loads(r["compile_status"])
        if not cs:
            continue
        if any(v == "ok" for v in cs.values()):
            ok += 1
            continue
        sigs = set()
        for status in cs.values():
            _, _, message = status.partition("fail:")
            sigs |= failure_signatures(message or status)
        blocked[r["candidate_id"]] = sigs
    return blocked, ok


def _locate_identifiers(idents):
    """Best-effort hint: first arcade file whose text looks like it defines
    each identifier (#define / typedef / enum body / declaration)."""
    hints = {}
    pats = {
        i: re.compile(
            r"(#\s*define\s+{0}\b|typedef\s.*\b{0}\s*;|^\s*{0}\s*[=,]"
            r"|\b{0}\s*(\[[^]]*\])?\s*[;=])".format(re.escape(i))
        )
        for i in idents
    }
    for path in sorted(extractmod.ARCADE.rglob("*.h")):
        if len(hints) == len(idents):
            break
        try:
            text = path.read_text(errors="replace")
        except OSError:
            continue
        for ident, pat in pats.items():
            if ident in hints or ident not in text:
                continue
            for n, line in enumerate(text.splitlines(), 1):
                if pat.search(line):
                    rel = path.relative_to(extractmod.ARCADE)
                    hints[ident] = f"{rel}:{n}: {line.strip()[:80]}"
                    break
    return hints


def _print_failure_histogram(blocked, args, locate=False):
    counts = Counter()
    for sigs in blocked.values():
        counts.update(sigs)  # each signature counted once per candidate
    top = counts.most_common(args.limit)
    print(f"\nblocked  signature (top {len(top)} of {len(counts)})")
    for sig, n in top:
        print(f"{n:>7}  {sig}")

    top_set = {sig for sig, _ in top}
    covered = sum(1 for sigs in blocked.values() if sigs <= top_set)
    print(f"\nfixing all {len(top)} shown clusters unblocks up to "
          f"{covered}/{len(blocked)} blocked candidates "
          "(lower bound: stderr capture is truncated)")

    if locate:
        idents = [
            sig.split(": ", 1)[1] for sig, _ in top
            if sig.startswith("undefined: ")
        ]
        hints = _locate_identifiers(idents)
        print("\narcade definition hints (heuristic):")
        for ident in idents:
            print(f"  {ident:<24} {hints.get(ident, '(not found in headers)')}")


def cmd_failures(args):
    conn, _ = _conn_store(args.data)
    # Which origins have any recorded compile results.
    origins = [r["origin"] for r in conn.execute(
        "SELECT DISTINCT origin FROM arcade_candidate WHERE compile_status != '{}'")]
    has_corpus = any(o != "arcade" for o in origins)

    if args.grep:
        blocked, _ = aggregate_failures(conn)
        hits = sorted(
            cid for cid, sigs in blocked.items()
            if any(args.grep in s for s in sigs)
        )
        print(f"{len(hits)} blocked candidates match {args.grep!r}:")
        for cid in hits:
            print(f"  {cid}")
        return

    if has_corpus:
        # Split by origin so corpus compile failures (e.g. reduced-TU static
        # deps) stay out of the arcade shim-gap histogram (002 edge case).
        any_blocked = False
        for origin in sorted(origins):
            blocked, ok = aggregate_failures(conn, origin)
            if not blocked and not ok:
                continue
            any_blocked = any_blocked or bool(blocked)
            print(f"\n== origin: {origin} == "
                  f"{len(blocked)} fail all tried flagsets ({ok} compile ok)")
            if blocked:
                # --locate greps the arcade tree, so only meaningful for arcade.
                _print_failure_histogram(blocked, args,
                                         locate=args.locate and origin == "arcade")
        if not any_blocked:
            print("no compile failures recorded (run `matrix ingest` first)")
        return

    blocked, ok = aggregate_failures(conn)
    if not blocked:
        print("no compile failures recorded (run `matrix ingest` first)")
        return
    print(f"{len(blocked)} candidates fail all tried flagsets ({ok} compile ok)")
    _print_failure_histogram(blocked, args, locate=args.locate)


# --- report (T025) -----------------------------------------------------------

def cmd_report(args):
    conn, _ = _conn_store(args.data)
    n_targets = conn.execute("SELECT COUNT(*) AS n FROM n64_target").fetchone()["n"]
    n_cands = conn.execute("SELECT COUNT(*) AS n FROM arcade_candidate").fetchone()["n"]
    n_cells = conn.execute("SELECT COUNT(*) AS n FROM matrix_entry").fetchone()["n"]
    print(f"matrix: {n_cells} cells scored ({n_targets} targets, {n_cands} candidates)")

    # Compile coverage, split by origin when corpus candidates exist (002);
    # arcade-only DBs print the original single line unchanged (SC-006).
    rows = conn.execute(
        "SELECT origin, compile_status FROM arcade_candidate").fetchall()
    by_origin = {}
    for r in rows:
        acc = by_origin.setdefault(r["origin"], [0, 0, 0])  # ok, fail, untried
        cs = json.loads(r["compile_status"])
        if not cs:
            acc[2] += 1
        elif any(v == "ok" for v in cs.values()):
            acc[0] += 1
        else:
            acc[1] += 1
    non_arcade = [o for o in by_origin if o != "arcade"]
    if non_arcade:
        print("candidate compile coverage (FR-002), by origin:")
        for origin in sorted(by_origin):
            ok, fail, untried = by_origin[origin]
            if ok + fail:
                print(f"  {origin:12} {ok} ok, {fail} fail, {untried} untried "
                      f"({100 * ok // max(ok + fail, 1)}% of tried)")
            elif untried:
                print(f"  {origin:12} {untried} untried")
    else:
        ok, fail, untried = by_origin.get("arcade", [0, 0, 0])
        if ok + fail:
            print(f"candidate compile coverage (FR-002): {ok} ok, {fail} fail, "
                  f"{untried} untried  ({100 * ok // max(ok + fail, 1)}% of tried)")

    rows = conn.execute(
        "SELECT status, COUNT(*) AS n FROM function_status GROUP BY status"
    ).fetchall()
    print("function status: " + "  ".join(f"{r['status']}={r['n']}" for r in rows))
    if args.target:
        print(f"\ntop candidates for {args.target}:")
        for r in rankings_for(conn, args.target):
            print(f"  {r['score']:>7}  {r['candidate_id']}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    parser.add_argument("--coordinator", default="http://127.0.0.1:8323")
    parser.add_argument("--token", default=None)
    sub = parser.add_subparsers(dest="command", required=True)

    p = sub.add_parser("extract")
    p.set_defaults(func=cmd_extract)

    p = sub.add_parser("submit")
    p.add_argument("--flagsets", nargs="*", default=list(FLAGSETS))
    p.add_argument("--dry-run", action="store_true")
    p.set_defaults(func=cmd_submit)

    p = sub.add_parser("ingest")
    p.set_defaults(func=cmd_ingest)

    p = sub.add_parser("report")
    p.add_argument("--target", default=None)
    p.set_defaults(func=cmd_report)

    p = sub.add_parser("failures")
    p.add_argument("--limit", type=int, default=30)
    p.add_argument("--grep", default=None,
                   help="list blocked candidates whose signatures contain this")
    p.add_argument("--locate", action="store_true",
                   help="grep arcade headers for likely definitions of top"
                        " undefined identifiers")
    p.set_defaults(func=cmd_failures)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
