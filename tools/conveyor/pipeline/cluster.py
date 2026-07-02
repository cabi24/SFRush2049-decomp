"""Near-clone clustering (US4, FR-008): group N64 targets by similarity so one
match unlocks its siblings.

    python3 -m tools.conveyor.pipeline.cluster run

Two stages, both cheap enough to run locally on the Pi (fingerprints are pure
computation; full scoring needs only mips objdump, not IDO — the reserved
cluster_score job type exists for scaling out later if the pool is idle):

1. Fingerprint every target: the multiset of opcode 4-grams (opcode = the
   instruction's primary+secondary op fields, operands ignored, so register
   allocation and addresses don't perturb it).
2. Union-find over pairs whose Jaccard similarity clears the threshold; only
   plausible pairs (size within 25%) are compared at all.
"""
import argparse
import hashlib
import json
import struct
from collections import defaultdict
from pathlib import Path

from ..coordinator import db as dbmod
from . import targets as targetsmod

DEFAULT_DATA = Path("~/.conveyor").expanduser()
JACCARD_THRESHOLD = 0.90
SIZE_TOLERANCE = 0.25
NGRAM = 4


def _opcode(word):
    """Stable opcode key for one MIPS instruction word: primary op field,
    plus the function field for SPECIAL/REGIMM so ALU ops stay distinct."""
    op = word >> 26
    if op == 0:  # SPECIAL
        return 0x40 | (word & 0x3F)
    if op == 1:  # REGIMM
        return 0x80 | ((word >> 16) & 0x1F)
    return op


def fingerprint(words):
    """Set of opcode n-grams for a function's instruction words."""
    ops = [_opcode(int(w, 16)) for w in words]
    return {tuple(ops[i : i + NGRAM]) for i in range(max(len(ops) - NGRAM + 1, 1))}


def _jaccard(a, b):
    if not a or not b:
        return 0.0
    inter = len(a & b)
    return inter / (len(a) + len(b) - inter)


def run_clustering(conn, threshold=JACCARD_THRESHOLD):
    rows = conn.execute(
        "SELECT target_id, address, insn_count FROM n64_target"
        " WHERE insn_count IS NOT NULL ORDER BY insn_count, target_id"
    ).fetchall()
    prints = {}
    for r in rows:
        try:
            words = targetsmod.function_words(r["address"], r["insn_count"] * 4)
        except (ValueError, OSError):
            continue
        prints[r["target_id"]] = fingerprint(words)

    # Union-find over size-plausible pairs.
    parent = {tid: tid for tid in prints}

    def find(x):
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    ordered = [r for r in rows if r["target_id"] in prints]
    similarities = {}
    for i, a in enumerate(ordered):
        for b in ordered[i + 1 :]:
            if b["insn_count"] > a["insn_count"] * (1 + SIZE_TOLERANCE):
                break  # rows sorted by size; nothing further is plausible
            sim = _jaccard(prints[a["target_id"]], prints[b["target_id"]])
            if sim >= threshold:
                ra, rb = find(a["target_id"]), find(b["target_id"])
                if ra != rb:
                    parent[rb] = ra
                similarities[f"{a['target_id']}|{b['target_id']}"] = round(sim, 3)

    clusters = defaultdict(list)
    for tid in prints:
        clusters[find(tid)].append(tid)

    n_clusters = 0
    with dbmod.tx(conn):
        conn.execute("DELETE FROM cluster")
        conn.execute("UPDATE n64_target SET cluster_id = NULL")
        for root, members in clusters.items():
            if len(members) < 2:
                continue  # singletons carry no sibling value
            cluster_id = "cl_" + hashlib.sha256(
                ",".join(sorted(members)).encode()
            ).hexdigest()[:12]
            sims = {k: v for k, v in similarities.items()
                    if k.split("|")[0] in members}
            conn.execute(
                "INSERT INTO cluster (cluster_id, member_ids, similarity)"
                " VALUES (?, ?, ?)",
                (cluster_id, json.dumps(sorted(members)), json.dumps(sims)),
            )
            for m in members:
                conn.execute(
                    "UPDATE n64_target SET cluster_id=? WHERE target_id=?",
                    (cluster_id, m),
                )
            n_clusters += 1
        # Fingerprints persisted for later incremental runs.
        for tid, fp in prints.items():
            fp_sha = hashlib.sha256(
                json.dumps(sorted(map(list, fp))).encode()
            ).hexdigest()
            conn.execute(
                "UPDATE n64_target SET fingerprint=? WHERE target_id=?",
                (fp_sha, tid),
            )
    clustered = sum(len(m) for m in clusters.values() if len(m) >= 2)
    return {"clusters": n_clusters, "clustered_targets": clustered,
            "total": len(prints)}


def seed_siblings(conn, store, matched_target_id, source_bytes):
    """FR-008 hook (called by the farm on a win): queue this source as the
    seed for every unmatched cluster sibling."""
    row = conn.execute(
        "SELECT cluster_id FROM n64_target WHERE target_id=?",
        (matched_target_id,),
    ).fetchone()
    if row is None or row["cluster_id"] is None:
        return 0
    members = conn.execute(
        "SELECT t.target_id, f.status FROM n64_target t"
        " JOIN function_status f USING (target_id)"
        " WHERE t.cluster_id=? AND t.target_id != ?",
        (row["cluster_id"], matched_target_id),
    ).fetchall()
    source_sha = store.put_bytes(source_bytes)
    seeded = 0
    for m in members:
        if m["status"] in ("matched", "verified", "in_search"):
            continue
        conn.execute(
            "UPDATE function_status SET status='candidate_identified',"
            " best_candidate_id=?, override=json_object('sibling_seed_sha', ?),"
            " human_flag=NULL, updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now')"
            " WHERE target_id=?",
            (f"matched:{matched_target_id}", source_sha, m["target_id"]),
        )
        seeded += 1
    return seeded


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    parser.add_argument("--threshold", type=float, default=JACCARD_THRESHOLD)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("run")
    p.set_defaults(cmd="run")
    args = parser.parse_args()
    conn = dbmod.connect(Path(args.data) / "conveyor.db")
    stats = run_clustering(conn, args.threshold)
    print(f"clusters: {stats['clusters']} multi-member groups covering "
          f"{stats['clustered_targets']} of {stats['total']} targets")


if __name__ == "__main__":
    main()
