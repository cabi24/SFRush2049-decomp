#!/usr/bin/env python3
"""Stage genuine, unchanged context and run the canonical group scorer.

Research only. Every unmodified baseline is a near miss, not a match claim.
No targets, locks, receipts, accepted source, or compiler code are changed.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[4]
FONT_BODIES = {
    "u8locals": "u8 a,b; a=object_type_byte2_get(); b=object_type_byte3_get(); return a+b;",
    "s32locals": "s32 a,b; a=object_type_byte2_get(); b=object_type_byte3_get(); return a+b;",
    "s16locals": "s16 a,b; a=object_type_byte2_get(); b=object_type_byte3_get(); return a+b;",
    "u8sum": "u8 a; a=object_type_byte2_get(); return (s16)(object_type_byte3_get()+a);",
    "empty_values": "u8 a,b; a=object_type_byte2_get(); b=object_type_byte3_get(); if(a) {} if(b) {} return a+b;",
    "compound": "s32 a; a=object_type_byte2_get(); a+=object_type_byte3_get(); return a;",
    "metrics_lifetime": "u8 a,b; a=object_type_byte2_get(); if(a){} b=object_type_byte3_get(); if(a){} if(b){} return a+b;",
    "get_both_cond": "u8 a,b; if((a=object_type_byte2_get())) {} if((b=object_type_byte3_get())) {} return a+b;",
}


def replace_once(text, old, new):
    if text.count(old) != 1:
        raise ValueError(f"Expected exactly one occurrence of {old!r}")
    return text.replace(old, new, 1)


def ghost_variant(text, name):
    if name == "reuse_g_intermediate":
        return replace_once(text, "g = *(*ghost)->header;", "g = (GhostHeader *)*ghost;\n            g = *((Ghost *)g)->header;")
    if name == "car_outer":
        text = replace_once(text, "s32 i;", "s32 i;\n    GhostPlayer *car;")
        text = replace_once(text, "g = *(*ghost)->header;", "car = &D_80152818[i];\n            g = *(*ghost)->header;")
        return text.replace("D_80152818[i].ready", "car->ready").replace("D_80152818[i].value", "car->value")
    if name == "loop_unsigned":
        return replace_once(text, "s32 i;", "u32 i;")
    if name == "stores_swap":
        return replace_once(text, "D_80152698[i] = 0;\n            D_8014A250[i].reset = 0;", "D_8014A250[i].reset = 0;\n            D_80152698[i] = 0;")
    raise ValueError(f"Unknown GhostEnd probe: {name}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("target", choices=("ghost", "font"))
    parser.add_argument("--variant", default="baseline", help="One documented root-only probe, not a sweep")
    parser.add_argument("--out", type=Path, required=True, help="New staging directory; refuses an existing path")
    parser.add_argument("--stage-only", action="store_true")
    args = parser.parse_args()
    out = args.out.resolve()
    source = ROOT / "src/blob/groups" / ("codex_heap_release_a25" if args.target == "ghost" else "slot_sound")
    spec = json.loads((source / "group.json").read_text())
    if args.target == "ghost":
        # InitSystem is unrelated to this root and contributes anonymous stubs.
        spec["files"].remove("initsystem.c")
        spec["members"].remove("game_late_init")
        spec["keep"].remove("game_late_init")
        roots = ["ghost_race_setup"]
        text = (ROOT / "cloud/work/frontier/w39b/ghost/base.c").read_text()
        if args.variant != "baseline":
            text = ghost_variant(text, args.variant)
        additions = {"ghost.c": text}
    else:
        roots = ["object_bytes23_sum", "object_bytes_sum_global", "func_800B3D0C"]
        text = (ROOT / "cloud/work/frontier/w38a/tb/d1.c").read_text()
        if args.variant != "baseline":
            if args.variant not in FONT_BODIES:
                parser.error(f"Unknown font probe: {args.variant}")
            text = replace_once(text, "return object_type_byte2_get() + object_type_byte3_get();", FONT_BODIES[args.variant])
        additions = {
            "height.c": text,
            "line.c": (ROOT / "cloud/work/frontier/w38a/tb/c1.c").read_text(),
            "leading.c": (ROOT / "src/blob/func_800B3D0C.c").read_text(),
        }
    out.mkdir(parents=True, exist_ok=False)
    for name in spec["files"]:
        shutil.copyfile(source / name, out / name)
        assert (source / name).read_bytes() == (out / name).read_bytes()
    for name, text in additions.items():
        (out / name).write_text(text)
    # Root-first order prevents unrelated trailing anonymous stubs from being
    # counted as the root's suffix. The scorer still checks the complete extent.
    spec["files"] = list(additions) + spec["files"]
    spec["members"] += roots
    spec["keep"] += roots
    spec["claims"] = []
    spec["note"] = "Research replay; unchanged accepted context; no match claims."
    spec["provenance"] = "dot_ghost_font_20261022; base dd4cf01d3"
    (out / "group.json").write_text(json.dumps(spec, indent=2) + "\n")
    print(f"Staged {args.target}/{args.variant}: {out}", flush=True)
    if not args.stage_only:
        return subprocess.call([sys.executable, str(ROOT / "tools/cloud/score.py"), "group", str(out)], cwd=ROOT)
    return 0


if __name__ == "__main__":
    sys.exit(main())
