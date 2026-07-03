"""Flag-sweep orchestration (US3): pin per-TU compiler settings (FR-007).

    python3 -m tools.conveyor.pipeline.sweep submit --tu src/libc/string.c \\
        --functions strlen,memchr
    python3 -m tools.conveyor.pipeline.sweep ingest
    python3 -m tools.conveyor.pipeline.sweep show

Functions are extracted from the repo source file (single-function TUs) and
scored against their n64_target objects on the pool. The winner lands in
flag_registry (source='sweep'); manual pins (source='manual_override',
FR-015) are never overwritten.
"""
import argparse
import json
import sys
import tarfile
import tempfile
from pathlib import Path

from ..bundles.build_job import build_job_bundle
from ..client import DEFAULT_DATA, Http, load_token
from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from ..seeds.extract_candidates import extract_named_function
from . import flags as flagsmod

REPO = Path(__file__).resolve().parents[3]

# The same shim the matrix and farm compile against — sweeping under a
# different set of typedefs would pin flags chosen from different codegen.
SHIM_INCLUDE = '#include "conveyor_shim.h"\n\n'


def cmd_submit(args):
    data = Path(args.data)
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    http = Http(args.coordinator, load_token(args.token, data))
    toolkit_sha = http.pinned_toolkit()

    names = [n.strip() for n in args.functions.split(",") if n.strip()]
    functions, files = [], {}
    for name in names:
        row = conn.execute(
            "SELECT target_o_sha FROM n64_target WHERE target_id=?", (name,)
        ).fetchone()
        if row is None or not row["target_o_sha"]:
            print(f"  skip {name}: no target object", file=sys.stderr)
            continue
        try:
            body = extract_named_function(REPO / args.tu, name)
        except (KeyError, OSError) as exc:
            print(f"  skip {name}: {exc}", file=sys.stderr)
            continue
        files[f"{name}.c"] = (SHIM_INCLUDE + body + "\n").encode()
        files[f"{name}.o"] = store.get(row["target_o_sha"]).read_bytes()
        functions.append({"name": name, "source": f"{name}.c",
                          "target": f"{name}.o"})
    if not functions:
        sys.exit("no usable functions")

    manifest = {
        "job_type": "flag_sweep", "toolkit_sha": toolkit_sha,
        "tu": args.tu, "flagsets": list(flagsmod.LATTICE),
        "functions": functions,
    }
    with tempfile.TemporaryDirectory() as tmp:
        bundle, m_sha = build_job_bundle(manifest, files, Path(tmp) / "s.tar.gz")
        _, out = http.call("POST", "/api/v1/blobs", raw=bundle.read_bytes())
    _, submitted = http.call("POST", "/api/v1/work", body=[{
        "job_type": "flag_sweep", "manifest_sha": m_sha,
        "bundle_sha": out["sha256"], "toolkit_sha": toolkit_sha,
        "priority": 50,
    }])
    print(f"sweep for {args.tu} ({len(functions)} functions, "
          f"{len(flagsmod.LATTICE)} flagsets): {submitted[0]}")


def cmd_ingest(args):
    data = Path(args.data)
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    rows = conn.execute(
        "SELECT job_id, result_sha FROM work_unit"
        " WHERE job_type='flag_sweep' AND state='DONE'"
        " AND result_sha IS NOT NULL AND ingested_at IS NULL"
    ).fetchall()
    pinned = 0
    for row in rows:
        path = store.get(row["result_sha"])
        if path is None:
            continue  # blob missing: retry next run
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
            payload = result["payload"]
            winner = payload["rankings"][0]["flagset"]
            existing = conn.execute(
                "SELECT source FROM flag_registry WHERE translation_unit=?",
                (payload["tu"],),
            ).fetchone()
            if existing and existing["source"] == "manual_override":
                continue  # FR-015: never clobber a manual pin
            conn.execute(
                "INSERT INTO flag_registry (translation_unit, pinned_flagset,"
                " evidence, source) VALUES (?, ?, ?, 'sweep')"
                " ON CONFLICT(translation_unit) DO UPDATE SET"
                " pinned_flagset=excluded.pinned_flagset,"
                " evidence=excluded.evidence, source='sweep'",
                (payload["tu"], winner, json.dumps(payload["rankings"])),
            )
            pinned += 1
    print(f"pinned {pinned} translation units")


def cmd_show(args):
    conn = dbmod.connect(Path(args.data) / "conveyor.db")
    for r in conn.execute("SELECT * FROM flag_registry ORDER BY translation_unit"):
        print(f"{r['translation_unit']:40} {r['pinned_flagset']:36} [{r['source']}]")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    parser.add_argument("--coordinator", default="http://127.0.0.1:8323")
    parser.add_argument("--token", default=None)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("submit")
    p.add_argument("--tu", required=True)
    p.add_argument("--functions", required=True)
    p.set_defaults(func=cmd_submit)
    p = sub.add_parser("ingest")
    p.set_defaults(func=cmd_ingest)
    p = sub.add_parser("show")
    p.set_defaults(func=cmd_show)
    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
