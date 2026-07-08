"""Arcade candidate extractor: walk the Rush The Rock source tree and pull
every function definition into the arcade_candidate table (FR-001).

The 1996 codebase mixes ANSI and K&R definitions and won't preprocess
cleanly, so the primary extractor is a comment-aware brace matcher; it makes
no attempt to parse types. The arcade tree is read-only reference material —
we only ever read it.
"""
import hashlib
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
ARCADE = REPO / "reference" / "repos" / "rushtherock"

# Portable-tier first (constitution IV); GUTS/3dfx code mostly has no N64
# counterpart but still provides candidates for shared utility functions.
DEFAULT_SUBDIRS = ("game", "LIB", "MB", "OS", "GUTS")

_NAME_RE = re.compile(r"\b([A-Za-z_][A-Za-z0-9_]*)\s*\($")
_EXCLUDE_HEADER = re.compile(r"[=;]|^\s*#|\btypedef\b")
_KEYWORDS = {"if", "while", "for", "switch", "return", "sizeof", "do", "else"}


def _mask_comments_and_strings(text):
    """Replace comment/string contents with spaces, preserving offsets."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            j = text.find("*/", i + 2)
            j = n if j == -1 else j + 2
            for k in range(i, j):
                if out[k] not in "\n":
                    out[k] = " "
            i = j
        elif c == "/" and i + 1 < n and text[i + 1] == "/":
            j = text.find("\n", i)
            j = n if j == -1 else j
            for k in range(i, j):
                out[k] = " "
            i = j
        elif c in "\"'":
            quote = c
            j = i + 1
            while j < n:
                if text[j] == "\\":
                    j += 2
                    continue
                if text[j] == quote:
                    break
                j += 1
            j = min(j + 1, n)
            for k in range(i + 1, j - 1):
                if out[k] != "\n":
                    out[k] = " "
            i = j
        else:
            i += 1
    return "".join(out)


def extract_functions(text):
    """Yield (name, start, end) for each top-level function definition."""
    masked = _mask_comments_and_strings(text)
    depth = 0
    boundary = 0  # last top-level ';' or '}' — the current header starts here
    i, n = 0, len(masked)
    while i < n:
        c = masked[i]
        if c == "{":
            if depth == 0:
                header = masked[boundary:i]
                name = _header_function_name(header)
                if name:
                    end = _match_brace(masked, i)
                    if end is not None:
                        yield name, _trim_start(masked, boundary), end + 1
                        i = end + 1
                        boundary = i
                        continue
            depth += 1
        elif c == "}":
            depth = max(0, depth - 1)
            if depth == 0:
                boundary = i + 1
        elif c == ";" and depth == 0:
            boundary = i + 1
        i += 1


def _header_function_name(header):
    """Function name if this top-level header introduces a definition."""
    # Preprocessor lines don't end in ';' so a file's include/#define
    # preamble gloms onto the first function's header — drop those lines
    # instead of rejecting the header (this silently skipped the first
    # function of every file with a preamble, e.g. memchr in string.c).
    header = re.sub(r"^\s*#.*$", "", header, flags=re.M)
    if _EXCLUDE_HEADER.search(header):
        return None
    # ANSI: name(args)  |  K&R: name(a, b) int a; char *b;   — in both cases
    # the *first* '(' at paren-depth 0 follows the function name.
    first_paren = header.find("(")
    if first_paren == -1:
        return None
    m = _NAME_RE.search(header[: first_paren + 1].rstrip().rstrip("(") + "(")
    if not m:
        return None
    name = m.group(1)
    if name in _KEYWORDS:
        return None
    return name


def _match_brace(masked, open_idx):
    depth = 0
    for i in range(open_idx, len(masked)):
        if masked[i] == "{":
            depth += 1
        elif masked[i] == "}":
            depth -= 1
            if depth == 0:
                return i
    return None


def _trim_start(text, boundary):
    """Skip whitespace and preprocessor lines so the body starts at the
    definition header itself, not the file's include/#define preamble
    (which precedes a first function with no ';' in between)."""
    n = len(text)
    while boundary < n:
        if text[boundary] in " \t\n":
            boundary += 1
        elif text[boundary] == "#":
            while boundary < n:  # skip the directive + any \ continuations
                nl = text.find("\n", boundary)
                if nl == -1:
                    return n
                cont = text[nl - 1] == "\\"
                boundary = nl + 1
                if not cont:
                    break
        else:
            break
    return boundary


def walk(subdirs=DEFAULT_SUBDIRS, arcade_root=None):
    """Yield (candidate_id, name, rel_file, body_text) across the tree."""
    root = Path(arcade_root or ARCADE)
    for sub in subdirs:
        base = root / sub
        if not base.is_dir():
            continue
        for path in sorted(base.rglob("*.c")):
            rel = str(path.relative_to(root))
            try:
                text = path.read_text(errors="replace")
            except OSError:
                continue
            for name, start, end in extract_functions(text):
                yield f"{rel}:{name}", name, rel, text[start:end]


def populate(conn, subdirs=DEFAULT_SUBDIRS, arcade_root=None):
    from ..coordinator import db as dbmod

    count, files = 0, set()
    with dbmod.tx(conn):
        for candidate_id, name, rel, body in walk(subdirs, arcade_root):
            body_sha = hashlib.sha256(body.encode()).hexdigest()
            conn.execute(
                "INSERT INTO arcade_candidate (candidate_id, name, src_file, body_sha)"
                " VALUES (?, ?, ?, ?)"
                " ON CONFLICT(candidate_id) DO UPDATE SET body_sha=excluded.body_sha",
                (candidate_id, name, rel, body_sha),
            )
            count += 1
            files.add(rel)
    return {"candidates": count, "files": len(files)}


def extract_named_function(source_path, name):
    """Extract one function's text from any C file using the comment/string-
    aware brace matcher (the naive regex counters miscount braces inside
    comments and string literals). Shared by sweep and the smoke path."""
    text = Path(source_path).read_text(errors="replace")
    for got_name, start, end in extract_functions(text):
        if got_name == name:
            return text[start:end]
    raise KeyError(f"{name} not found in {source_path}")


def get_body(candidate_id, arcade_root=None):
    """Re-extract one candidate's body text (bodies are not stored in the DB)."""
    rel, _, name = candidate_id.partition(":")
    root = Path(arcade_root or ARCADE)
    text = (root / rel).read_text(errors="replace")
    for got_name, start, end in extract_functions(text):
        if got_name == name:
            return text[start:end]
    raise KeyError(candidate_id)


def main():
    import argparse

    from ..coordinator import db as dbmod

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(Path("~/.conveyor").expanduser()))
    parser.add_argument("--subdirs", default=",".join(DEFAULT_SUBDIRS))
    args = parser.parse_args()
    conn = dbmod.connect(Path(args.data) / "conveyor.db")
    summary = populate(conn, subdirs=args.subdirs.split(","))
    print(f"candidates: {summary['candidates']} functions from {summary['files']} files")


if __name__ == "__main__":
    sys.exit(main())
