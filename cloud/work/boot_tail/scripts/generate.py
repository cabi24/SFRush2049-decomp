#!/usr/bin/env python3
"""Generate Packet 1's metadata-only boot-tail ledger; no compiler or ROM needed."""
import argparse
from collections import Counter
import csv
import hashlib
import io
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[4]
WORK = ROOT / "cloud/work/boot_tail"
INVENTORY = ROOT / "specs/015-boot-tail-runtime/inventory.json"
SYMBOLS = ROOT / "symbol_addrs.us.txt"
FIELDS = ["address", "name", "size", "cluster", "name_hypothesis", "evidence_level",
          "status", "owner", "flags", "diff_words", "source_path", "pr", "note"]
LEVELS = {"HYPOTHESIS", "SOURCE-LEAD", "PARTIAL-SOURCE", "COMPLETE-NONMATCH", "VERIFIED-BODY"}
STATES = {"open", "claimed", "nonmatch", "verified_body", "non_c", "excluded"}
ANNOTATION_FIELDS = set(FIELDS) - {"address", "name", "size", "cluster"} | {"matching_blocker"}


def addr(value):
    return int(value, 16)


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def rows_in(rows, lo, hi, scope=None):
    return [r for r in rows if lo <= addr(r["address"]) < hi
            and (scope is None or r["scope"] == scope)]


def validate(rows, research):
    starts = [addr(r["address"]) for r in rows]
    if starts != sorted(set(starts)):
        raise ValueError("inventory must be sorted, unique function starts")
    if any(r["size"] <= 0 or r["size"] % 4 for r in rows):
        raise ValueError("inventory sizes must be positive word multiples")
    if any(addr(a["address"]) + a["size"] > addr(b["address"])
           for a, b in zip(rows, rows[1:])):
        raise ValueError("overlapping inventory extents")
    population = Counter(r["scope"] for r in rows)
    totals = {s: sum(r["size"] for r in rows if r["scope"] == s) for s in population}
    if population != {"in_scope": 412, "ultralib_identified": 21, "stub_unclassified": 6}:
        raise ValueError("inventory population drift; escalate instead of silently regenerating")
    if totals != {"in_scope": 95012, "ultralib_identified": 4048, "stub_unclassified": 60}:
        raise ValueError("inventory byte totals drift")
    known = {r["address"] for r in rows}
    for r in rows:
        if len(r["callees_tail"]) != len(set(r["callees_tail"])):
            raise ValueError("duplicate scanned edge")
        if any(t not in known for t in r["callees_tail"] + r["callers_tail"]):
            raise ValueError("scanned tail edge has unknown start; escalate extent conflict")
        expected_callers = sorted(x["address"] for x in rows if r["address"] in x["callees_tail"])
        if sorted(r["callers_tail"]) != expected_callers:
            raise ValueError("caller/callee edge reciprocity drift")
    cohorts = research["cohorts"]
    if len({c["id"] for c in cohorts}) != len(cohorts):
        raise ValueError("duplicate cohort ID")
    if any(addr(a["end"]) != addr(b["start"]) for a, b in zip(cohorts, cohorts[1:])):
        raise ValueError("cohorts must form contiguous disjoint half-open runs")
    if any(addr(c["start"]) >= addr(c["end"]) for c in cohorts):
        raise ValueError("empty/reversed cohort")
    for r in rows:
        if r["scope"] == "in_scope":
            owners = [c for c in cohorts if addr(c["start"]) <= addr(r["address"]) < addr(c["end"])]
            if len(owners) != 1 or addr(r["address"]) + r["size"] > addr(owners[0]["end"]):
                raise ValueError("in-scope function missing, repeated or cut by cohort boundary")
    barriers = [addr(x) for x in research["barriers"]]
    if barriers != sorted(set(barriers)) or not set(barriers).issubset(
            {addr(c["start"]) for c in cohorts} | {addr(cohorts[-1]["end"])}):
        raise ValueError("semantic barriers must be sorted unique cohort boundaries")
    for a, annotation in research["function_annotations"].items():
        if set(annotation) - ANNOTATION_FIELDS:
            raise ValueError("annotation cannot override structural fields or introduce unknown fields")
        if a not in {r["address"] for r in rows if r["scope"] == "in_scope"}:
            raise ValueError("function annotation is outside in-scope population")
        if annotation.get("evidence_level", "HYPOTHESIS") not in LEVELS:
            raise ValueError("unknown evidence level")
        if annotation.get("status", "open") not in STATES:
            raise ValueError("unknown status")
        if annotation.get("status") == "verified_body":
            if (annotation.get("evidence_level") != "VERIFIED-BODY"
                    or str(annotation.get("diff_words")) != "0"
                    or not all(annotation.get(k) for k in ["flags", "source_path", "pr", "note"])):
                raise ValueError("verified_body requires explicit zero-score/source/flags/PR provenance")


def symbol_names(text):
    out = {}
    for name, value in re.findall(r"^([^\s=]+)\s*=\s*(0x[0-9a-fA-F]+)", text, re.M):
        out.setdefault(int(value, 16), []).append(name)
    return {k: sorted(v) for k, v in out.items()}


def graph_stats(rows, lo, hi):
    members = rows_in(rows, lo, hi, "in_scope")
    internal_ids = {r["address"] for r in members}
    scoped = {r["address"] for r in rows if r["scope"] == "in_scope"}
    edges = [(r["address"], t) for r in members for t in r["callees_tail"]]
    inside = sum(t in internal_ids for _, t in edges)
    cross = sum(t in scoped and t not in internal_ids for _, t in edges)
    excluded = sum(t not in scoped for _, t in edges)
    static = sum(len(r["callees_static"]) for r in members)
    return {"internal_tail_edges": inside, "cross_cluster_tail_edges": cross,
            "excluded_tail_edges": excluded, "total_tail_edges": len(edges),
            "static_edges": static,
            "internal_fraction": round(inside / len(edges), 6) if edges else None,
            "game_called_functions": sum(r["called_from_game"] for r in members)}


def merge_cohorts(rows, research):
    """Greedy, stable, address-ordered; barriers preserve family evidence."""
    groups = [[c] for c in research["cohorts"]]
    barriers = set(research["barriers"])
    trace = []
    while True:
        merged = False
        for i, group in enumerate(groups):
            lo, hi = addr(group[0]["start"]), addr(group[-1]["end"])
            current = graph_stats(rows, lo, hi)
            if not current["total_tail_edges"] or current["internal_tail_edges"] * 2 >= current["total_tail_edges"]:
                continue
            options = []
            for j in [i - 1, i + 1]:
                if j < 0 or j >= len(groups):
                    continue
                left, right = min(i, j), max(i, j)
                if groups[right][0]["start"] in barriers:
                    continue
                combined = groups[left] + groups[right]
                start, end = addr(combined[0]["start"]), addr(combined[-1]["end"])
                stats = graph_stats(rows, start, end)
                size = sum(r["size"] for r in rows_in(rows, start, end, "in_scope"))
                # Max cohesion, then fewer bytes, then lower-address neighbor.
                options.append((stats["internal_fraction"] or 0, -size, -j, j, combined))
            if not options:
                continue
            _, _, _, j, combined = max(options)
            left, right = min(i, j), max(i, j)
            trace.append({"left": [c["id"] for c in groups[left]],
                          "right": [c["id"] for c in groups[right]],
                          "reason": "leftmost mergeable cohort below 50% internal tail edges"})
            groups[left:right + 1] = [combined]
            merged = True
            break
        if not merged:
            return groups, trace


def describe(rows, cohorts, cluster_id, names, annotations):
    lo, hi = addr(cohorts[0]["start"]), addr(cohorts[-1]["end"])
    members = rows_in(rows, lo, hi, "in_scope")
    refs = Counter(ref for r in members for ref in r["hilo_refs"])
    shared = [{"address": ref, "functions": n} for ref, n in sorted(refs.items(), key=lambda p: (-p[1], p[0])) if n >= 2]
    static = Counter(t for r in members for t in r["callees_static"])
    candidates = sorted((r for r in members if r["size"] < 256
                         and annotations.get(r["address"], {}).get("status", r["status"]) == "open"
                         and not annotations.get(r["address"], {}).get("matching_blocker")),
                        key=lambda r: (r["size"] >= 64, bool(r["callees_tail"] or r["callees_static"]),
                                       len(r["callers_tail"]), len(r["callees_tail"]) + len(r["callees_static"]),
                                       r["size"], r["address"]))
    result = {"id": cluster_id, "interval": {"start": cohorts[0]["start"], "end_exclusive": cohorts[-1]["end"]},
              "role_hypothesis": "; ".join(c["role"] for c in cohorts), "evidence_level": "HYPOTHESIS",
              "boundary_confidence": "provisional dependency partition; original translation units unproved",
              "cohort_ids": [c["id"] for c in cohorts], "functions": len(members),
              "bytes": sum(r["size"] for r in members), "members": [r["address"] for r in members],
              "excluded_interleaved": [{"address": r["address"], "scope": r["scope"], "size": r["size"]}
                                       for r in rows_in(rows, lo, hi) if r["scope"] != "in_scope"],
              "graph": graph_stats(rows, lo, hi), "shared_reference_leads": shared,
              "hardware_0xa4_reference_leads": sorted(r for r in refs if addr(r) >> 24 == 0xA4),
              "static_call_leads": [{"address": t, "historical_names": names.get(addr(t), []), "caller_functions": n}
                                     for t, n in sorted(static.items())],
              "small_first_candidates": [r["address"] for r in candidates[:10]],
              "reference_ids": sorted({x for c in cohorts for x in c["reference_ids"]})}
    result["priority_key"] = [candidates[0]["size"] >= 64, bool(candidates[0]["callees_tail"] or candidates[0]["callees_static"]),
                              len(candidates[0]["callers_tail"]), candidates[0]["size"], lo] if candidates else [True, True, 999, 99999, lo]
    return result


def build(rows, research, names, provenance):
    validate(rows, research)
    groups, trace = merge_cohorts(rows, research)
    clusters = [describe(rows, g, "BT%02d" % (i + 1), names, research["function_annotations"]) for i, g in enumerate(groups)]
    ordered = sorted(clusters, key=lambda c: c["priority_key"])
    for rank, c in enumerate(ordered, 1):
        c["matching_order"] = rank
        del c["priority_key"]
    mapping = {a: c["id"] for c in clusters for a in c["members"]}
    cohorts = [dict(c, graph=graph_stats(rows, addr(c["start"]), addr(c["end"])),
                    functions=len(rows_in(rows, addr(c["start"]), addr(c["end"]), "in_scope")),
                    bytes=sum(r["size"] for r in rows_in(rows, addr(c["start"]), addr(c["end"]), "in_scope")))
               for c in research["cohorts"]]
    ledger = []
    for r in rows:
        if r["scope"] != "in_scope":
            continue
        cohort = next(c for c in cohorts if addr(c["start"]) <= addr(r["address"]) < addr(c["end"]))
        record = dict.fromkeys(FIELDS, "")
        record.update(address=r["address"], name=r["name"], size=r["size"], cluster=mapping[r["address"]],
                      name_hypothesis=cohort["name_prefix"] + "_" + r["address"][2:],
                      evidence_level="HYPOTHESIS", status="open",
                      note="%s; role/ABI unproved; metadata scan leads only; unclaimed matching target." % cohort["id"])
        annotation = research["function_annotations"].get(r["address"], {})
        record.update({k: v for k, v in annotation.items() if k in FIELDS})
        if annotation.get("matching_blocker"):
            record["note"] += " Blocked-for-scoring: " + annotation["matching_blocker"]
        ledger.append(record)
    buf = io.StringIO(newline="")
    writer = csv.DictWriter(buf, fieldnames=FIELDS, lineterminator="\n")
    writer.writeheader()
    writer.writerows(ledger)
    summary = {"inventory_functions": len(rows), "inventory_function_bytes": sum(r["size"] for r in rows),
               "in_scope_functions": len(ledger), "in_scope_bytes": sum(r["size"] for r in ledger),
               "preexisting_verified_functions": sum(r["status"] == "verified_body" and r["address"] == "0x80010A00" for r in ledger),
               "preexisting_verified_bytes": sum(r["size"] for r in ledger if r["status"] == "verified_body" and r["address"] == "0x80010A00"),
               "new_verified_functions": sum(r["status"] == "verified_body" and r["address"] != "0x80010A00" for r in ledger),
               "new_verified_bytes": sum(r["size"] for r in ledger if r["status"] == "verified_body" and r["address"] != "0x80010A00"),
               "open_functions": sum(r["status"] == "open" for r in ledger),
               "open_bytes": sum(r["size"] for r in ledger if r["status"] == "open"),
               "non_c_functions": sum(r["status"] == "non_c" for r in ledger),
               "ultralib_excluded_functions": 21, "ultralib_excluded_bytes": 4048,
               "identify_only_stubs": 6, "identify_only_stub_bytes": 60,
               "game_called_all_inventory": sum(r["called_from_game"] for r in rows),
               "game_called_in_scope": sum(r["called_from_game"] for r in rows if r["scope"] == "in_scope"),
               "all_tail_distinct_directed_edges": sum(len(r["callees_tail"]) for r in rows),
               "cluster_count": len(clusters), "cohort_count": len(cohorts)}
    payload = {"schema_version": 1, "provenance": provenance, "summary": summary,
               "method": {"intervals": "half-open; bytes sum function extents, never interval spans",
                          "input": "inventory jal/hi-lo scan, historical static symbol labels, reviewed cohort rationale",
                          "merging": "leftmost under-50%-internal run, choose adjacent permitted merge maximizing rounded internal fraction, then fewer in-scope bytes, then lower-address neighbor; repeat",
                          "denominator": "all outgoing distinct tail edges including excluded/stub destinations; static edges reported separately",
                          "barriers": research["barriers"], "merge_trace": trace,
                          "limitation": "metadata-only dependency partitions; no source/TU/ABI/native-match proof"},
               "clusters": clusters, "cohorts": cohorts,
               "matching_blockers": {a: note["matching_blocker"] for a, note in research["function_annotations"].items() if note.get("matching_blocker")},
               "identify_only_stubs": [r for r in rows if r["scope"] == "stub_unclassified"],
               "excluded_ultralib": [{"address": r["address"], "size": r["size"]} for r in rows if r["scope"] == "ultralib_identified"]}
    return buf.getvalue(), payload


def readme(payload):
    summary = payload["summary"]
    lines = ["| Order | Cluster | Half-open interval | Functions / bytes | Internal / all tail edges | Candidate roles |",
             "|---:|---|---|---:|---:|---|"]
    for c in sorted(payload["clusters"], key=lambda c: c["matching_order"]):
        g = c["graph"]
        lines.append("| %d | %s | `%s–%s` | %d / %s | %d / %d | %s (%s) |" % (
            c["matching_order"], c["id"], c["interval"]["start"], c["interval"]["end_exclusive"],
            c["functions"], format(c["bytes"], ","), g["internal_tail_edges"], g["total_tail_edges"],
            ", ".join(c["cohort_ids"]), c["evidence_level"]))
    cohorts = []
    for c in payload["cohorts"]:
        cohorts.extend(["### %s: %s" % (c["id"], c["role"]), "", "`[%s, %s)`; %d functions / %s B. HYPOTHESIS." %
                        (c["start"], c["end"], c["functions"], format(c["bytes"], ",")), "", c["rationale"], ""])
    text = (WORK / "README.template.md").read_text()
    for key, value in summary.items():
        if isinstance(value, int):
            text = text.replace("@@" + key.upper() + "@@", format(value, ","))
    return text.replace("@@CLUSTERS@@", "\n".join(lines)).replace("@@COHORTS@@", "\n".join(cohorts)).replace(
        "@@SUMMARY@@", json.dumps(summary, sort_keys=True, indent=2)).rstrip() + "\n"


def generated():
    rows = json.loads(INVENTORY.read_text())["functions"]
    research_path = WORK / "research.json"
    research = json.loads(research_path.read_text())
    provenance = {"base_commit": research["base_commit"], "inventory_spec_commit": research["inventory_spec_commit"],
                  "inputs_sha256": {str(p.relative_to(ROOT)): sha256(p) for p in [INVENTORY, SYMBOLS, research_path]},
                  "compiler_used": False, "rom_used": False, "scoring_used": False}
    status, clusters = build(rows, research, symbol_names(SYMBOLS.read_text()), provenance)
    return {"STATUS.csv": status, "clusters.json": json.dumps(clusters, indent=2, sort_keys=True) + "\n",
            "README.md": readme(clusters)}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="fail on generated-output drift; write nothing")
    args = parser.parse_args()
    drift = []
    for name, text in generated().items():
        path = WORK / name
        if args.check:
            if not path.exists() or path.read_bytes() != text.encode("utf-8"):
                drift.append(name)
        else:
            path.write_bytes(text.encode("utf-8"))
    if drift:
        print("generated-output drift: " + ", ".join(drift), file=sys.stderr)
        return 1
    print("boot-tail metadata: " + ("reproducible" if args.check else "generated") + "; 412 functions / 95012 B")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
