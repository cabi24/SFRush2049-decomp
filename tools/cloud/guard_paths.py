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
    entries = [("src/blob/", entry) for entry in document("blob_matched.lock.json").values()]
    # Runtime images A/B (pipeline.ovl_splice), once the base branch has their lock:
    # {"a": {name: entry}, "b": {...}}, sources under src/ovl_<x>/.
    if git(repo, "ls-tree", "--name-only", revision, "--", OVL_LOCK).strip():
        for image, image_entries in document(OVL_LOCK).items():
            entries += [(f"src/ovl_{image}/", entry) for entry in image_entries.values()]
    for root, entry in entries:
        source = entry["source"]
        if not source.startswith(root):
            continue
        protected.add(source)
        if entry.get("group"):
            manifests.add((root, source))
    for root, source in sorted(manifests):
        directory = str(PurePosixPath(source).parent)
        for name in document(source)["files"]:
            path = posixpath.normpath(posixpath.join(directory, name))
            if not path.startswith(root):
                raise ValueError(f"locked group source outside {root.rstrip('/')}: {path}")
            protected.add(path)
    return protected


OVL_LOCK = "ovl_matched.lock.json"


# Generated scoring targets: the game image, the two runtime images at
# 0x8038A400, and the boot-segment tail (tools.conveyor.pipeline.ovl_targets).
PROTECTED_TARGET_DIRS = ("asm/us/blob/", "asm/us/ovl_a/", "asm/us/ovl_b/", "asm/us/boot_tail/")


def violations(repo, paths, lock_revision):
    locked = locked_paths(repo, lock_revision)
    return sorted(path for path in paths if (
        path.startswith(PROTECTED_TARGET_DIRS)
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
