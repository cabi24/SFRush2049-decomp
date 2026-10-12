#!/usr/bin/env python3
"""w23g: drop prototype lines the game-side rename turned into conflicting redeclarations.

The generated preludes of src/blob sources declare both an old game-side name
(e.g. osPfsReAllocate, which named 0x8000BE50) with that old name's ultralib
signature, and the true function's ultralib name (osGetThreadId) with ITS
signature. The w22g permutation renames the first into the second, so the file
now declares osGetThreadId twice with different types (cfe: "Incompatible type
for the function parameter"). The renamed line carried the signature of a
different function; the surviving line is the ultralib prototype of the
function at that address. A renamed single-declarator prototype statement (it may span lines) is removed
only when (a) the base line declared an old name, (b) its new name is declared
again elsewhere in the file, and (c) the file never calls that name.

    python3 cloud/work/frontier/w23g/drop_renamed_dups.py MAP.json BASE FILE...
"""
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))
from tools.conveyor.pipeline import static_rename as sr  # noqa: E402

DECL = re.compile(r"^[ \t]*(?:extern[ \t]+)?[A-Za-z_][\w \t*]*?\b([A-Za-z_]\w*)[ \t]*\(([^;{}()]|\([^;{}()]*\))*\)[ \t]*;[ \t]*(?:\n|$)", re.M)


def spans(text):
    """[(name, start, end)] of single-declarator prototype statements (may span lines)."""
    code = re.sub(r"/\*.*?\*/|//[^\n]*", lambda m: re.sub(r"[^\n]", " ", m.group(0)), text, flags=re.S)
    return [(m.group(1), m.start(), m.end()) for m in DECL.finditer(code)]


def called(text, name, decls):
    code = re.sub(r"/\*.*?\*/|//[^\n]*", lambda m: re.sub(r"[^\n]", " ", m.group(0)), text, flags=re.S)
    for _n, s, e in sorted(decls, key=lambda d: -d[1]):
        code = code[:s] + " " * (e - s) + code[e:]
    return re.search(r"\b" + re.escape(name) + r"\s*\(", code) is not None


def fix(path, base, names):
    old = subprocess.run(["git", "show", f"{base}:{path}"], cwd=REPO,
                         capture_output=True, text=True).stdout
    new = Path(path).read_text()
    if not old or sr.permute_c(old, names) != new:
        return 0                      # only a file that is exactly the renamed base
    ospans, nspans = spans(old), spans(new)
    if [names.get(n, n) for n, _s, _e in ospans] != [n for n, _s, _e in nspans]:
        raise SystemExit(f"{path}: declaration statements do not correspond")
    drop = []
    for (oname, _os, _oe), (name, s, e) in zip(ospans, nspans):
        if oname not in names:
            continue
        others = [d for d in nspans if d[0] == name and d[1] != s]
        if others and not called(new, name, nspans):
            drop.append((s, e))
    for s, e in sorted(drop, reverse=True):
        new = new[:s] + new[e:]
    if drop:
        Path(path).write_text(new)
    return len(drop)


def main():
    names = sr.mapping(sr.load_map(sys.argv[1]))
    base, files = sys.argv[2], sys.argv[3:]
    total = touched = 0
    for f in files:
        n = fix(f, base, names)
        total += n
        touched += bool(n)
    print(f"dropped {total} renamed redeclarations in {touched} of {len(files)} files")


if __name__ == "__main__":
    main()
