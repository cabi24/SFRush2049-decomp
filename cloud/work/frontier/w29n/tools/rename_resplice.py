#!/usr/bin/env python3
"""w29n: migrate blob_matched.lock.json across a pure GAME rename, then re-prove the image.

    python3 cloud/work/frontier/w29n/tools/rename_resplice.py prove MAP.json [--base REV]
    python3 cloud/work/frontier/w29n/tools/rename_resplice.py apply MAP.json [--base REV]

Lane w23g's rename_resplice.py proved a rename of STATIC names inside locked
game sources (lock keys unchanged). A game rename also renames lock KEYS (the
locked function itself) and may move a single's source file
(src/blob/OLD.c -> src/blob/NEW.c). This script:

  1. re-keys the lock: every entry whose target_id is an old name moves to the
     new name; a single whose source path is src/blob/OLD.c moves to
     src/blob/NEW.c (the file must have been `git mv`ed);
  2. for every lock entry (single or group) whose files differ from --base,
     requires that the files at --base hash to the old lock's source_sha256
     (the locked text) and that every changed file on disk is EXACTLY the
     --base text renamed: C/H by static_rename.permute_c (identifiers in code
     only; comments and strings untouched), group.json/JSON by renaming the
     string values that equal an old name (same json.dumps format). A file
     left unchanged must not USE an old name (users.py: only generated-prelude
     declarations of it may remain). Anything else refuses (nothing written).
  3. `apply` recompiles the changed singles (blob_splice.compile_on_builder,
     per flagset) and groups (blob_group.compile_group) on the builder in this
     lane's scratch, links EVERY locked body (blob_splice.spliced_bodies)
     against today's symbol table (which must already carry the new names:
     the renamed layout from the renamed inventory, GAME_SYMBOLS), runs the
     image gate, and only then writes the lock. The proof goes to --out.
"""
import argparse
import hashlib
import json
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(REPO))
from tools.conveyor.pipeline import blob_group, blob_splice, static_rename  # noqa: E402
sys.path.insert(0, str(Path(__file__).resolve().parent))
import users  # noqa: E402

WORK = "rush2049/scratch/frontier/w29n"          # builder dirs, relative to $HOME
OUT_DIR = Path(__file__).resolve().parent.parent


def git_show(base, rel):
    proc = subprocess.run(["git", "show", f"{base}:{rel}"], cwd=REPO, capture_output=True)
    return proc.stdout.decode() if proc.returncode == 0 else None


def rename_json_text(text, names):
    """The JSON text with every string value equal to an old name renamed, in
    the file's own json.dumps format (indent 1 or 2); None if the original is
    not in that canonical format."""
    doc = json.loads(text)

    def walk(x):
        if isinstance(x, str):
            return names.get(x, x)
        if isinstance(x, list):
            return [walk(v) for v in x]
        if isinstance(x, dict):
            return {k: walk(v) for k, v in x.items()}
        return x
    for indent in (1, 2):
        if json.dumps(doc, indent=indent) + "\n" == text:
            return json.dumps(walk(doc), indent=indent) + "\n"
    return None


def renamed(old, rel, names):
    if rel.endswith((".c", ".h")):
        return static_rename.permute_c(old, names)
    if rel.endswith(".json"):
        return rename_json_text(old, names)
    return old


def rekey(lock, names):
    new_lock = {}
    for target_id, entry in lock.items():
        entry = dict(entry)
        if not entry.get("group"):
            src = Path(entry["source"])
            if src.parent == Path("src/blob") and src.stem in names:
                entry["source"] = str(src.with_name(names[src.stem] + ".c"))
        new_lock[names.get(target_id, target_id)] = entry
    if len(new_lock) != len(lock):
        raise SystemExit("refusing: re-keying merged two lock entries")
    return new_lock


def entry_files(entry):
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


def prove(names, base):
    old_lock = blob_splice.load_lock()
    inverse = {v: k for k, v in names.items()}
    lock = rekey(old_lock, names)
    seen, singles, groups, proofs = set(), {}, {}, []
    for target_id, entry in sorted(lock.items()):
        key = ("group", entry["group"]) if entry.get("group") else ("single", target_id)
        if key in seen:
            continue
        seen.add(key)
        files, spec = entry_files(entry)
        old_entry = old_lock[inverse.get(target_id, target_id)]
        old_files = files if spec else [old_entry["source"]]
        old = [git_show(base, rel) for rel in old_files]
        new = [(REPO / rel).read_text() for rel in files]
        moved = old_files != files
        if old == new and not moved:
            continue
        if any(o is None for o in old):
            raise SystemExit(f"refusing: {key}: a pinned file is missing at {base}")
        pin = group_sha(old, files) if spec else blob_splice.source_sha(old[0])
        if pin != old_entry["source_sha256"]:
            raise SystemExit(f"refusing: {key}: the text at {base} does not hold the lock pin")
        for rel, o, n in zip(files, old, new):
            if n == o and rel.endswith((".c", ".h")) and users.uses(o, set(names)):
                raise SystemExit(f"refusing: {key}: {rel} still uses an old name")
            if n != o and renamed(o, rel, names) != n:
                raise SystemExit(f"refusing: {key}: {rel} is not the {base} text renamed")
        now = group_sha(new, files) if spec else blob_splice.source_sha(new[0])
        members = (sorted(t for t, e in lock.items() if e.get("group") == key[1]) if spec else [target_id])
        proofs.append({"kind": key[0], "name": key[1], "old_sha256": pin, "new_sha256": now,
                       "changed_files": [rel for rel, o, n in zip(files, old, new) if o != n],
                       "moved_from": old_files[0] if moved else None,
                       "renamed_members": sorted(m for m in members if m in inverse),
                       "members": members})
        (groups if spec else singles)[key[1]] = (entry, spec)
    rekeyed = sorted(f"{inverse[t]} -> {t}" for t in lock if t in inverse)
    return lock, singles, groups, proofs, rekeyed


def apply(names, base, out):
    lock, singles, groups, proofs, rekeyed = prove(names, base)
    print(f"pure-rename proof: {len(singles)} singles, {len(groups)} groups; re-keyed {len(rekeyed)}")
    pins = {m: p["new_sha256"] for p in proofs for m in p["members"]}
    for t, e in lock.items():       # objects left bound to another text (an earlier run) are rebuilt too
        e = dict(e, source_sha256=pins.get(t, e["source_sha256"]))
        obj = (blob_group.object_path(e["group"], blob_splice.OBJ_DIR / "groups") if e.get("group")
               else blob_splice.OBJ_DIR / f"{t}.o")
        if obj.is_file() and blob_splice.provenance_problem(obj, e, required=False):
            if e.get("group"):
                groups.setdefault(e["group"], (e, blob_group.load(e["group"])))
            else:
                singles.setdefault(t, (e, None))
    print(f"compiling {len(singles)} singles, {len(groups)} groups (renamed + stale objects)")
    by_flags = {}
    for target_id, (entry, _spec) in singles.items():
        by_flags.setdefault(entry["flagset"], {})[target_id] = REPO / entry["source"]
    for flags, sources in sorted(by_flags.items()):
        built, failed = blob_splice.compile_on_builder(sources, flags, workdir=f"{WORK}/splice")
        if failed:
            raise SystemExit(f"compile failed: {failed}")
        print(f"compiled {len(built)} singles ({flags})")
    new_lock = json.loads(json.dumps(lock))
    for proof in proofs:
        for member in proof["members"]:
            new_lock[member]["source_sha256"] = proof["new_sha256"]
    for group, (_entry, spec) in sorted(groups.items()):
        blob_group.compile_group(spec, workdir=f"{WORK}/group")
        print(f"compiled group {group}")
    stale = []      # compile_on_builder/compile_group bind each object to its source
    for t, e in new_lock.items():
        obj = (blob_group.object_path(e["group"], blob_splice.OBJ_DIR / "groups") if e.get("group")
               else blob_splice.OBJ_DIR / f"{t}.o")
        if blob_splice.provenance_problem(obj, e, required=False):
            stale.append(t)
    if stale:
        raise SystemExit(f"objects bound to other sources: {stale[:10]}")
    bodies = blob_splice.spliced_bodies(new_lock, require_provenance=False)
    ok, sha, message = blob_splice.build_with(bodies)
    print(message.splitlines()[0] if message else "", sha)
    if not ok:
        raise SystemExit("IMAGE GATE FAILED; lock not written")
    blob_splice.save_lock(new_lock)
    (OUT_DIR / out).write_text(json.dumps({"map": names, "base": base, "image_sha256": sha,
                                           "bodies": len(bodies), "rekeyed": rekeyed,
                                           "entries": proofs}, indent=1) + "\n")
    print(f"lock: {len(rekeyed)} keys renamed, {sum(len(p['members']) for p in proofs)} entries re-pinned;"
          f" image {sha}")


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("command", choices=("prove", "apply"))
    p.add_argument("map")
    p.add_argument("--base", default="HEAD")
    p.add_argument("--out", default="blob_lock_migration.json", help="proof file (lane dir)")
    a = p.parse_args()
    names = static_rename.mapping(static_rename.load_map(a.map))
    if a.command == "prove":
        _lock, singles, groups, proofs, rekeyed = prove(names, a.base)
        for proof in proofs:
            print(f"{proof['kind']:6} {proof['name']}: {proof['old_sha256'][:12]} -> "
                  f"{proof['new_sha256'][:12]} ({len(proof['members'])} lock entries"
                  + (f", renamed {proof['renamed_members']}" if proof["renamed_members"] else "")
                  + (f", moved from {proof['moved_from']}" if proof["moved_from"] else "") + ")")
        print(f"pure-rename proof: {len(singles)} singles, {len(groups)} groups; re-keyed: {rekeyed}")
    else:
        apply(names, a.base, a.out)


if __name__ == "__main__":
    main()
