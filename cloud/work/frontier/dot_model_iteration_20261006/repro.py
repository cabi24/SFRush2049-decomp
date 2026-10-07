#!/usr/bin/env python3
"""Compiler-only reproduction; all objects and temporary context stay in build/."""
from pathlib import Path
import json
import re
import sys

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor.pipeline.blob_unit import scan_defs

NAME = "func_800E6AF8"
BUILD = ROOT / "build/dot_model_iteration_20261006"
ARCHIVE = ROOT / "cloud/work/ipa-groups/func_800E56F8/group.c"


def replace(text, name, replacement):
    rows = [d for d in scan_defs(text) if d["name"] == name]
    if len(rows) != 1:
        raise SystemExit("Expected one definition: " + name)
    d = rows[0]
    return text[:d["head"]] + "\n" + replacement + text[d["close"] + 1:]


def report(obj, label):
    r = score.compare(obj, NAME, show=0)
    data, sections = score._elf(obj)
    symbol = next(s for i, section in enumerate(sections) if section["type"] == 2
                  for s in score._symbol_table(data, sections, i) if s["name"] == NAME)
    words = score.text_words(obj)[symbol["value"] // 4:][:40]
    frame = next(65536 - (w & 65535) for w in words if w >> 16 == 0x27BD)
    print("%s: %d/%d words differ; emitted=%d; frame=%d; extra=%d; "
          "unresolved=%d; unverified=%d; errors=%d" %
          (label, r.differing, r.total, symbol["size"] // 4, frame,
           r.extra_words, len(r.unresolved), len(r.unverified), len(r.errors)))


def main():
    BUILD.mkdir(parents=True, exist_ok=True)
    text = replace(ARCHIVE.read_text(), "__standin_func_800E56F8", "")
    text = re.sub(r"    volatile s32 pad[\w]*\[[0-9]+\];\n", "", text)
    group = BUILD / "group"
    group.mkdir(exist_ok=True)
    spec = dict(files=["group.c"], members=[NAME],
                context=["func_800E56F8", "func_800E4B58", "func_800E398C",
                         "func_800E451C", "func_800E4300"],
                keep=["func_800E6AF8", "func_800E4B58"], claims=[],
                flags="-g0 -O3 -mips2 -G 0 -non_shared")
    (group / "group.json").write_text(json.dumps(spec, indent=2) + "\n")
    (group / "group.c").write_text(text)
    score.compile_group(group, BUILD / "baseline.o")
    report(BUILD / "baseline.o", "real-caller baseline")
    text = text.replace("extern u16 D_801525F0;", "extern volatile u16 D_801525F0;")
    text = text.replace("extern s16 D_80153FD2;", "extern volatile s16 D_80153FD2;")
    text = replace(text, NAME, HERE.joinpath("candidate.c").read_text())
    (group / "group.c").write_text(text)
    score.compile_group(group, BUILD / "candidate.o")
    report(BUILD / "candidate.o", "candidate NONMATCH")


if __name__ == "__main__":
    main()
