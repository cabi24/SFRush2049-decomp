#!/usr/bin/env python3
"""PreToolUse guard for direct Edit/Write operations on protected repo files."""
import json
import os
from pathlib import Path
import sys

REPO = Path(__file__).resolve().parents[2]


def protected(path):
    try:
        relative = path.relative_to(REPO).as_posix()
    except ValueError:
        return False
    return (relative == "asm/us/blob" or relative.startswith("asm/us/blob/")
            or relative.endswith(".lock.json")
            or relative in ("us.sha1", "src/blob/blob.ld", "tools/cloud/score.py"))


def main():
    try:
        event = json.load(sys.stdin)
        if event["tool_name"] not in ("Edit", "Write"):
            return 0
        name = event["tool_input"]["file_path"]
        if not isinstance(name, str) or not name:
            raise ValueError("missing file_path")
        path = Path(name).expanduser()
        if not path.is_absolute():
            path = Path(event.get("cwd", os.getcwd())) / path
        # Cover both replacing a protected symlink and editing through an alias.
        paths = (Path(os.path.abspath(path)), path.resolve())
        if any(protected(candidate) for candidate in paths):
            print(f"Edit/Write denied for protected path: {name}. "
                  "Use the documented maintainer generation/verification workflow.",
                  file=sys.stderr)
            return 2
    except (OSError, ValueError, KeyError, TypeError, RuntimeError) as exc:
        print(f"Protected-file hook cannot validate this request: {exc}", file=sys.stderr)
        return 2
    # No allow override: leave normal permissions and other hooks in effect.
    return 0


if __name__ == "__main__":
    sys.exit(main())
