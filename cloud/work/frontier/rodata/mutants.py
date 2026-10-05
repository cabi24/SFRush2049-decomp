#!/usr/bin/env python3
"""The four IDO mutants of tests/conveyor/test_cloud_score.py
(test_wrong_sound_handles_clear_fails), for a builder without pytest.

    IDO_DIR=... python3 cloud/work/frontier/rodata/mutants.py

Uses whatever tools/cloud/score.py is in this tree (the patched copy in the
scratch tree). Every mutant must be refused even with --allow-unverified.
"""
import json
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))
from tools.cloud import score  # noqa: E402

LOCK = json.loads((REPO / "blob_matched.lock.json").read_text())


def main():
    original = (REPO / "src/blob/sound_handles_clear.c").read_text()
    failed = 0
    for mutant in ("none", "addend", "global", "callee", "known_global"):
        source = original
        if mutant == "addend":
            source = source.replace("entry = (s8 *) &D_80116DE4;",
                                    "entry = (s8 *) &D_80116DE4 + 0x40;")
        elif mutant in ("global", "known_global"):
            name = "D_WRONG" if mutant == "global" else "D_80116FE4"
            if mutant == "global":
                source = source.replace("void sound_handles_clear(s32 arg0)",
                                        f"extern s8 {name};\nvoid sound_handles_clear(s32 arg0)")
            source = source.replace("entry = (s8 *) &D_80116DE4;", f"entry = (s8 *) &{name};")
        elif mutant == "callee":
            source = source.replace(
                "void sound_handles_clear(s32 arg0)",
                "extern void some_other_fn(s32);\nvoid sound_handles_clear(s32 arg0)")
            source = source.replace("sound_stop(h);", "some_other_fn(h);")
        assert (source == original) == (mutant == "none")
        with tempfile.TemporaryDirectory() as tmp:
            src, obj = Path(tmp) / "mutant.c", Path(tmp) / "out.o"
            src.write_text(source)
            score.compile_single(src, LOCK["sound_handles_clear"]["flagset"], obj)
            result = score.compare(obj, "sound_handles_clear", show=0)
        accepted = result.accepted(True)
        ok = accepted == (mutant == "none")
        failed += not ok
        print(f"{mutant:13s} accepted={accepted!s:5s} {'ok' if ok else 'WRONG'}  "
              f"{result.summary()[:110]}")
    print("patched scorer" if hasattr(score, "own_data") else "UNPATCHED scorer")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
