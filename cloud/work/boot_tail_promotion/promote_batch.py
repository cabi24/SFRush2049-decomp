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
        [--only fn,fn,...] [--prefix-dedup rom/lib_X,...]

`--only` (wave 3) restricts the plan to the named slots; every other slot in
the segments is skipped without a refusal row. All gates are unchanged.

Compiler (w17k): a segment pinned to IDO 7.1 in rom_compilers.json builds its
TU with 7.1 (opt_overrides.mk `CC :=`). A candidate is refused unless its lock
was proven with the segment's compiler (`lock add --toolkit <7.1 toolkit>`
records `compiler`) and its context row was computed with it
(`context_check.py --compiler ido7.1`). Migrated locks keep the compiler.

Owned data (w19c): a body whose object carries data is promotable only into
an owned slot (rom_owned_data.json) that it owns or co-owns, with a context row
listing it under "owned" (context_check.py). All members of the slot (owner +
co_owners) must be candidates of the same batch; the splice removes the slot's
passthrough companion, and the members are gated, accepted or refused as one
unit. The registry row and companion must already be committed (`owned_data
prepare`, its own ROM gate). A context row carrying "flagset" (context_check.py
--flags) must equal the segment's.

A function may belong to several slots of its TU (w22i: func_80023E9C owns
lib_22300's .bss block and co-owns its .rodata); the slots' members then form
one unit. A slot's members are the ones still passthrough in the TU.
`--replace-slot ROW.json` (w22i) grows a fully promoted owner's slot into the
co-owned slot of the batch's new members (`owned_data.replace_slot`: there is
no hash-neutral passthrough state for it). The registry row, companion file
and linker script are rewritten for the plan and the gates, committed with the
promotion only if every new member passes, and restored otherwise (always on
--dry-run).

`--dry-run` plans only: it prints every candidate and refusal (refusals are not
appended to refusals.jsonl) and touches nothing. `--splice-out DIR` (implies
--dry-run) also writes the spliced TUs to DIR/<tu>.c instead of src/rom, for a
scratch build. `--lockfile PATH` plans against another lockfile (dry runs only).
"""
import hashlib
import json
import os
import re
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor import compilers as compilersmod  # noqa: E402
from tools.conveyor.client import DEFAULT_DATA  # noqa: E402
from tools.conveyor.coordinator import db as dbmod  # noqa: E402
from tools.conveyor.pipeline import layout as layoutmod  # noqa: E402
from tools.conveyor.pipeline import lock as lockmod  # noqa: E402
from tools.conveyor.pipeline import owned_data  # noqa: E402
from tools.conveyor.pipeline import promote as promotemod  # noqa: E402
from tools.conveyor.seeds.extract_candidates import REPO, extract_named_function  # noqa: E402

HERE = Path("cloud/work/boot_tail_promotion")
REFUSALS = HERE / "refusals.jsonl"
BUILDER, BUILDER_REPO = promotemod.BUILDER, promotemod.BUILDER_REPO
# Parallel jobs of the gate build (w23i: a lane's scratch repo builds with -j2).
BUILDER_JOBS = int(os.environ.get("CONVEYOR_BUILDER_JOBS", "16"))
RSYNC_EXCLUDES = ["venv/", "build/", "reference/repos/", "tools/ido-static-recomp/",
                  "__pycache__/", "*.pyc", ".pytest_cache/", "backup/"]


def run(cmd, timeout=3600):
    return subprocess.run(cmd, cwd=REPO, capture_output=True, text=True, timeout=timeout)


def refuse(fn, reason):
    if DRY_RUN:
        print(f"  REFUSED {fn}: {reason}")
        return
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


ONLY = None
PREFIX_DEDUP = set()
CONTEXT_PATH = HERE / "context.jsonl"
DRY_RUN = False
LOCKFILE = None  # None = matched.lock.json
REPLACED = None  # (row, {path: original}) from --replace-slot


def plan(segments, context):
    mapping = layoutmod.derive()
    entries = lockmod.load_lock(LOCKFILE) if LOCKFILE else lockmod.load_lock()
    out = []
    for name in segments:
        seg = layoutmod._segment_by_name(mapping, name)
        if seg is None or not seg["converted"]:
            sys.exit(f"segment {name} is missing or not converted")
        for f in seg["functions"]:
            fn = f["name"]
            if ONLY is not None and fn not in ONLY:
                continue
            src = source_for(fn, entries)
            if src is None:
                continue  # no verified body for this slot
            if f["state"] != "passthrough":
                continue
            e = entries[src + ":" + fn]
            if e.get("verified") != "score0":
                refuse(fn, f"lock evidence is {e.get('verified')}, not score0")
                continue
            if not seg["flagset"] or layoutmod.normalise_flagset(seg["flagset"]) != layoutmod.normalise_flagset(e["flagset"]):
                refuse(fn, f"segment flagset {seg['flagset']!r} != lock {e['flagset']!r}")
                continue
            why = layoutmod.compiler_refusal(seg, e)
            if why:
                refuse(fn, why)
                continue
            row = context.get(fn)
            if row is None or row["status"] != "ok":
                refuse(fn, "context: " + (row["reason"] if row else "no context_check row"))
                continue
            row_compiler = row.get("compiler") or compilersmod.DEFAULT
            if row_compiler != seg["compiler"]:
                refuse(fn, f"context row computed with {row_compiler}, segment builds with "
                           f"{seg['compiler']} (context_check.py --compiler {seg['compiler']})")
                continue
            if row.get("flagset") and layoutmod.normalise_flagset(row["flagset"]) != layoutmod.normalise_flagset(seg["flagset"]):
                refuse(fn, f"context row computed at {row['flagset']!r}, segment builds at "
                           f"{seg['flagset']!r} (context_check.py --flags)")
                continue
            why, slot = owned_slot(fn, seg, row)
            if why:
                refuse(fn, why)
                continue
            out.append(dict(fn=fn, seg=seg, src=src, row=row, slot=slot))
    return slot_closure(out)


def slot_closure(cands):
    """An owned slot changes hands only with all of its members: drop (and
    refuse) every member of a slot whose other members are not candidates."""
    while True:
        names = {c["fn"] for c in cands}
        kept = []
        for c in cands:
            missing = [m for m in (c["slot"]["members"] if c.get("slot") else []) if m not in names]
            if missing:
                refuse(c["fn"], f"owned slot of {c['slot']['owner']} needs {', '.join(missing)} "
                                "promoted in the same batch")
            else:
                kept.append(c)
        if len(kept) == len(cands):
            return kept
        cands = kept


def owned_slot(fn, seg, row):
    """(refusal, slot) for a candidate: the passthrough-companion slots it is a
    member of in its TU (None if none), checked against its context row. A
    slot counts when its companion block is in the TU, or it is this run's
    --replace-slot row; its members are those still passthrough. Several
    slots merge into one record (their members form one unit)."""
    tu = "src/" + seg["rom_tu"] + ".c"
    text = (REPO / tu).read_text()
    replaced = REPLACED[0]["owner"] if REPLACED else None
    slots = [r for r in owned_data.slots_for_function(REPO, fn, tu)
             if owned_data.companion_block(r) in text or r["owner"] == replaced]
    owned = row.get("owned") or []
    if not slots:
        if owned:
            return "context row has owned data but the TU holds no companion for it", None
        return None, None
    if len({r["owner_section"] for r in slots}) != len(slots):
        return f"member of {len(slots)} owned slots of one section", None
    by_section = {r["owner_section"]: r for r in slots}
    for o in owned:
        r = by_section.get(o.get("section"))
        if r is None or o.get("slot") != r["owner"] or o.get("tu") != tu:
            return f"context row's owned data {o} is not a slot of {fn}", None
    parts = [{"owner": r["owner"], "members": owned_data.pending_members(r, text), "section": r["owner_section"]}
             for r in slots]
    members = []
    for part in parts:
        members += [m for m in part["members"] if m not in members]
    return None, {"owner": ",".join(p["owner"] for p in parts), "members": members, "tu": tu,
                  "section": ",".join(p["section"] for p in parts), "slots": parts}


def splice(cands, out_dir=None):
    texts = {}
    for c in cands:
        tu = REPO / "src" / (c["seg"]["rom_tu"] + ".c")
        text = texts.get(tu) or tu.read_text()
        pragma = f'#pragma GLOBAL_ASM("asm/us/nonmatchings/{c["seg"]["rom_tu"]}/{c["fn"]}.s")'
        assert text.count(pragma) == 1, c["fn"]
        # Declarations already present are skipped. In a --prefix-dedup TU only
        # text BEFORE the slot counts (a later declaration is not yet visible
        # at the insertion point); this mirrors the audio_record_contracts
        # actual-TU proof. Other TUs keep the whole-file rule their packets proved.
        if c["seg"]["rom_tu"] in PREFIX_DEDUP:
            have = tu_statements(text[:text.index(pragma)])
        else:
            have = tu_statements(text)
        # Preprocessor lines (#pragma pack(1) ... #pragma pack()) are positional
        # and always kept; only declaration statements are deduplicated.
        # An #include already made before the slot is a guarded no-op; skip it.
        before = tu_statements(text[:text.index(pragma)])
        decls = [s for s in c["row"]["preamble"]
                 if (s.startswith("#") and not (s.startswith("#include") and norm(s) in before))
                 or (not s.startswith("#") and norm(s) not in have)]
        body = extract_named_function(REPO / c["src"], c["fn"])
        header = promotemod._provenance_header(
            c["fn"], f"{c['src']} (in-repo, locked)", c["seg"]["flagset"],
            f"lock:{c['src']}:{c['fn']} (score0); rom_tu.h context: "
            f"{CONTEXT_PATH}")
        block = header + "\n" + "".join(d + "\n" for d in decls) + body.rstrip() + "\n"
        texts[tu] = text.replace(pragma, block, 1)
    # Remove each fully promoted owned slot's passthrough companion (a
    # --replace-slot row has none), once, at its first pending member.
    for c in cands:
        slot = c.get("slot")
        for part in (slot or {}).get("slots", []):
            if c["fn"] != part["members"][0]:
                continue
            tu = REPO / slot["tu"]
            row = next(r for r in owned_data.rom_slots(REPO, check_source=False) if r["owner"] == part["owner"])
            if owned_data.companion_block(row) in texts[tu]:
                texts[tu] = owned_data.strip_companion(texts[tu], part["owner"], REPO, slot["tu"])
    for tu, text in texts.items():
        (Path(out_dir) / tu.name if out_dir else tu).write_text(text)
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
                 f"make COMPILER=ido -j{BUILDER_JOBS} && make test && {newer}"])
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
    print(f"  gate failed for all {len(cands)}; accepting one unit at a time")
    accepted = []
    for unit in units(cands):
        tus = splice(accepted + unit)
        ok, detail = gate(tus)
        restore(tus)
        if ok:
            accepted += unit
        else:
            for c in unit:
                refuse(c["fn"], "full-ROM SHA-1 gate failed in TU context: " + detail[-200:])
    return accepted


def units(cands):
    """Candidates in order; the members of one owned slot (transitively, of
    slots sharing a member) form one unit, placed at its first member's
    position."""
    parent = {}

    def find(x):
        while parent.setdefault(x, x) != x:
            x = parent[x]
        return x
    for c in cands:
        for m in (c["slot"]["members"] if c.get("slot") else []):
            parent[find(m)] = find(c["fn"])
    out, by_slot = [], {}
    for c in cands:
        key = find(c["fn"]) if c.get("slot") else None
        if key is None:
            out.append([c])
        elif key in by_slot:
            by_slot[key].append(c)
        else:
            by_slot[key] = [c]
            out.append(by_slot[key])
    return out


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
        if c["seg"]["compiler"] != compilersmod.DEFAULT:
            entries[f"{rel}:{c['fn']}"]["compiler"] = c["seg"]["compiler"]
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
                 json.dumps(dict({"evidence": f"lock:{c['src']}:{c['fn']} (score0)", "batch": label},
                                 **({"owned_slot": c["slot"]["owner"]} if c.get("slot") else {}))),
                 c["seg"]["rom_tu"]))
    lockmod.save_lock(entries)
    tus = sorted({"src/" + c["seg"]["rom_tu"] + ".c" for c in passed})
    nbytes = sum(next(f["size"] for f in c["seg"]["functions"] if f["name"] == c["fn"]) for c in passed)
    msg = (f"Promote {len(passed)} boot-tail functions ({label}, ROM SHA-1 exact)\n\n"
           f"Segments: {', '.join(sorted({c['seg']['yaml_name'] for c in passed}))}; "
           f"{nbytes} slot bytes.\nEach body: score-0 lock against the splat-derived reloc-aware "
           f"target, rom_tu.h context check\n({CONTEXT_PATH}), then one "
           "full matching ROM build + make test\non watchman2 for the batch (greedy per-function acceptance on failure).\n\n"
           "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
    replaced = [str(Path(p).relative_to(REPO)) for p in REPLACED[1]] if REPLACED else []
    if replaced:
        msg = msg.replace("\n\nCo-Authored-By", f"\nOwned slot of {REPLACED[0]['owner']} replaced "
                          f"(--replace-slot): {', '.join(replaced)}.\n\nCo-Authored-By")
    c = run(["git", "commit", "-q", "-m", msg, "--"] + tus + ["matched.lock.json"] + replaced)
    if c.returncode:
        sys.exit("commit failed after a PASSED gate:\n" + c.stderr[-500:])
    print(f"PROMOTED {len(passed)} ({nbytes} B) @ {run(['git','rev-parse','--short','HEAD']).stdout.strip()}")


def main():
    global ONLY, CONTEXT_PATH, PREFIX_DEDUP, DRY_RUN, LOCKFILE, REPLACED
    argv = sys.argv[1:]
    splice_out = None
    if "--dry-run" in argv:
        DRY_RUN = True
        argv.remove("--dry-run")
    if "--splice-out" in argv:
        i = argv.index("--splice-out")
        splice_out = Path(argv[i + 1])
        DRY_RUN = True
        del argv[i:i + 2]
    if "--lockfile" in argv:
        i = argv.index("--lockfile")
        LOCKFILE = Path(argv[i + 1])
        del argv[i:i + 2]
        if not DRY_RUN:
            sys.exit("--lockfile is for dry runs only (the real batch migrates matched.lock.json)")
    if "--prefix-dedup" in argv:
        i = argv.index("--prefix-dedup")
        PREFIX_DEDUP = set(argv[i + 1].split(","))
        del argv[i:i + 2]
    if "--only" in argv:
        i = argv.index("--only")
        ONLY = set(argv[i + 1].split(","))
        del argv[i:i + 2]
    replace_row = None
    if "--replace-slot" in argv:
        i = argv.index("--replace-slot")
        replace_row = json.loads(Path(argv[i + 1]).read_text())
        del argv[i:i + 2]
    CONTEXT_PATH = Path(argv[0])
    context = {json.loads(l)["function"]: json.loads(l) for l in CONTEXT_PATH.read_text().splitlines()}
    segments = argv[1:]
    guarded = ["src/rom", "matched.lock.json"]
    if replace_row is not None:
        guarded += [owned_data.REGISTRY, "rush2049.us.ld", replace_row.get("passthrough_asm") or owned_data.REGISTRY]
    dirty = run(["git", "status", "--porcelain", "--"] + guarded).stdout.strip()
    if dirty and not DRY_RUN:
        sys.exit("refusing: dirty src/rom, lockfile or replaced-slot paths\n" + dirty)
    if replace_row is None:
        return run_batch(segments, context, splice_out)
    REPLACED = owned_data.replace_slot(REPO, replace_row)
    print(f"replace-slot {REPLACED[0]['owner']}: {REPLACED[0]['owner_section']} "
          f"{REPLACED[0]['container_vram'] + REPLACED[0]['offset']:#x} size {REPLACED[0]['size']:#x}, "
          f"members {', '.join(owned_data.slot_members(REPLACED[0]))}")
    published = False
    try:
        published = run_batch(segments, context, splice_out)
    finally:
        if not published and REPLACED:
            owned_data.restore(REPLACED[1])
            print(f"replace-slot {REPLACED[0]['owner']}: restored")


def run_batch(segments, context, splice_out):
    """Plan, gate and publish; True iff a --replace-slot row was committed."""
    global REPLACED
    cands = plan(segments, context)
    print(f"batch {segments[0]}..{segments[-1]}: {len(cands)} candidates")
    if DRY_RUN:
        for c in cands:
            size = next(f["size"] for f in c["seg"]["functions"] if f["name"] == c["fn"])
            owned = f", owned {c['slot']['section']} slot of {c['slot']['owner']}" if c.get("slot") else ""
            print(f"  PLAN {c['fn']} -> {c['seg']['rom_tu']} ({size} B, {c['seg']['compiler']}, "
                  f"{c['seg']['flagset']}{owned})")
        if splice_out:
            splice_out.mkdir(parents=True, exist_ok=True)
            for tu in splice(cands, splice_out):
                print(f"  spliced {splice_out / tu.name}")
        return False
    passed = attempt(cands)
    if REPLACED:
        tu_text = (REPO / REPLACED[0]["tu"]).read_text()
        new = owned_data.pending_members(REPLACED[0], tu_text)
        if not set(new) <= {c["fn"] for c in passed}:
            # The grown slot only holds once every new member is C: drop the
            # replacement (and its members) before the final gate.
            for c in passed:
                if c["fn"] in new:
                    refuse(c["fn"], f"--replace-slot {REPLACED[0]['owner']} needs {', '.join(new)} accepted together")
            passed = [c for c in passed if c["fn"] not in new]
            owned_data.restore(REPLACED[1])
            REPLACED = None
    if not passed:
        return False
    # Gate the final passed set exactly as it will be committed.
    tus = splice(passed)
    ok, detail = gate(tus)
    if not ok:
        restore(tus)
        sys.exit("combined passed set failed the gate: " + detail)
    publish(passed, f"{segments[0]}..{segments[-1]}")
    return REPLACED is not None


if __name__ == "__main__":
    main()
