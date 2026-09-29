#!/usr/bin/env python3
"""Reject PR changes to ground truth and locked blob sources.

Maintainers regenerate protected files on the coordinator and commit directly
to master. There is deliberately no PR exception for symbols.json.
"""
import argparse
import json
from pathlib import PurePosixPath
import posixpath
import subprocess
import sys

from .check_submissions import REPO, changed_paths, commit, git


def locked_paths(repo, revision):
    """Read protection metadata from the base branch, never the proposed lock."""
    revision = commit(repo, revision)

    def document(path):
        return json.loads(git(repo, "show", revision + ":" + path))

    protected = set()
    manifests = set()
    for entry in document("blob_matched.lock.json").values():
        source = entry["source"]
        if not source.startswith("src/blob/"):
            continue
        protected.add(source)
        if entry.get("group"):
            manifests.add(source)
    for source in sorted(manifests):
        directory = str(PurePosixPath(source).parent)
        for name in document(source)["files"]:
            path = posixpath.normpath(posixpath.join(directory, name))
            if not path.startswith("src/blob/"):
                raise ValueError(f"locked group source outside src/blob: {path}")
            protected.add(path)
    return protected


def violations(repo, paths, lock_revision):
    locked = locked_paths(repo, lock_revision)
    return sorted(path for path in paths if (
        path.startswith("asm/us/blob/")
        or path in ("src/blob/blob.ld", "us.sha1")
        or path.endswith(".lock.json")
        or path in locked))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", required=True, help="PR merge base")
    parser.add_argument("--head", default="HEAD")
    parser.add_argument("--lock-revision", help="base-branch commit; defaults to --base")
    args = parser.parse_args()
    try:
        blocked = violations(REPO, changed_paths(REPO, args.base, args.head),
                             args.lock_revision or args.base)
    except (OSError, ValueError, KeyError, TypeError, subprocess.CalledProcessError) as exc:
        print(f"protected-path check failed: {exc}", file=sys.stderr)
        return 1
    if blocked:
        print("PR changes to protected files are not allowed:", file=sys.stderr)
        for path in blocked:
            print("  " + path, file=sys.stderr)
        print("Maintainers must regenerate/verify these files on the coordinator "
              "and commit directly to master.", file=sys.stderr)
        return 1
    print("Protected paths unchanged")
    return 0


if __name__ == "__main__":
    sys.exit(main())
