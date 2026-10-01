#!/usr/bin/env python3
"""Vendor (or update) the n64-decomp-workbench into third_party/.

    python3 tools/vendor_workbench.py sync [--ref REF]     # clone REF (default: main), copy the subset, pin it
    python3 tools/vendor_workbench.py status               # pinned commit vs upstream HEAD

The workbench is CC0-1.0, so it can live in this repository: cloud agents see only GitHub and need the tool and
its field guide there. We keep the Python package, the markdown docs (without images and history), the small
fixtures, README and LICENSE; tests, the research archive and release machinery stay upstream.
`third_party/n64-decomp-workbench/UPSTREAM.md` records the pinned commit. Run it from anywhere; stdlib only.
"""
import argparse
import datetime
import json
import shutil
import subprocess
import sys
import tempfile
import urllib.error
import urllib.request
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
DEST = REPO / "third_party" / "n64-decomp-workbench"
UPSTREAM = "akratch/n64-decomp-workbench"
URL = f"https://github.com/{UPSTREAM}.git"

# (source, destination) relative to the upstream root; directories are copied whole.
KEEP_FILES = ["LICENSE.md", "README.md", "pyproject.toml", "MANIFEST.in"]
KEEP_DIRS = {
    "src/decomp_workbench": "src/decomp_workbench",
    "docs": "docs",
    "examples/fixtures": "examples/fixtures",
}
SKIP_NAMES = {"__pycache__", "assets", "history", ".git", "n64_decomp_workbench.egg-info"}


def _ignore(directory, names):
    return [n for n in names if n in SKIP_NAMES or n.endswith(".pyc")]


def git(*args, cwd=None):
    return subprocess.run(["git", *args], cwd=cwd, check=True, capture_output=True,
                          text=True).stdout.strip()


def pinned_commit(dest=DEST):
    info = Path(dest) / "UPSTREAM.md"
    if not info.exists():
        return None
    for line in info.read_text().splitlines():
        if line.startswith("- commit:"):
            return line.split(":", 1)[1].strip()
    return None


def sync(ref="main", dest=DEST):
    dest = Path(dest)
    with tempfile.TemporaryDirectory(prefix="wbvendor-") as tmp:
        clone = Path(tmp) / "wb"
        git("clone", "-q", URL, str(clone))
        git("checkout", "-q", ref, cwd=clone)
        commit = git("rev-parse", "HEAD", cwd=clone)
        date = git("log", "-1", "--format=%cI", cwd=clone)
        subject = git("log", "-1", "--format=%s", cwd=clone)
        if dest.exists():
            shutil.rmtree(dest)
        dest.mkdir(parents=True)
        for name in KEEP_FILES:
            shutil.copy2(clone / name, dest / name)
        for src, dst in KEEP_DIRS.items():
            shutil.copytree(clone / src, dest / dst, ignore=_ignore)
        (dest / "UPSTREAM.md").write_text(
            "# Vendored copy of n64-decomp-workbench\n\n"
            f"- source: https://github.com/{UPSTREAM}\n"
            f"- commit: {commit}\n"
            f"- commit date: {date}\n"
            f"- subject: {subject}\n"
            f"- synced: {datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')}\n"
            "- licence: CC0-1.0 (LICENSE.md)\n\n"
            "Kept: the `decomp_workbench` package, markdown docs (no images or history), `examples/fixtures`,\n"
            "README, LICENSE, pyproject. Not kept: tests, research archive, case studies, release tooling.\n"
            "Update with `python3 tools/vendor_workbench.py sync` and review the diff. Do not edit these files\n"
            "by hand: local findings go in `cloud/PLAYBOOK.md` and `docs/`.\n\n"
            "Run without installing: `PYTHONPATH=third_party/n64-decomp-workbench/src python3 -m decomp_workbench ...`\n"
            "(Python 3.10+; real object comparison needs a GNU MIPS objdump).\n")
    return commit, date


def upstream_head():
    url = f"https://api.github.com/repos/{UPSTREAM}/commits/main"
    request = urllib.request.Request(url, headers={"User-Agent": "rush2049-vendor-workbench"})
    try:
        with urllib.request.urlopen(request, timeout=30) as response:
            data = json.loads(response.read())
        return data["sha"], data["commit"]["committer"]["date"]
    except (urllib.error.URLError, OSError, ValueError, KeyError):
        return None, None


def status_line(pinned, head):
    if pinned is None:
        return "not vendored yet"
    if head is None:
        return f"pinned {pinned[:10]}; upstream unreachable"
    if head == pinned:
        return f"pinned {pinned[:10]} is current"
    return f"pinned {pinned[:10]}; upstream is at {head[:10]} (run `tools/vendor_workbench.py sync` and review)"


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = parser.add_subparsers(dest="cmd", required=True)
    s = sub.add_parser("sync")
    s.add_argument("--ref", default="main")
    sub.add_parser("status")
    args = parser.parse_args(argv)
    if args.cmd == "sync":
        commit, date = sync(args.ref)
        print(f"vendored {UPSTREAM} {commit[:10]} ({date}) -> {DEST.relative_to(REPO)}")
        return 0
    head, _ = upstream_head()
    print(status_line(pinned_commit(), head))
    return 0


if __name__ == "__main__":
    sys.exit(main())
