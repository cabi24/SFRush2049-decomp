#!/usr/bin/env python3
"""Re-rank near misses by strict score.py results (requires x86 IDO).

Updates INDEX.md only after every entry has been evaluated. Previously reported
strict matches are rescored too, making repeated runs deterministic and keeping
their flags/provenance available for the maintainer.
"""
import argparse
from dataclasses import dataclass
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys
import tempfile
from typing import Optional

if __package__:
    from . import score
else:
    import score

REPO = Path(__file__).resolve().parents[2]
INDEX = REPO / "cloud/work/near-miss/INDEX.md"


@dataclass
class Entry:
    name: str
    pipeline_score: int
    source: str
    flags: str


@dataclass
class Result:
    entry: Entry
    words: Optional[int]
    summary: str
    matched: bool = False


def read_entries(text):
    """Read both the original table and the ranked/matched tables on reruns."""
    entries, names, columns = [], set(), None
    for line in text.splitlines():
        if not line.startswith("|"):
            columns = None
            continue
        cells = [cell.strip() for cell in line.strip().strip("|").split("|")]
        if cells[0] == "function":
            columns = cells
            continue
        if columns is None or re.fullmatch(r"[-:]+", cells[0]):
            continue
        if len(cells) != len(columns):
            raise ValueError(f"malformed INDEX row: {line}")
        row = dict(zip(columns, cells))
        name = row["function"]
        if not re.fullmatch(r"[A-Za-z_]\w*", name, flags=re.ASCII) or name in names:
            raise ValueError(f"invalid or duplicate function: {name}")
        flags = row["flags"]
        if not (flags.startswith("`") and flags.endswith("`")) or not shlex.split(flags[1:-1]):
            raise ValueError(f"{name}: missing or invalid flags")
        entries.append(Entry(name, int(row.get("pipeline score", row.get("score", ""))),
                             row["source"], flags[1:-1]))
        names.add(name)
    if not entries:
        raise ValueError("INDEX contains no function rows")
    return entries


def evaluate(entry, directory):
    source = directory / entry.name / "base.c"
    proc = subprocess.run(
        [sys.executable, str(REPO / "tools/cloud/score.py"), "fn", str(source),
         entry.name, "--flags=" + entry.flags], capture_output=True, text=True)
    lines = [line.strip() for line in proc.stdout.splitlines() if line.strip()]
    summary = lines[-1] if lines else ""
    if proc.returncode == 0 and summary == "MATCH":
        return Result(entry, 0, summary, True)
    # Reject unexpected successful output, runtime/tool failures and changed
    # protected inputs. They must not silently turn the whole list into N/A.
    if (proc.returncode != 1 or "target integrity check failed" in proc.stderr
            or "Traceback (most recent call last)" in proc.stderr
            or "missing: run tools/cloud/setup.sh" in proc.stderr):
        raise RuntimeError(f"{entry.name}: scorer failed unexpectedly:\n{proc.stdout}{proc.stderr}")
    match = re.fullmatch(r"(?:(\d+)/\d+ words differ|MATCH|MISMATCH|NOT VERIFIED)(?: \(.*\))?",
                         summary)
    if match:
        words = int(match[1] or 0)
        extra = re.search(r"(\d+) extra words", summary)
        words += int(extra[1]) if extra else 0
        # An exit-1 plain MATCH is inconsistent, not a verified free win.
        if summary == "MATCH":
            raise RuntimeError(f"{entry.name}: scorer printed MATCH but exited 1")
        return Result(entry, words, summary)
    error = proc.stderr.strip()
    if not error.startswith(("IDO compile failed:", "no target section ", entry.name + " is not a defined function")):
        raise RuntimeError(f"{entry.name}: unrecognized scorer output:\n{proc.stdout}{proc.stderr}")
    print(error, file=sys.stderr, flush=True)
    concise = " ".join(error.replace(str(REPO) + "/", "").split())
    return Result(entry, None, "Not scored: " + concise[:240])


def render(results):
    ranked = sorted((r for r in results if not r.matched),
                    key=lambda r: (r.words is None, r.words or 0, r.entry.name))
    matches = sorted((r for r in results if r.matched), key=lambda r: r.entry.name)
    lines = [
        "# Single-function near misses", "",
        "Pipeline score is the historical heuristic (stack offsets ignored), not matching evidence.",
        "Strict words differing counts score.py's full-word differences plus nonzero words beyond",
        "the target extent. Unverified relocations and unresolved symbols remain visible in the",
        "result column: zero differences alone is not a verified match. Unscorable entries sort last.",
        "Each directory contains base.c for the whole translation unit. Verify with:", "",
        '    python3 tools/cloud/score.py fn cloud/work/near-miss/<name>/base.c <name> --flags "<flags>"',
        "", "Re-rank on x86 Linux:", "", "    python3 tools/cloud/rank_near_miss.py", "",
        f"Rescored {len(results)} entries: {len(ranked)} remain below. Strict matches found: {len(matches)}.",
        "Strict matches are reported separately for maintainer review and removed from the ranked worklist.", "",
    ]

    def table(rows):
        lines.extend(["| function | pipeline score | strict words differing | source | flags | score.py result |",
                      "|---|---|---|---|---|---|"])
        for result in rows:
            e = result.entry
            # Escape table separators in diagnostics without adding new columns.
            message = result.summary.replace("|", "&#124;")
            count = "—" if result.words is None else str(result.words)
            lines.append(f"| {e.name} | {e.pipeline_score} | {count} | {e.source} | `{e.flags}` | {message} |")

    table(ranked)
    lines.extend(["", "## Strict matches removed from the worklist", "",
                  "These are matches for the source and flags shown, not new ROM coverage.",
                  "Maintainers must check whether they are already locked and run the splice/image/ROM",
                  "gates before promotion. Source directories are retained.", ""])
    if matches:
        table(matches)
    else:
        lines.append("None.")
    return "\n".join(lines) + "\n"


def rank(index):
    original = index.read_text(encoding="utf-8")
    entries = read_entries(original)
    for entry in entries:
        if not (index.parent / entry.name / "base.c").is_file():
            raise ValueError(f"{entry.name}: missing base.c")
    # Validate shared inputs once up front; each score command also verifies them.
    manifest = (score.ASM_DIR / "SHA256SUMS").read_bytes()
    score.targets()
    score.image_symbols()
    score.ido("cc")
    results = []
    for i, entry in enumerate(entries, 1):
        result = evaluate(entry, index.parent)
        results.append(result)
        print(f"[{i}/{len(entries)}] {entry.name}: {result.summary}", flush=True)
    if index.read_text(encoding="utf-8") != original:
        raise RuntimeError("INDEX changed during ranking; refusing to overwrite it")
    if (score.ASM_DIR / "SHA256SUMS").read_bytes() != manifest:
        raise RuntimeError("target manifest changed during ranking; rerun against one revision")
    with tempfile.NamedTemporaryFile(mode="w", encoding="utf-8", dir=index.parent,
                                     prefix=".rank-", delete=False) as stream:
        temporary = Path(stream.name)
        try:
            stream.write(render(results))
            stream.flush()
            temporary.chmod(index.stat().st_mode)
            os.replace(temporary, index)
        finally:
            temporary.unlink(missing_ok=True)
    print(f"Updated {index}; strict matches reported separately: {sum(r.matched for r in results)}")
    return results


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--index", type=Path, default=INDEX)
    args = parser.parse_args()
    try:
        rank(args.index.resolve())
    except (OSError, ValueError, KeyError, RuntimeError) as exc:
        print(f"ranking aborted: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
