"""Shared TU-reduction and header-resolution helpers.

Moved out of pipeline/lock.py so the regression lock and the corpus pipeline
compile candidates through one mechanism. The helpers are parameterized on
`(repo_root, include_dirs)` instead of assuming the game repo, so any corpus
root's headers resolve the same way the build does — the includer's own
directory first, then each configured include dir.
"""
import re
from pathlib import Path

from .extract_candidates import extract_functions

# Follow both quoted and angle-bracket includes. Under -nostdinc (the N64 SDK
# convention, e.g. ultralib) `<PR/foo.h>` resolves from the -I dirs just like a
# quoted include, and those headers must travel in the bundle. Only files that
# actually exist in the includer's dir or an include dir are bundled, so a true
# system `<stdio.h>` in a game-repo TU simply resolves to nothing (unchanged).
_INCLUDE_RE = re.compile(r'^\s*#\s*include\s*[<"]([^">]+)[>"]', re.M)


def strip_comments(text):
    """Remove C `/* */` and C++ `//` comments, preserving string/char literals
    verbatim and keeping newlines (so preprocessor directives stay on their own
    lines). Comments never affect codegen, so this is score-neutral — it just
    lets IDO cfe accept modern-decomp `//` sources without `-Xcpluscomm`, which
    the confirmed flagsets deliberately omit."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            j = text.find("*/", i + 2)
            i = n if j == -1 else j + 2
            out.append(" ")  # a space so `a/**/b` doesn't become `ab`
        elif c == "/" and i + 1 < n and text[i + 1] == "/":
            j = text.find("\n", i)
            i = n if j == -1 else j  # drop the comment, keep the newline
        elif c in "\"'":
            q = c
            out.append(c)
            j = i + 1
            while j < n:
                ch = text[j]
                out.append(ch)
                if ch == "\\" and j + 1 < n:
                    out.append(text[j + 1])
                    j += 2
                    continue
                if ch == q:
                    j += 1
                    break
                j += 1
            i = j
        else:
            out.append(c)
            i += 1
    return "".join(out)


def reduced_tu(text, keep_name):
    """The TU with every function definition except keep_name removed —
    headers, types, and file-scope declarations survive untouched."""
    spans = [
        (start, end) for name, start, end in extract_functions(text)
        if name != keep_name
    ]
    out, pos = [], 0
    for start, end in spans:
        out.append(text[pos:start])
        pos = end
    out.append(text[pos:])
    return "".join(out)


def resolve_headers(tu_path, repo, include_dirs):
    """{repo-relative name: text} for the TU's `#include "..."` closure,
    searched the way the build does: the includer's own directory first,
    then each entry of include_dirs (repo-root-relative)."""
    repo = Path(repo)
    found, queue = {}, [Path(tu_path)]
    while queue:
        path = queue.pop()
        text = path.read_text(errors="replace")
        for inc in _INCLUDE_RE.findall(text):
            candidates = [path.parent / inc] + [
                repo / d / inc for d in include_dirs
            ]
            for cand in candidates:
                if cand.is_file():
                    rel = str(cand.resolve().relative_to(repo.resolve()))
                    if rel not in found:
                        found[rel] = cand.read_text(errors="replace")
                        queue.append(cand)
                    break
    return found
