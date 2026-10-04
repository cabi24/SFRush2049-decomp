"""Batch static promotion of verified boot-tail bodies (Phase 2 driver).

`pipeline.promote run` is the reference transaction: evidence (a score-0 lock
against the splat-derived reloc-aware target), a passthrough slot in a
converted segment with a pinned flagset, a clean tree, splice, full matching
ROM build + SHA-1, lock migration, promotion record, commit. It builds the
whole ROM once per function. This driver runs the SAME preconditions and the
SAME gate for many functions at once:

  * every candidate must hold a `verified: score0` lock for its source, sit in
    a passthrough slot of a converted segment with a pinned flagset, and have
    a context_check.py `ok` row (rom_tu.h context fits without changing the
    function's disassembly+relocations, and the object carries no data);
  * the splice is promote.py's: provenance header (PROMOTED ... — fn) +
    body, with the body's own file-scope declarations placed immediately
    before it (skipping any statement already present in the TU);
  * the gate is the full matching ROM build + `make test` on the builder after
    a full repo sync with `rsync -a --delete asm/`, with every touched TU
    touched and its object required to be newer than the source;
  * after every gate the TUs are restored byte-for-byte; when the whole
    batch fails, candidates are accepted one at a time on top of those
    already accepted, and a candidate whose addition fails is refused;
  * on a passed gate locks migrate to the ROM TU (`verified: rom-sha1`),
    promotion_record rows are written, and the batch is committed.

Usage (repo root, Pi):
    python3 cloud/work/boot_tail_promotion/promote_batch.py CONTEXT.jsonl SEG [SEG...]
"""
import hashlib
import json
import re
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.client import DEFAULT_DATA  # noqa: E402
from tools.conveyor.coordinator import db as dbmod  # noqa: E402
from tools.conveyor.pipeline import layout as layoutmod  # noqa: E402
from tools.conveyor.pipeline import lock as lockmod  # noqa: E402
from tools.conveyor.pipeline import promote as promotemod  # noqa: E402
from tools.conveyor.seeds.extract_candidates import REPO, extract_named_function  # noqa: E402

HERE = Path("cloud/work/boot_tail_promotion")
REFUSALS = HERE / "refusals.jsonl"
BUILDER, BUILDER_REPO = promotemod.BUILDER, promotemod.BUILDER_REPO
RSYNC_EXCLUDES = ["venv/", "build/", "reference/repos/", "tools/ido-static-recomp/",
                  "__pycache__/", "*.pyc", ".pytest_cache/", "backup/"]


def run(cmd, timeout=3600):
    return subprocess.run(cmd, cwd=REPO, capture_output=True, text=True, timeout=timeout)


def refuse(fn, reason):
    with REFUSALS.open("a") as f:
        f.write(json.dumps({"function": fn, "reason": reason,
                            "at": time.strftime("%Y-%m-%dT%H:%M:%S")}) + "\n")
    print(f"  REFUSED {fn}: {reason}")


def norm(stmt):
    return " ".join(stmt.split())


def tu_statements(text):
    """Collapsed lines already present in a TU. Spliced declarations are
    single-line statements (context_check collapses them), so a line match
    is an exact statement match."""
    return {norm(line) for line in text.splitlines() if line.strip()}


def source_for(fn, entries):
    specs = [s for s in entries if s.endswith(":" + fn) and not s.startswith("src/rom/")]
    return specs[0].rpartition(":")[0] if len(specs) == 1 else None


def plan(segments, context):
    mapping = layoutmod.derive()
    entries = lockmod.load_lock()
    out = []
    for name in segments:
        seg = layoutmod._segment_by_name(mapping, name)
        if seg is None or not seg["converted"]:
            sys.exit(f"segment {name} is missing or not converted")
        for f in seg["functions"]:
            fn = f["name"]
            src = source_for(fn, entries)
            if src is None:
                continue  # no verified body for this slot
            if f["state"] != "passthrough":
                continue
            e = entries[src + ":" + fn]
            if e.get("verified") != "score0":
                refuse(fn, f"lock evidence is {e.get('verified')}, not score0")
                continue
            if not seg["flagset"] or seg["flagset"] != e["flagset"]:
                refuse(fn, f"segment flagset {seg['flagset']!r} != lock {e['flagset']!r}")
                continue
            row = context.get(fn)
            if row is None or row["status"] != "ok":
                refuse(fn, "context: " + (row["reason"] if row else "no context_check row"))
                continue
            out.append(dict(fn=fn, seg=seg, src=src, row=row))
    return out


def splice(cands):
    texts = {}
    for c in cands:
        tu = REPO / "src" / (c["seg"]["rom_tu"] + ".c")
        text = texts.get(tu) or tu.read_text()
        pragma = f'#pragma GLOBAL_ASM("asm/us/nonmatchings/{c["seg"]["rom_tu"]}/{c["fn"]}.s")'
        assert text.count(pragma) == 1, c["fn"]
        have = tu_statements(text)
        # Preprocessor lines (#pragma pack(1) ... #pragma pack()) are positional
        # and always kept; only declaration statements are deduplicated.
        decls = [s for s in c["row"]["preamble"] if s.startswith("#") or norm(s) not in have]
        body = extract_named_function(REPO / c["src"], c["fn"])
        header = promotemod._provenance_header(
            c["fn"], f"{c['src']} (in-repo, locked)", c["seg"]["flagset"],
            f"lock:{c['src']}:{c['fn']} (score0); rom_tu.h context: "
            f"{HERE}/context.jsonl")
        block = header + "\n" + "".join(d + "\n" for d in decls) + body.rstrip() + "\n"
        texts[tu] = text.replace(pragma, block, 1)
    for tu, text in texts.items():
        tu.write_text(text)
    return sorted(texts)


def gate(tus):
    sync = run(["rsync", "-a"] + [f"--exclude={x}" for x in RSYNC_EXCLUDES]
               + [str(REPO) + "/", f"{BUILDER}:{BUILDER_REPO}/"])
    if sync.returncode:
        return False, "rsync failed: " + sync.stderr[-300:]
    sync = run(["rsync", "-a", "--delete", str(REPO / "asm") + "/", f"{BUILDER}:{BUILDER_REPO}/asm/"])
    if sync.returncode:
        return False, "asm rsync failed: " + sync.stderr[-300:]
    rels = [str(t.relative_to(REPO)) for t in tus]
    newer = " && ".join(f"[ build/us/{r[:-2]}.o -nt {r} ]" for r in rels) or "true"
    build = run(["ssh", BUILDER, f"cd {BUILDER_REPO} && touch {' '.join(rels)} && "
                 f"make COMPILER=ido -j16 && make test && {newer}"])
    ok = build.returncode == 0 and "ROM matches!" in build.stdout
    return ok, " / ".join((build.stdout + build.stderr).strip().splitlines()[-3:])


def restore(tus):
    run(["git", "checkout", "--"] + [str(t.relative_to(REPO)) for t in tus])


def attempt(cands):
    """Greedy, order-preserving acceptance: each candidate is spliced on top of
    everything already accepted and the full gate is run; a candidate that
    fails (alone or through a declaration conflict with an accepted body) is
    refused. The tree is restored after every gate. A first gate of the whole
    set short-circuits the common all-pass case."""
    if not cands:
        return []
    tus = splice(cands)
    ok, detail = gate(tus)
    restore(tus)
    if ok:
        return cands
    print(f"  gate failed for all {len(cands)}; accepting one at a time")
    accepted = []
    for c in cands:
        tus = splice(accepted + [c])
        ok, detail = gate(tus)
        restore(tus)
        if ok:
            accepted.append(c)
        else:
            refuse(c["fn"], "full-ROM SHA-1 gate failed in TU context: " + detail[-200:])
    return accepted


def publish(passed, label):
    entries = lockmod.load_lock()
    conn = dbmod.connect(Path(DEFAULT_DATA) / "conveyor.db")
    day = time.strftime("%Y-%m-%d")
    for c in passed:
        tu = REPO / "src" / (c["seg"]["rom_tu"] + ".c")
        rel = str(tu.relative_to(REPO))
        entries[f"{rel}:{c['fn']}"] = {
            "body_sha256": lockmod.body_sha(tu, c["fn"]), "target_id": c["fn"],
            "flagset": c["seg"]["flagset"], "verified": "rom-sha1",
            "toolkit_sha": None, "verified_at": day}
        entries.pop(f"{c['src']}:{c['fn']}", None)
        body = extract_named_function(REPO / c["src"], c["fn"])
        with dbmod.tx(conn):
            conn.execute(
                "INSERT INTO promotion_record (target_id, source_sha, build_ok, sha1_ok, outcome,"
                " created_at, source, flags, evidence, rom_tu) VALUES (?, ?, 1, 1, 'promoted',"
                " strftime('%Y-%m-%dT%H:%M:%fZ','now'), ?, ?, ?, ?)"
                " ON CONFLICT(target_id) WHERE outcome = 'promoted' DO UPDATE SET"
                " source_sha=excluded.source_sha, build_ok=1, sha1_ok=1,"
                " created_at=excluded.created_at, source=excluded.source,"
                " flags=excluded.flags, evidence=excluded.evidence, rom_tu=excluded.rom_tu",
                (c["fn"], hashlib.sha256(body.encode()).hexdigest(), c["src"], c["seg"]["flagset"],
                 json.dumps({"evidence": f"lock:{c['src']}:{c['fn']} (score0)", "batch": label}),
                 c["seg"]["rom_tu"]))
    lockmod.save_lock(entries)
    tus = sorted({"src/" + c["seg"]["rom_tu"] + ".c" for c in passed})
    nbytes = sum(next(f["size"] for f in c["seg"]["functions"] if f["name"] == c["fn"]) for c in passed)
    msg = (f"Promote {len(passed)} boot-tail functions ({label}, ROM SHA-1 exact)\n\n"
           f"Segments: {', '.join(sorted({c['seg']['yaml_name'] for c in passed}))}; "
           f"{nbytes} slot bytes.\nEach body: score-0 lock against the splat-derived reloc-aware "
           "target, rom_tu.h context check\n(cloud/work/boot_tail_promotion/context.jsonl), then one "
           "full matching ROM build + make test\non watchman2 for the batch (greedy per-function acceptance on failure).\n\n"
           "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
    c = run(["git", "commit", "-q", "-m", msg, "--"] + tus + ["matched.lock.json"])
    if c.returncode:
        sys.exit("commit failed after a PASSED gate:\n" + c.stderr[-500:])
    print(f"PROMOTED {len(passed)} ({nbytes} B) @ {run(['git','rev-parse','--short','HEAD']).stdout.strip()}")


def main():
    context = {json.loads(l)["function"]: json.loads(l) for l in Path(sys.argv[1]).read_text().splitlines()}
    segments = sys.argv[2:]
    dirty = run(["git", "status", "--porcelain", "--", "src/rom", "matched.lock.json"]).stdout.strip()
    if dirty:
        sys.exit("refusing: dirty src/rom or lockfile\n" + dirty)
    cands = plan(segments, context)
    print(f"batch {segments[0]}..{segments[-1]}: {len(cands)} candidates")
    passed = attempt(cands)
    if not passed:
        return
    # Gate the final passed set exactly as it will be committed.
    tus = splice(passed)
    ok, detail = gate(tus)
    if not ok:
        restore(tus)
        sys.exit("combined passed set failed the gate: " + detail)
    publish(passed, f"{segments[0]}..{segments[-1]}")


if __name__ == "__main__":
    main()
