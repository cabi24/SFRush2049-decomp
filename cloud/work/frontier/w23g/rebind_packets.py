#!/usr/bin/env python3
"""w23g: rebind research packets that pinned pre-rename game sources.

    python3 cloud/work/frontier/w23g/rebind_packets.py [--dry-run] PATH...

PATH is a packet directory (cloud/work/...) or a test file. Lane w23g renamed
the game callers of the static functions in static_rename.RENAME_TABLES; the
packets recorded before that pin src/blob sources, cloud/matches submissions,
asm/us/blob/symbols.json, their own candidate sources and the names those
sources call. Rebinding is the same rename, applied to the packet:

  1. C sources in a packet directory get static_rename.permute_c with the union
     of the tables plus the redeclaration drop (drop_renamed_dups), exactly as
     src/blob did.
  2. Python files (packet verifiers and tests) get their string literals that
     spell a renamed function exactly ('osJamMesg') renamed; code is untouched.
  3. JSON/text receipts get (a) every exact name string renamed the same way
     and (b) every sha256 that pinned a file whose current form is a proven
     rename of the pinned bytes replaced by the current file's sha256.
     "Proven" = static_rename.pinned_bytes() accepts the RENAME_BASE bytes
     (C: renamed_game_text; symbols.json: same addresses, renamed keys), or the
     file is one this tool rewrote by rules 1-3 (its old bytes at RENAME_BASE
     hashed to the pin).
Steps 3(b) repeats until no pin changes (receipts may pin each other).
Every rewritten file is logged in cloud/work/frontier/w23g/rebind_log.json
(rule; for pins: path, old and new sha256). Rules 1-2 run only with --sources
and rule 3a only with --names (a packet that compares its own sources against
older revisions must keep them).
"""
import argparse
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))
sys.path.insert(0, str(Path(__file__).resolve().parent))
from tools.conveyor.pipeline import static_rename as sr  # noqa: E402
import drop_renamed_dups  # noqa: E402

BASE = sr.RENAME_BASE
HEX = re.compile(r"\b[0-9a-f]{64}\b")
KEYED = re.compile(r'"([^"\n]+)"\s*:\s*"([0-9a-f]{64})"')
PATH = re.compile(r"[A-Za-z0-9_][A-Za-z0-9_./-]*\.(?:c|h|s|py|json|jsonl|txt|ld|inc)\b")
TEXT = (".json", ".jsonl", ".txt", ".md", ".log", ".csv")


def sha(data):
    return hashlib.sha256(data).hexdigest()


def git_bytes(rel):
    p = subprocess.run(["git", "show", f"{BASE}:{rel}"], cwd=REPO, capture_output=True)
    return p.stdout if p.returncode == 0 else None


def rel(path):
    return str(Path(path).resolve().relative_to(REPO.resolve()))


def changed_since_base():
    out = subprocess.run(["git", "diff", "--name-only", BASE, "--"], cwd=REPO,
                         capture_output=True, text=True, check=True).stdout.split()
    return [p for p in out if (REPO / p).is_file()]


def rename_strings_py(text, names):
    def sub(m):
        q, body = m.group(1), m.group(2)
        return q + names.get(body, body) + q
    return re.sub(r"""(['"])([A-Za-z_]\w*)\1""", sub, text)


def rename_strings_json(text, names):
    return re.sub(r'"([A-Za-z_]\w*)"', lambda m: '"%s"' % names.get(m.group(1), m.group(1)), text)


def files_of(paths):
    out = []
    for p in paths:
        p = (REPO / p) if not Path(p).is_absolute() else Path(p)
        if p.is_dir():
            out += sorted(f for f in p.rglob("*") if f.is_file() and "/v/" not in str(f)
                          and "__pycache__" not in str(f))
        elif p.is_file():
            out.append(p)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("paths", nargs="+")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--sources", action="store_true",
                    help="also apply rules 1-2 (rename packet C sources and Python name literals)")
    ap.add_argument("--py", action="store_true",
                    help="apply rule 2 only (Python name literals), not rule 1")
    ap.add_argument("--names", action="store_true",
                    help="also apply rule 3a (rename exact name strings in receipts)")
    a = ap.parse_args()
    names = sr.libultra_names()
    files = files_of(a.paths)
    log = {}                                  # rel -> {"old", "new", "rule"}
    # 1-2: sources
    kinds = (".c", ".h", ".py") if a.sources else (".py",) if a.py else ()
    for f in files:
        r = rel(f)
        old = f.read_text(errors="surrogateescape") if f.suffix in kinds else None
        if old is None:
            continue
        if f.suffix in (".c", ".h"):
            new = sr.permute_c(old, names)
            rule = "C rename (static_rename.permute_c) + redeclaration drop"
        else:
            new = rename_strings_py(old, names)
            rule = "Python string literals naming renamed functions"
        if new == old:
            continue
        if not a.dry_run:
            f.write_text(new, errors="surrogateescape")
            if f.suffix in (".c", ".h"):
                base_text = git_bytes(r)
                if base_text is not None and base_text.decode(errors="surrogateescape") == old:
                    drop_renamed_dups.fix(r, BASE, names)
        log[r] = {"rule": rule}
    # 3a: receipts' name strings
    for f in files if a.names else []:
        if f.suffix not in TEXT:
            continue
        old = f.read_text(errors="surrogateescape")
        new = rename_strings_json(old, names) if f.suffix in (".json", ".jsonl") else old
        if new != old and not a.dry_run:
            f.write_text(new, errors="surrogateescape")
            log[rel(f)] = {"rule": "receipt name strings renamed"}
    # 3b: sha pins, to a fixpoint
    candidates = set(changed_since_base()) | set(log)
    skipped = {}
    for _round in range(10):
        pins = {}
        for r in sorted(candidates):
            base = git_bytes(r)
            if base is None:
                continue
            live = (REPO / r).read_bytes()
            old_sha, new_sha = sha(base), sha(live)
            if old_sha == new_sha:
                continue
            proven = r in log or sr.pinned_bytes(REPO / r, old_sha) == base
            if proven:
                pins[old_sha] = (new_sha, r)
        changed = False
        # A pin is rebound only when it cannot mean an unchanged file: any
        # file a packet or test mentions whose CURRENT bytes hash to the old
        # value (an identical copy, e.g. a packet's group.c beside the renamed
        # src/blob/groups copy) makes the bare value ambiguous; such a value is
        # rebound only where a JSON object keys it by the renamed file's path.
        ambiguous = {}
        for f in files:
            if f.suffix not in TEXT + (".py",):
                continue
            for m in PATH.finditer(f.read_text(errors="surrogateescape")):
                for cand in (REPO / m.group(0), f.parent / m.group(0)):
                    try:
                        if cand.is_file() and rel(cand) not in candidates:
                            ambiguous.setdefault(sha(cand.read_bytes()), rel(cand))
                    except (ValueError, OSError):
                        pass
        for f in files:
            if f.suffix not in TEXT + (".py",):
                continue
            text = f.read_text(errors="surrogateescape")
            r_f = rel(f)
            hits = []

            def keyed(m):
                key, value = m.group(1), m.group(2)
                if value in pins and (key == pins[value][1] or key.endswith("/" + pins[value][1])):
                    hits.append(value)
                    return m.group(0).replace(value, pins[value][0])
                return m.group(0)
            new = KEYED.sub(keyed, text)

            def bare(m):
                v = m.group(0)
                if v in pins and v not in ambiguous:
                    hits.append(v)
                    return pins[v][0]
                if v in pins:
                    skipped.setdefault(r_f, set()).add((pins[v][1], ambiguous[v]))
                return v
            new = HEX.sub(bare, new)
            if new != text:
                changed = True
                if not a.dry_run:
                    f.write_text(new, errors="surrogateescape")
                log.setdefault(r_f, {"rule": "sha256 pins rebound"})
                log[r_f].setdefault("pins", [])
                for v in hits:
                    log[r_f]["pins"].append({"path": pins[v][1], "old": v, "new": pins[v][0]})
                candidates.add(r_f)
        if not changed or a.dry_run:
            break
    for r, entry in sorted(log.items()):
        print(f"{r}: {entry['rule']}" + (f" ({len(entry.get('pins', []))} pins)" if entry.get("pins") else ""))
    for r_f, pairs in sorted(skipped.items()):
        for changed_path, twin in sorted(pairs):
            print(f"{r_f}: left a bare pin of {changed_path} (same bytes as unchanged {twin})")
    if not a.dry_run and log:
        out = Path(__file__).resolve().parent / "rebind_log.json"
        prev = json.loads(out.read_text()) if out.is_file() else {}
        prev.update(log)
        out.write_text(json.dumps(prev, indent=1, sort_keys=True) + "\n")


if __name__ == "__main__":
    main()
