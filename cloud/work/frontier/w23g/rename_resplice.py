#!/usr/bin/env python3
"""w23g: migrate blob_matched.lock.json across a pure rename, then re-prove the image.

    python3 cloud/work/frontier/w23g/rename_resplice.py prove   MAP.json [--base REV]
    python3 cloud/work/frontier/w23g/rename_resplice.py apply   MAP.json [--base REV]

MAP.json is a static_rename table (old -> new, "accepted" rows). `prove` finds
every blob lock entry (single or group) whose source files differ from --base
and requires, for each of them:

  1. the files at --base hash to the lock's source_sha256 (the locked text), and
  2. every changed file on disk equals the --base text with the permutation
     applied by static_rename.permute_c (code identifiers only; comments and
     ordinary strings untouched) -- a pure rename -- except that whole
     prototype statements may be missing when each declares a name the file
     still declares elsewhere (the rename turned them into conflicting
     redeclarations; drop_renamed_dups.py). Nothing else.

Anything else refuses (nothing is written). `apply` additionally recompiles
the changed singles (blob_splice.compile_on_builder, per flagset) and groups
(blob_group.compile_group) on the builder in this lane's own scratch, links
EVERY locked body (blob_splice.spliced_bodies) against today's symbol table and
runs the image gate (blob_splice.build_with); only when the gate passes are
the moved source_sha256 pins written. `--resplice NAME` admits a single whose
source is new (not a rename): it is recompiled and gated like the rest and
recorded as kind "resplice". The proof is written to
--out (blob_lock_migration.json) next to this script.
"""
import argparse
import difflib
import hashlib
import json
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))
from tools.conveyor.pipeline import blob_group, blob_splice, static_rename  # noqa: E402
sys.path.insert(0, str(Path(__file__).resolve().parent))
from drop_renamed_dups import spans  # noqa: E402

WORK = "rush2049/scratch/frontier/w23g"          # builder dirs, relative to $HOME
OUT_DIR = Path(__file__).resolve().parent


def renamed_minus_redeclarations(old, new, names):
    """Number of prototype statements dropped, if `new` is `old` renamed with
    only whole redeclaration statements removed (each removed statement
    declares a name that stays declared in `new`); None otherwise."""
    want = static_rename.permute_c(old, names)
    if want == new:
        return 0
    a, b = want.splitlines(keepends=True), new.splitlines(keepends=True)
    offsets, pos = [], 0
    for line in a:
        offsets.append(pos)
        pos += len(line)
    offsets.append(pos)
    declared = {s: (n, e) for n, s, e in spans(want)}
    kept = {n for n, _s, _e in spans(new)}
    dropped = 0
    for tag, i1, i2, _j1, _j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if tag == "equal":
            continue
        if tag != "delete":
            return None
        s, end = offsets[i1], offsets[i2]
        while s < end:                      # the block must be whole declarations
            if s not in declared:
                return None
            name, e = declared[s]
            if name not in kept:
                return None
            dropped += 1
            s = e
        if s != end:
            return None
    return dropped


def git_show(base, rel):
    proc = subprocess.run(["git", "show", f"{base}:{rel}"], cwd=REPO, capture_output=True)
    return proc.stdout.decode() if proc.returncode == 0 else None


def entry_files(target_id, entry):
    """[(rel path)] the lock pin covers, in hashing order (groups: group.json first)."""
    if entry.get("group"):
        spec = blob_group.load(entry["group"])
        d = spec["dir"].relative_to(REPO)
        return [str(d / "group.json")] + [str(d / f) for f in spec["files"]], spec
    return [entry["source"]], None


def group_sha(texts, files):
    h = hashlib.sha256()
    h.update(texts[0].encode())
    for rel, text in zip(files[1:], texts[1:]):
        h.update(b"\0" + Path(rel).name.encode() + b"\0")
        h.update(text.encode())
    return h.hexdigest()


def prove(names, base, extra=()):
    lock = blob_splice.load_lock()
    seen, singles, groups, proofs = set(), {}, {}, []
    for target_id, entry in sorted(lock.items()):
        key = ("group", entry["group"]) if entry.get("group") else ("single", target_id)
        if key in seen:
            continue
        seen.add(key)
        files, spec = entry_files(target_id, entry)
        old = [git_show(base, rel) for rel in files]
        new = [(REPO / rel).read_text() for rel in files]
        if old == new:
            continue
        if any(o is None for o in old):
            raise SystemExit(f"refusing: {key}: a pinned file is missing at {base}")
        pin = group_sha(old, files) if spec else blob_splice.source_sha(old[0])
        if pin != entry["source_sha256"]:
            raise SystemExit(f"refusing: {key}: the text at {base} does not hold the lock pin")
        dropped = 0
        for rel, o, n in zip(files, old, new):
            if key[1] in extra:
                break             # a new source, proven by the image gate alone
            if rel.endswith((".c", ".h")):
                d = renamed_minus_redeclarations(o, n, names)
            else:
                d = 0 if n == o else None
            if d is None:
                raise SystemExit(f"refusing: {key}: {rel} is not the {base} text renamed")
            dropped += d
        now = group_sha(new, files) if spec else blob_splice.source_sha(new[0])
        changed = [rel for rel, o, n in zip(files, old, new) if o != n]
        proofs.append({"kind": "resplice" if key[1] in extra else key[0], "name": key[1], "old_sha256": pin, "new_sha256": now,
                       "changed_files": changed, "redeclarations_dropped": dropped,
                       "members": sorted(t for t, e in lock.items() if e.get("group") == key[1])
                       if spec else [target_id]})
        (groups if spec else singles)[key[1]] = (entry, spec)
    return lock, singles, groups, proofs


def apply(names, base, extra=(), out="blob_lock_migration.json"):
    lock, singles, groups, proofs = prove(names, base, extra)
    print(f"pure-rename proof: {len(singles)} singles, {len(groups)} groups")
    by_flags = {}
    for target_id, (entry, _spec) in singles.items():
        by_flags.setdefault(entry["flagset"], {})[target_id] = REPO / entry["source"]
    for flags, sources in sorted(by_flags.items()):
        built, failed = blob_splice.compile_on_builder(sources, flags, workdir=f"{WORK}/splice")
        if failed:
            raise SystemExit(f"compile failed: {failed}")
        print(f"compiled {len(built)} singles ({flags})")
    for group, (_entry, spec) in sorted(groups.items()):
        blob_group.compile_group(spec, workdir=f"{WORK}/group")
        print(f"compiled group {group}")
    new_lock = json.loads(json.dumps(lock))
    for proof in proofs:
        for member in proof["members"]:
            new_lock[member]["source_sha256"] = proof["new_sha256"]
    bodies = blob_splice.spliced_bodies(new_lock, require_provenance=False)
    stale = [t for t in new_lock
             if blob_splice.provenance_problem(
                 blob_group.object_path(new_lock[t]["group"], blob_splice.OBJ_DIR / "groups")
                 if new_lock[t].get("group") else blob_splice.OBJ_DIR / f"{t}.o",
                 new_lock[t], required=False)]
    if stale:
        raise SystemExit(f"objects bound to other sources: {stale[:10]}")
    ok, sha, message = blob_splice.build_with(bodies)
    print(message.splitlines()[0] if message else "", sha)
    if not ok:
        raise SystemExit("IMAGE GATE FAILED; lock not written")
    blob_splice.save_lock(new_lock)
    (OUT_DIR / out).write_text(json.dumps({"map": names, "base": base, "image_sha256": sha,
                               "bodies": len(bodies), "entries": proofs}, indent=1) + "\n")
    print(f"lock: {sum(len(p['members']) for p in proofs)} entries re-pinned; image {sha}")


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("command", choices=("prove", "apply"))
    p.add_argument("map", nargs="+")
    p.add_argument("--base", default="HEAD")
    p.add_argument("--out", default="blob_lock_migration.json",
                   help="proof file name (next to this script)")
    p.add_argument("--resplice", action="append", default=[],
                   help="a single whose source changed beyond the rename (new source); "
                        "recompiled and re-pinned under the image gate only")
    a = p.parse_args()
    names = {}
    for m in a.map:
        names.update(static_rename.mapping(static_rename.load_map(m)))
    if a.command == "prove":
        _lock, singles, groups, proofs = prove(names, a.base, set(a.resplice))
        for proof in proofs:
            print(f"{proof['kind']:6} {proof['name']}: {proof['old_sha256'][:12]} -> "
                  f"{proof['new_sha256'][:12]} ({len(proof['members'])} lock entries)")
        print(f"pure-rename proof: {len(singles)} singles, {len(groups)} groups")
    else:
        apply(names, a.base, set(a.resplice), a.out)


if __name__ == "__main__":
    main()
