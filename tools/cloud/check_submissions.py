#!/usr/bin/env python3
"""Strictly rescore cloud submissions changed between two Git revisions.

Run from a checkout of --head. PR callers pass the merge base as --base;
push callers pass the previous commit. An all-zero base checks every file.
No ROM, coordinator state, or third-party Python packages are needed.
"""
import argparse
from pathlib import Path, PurePosixPath
import re
import subprocess
import sys

REPO = Path(__file__).resolve().parents[2]
FLAGS = re.compile(r"/\* flags: (.+?) \*/")


def git(repo, *args):
    return subprocess.run(["git", *args], cwd=repo, check=True,
                          stdout=subprocess.PIPE).stdout


def commit(repo, revision):
    return git(repo, "rev-parse", "--verify", "--end-of-options",
               revision + "^{commit}").decode().strip()


def changed_paths(repo, base, head):
    head = commit(repo, head)
    if head != commit(repo, "HEAD"):
        raise ValueError("checkout must be at --head before scoring")
    if base and set(base) == {"0"}:
        raw = git(repo, "ls-tree", "-r", "--name-only", "-z", head)
    else:
        base = commit(repo, base)
        # Disabling rename detection checks both sides of moves, including
        # a moved submission whose bytes did not change.
        raw = git(repo, "diff", "--name-only", "--no-renames", "-z", base, head, "--")
    return [p.decode("utf-8") for p in raw.split(b"\0") if p]


def commands(repo, paths):
    singles, groups = set(), set()
    for name in paths:
        parts = PurePosixPath(name).parts
        if len(parts) == 3 and parts[:2] == ("cloud", "matches") and name.endswith(".c"):
            singles.add(name)
        if len(parts) >= 5 and parts[:3] == ("cloud", "work", "ipa-groups"):
            groups.add("/".join(parts[:4]))
    scorer = str(repo / "tools/cloud/score.py")
    for name in sorted(singles):
        source = repo / name
        if not source.exists():  # A deleted submission has nothing to score.
            continue
        if not re.fullmatch(r"[A-Za-z_]\w*", source.stem, flags=re.ASCII):
            raise ValueError(f"{name}: filename must name the target function")
        with source.open(encoding="utf-8") as stream:
            header = FLAGS.fullmatch(stream.readline().rstrip("\r\n"))
        if not header or not header[1].strip():
            raise ValueError(f"{name}: line 1 must be /* flags: <IDO flags> */")
        yield [sys.executable, scorer, "fn", str(source), source.stem,
               "--flags=" + header[1]]
    for name in sorted(groups):
        directory = repo / name
        if not directory.exists():
            continue
        if not (directory / "group.json").is_file():
            raise ValueError(f"{name}: changed group is missing group.json")
        yield [sys.executable, scorer, "group", str(directory)]


def check(repo, paths):
    # Validate every submission header before starting any compiler work.
    jobs = list(commands(repo, paths))
    failed = False
    for job in jobs:
        print("Scoring " + job[3], flush=True)
        failed |= subprocess.run(job, cwd=repo).returncode != 0
    print(f"Checked {len(jobs)} changed cloud submissions", flush=True)
    return 1 if failed else 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", required=True)
    parser.add_argument("--head", default="HEAD")
    args = parser.parse_args()
    try:
        return check(REPO, changed_paths(REPO, args.base, args.head))
    except (OSError, ValueError, subprocess.CalledProcessError) as exc:
        print(f"submission check failed: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
