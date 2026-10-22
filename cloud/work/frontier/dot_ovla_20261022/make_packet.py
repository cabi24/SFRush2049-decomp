#!/usr/bin/env python3
"""Build an unclaimed research packet without editing the installed group."""
import argparse
from pathlib import Path
import shutil


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", type=Path, help="new scratch packet directory")
    args = parser.parse_args()
    lane = Path(__file__).resolve().parent
    repo = lane.parents[3]
    group = repo / "src/ovl_a/groups/runtime_a_profile_menu_20261006"
    source = (group / "group.c").read_text()
    start_marker = "void func_803A9088(s32 player)\n{"
    end_marker = "void func_803A7B58(s32);\nvoid func_803A87D4(s32);"
    if source.count(start_marker) != 1 or source.count(end_marker) != 1:
        raise SystemExit("installed source changed; review the splice boundaries")
    start = source.index(start_marker)
    end = source.index(end_marker, start)
    body = (lane / "A9088_body.c").read_text()
    args.output.mkdir(parents=True, exist_ok=False)
    (args.output / "group.c").write_text(source[:start] + body + source[end:])
    # Inherit the exact manifest: A9088 remains NONMATCH context, not a member.
    shutil.copyfile(group / "group.json", args.output / "group.json")
    print(args.output)


if __name__ == "__main__":
    main()
