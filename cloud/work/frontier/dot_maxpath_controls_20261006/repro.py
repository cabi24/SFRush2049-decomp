#!/usr/bin/env python3
"""Compile the genuine archived caller group with the selected donor adaptation.

Run from the repository root. Native input and temporary objects remain in build/.
This is a compiler/score reproduction, not an acceptance or behavior test.
"""
from pathlib import Path
import json
import sys

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor.pipeline.blob_unit import scan_defs

NAME = "func_800E4B58"
ARCHIVE = ROOT / "cloud/work/ipa-groups/codex_path_search_a7/group.c"
BUILD = ROOT / "build/dot_maxpath_controls_20261006"
FLAGS = "-g0 -O3 -mips2 -G 0 -non_shared"


def definition(text, name):
    rows = [row for row in scan_defs(text) if row["name"] == name]
    if len(rows) != 1:
        raise SystemExit("Expected one real definition of " + name)
    row = rows[0]
    return row, text[row["head"]:row["close"] + 1]


def report(obj, label):
    result = score.compare(obj, NAME, show=0)
    data, sections = score._elf(obj)
    symbol = next(symbol for i, section in enumerate(sections)
                  if section["type"] == 2
                  for symbol in score._symbol_table(data, sections, i)
                  if symbol["name"] == NAME)
    start = symbol["value"] // 4
    words = score.text_words(obj)[start:start + symbol["size"] // 4]
    frame = next(65536 - (word & 65535) for word in words[:40]
                 if word >> 16 == 0x27BD)
    print("%s: %d/%d words differ; emitted=%d words; frame=%d; "
          "extra_nonzero=%d; unresolved=%d; unverified=%d; errors=%d" %
          (label, result.differing, result.total, symbol["size"] // 4,
           frame, result.extra_words, len(result.unresolved),
           len(result.unverified), len(result.errors)))
    return result


def main():
    BUILD.mkdir(parents=True, exist_ok=True)
    original = ARCHIVE.read_text()
    row, _ = definition(original, NAME)
    candidate = HERE.joinpath("candidate.c").read_text()
    group = BUILD / "group"
    group.mkdir(exist_ok=True)
    spec = dict(files=["group.c"], members=[NAME],
                context=["func_800E398C", "func_800E451C", "func_800E4300"],
                keep=["func_800E4B58"], claims=[], flags=FLAGS)
    (group / "group.c").write_text(original)
    (group / "group.json").write_text(json.dumps(spec, indent=2) + "\n")
    score.compile_group(group, BUILD / "baseline.o")
    report(BUILD / "baseline.o", "archived baseline")

    (group / "group.c").write_text(
        original[:row["head"]] + "\n" + candidate + original[row["close"] + 1:])
    (group / "group.json").write_text(json.dumps(spec, indent=2) + "\n")
    score.compile_group(group, BUILD / "candidate.o")
    report(BUILD / "candidate.o", "donor candidate NONMATCH")


if __name__ == "__main__":
    main()
