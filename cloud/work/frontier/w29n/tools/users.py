#!/usr/bin/env python3
"""w29n: which C files USE a name (not only declare it in a generated prelude).

A use is any occurrence of the identifier in code outside comments, outside
string literals and outside top-level declaration-only statements (function
prototypes and `extern` object declarations without an initializer). A
function definition, a call, an address-of, a data access, or an `extern`
declaration WITH an initializer all count as uses.

    python3 cloud/work/frontier/w29n/tools/users.py MAP.json FILE...
"""
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(REPO))
from tools.conveyor.pipeline import static_rename as sr  # noqa: E402

_PROTO = re.compile(r"^[ \t]*(?:extern[ \t]+)?[A-Za-z_][\w \t*]*?\b([A-Za-z_]\w*)[ \t]*"
                    r"\(([^;{}()]|\([^;{}()]*\))*\)[ \t]*;", re.M)
_EXTERN = re.compile(r"^[ \t]*extern[ \t]+[^;{}=()]*;", re.M)


def code_only(text):
    """Comments and string/char literals blanked (newlines kept)."""
    out, i, n = [], 0, len(text)
    while i < n:
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j == -1 else j + 2
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j == -1 else j
        elif text[i] in "\"'":
            q, j = text[i], i + 1
            while j < n and text[j] != q:
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
        else:
            out.append(text[i])
            i += 1
            continue
        out.append(re.sub(r"[^\n]", " ", text[i:j]))
        i = j
    return "".join(out)


def strip_decls(code):
    for rx in (_PROTO, _EXTERN):
        code = rx.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)), code)
    return code


def uses(text, names):
    """{name} of the given names used (not only declared) in text."""
    code = strip_decls(code_only(text))
    found = set()
    for m in re.finditer(r"[A-Za-z_]\w*", code):
        if m.group(0) in names:
            found.add(m.group(0))
    return found


def main():
    names = set(sr.mapping(sr.load_map(sys.argv[1])))
    for f in sys.argv[2:]:
        u = uses(Path(f).read_text(errors="replace"), names)
        if u:
            print(f"{f}: {' '.join(sorted(u))}")


if __name__ == "__main__":
    main()
