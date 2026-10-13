#!/usr/bin/env python3
"""w29n: rename the src/blob JSON and single-file names of a game rename table.

    python3 cloud/work/frontier/w29n/tools/json_rename.py MAP.json

Every string value equal to an old name in src/blob/unit_overrides.json and
src/blob/groups/*/group.json is renamed (same json.dumps format; provenance
text is left alone), and every locked single src/blob/OLD.c is `git mv`ed to
src/blob/NEW.c. Group directory and file names are ids, not symbols: kept.
"""
import json
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[4]
sys.path.insert(0, str(REPO))
sys.path.insert(0, str(HERE))
from tools.conveyor.pipeline import static_rename  # noqa: E402
from rename_resplice import rename_json_text  # noqa: E402


def has(x, names):
    if isinstance(x, str):
        return x in names
    if isinstance(x, list):
        return any(has(v, names) for v in x)
    if isinstance(x, dict):
        return any(has(v, names) for v in x.values())
    return False


def main():
    names = static_rename.mapping(static_rename.load_map(sys.argv[1]))
    blob = REPO / "src/blob"
    for p in [blob / "unit_overrides.json"] + sorted((blob / "groups").glob("*/group.json")):
        raw = p.read_text()
        if not has(json.loads(raw), names):
            continue
        new = rename_json_text(raw, names)
        if new is None:
            sys.exit(f"refusing: {p} is not in json.dumps format")
        if new != raw:
            p.write_text(new)
            print(f"json {p.relative_to(REPO)}")
    for old, new in sorted(names.items()):
        if (blob / f"{old}.c").is_file():
            subprocess.run(["git", "mv", f"src/blob/{old}.c", f"src/blob/{new}.c"], cwd=REPO, check=True)
            print(f"git mv src/blob/{old}.c src/blob/{new}.c")


if __name__ == "__main__":
    main()
