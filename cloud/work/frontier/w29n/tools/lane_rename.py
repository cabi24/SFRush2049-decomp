#!/usr/bin/env python3
"""w32n: rename a batch's old function names in the lane directories' draft_check
arguments and group packets (the C/H drafts are renamed by static_rename in replay.sh).

    python3 cloud/work/frontier/w29n/tools/lane_rename.py BATCH_TABLE.json [--write]

Files: every tracked `dc.args` and `group.json` under cloud/work/frontier/w2[89]?/ and
w3[0-9]?/ (not the rename lanes w29n/w30n/w32n).
  - dc.args: the batch's old names become the new ones as whole identifiers on the
    argument lines (`--name`, `--unit A,B`, `--params MEMBER:...`, `--callee 'proto'`);
    `#` comment lines are history and stay. Directory names are ids and stay (draft_check
    maps a directory named after an old function through renames.json, Phase 10).
  - group.json: every string VALUE equal to an old name (members, claims, keep, ...),
    in the file's own json.dumps format (rename_resplice.rename_json_text; provenance
    prose is not an exact name and stays); a hand-formatted file gets exact `"OLD"`
    string edits, refused unless they change exactly those values (no key).
Dry run without --write; every change is printed.
"""
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[4]
sys.path.insert(0, str(REPO))
sys.path.insert(0, str(HERE))
from rename_resplice import rename_json_text  # noqa: E402

SKIP = ("cloud/work/frontier/w29n/", "cloud/work/frontier/w30n/", "cloud/work/frontier/w32n/")
PATTERNS = ["cloud/work/frontier/w2[89]?/*dc.args", "cloud/work/frontier/w3[0-9]?/*dc.args",
            "cloud/work/frontier/w2[89]?/*group.json", "cloud/work/frontier/w3[0-9]?/*group.json"]


def table(path):
    return {r["old"]: r["new"] for r in json.loads(Path(path).read_text())["renames"]
            if r.get("status", "accepted") == "accepted" and r.get("kind", "function") == "function"}


def dc_args(text, names):
    rx = re.compile(r"(?<![A-Za-z0-9_])(%s)(?![A-Za-z0-9_])"
                    % "|".join(map(re.escape, sorted(names, key=len, reverse=True))))
    out = []
    for line in text.splitlines(True):
        out.append(line if line.lstrip().startswith("#") else rx.sub(lambda m: names[m.group(1)], line))
    return "".join(out)


def _values(x, names):
    if isinstance(x, str):
        return names.get(x, x)
    if isinstance(x, list):
        return [_values(v, names) for v in x]
    if isinstance(x, dict):
        return {k: _values(v, names) for k, v in x.items()}
    return x


def group_json(text, names):
    """The group.json text with every string value equal to an old name renamed: in the
    json.dumps format when the file has it, else by exact `"OLD"` string edits, proved
    to change values only (the parsed result equals the parsed original renamed)."""
    doc = json.loads(text)
    want = _values(doc, names)
    if want == doc:
        return text
    new = rename_json_text(text, names)
    if new is not None:
        return new
    new = text
    for o, n in names.items():
        new = new.replace('"%s"' % o, '"%s"' % n)
    return new if json.loads(new) == want else None


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("table")
    p.add_argument("--write", action="store_true")
    a = p.parse_args()
    names = table(a.table)
    files = subprocess.run(["git", "ls-files", "--"] + PATTERNS, cwd=REPO, capture_output=True,
                           text=True, check=True).stdout.split()
    for rel in sorted(set(files)):
        if rel.startswith(SKIP) or not rel.endswith(("/dc.args", "/group.json")):
            continue
        path = REPO / rel
        old = path.read_text()
        if rel.endswith("dc.args"):
            new = dc_args(old, names)
        else:
            new = group_json(old, names)
            if new is None:
                sys.exit(f"refusing: {rel}: a renamed name is also a key (not in json.dumps format)")
        if new != old:
            print(f"{'rewrite' if a.write else 'would rewrite'} {rel}")
            if a.write:
                path.write_text(new)
    return 0


if __name__ == "__main__":
    sys.exit(main())
