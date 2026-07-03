#!/usr/bin/env python3
"""conveyor CLI — operate the matching pipeline from the Pi.

    python3 -m tools.conveyor.cli serve [--port 8323] [--data ~/.conveyor]
    python3 -m tools.conveyor.cli publish-toolkit <toolkit.tar.gz> [--coordinator URL]
    python3 -m tools.conveyor.cli nodes | status
    python3 -m tools.conveyor.cli submit <jobs.json>
    python3 -m tools.conveyor.cli smoke [--function strlen] [--wait 600]
"""
import argparse
import hashlib
import json
import re
import sys
import tempfile
import time
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
DEFAULT_DATA = Path("~/.conveyor").expanduser()
API = "/api/v1"

# Known-good flag sets (docs/COMPILER_SETTINGS.md).
FLAGSET_O2 = "-g0 -O2 -mips2 -G 0 -non_shared"

# Smoke fixture: strlen — hand-confirmed perfect match at -O2.
SMOKE_FUNCTIONS = {
    "strlen": {
        "asm_file": "asm/us/8800.s",
        "label": "func_80007C40",
        "source_file": "src/libc/string.c",
        "c_name": "strlen",
        "flagset": FLAGSET_O2,
    },
}


def _client(args):
    from .client import Http, load_token

    return Http(args.coordinator, load_token(args.token))


# --- commands ---------------------------------------------------------------

def cmd_serve(args):
    from .coordinator.server import serve

    serve(args.data, port=args.port)


def cmd_publish_toolkit(args):
    client = _client(args)
    data = Path(args.toolkit).read_bytes()
    local_sha = hashlib.sha256(data).hexdigest()
    status, out = client.call("POST", f"{API}/blobs?kind=toolkit", raw=data)
    if status != 201:
        sys.exit(f"blob upload failed: {status} {out}")
    assert out["sha256"] == local_sha
    status, _ = client.call("PUT", f"{API}/meta/toolkit_sha", body={"value": local_sha})
    if status != 200:
        sys.exit(f"failed to pin toolkit: {status}")
    print(f"toolkit sha256 {local_sha} pinned as current")


def cmd_nodes(args):
    client = _client(args)
    status, nodes = client.call("GET", f"{API}/nodes")
    if status != 200:
        sys.exit(f"error {status}")
    if not nodes:
        print("no nodes have ever registered")
        return
    for n in nodes:
        caps = ",".join(n["capabilities"])
        print(f"{n['hostname']:20} cores={n['cores']:<3} caps={caps:20} "
              f"running={n['running']} last_seen={n['last_seen']}")


def cmd_status(args):
    client = _client(args)
    status, out = client.call("GET", f"{API}/status")
    if status != 200:
        sys.exit(f"error {status}")
    f, q = out["functions"], out["queue"]
    total = sum(f.values())
    print(f"functions ({total}): " + "  ".join(f"{k}={v}" for k, v in f.items()))
    print("queue:            " + "  ".join(f"{k}={v}" for k, v in q.items()))
    print(f"nodes:            {len(out['nodes'])} registered")
    if out["human_attention"]:
        print(f"needs attention:  {len(out['human_attention'])} functions "
              f"(top: {', '.join(a['target_id'] for a in out['human_attention'][:5])})")


def cmd_submit(args):
    client = _client(args)
    jobs = json.loads(Path(args.jobs_file).read_text())
    status, out = client.call("POST", f"{API}/work", body=jobs)
    print(json.dumps(out, indent=2))
    sys.exit(0 if status == 201 else 1)


# --- farm verbs (US2) ---------------------------------------------------------

def _open_db(args):
    from .coordinator import db as dbmod
    from .coordinator.store import BlobStore

    data = Path(getattr(args, "data", None) or DEFAULT_DATA)
    return dbmod.connect(data / "conveyor.db"), BlobStore(data / "blobs")


def cmd_seed(args):
    """Manual seed (FR-015): queue a search from a hand-written source file."""
    from .coordinator import db as dbmod
    from .pipeline import seeds as seedsmod
    from .pipeline import status as statusmod

    conn, store = _open_db(args)
    client = _client(args)
    toolkit_sha = client.pinned_toolkit()
    row = conn.execute(
        "SELECT status, flagset FROM function_status WHERE target_id=?",
        (args.target,),
    ).fetchone()
    if row is None:
        sys.exit(f"unknown target {args.target}")
    if row["status"] == "verified" and not args.force:
        sys.exit(f"{args.target} is already verified (committed to the repo); "
                 "re-seeding would supersede a promoted match — pass --force "
                 "if you really mean it")
    source_text = seedsmod.seed_source(Path(args.source_file).read_text())
    flagset = args.flagset or row["flagset"] or "-g0 -O2 -mips2 -G 0 -non_shared"
    bundle, m_sha, job = seedsmod.build_search_bundle(
        conn, store, args.target, source_text, flagset, toolkit_sha,
        budget={"wall_seconds": args.budget, "iterations": None},
    )
    status, out = client.call("POST", f"{API}/blobs", raw=bundle.read_bytes())
    job.update(bundle_sha=out["sha256"], target_id=args.target, priority=5)
    status, submitted = client.call("POST", f"{API}/work", body=[job])
    with dbmod.tx(conn):
        source_sha = store.put_bytes(source_text.encode())
        conn.execute(
            "UPDATE function_status SET seed_kind='manual', seed_source_sha=?,"
            " override=json_object('manual_seed', ?),"
            " updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now') WHERE target_id=?",
            (source_sha, args.source_file, args.target),
        )
        statusmod.transition(conn, args.target, "in_search",
                             human_flag=None, force=True)
    print(f"seeded {args.target}: job {submitted[0].get('job_id')}")


def cmd_best(args):
    """Show best-so-far source and score for a target's searches."""
    import gzip as gzmod

    conn, store = _open_db(args)
    row = conn.execute(
        "SELECT job_id, state, best_score, best_source_sha FROM work_unit"
        " WHERE target_id=? AND job_type='permuter_search'"
        " ORDER BY best_score IS NULL, best_score LIMIT 1",
        (args.target,),
    ).fetchone()
    if row is None:
        sys.exit(f"no search jobs for {args.target}")
    print(f"job {row['job_id']} state={row['state']} best_score={row['best_score']}")
    if row["best_source_sha"]:
        path = store.get(row["best_source_sha"])
        if path:
            data = path.read_bytes()
            try:
                data = gzmod.decompress(data)
            except OSError:
                pass
            print(data.decode(errors="replace"))


def cmd_pin_flags(args):
    """Manually pin a TU's compiler flags (FR-015; survives sweep re-runs)."""
    from .coordinator import db as dbmod

    conn, _ = _open_db(args)
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO flag_registry (translation_unit, pinned_flagset,"
            " evidence, source) VALUES (?, ?, '[]', 'manual_override')"
            " ON CONFLICT(translation_unit) DO UPDATE SET"
            " pinned_flagset=excluded.pinned_flagset, source='manual_override'",
            (args.tu, args.flags),
        )
    print(f"pinned {args.tu} -> {args.flags} (manual_override)")


def cmd_pair(args):
    """Manually pair a target with an arcade candidate (FR-015)."""
    from .coordinator import db as dbmod
    from .pipeline import status as statusmod

    conn, _ = _open_db(args)
    row = conn.execute(
        "SELECT status FROM function_status WHERE target_id=?", (args.target,)
    ).fetchone()
    if row is None:
        sys.exit(f"unknown target {args.target}")
    if row["status"] in ("matched", "verified"):
        sys.exit(f"{args.target} is already {row['status']}; re-pairing would "
                 "discard a match — unflag it first if that is intended")
    with dbmod.tx(conn):
        conn.execute(
            "UPDATE function_status SET best_candidate_id=?,"
            " override=json_object('manual_pair', ?),"
            " updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now') WHERE target_id=?",
            (args.candidate, args.candidate, args.target),
        )
        statusmod.transition(conn, args.target, "candidate_identified",
                             human_flag=None, force=True)
    print(f"paired {args.target} <- {args.candidate} (manual override)")


def cmd_attention(args):
    """Ranked human-attention queue (FR-012)."""
    conn, _ = _open_db(args)
    rows = conn.execute(
        "SELECT target_id, status, human_flag, best_score FROM function_status"
        " WHERE human_flag IS NOT NULL"
        " ORDER BY best_score IS NULL, best_score LIMIT ?",
        (args.limit,),
    ).fetchall()
    if not rows:
        print("nothing needs attention")
        return
    for r in rows:
        print(f"{r['target_id']:32} {r['status']:22} {r['human_flag']:24} "
              f"best={r['best_score']}")


def cmd_report(args):
    """Full progress roll-up (FR-012): counts, match %, attention, health."""
    from .pipeline import status as statusmod

    conn, _ = _open_db(args)
    c = statusmod.counts(conn)
    total = sum(c.values())
    done = c["matched"] + c["verified"]
    print(f"functions: {total} tracked, {done} matched "
          f"({100 * done // max(total, 1)}%), {c['verified']} verified")
    print("  " + "  ".join(f"{k}={v}" for k, v in c.items()))
    n_cells = conn.execute("SELECT COUNT(*) AS n FROM matrix_entry").fetchone()["n"]
    n_clusters = conn.execute("SELECT COUNT(*) AS n FROM cluster").fetchone()["n"]
    n_pins = conn.execute("SELECT COUNT(*) AS n FROM flag_registry").fetchone()["n"]
    print(f"matrix cells: {n_cells}   clusters: {n_clusters}   pinned TUs: {n_pins}")
    problems = statusmod.reconcile(conn)
    if problems:
        print(f"RECONCILE PROBLEMS ({len(problems)}):")
        for p in problems[:10]:
            print(f"  ! {p}")
    else:
        print("reconcile: clean")
    rows = conn.execute(
        "SELECT target_id, human_flag, best_score FROM function_status"
        " WHERE human_flag IS NOT NULL ORDER BY best_score IS NULL, best_score"
        " LIMIT 10").fetchall()
    if rows:
        print("needs attention (top 10):")
        for r in rows:
            print(f"  {r['target_id']:32} {r['human_flag']:24} best={r['best_score']}")


# --- smoke ------------------------------------------------------------------

_WORD_RE = re.compile(r"/\*\s*\w+\s+\w+\s+([0-9A-Fa-f]{8})\s*\*/")


def extract_target_words(asm_file, label):
    """Instruction words for one function from a splat .s file."""
    lines = (REPO / asm_file).read_text().splitlines()
    words, active = [], False
    for line in lines:
        if line.startswith(f"glabel {label}"):
            active = True
            continue
        if active and line.startswith("endlabel"):
            break
        if active:
            m = _WORD_RE.search(line)
            if m:
                words.append(m.group(1))
    if not words:
        raise SystemExit(f"could not extract {label} from {asm_file}")
    return words


def assemble_words(words, out_o, func_name):
    """Single implementation lives beside the target inventory builder."""
    from .pipeline.targets import assemble_words as _impl

    _impl(words, out_o, func_name)


# The toolkit shim is on every compile's include path, so smoke compiles
# under the exact same typedefs as matrix/farm/sweep jobs.
SMOKE_INCLUDE = '#include "conveyor_shim.h"\n\n'


def cmd_smoke(args):
    from .bundles.build_job import build_job_bundle

    spec = SMOKE_FUNCTIONS.get(args.function)
    if spec is None:
        sys.exit(f"no smoke fixture for {args.function!r} "
                 f"(available: {', '.join(SMOKE_FUNCTIONS)})")
    client = _client(args)

    status, meta = client.call("GET", f"{API}/meta/toolkit_sha")
    if status != 200:
        sys.exit("no toolkit pinned — run publish-toolkit first")
    toolkit_sha = meta["value"]

    with tempfile.TemporaryDirectory() as tmp:
        from .seeds.extract_candidates import extract_named_function

        target_o = Path(tmp) / "target.o"
        words = extract_target_words(spec["asm_file"], spec["label"])
        assemble_words(words, target_o, spec["c_name"])
        source = SMOKE_INCLUDE + extract_named_function(
            REPO / spec["source_file"], spec["c_name"]
        ) + "\n"

        manifest = {
            "job_type": "compile_score",
            "toolkit_sha": toolkit_sha,
            "cells": [{
                "candidate_id": f"{spec['source_file']}:{spec['c_name']}",
                "source": "candidate.c",
                "flagset": spec["flagset"],
                "targets": [{"target_id": spec["c_name"], "file": "target.o"}],
            }],
        }
        bundle_path, manifest_sha = build_job_bundle(
            manifest,
            {"candidate.c": source.encode(), "target.o": target_o.read_bytes()},
            Path(tmp) / "job.tar.gz",
        )
        status, out = client.call("POST", f"{API}/blobs", raw=bundle_path.read_bytes())
        assert status == 201, out
        status, submitted = client.call("POST", f"{API}/work", body=[{
            "job_type": "compile_score",
            "manifest_sha": manifest_sha,
            "bundle_sha": out["sha256"],
            "toolkit_sha": toolkit_sha,
            "priority": 1,
        }])
        assert status == 201, submitted
        entry = submitted[0]
        if "cached_result" in entry:
            result_sha = entry["cached_result"]
            print("(cached result)")
        else:
            job_id = entry["job_id"]
            print(f"smoke job {job_id} submitted; waiting for a node…")
            deadline = time.time() + args.wait
            result_sha = None
            while time.time() < deadline:
                _, job = client.call("GET", f"{API}/work/{job_id}")
                if job["state"] == "DONE":
                    result_sha = job["result_sha"]
                    break
                if job["state"] == "FAILED":
                    sys.exit("smoke job FAILED (all attempts exhausted)")
                time.sleep(5)
            if result_sha is None:
                sys.exit(f"timed out after {args.wait}s — is a node attached?")

        result_path = Path(tmp) / "results.tar.gz"
        client.download(result_sha, result_path)
        import tarfile

        with tarfile.open(result_path) as tar:
            result = json.loads(tar.extractfile("result.json").read())

    if result["exit"] != "ok":
        sys.exit(f"job errored: {result['error']}")
    cell = result["payload"]["cells"][0]
    print(f"{cell['candidate_id']} @ {cell['flagset']}: "
          f"compile={cell['compile']} score={cell['score']}")
    if cell["score"] == 0:
        print("SMOKE PASS — fabric proven end to end (score 0 = byte-identical)")
    else:
        sys.exit("SMOKE FAIL — nonzero score")


def main():
    parser = argparse.ArgumentParser(prog="conveyor", description=__doc__)
    parser.add_argument("--coordinator", default="http://127.0.0.1:8323")
    parser.add_argument("--token", default=None)
    sub = parser.add_subparsers(dest="command", required=True)

    p = sub.add_parser("serve")
    p.add_argument("--port", type=int, default=8323)
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.set_defaults(func=cmd_serve)

    p = sub.add_parser("publish-toolkit")
    p.add_argument("toolkit")
    p.set_defaults(func=cmd_publish_toolkit)

    p = sub.add_parser("nodes")
    p.set_defaults(func=cmd_nodes)

    p = sub.add_parser("status")
    p.set_defaults(func=cmd_status)

    p = sub.add_parser("submit")
    p.add_argument("jobs_file")
    p.set_defaults(func=cmd_submit)

    p = sub.add_parser("smoke")
    p.add_argument("--function", default="strlen")
    p.add_argument("--wait", type=int, default=600)
    p.set_defaults(func=cmd_smoke)

    p = sub.add_parser("seed")
    p.add_argument("target")
    p.add_argument("source_file")
    p.add_argument("--force", action="store_true",
                   help="allow re-seeding a verified function")
    p.add_argument("--flagset", default=None)
    p.add_argument("--budget", type=int, default=4 * 3600)
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.set_defaults(func=cmd_seed)

    p = sub.add_parser("best")
    p.add_argument("target")
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.set_defaults(func=cmd_best)

    p = sub.add_parser("attention")
    p.add_argument("--limit", type=int, default=40)
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.set_defaults(func=cmd_attention)

    p = sub.add_parser("pin-flags")
    p.add_argument("tu")
    p.add_argument("flags")
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.set_defaults(func=cmd_pin_flags)

    p = sub.add_parser("pair")
    p.add_argument("target")
    p.add_argument("candidate")
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.set_defaults(func=cmd_pair)

    p = sub.add_parser("report")
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.set_defaults(func=cmd_report)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
