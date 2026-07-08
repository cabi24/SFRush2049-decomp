"""Regression lock for verified-matched functions.

    python3 -m tools.conveyor.pipeline.lock check              # fast, local
    python3 -m tools.conveyor.pipeline.lock add src/libc/string.c:strlen \
        --flags "-g0 -O2 -mips2 -G 0 -non_shared"              # needs pool
    python3 -m tools.conveyor.pipeline.lock verify [spec]      # re-prove
    python3 -m tools.conveyor.pipeline.lock list | remove <spec>

`matched.lock.json` (checked in at the repo root) pins the normalized
source-body hash of every function proven byte-identical to the ROM.
`check` re-hashes in milliseconds — no IDO, no network — so drift in a
matched function is caught at commit time, not months later when a
builder node happens to be powered on (the strlen lesson, 2026-07-08).

`add` refuses to pin on trust: it compiles the function through the pool
with the entry's pinned flags and requires score 0 against the target .o
from the inventory (pass --skip-verify to record an `unverified` entry).
Verification compiles a *reduced TU*: the real source file with every
other function definition stripped, plus its repo headers resolved
transitively — same types and declarations as the real build, and no
dependence on the toolkit shim.

Hash normalization strips comments and collapses whitespace (string and
char literals preserved verbatim), so reflowing or re-commenting a
matched function does not trip the lock — changing its code does.
"""
import argparse
import hashlib
import json
import re
import sys
import tarfile
import tempfile
import time
from pathlib import Path

from ..client import DEFAULT_DATA, Http, load_token
from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from ..seeds import context as contextmod
from ..seeds.extract_candidates import REPO, extract_functions

LOCKFILE = REPO / "matched.lock.json"
INCLUDE_DIRS = ("include", "include/PR")  # mirrors Makefile INCLUDE_CFLAGS


# --- source normalization -----------------------------------------------------

def normalize_body(text):
    """Comment-stripped, whitespace-collapsed body text. String and char
    literals are preserved verbatim so the hash still moves when they
    change; everything a compiler ignores is squeezed out."""
    segments, buf = [], []
    i, n = 0, len(text)

    def flush():
        if buf:
            segments.append(re.sub(r"\s+", " ", "".join(buf)))
            buf.clear()

    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            j = text.find("*/", i + 2)
            i = n if j == -1 else j + 2
            buf.append(" ")
        elif c == "/" and i + 1 < n and text[i + 1] == "/":
            j = text.find("\n", i)
            i = n if j == -1 else j
        elif c in "\"'":
            j = i + 1
            while j < n:
                if text[j] == "\\":
                    j += 2
                    continue
                if text[j] == c:
                    break
                j += 1
            j = min(j + 1, n)
            flush()
            segments.append(text[i:j])
            i = j
        else:
            buf.append(c)
            i += 1
    flush()
    return "".join(segments).strip()


def body_sha(source_path, name):
    """Normalized-body hash of one function, or None if it can't be found."""
    try:
        text = Path(source_path).read_text(errors="replace")
    except OSError:
        return None
    for got, start, end in extract_functions(text):
        if got == name:
            return hashlib.sha256(
                normalize_body(text[start:end]).encode()
            ).hexdigest()
    return None


# --- lockfile -----------------------------------------------------------------

def load_lock(path=LOCKFILE):
    if not Path(path).is_file():
        return {}
    return json.loads(Path(path).read_text())


def save_lock(entries, path=LOCKFILE):
    Path(path).write_text(json.dumps(entries, indent=2, sort_keys=True) + "\n")


def check(entries, repo=REPO):
    """[(spec, problem)] for every locked function that no longer hashes to
    its pinned value. Empty list = all locks hold."""
    problems = []
    for spec, entry in sorted(entries.items()):
        rel, _, name = spec.partition(":")
        sha = body_sha(Path(repo) / rel, name)
        if sha is None:
            problems.append((spec, "function not found (moved or deleted?)"))
        elif sha != entry["body_sha256"]:
            problems.append((spec, "body drifted from the verified-matched form"))
    return problems


# --- verification through the pool ---------------------------------------------

# The reduced-TU and header-resolution helpers now live in seeds/context.py so
# the corpus pipeline shares them; the lock binds them to the game repo's
# include layout here.
reduced_tu = contextmod.reduced_tu


def resolve_headers(tu_path, repo=REPO):
    """The TU's `#include "..."` closure against the game repo's include dirs."""
    return contextmod.resolve_headers(tu_path, repo, INCLUDE_DIRS)


def verify_entry(spec, flagset, target_id, args):
    """Compile spec's function through the pool and score it against the
    inventory target .o. Returns (score, toolkit_sha); raises on failure."""
    from ..bundles.build_job import build_job_bundle

    rel, _, name = spec.partition(":")
    tu_path = REPO / rel
    text = tu_path.read_text(errors="replace")

    data = Path(args.data)
    conn = dbmod.connect(data / "conveyor.db")
    row = conn.execute(
        "SELECT target_o_sha FROM n64_target WHERE target_id=?", (target_id,)
    ).fetchone()
    if row is None or not row["target_o_sha"]:
        raise RuntimeError(f"no target .o in inventory for {target_id!r} "
                           "(run `matrix extract` first)")
    target_o_sha = row["target_o_sha"]
    target_o = BlobStore(data / "blobs").get(target_o_sha).read_bytes()

    files = {"candidate.c": reduced_tu(text, name).encode()}
    for hdr_rel, hdr_text in resolve_headers(tu_path).items():
        files[hdr_rel] = hdr_text.encode()
    files["target.o"] = target_o

    http = Http(args.coordinator, load_token(args.token, args.data))
    toolkit_sha = http.pinned_toolkit()
    manifest = {
        "job_type": "compile_score",
        "toolkit_sha": toolkit_sha,
        "include_dirs": list(INCLUDE_DIRS),
        "cells": [{
            "candidate_id": spec, "source": "candidate.c", "flagset": flagset,
            "targets": [{"target_id": target_id, "file": "target.o",
                         "target_o_sha": target_o_sha}],
        }],
    }
    with tempfile.TemporaryDirectory() as tmp:
        bundle, m_sha = build_job_bundle(manifest, files, Path(tmp) / "job.tar.gz")
        _, out = http.call("POST", "/api/v1/blobs", raw=bundle.read_bytes())
        _, submitted = http.call("POST", "/api/v1/work", body=[{
            "job_type": "compile_score", "manifest_sha": m_sha,
            "bundle_sha": out["sha256"], "toolkit_sha": toolkit_sha,
            "priority": 1,
        }])
        entry = submitted[0]
        if "cached_result" in entry:
            result_sha = entry["cached_result"]
        else:
            job_id = entry["job_id"]
            print(f"verify job {job_id} submitted; waiting for a node…")
            deadline = time.time() + args.wait
            result_sha = None
            while time.time() < deadline:
                _, job = http.call("GET", f"/api/v1/work/{job_id}")
                if job["state"] == "DONE":
                    result_sha = job["result_sha"]
                    break
                if job["state"] == "FAILED":
                    raise RuntimeError("verify job FAILED on the pool")
                time.sleep(5)
            if result_sha is None:
                raise RuntimeError(f"timed out after {args.wait}s — "
                                   "is a node attached?")
        result_path = Path(tmp) / "results.tar.gz"
        http.download(result_sha, result_path)
        with tarfile.open(result_path) as tar:
            result = json.loads(tar.extractfile("result.json").read())
    if result["exit"] != "ok":
        raise RuntimeError(f"verify job errored: {result.get('error')}")
    cell = result["payload"]["cells"][0]
    if cell["compile"] != "ok":
        raise RuntimeError(f"compile failed: {cell['compile'][:300]}")
    return cell["score"], toolkit_sha


# --- commands -------------------------------------------------------------------

def cmd_check(args):
    entries = load_lock(args.lockfile)
    if not entries:
        print("matched.lock.json is empty — nothing locked yet")
        return
    problems = check(entries)
    if not args.quiet:
        for spec in sorted(entries):
            mark = "DRIFT" if any(s == spec for s, _ in problems) else "ok"
            print(f"  {mark:<5}  {spec}")
    if problems:
        print(f"\n{len(problems)} locked function(s) drifted:", file=sys.stderr)
        for spec, why in problems:
            print(f"  {spec}: {why}", file=sys.stderr)
        print("A matched function was edited. Revert it, or re-verify and "
              "re-pin with:\n  python3 -m tools.conveyor.pipeline.lock add "
              "<file>:<fn> --flags '...'", file=sys.stderr)
        sys.exit(1)
    print(f"all {len(entries)} locked functions intact")


def cmd_list(args):
    for spec, e in sorted(load_lock(args.lockfile).items()):
        print(f"  {spec}  target={e['target_id']}  {e['verified']}"
              f"  ({e['verified_at']})")


def cmd_remove(args):
    entries = load_lock(args.lockfile)
    if entries.pop(args.spec, None) is None:
        sys.exit(f"{args.spec} is not locked")
    save_lock(entries, args.lockfile)
    print(f"unlocked {args.spec}")


def cmd_add(args):
    rel, _, name = args.spec.partition(":")
    if not name:
        sys.exit("spec must be <repo-relative-file>:<function>")
    sha = body_sha(REPO / rel, name)
    if sha is None:
        sys.exit(f"cannot extract {name!r} from {rel}")
    target_id = args.target or name

    if args.skip_verify:
        verified, toolkit_sha = "unverified", None
        print(f"WARNING: pinning {args.spec} without verification — "
              "the lock now asserts a match nobody has proven")
    else:
        score, toolkit_sha = verify_entry(args.spec, args.flags, target_id, args)
        print(f"{args.spec} @ {args.flags}: score={score}")
        if score != 0:
            sys.exit("refusing to lock: score is nonzero (not byte-identical)")
        verified = "score0"

    entries = load_lock(args.lockfile)
    entries[args.spec] = {
        "body_sha256": sha,
        "target_id": target_id,
        "flagset": args.flags,
        "verified": verified,
        "toolkit_sha": toolkit_sha,
        "verified_at": time.strftime("%Y-%m-%d"),
    }
    save_lock(entries, args.lockfile)
    print(f"locked {args.spec} ({verified})")


def cmd_verify(args):
    entries = load_lock(args.lockfile)
    specs = [args.spec] if args.spec else sorted(entries)
    failures = 0
    for spec in specs:
        e = entries.get(spec)
        if e is None:
            sys.exit(f"{spec} is not locked")
        score, _ = verify_entry(spec, e["flagset"], e["target_id"], args)
        print(f"  {spec}: score={score}")
        if score != 0:
            failures += 1
    if failures:
        sys.exit(f"{failures} locked function(s) no longer verify at score 0")
    print(f"all {len(specs)} re-verified byte-identical")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    parser.add_argument("--coordinator", default="http://127.0.0.1:8323")
    parser.add_argument("--token", default=None)
    parser.add_argument("--lockfile", default=str(LOCKFILE))
    sub = parser.add_subparsers(dest="command", required=True)

    p = sub.add_parser("check")
    p.add_argument("--quiet", action="store_true")
    p.set_defaults(func=cmd_check)

    p = sub.add_parser("list")
    p.set_defaults(func=cmd_list)

    p = sub.add_parser("add")
    p.add_argument("spec", help="<repo-relative-file>:<function>")
    p.add_argument("--flags", required=True)
    p.add_argument("--target", default=None,
                   help="n64_target id (default: the function name)")
    p.add_argument("--skip-verify", action="store_true")
    p.add_argument("--wait", type=int, default=600)
    p.set_defaults(func=cmd_add)

    p = sub.add_parser("verify")
    p.add_argument("spec", nargs="?", default=None)
    p.add_argument("--wait", type=int, default=600)
    p.set_defaults(func=cmd_verify)

    p = sub.add_parser("remove")
    p.add_argument("spec")
    p.set_defaults(func=cmd_remove)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
