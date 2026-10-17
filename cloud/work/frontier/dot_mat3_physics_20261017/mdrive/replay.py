#!/usr/bin/env python3
"""Replay the stock-IDO match and bounded negative controls, without installation."""
import argparse
import contextlib
import hashlib
import io
import json
import re
from pathlib import Path
import struct
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

NAME = "func_800E543C"
FLAGS = "-g0 -O3 -mips2 -G 0 -non_shared"


def replace_once(source, old, new):
    if source.count(old) != 1:
        raise RuntimeError("control anchor is missing or ambiguous")
    return source.replace(old, new, 1)


def result(obj):
    with contextlib.redirect_stdout(io.StringIO()):
        r = score.compare(obj, NAME, show=0)
    words = score.text_words(obj)
    start = score.symbols(obj)[NAME] // 4
    frames = [65536 - (w & 65535) for w in words[start:start + 15]
              if w >> 16 == 0x27BD]
    return dict(differing_words=r.differing, target_words=r.total,
                extra_nonzero_words=r.extra_words, unresolved=r.unresolved,
                unverified=r.unverified, errors=r.errors,
                strict_match=r.accepted(), frame_bytes=frames[0] if frames else None)


def inputs():
    source = (HERE / "group.c").read_text()
    spec = json.loads((HERE / "group.json").read_text())
    symbols = score.image_symbols()
    tokens = set(re.findall(r"\b[A-Za-z_]\w*\b", re.sub(r"/\*.*?\*/|//[^\n]*", "", source, flags=re.S)))
    references = {name: symbols.get(name, score.address_named(name))
                  for name in sorted(tokens)
                  if name in symbols or score.address_named(name) is not None}
    words = score.targets()[NAME]
    return dict(protocol_version=2, compiler="ido5.3", population="game_blob",
                recipe=spec, files={name: hashlib.sha256((HERE / name).read_bytes()).hexdigest()
                for name in ("group.c", "group.json", "replay.py", "semantics.c")},
                targets={NAME: dict(address=hex(symbols[NAME]), size=len(words) * 4,
                words_sha256=hashlib.sha256(b"".join(struct.pack(">I", w) for w in words)).hexdigest())},
                references=references)


def verify():
    source = (HERE / "group.c").read_text()
    spec = json.loads((HERE / "group.json").read_text())
    if spec["claims"] != [NAME] or spec["members"] != [NAME]:
        raise RuntimeError("the complete claim set must contain only Init_MDrive")
    controls = {
        "wrong_velocity_threshold": replace_once(source, "vehicle->speed>=44.0f", "vehicle->speed>=45.0f"),
        "missing_live_config_pointer": replace_once(replace_once(replace_once(
            source, " Config8 *config;\n", ""),
            "  config=&D_80153E88[index];\n", ""),
            "if(config->mode==6)", "if(D_80153E88[index].mode==6)"),
        "swapped_clear_order": replace_once(source,
            "  vehicle->wheels[i]=0.0f;\n  vehicle->tires[i].a=0.0f;\n  vehicle->tires[i].b=0.0f;",
            "  vehicle->tires[i].b=0.0f;\n  vehicle->tires[i].a=0.0f;\n  vehicle->wheels[i]=0.0f;"),
        "joined_active_state_line": replace_once(source,
            "  model->active=1;\n  vehicle->state=0;", "  model->active=1;vehicle->state=0;"),
        "joined_mode_arm_lines": replace_once(source,
            "if(vehicle->speed>=44.0f)\n    vehicle->mode=2;\n   else\n    vehicle->mode=1;",
            "if(vehicle->speed>=44.0f)vehicle->mode=2;else vehicle->mode=1;"),
    }
    report = {
        "inputs": inputs(), "name": NAME, "target_bytes": 700,
        "flags": FLAGS, "as1_extra_flag": score.R4300_AS1,
        "positive": {}, "negative_controls": {}, "claims": [NAME],
        "individually_strict_bytes": 700, "accepted_image_bytes": 0,
        "image_gate": "not run", "rom_gate": "not run",
    }
    with tempfile.TemporaryDirectory(prefix="rush-mdrive-proof-") as tmp:
        d = Path(tmp)
        (d / "group.json").write_text(json.dumps(spec))
        (d / "group.c").write_text(source)
        for label in ("group_first", "group_repeat"):
            obj = d / (label + ".o")
            score.compile_group(d, obj)
            report["positive"][label] = result(obj)
        obj = d / "single.o"
        score.compile_single(d / "group.c", FLAGS, obj)
        report["positive"]["single"] = result(obj)
        for label, text in controls.items():
            (d / "group.c").write_text(text)
            obj = d / (label + ".o")
            score.compile_group(d, obj)
            report["negative_controls"][label] = result(obj)
    if any(not r["strict_match"] or r["target_words"] != 175 or r["frame_bytes"] != 72
           for r in report["positive"].values()):
        raise RuntimeError("positive replay failed")
    if any(r["strict_match"] for r in report["negative_controls"].values()):
        raise RuntimeError("negative control was incorrectly accepted")
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="validate stored inputs and replay the receipt")
    args = parser.parse_args()
    if args.check:
        saved = json.loads((HERE / "proof.json").read_text())
        if inputs() != saved["inputs"]:
            raise RuntimeError("packet inputs differ from the saved protocol-v2 receipt")
    report = verify()
    if args.check and report != saved:
        raise RuntimeError("fresh proof differs from the stored receipt")
    print(json.dumps(report, indent=2, sort_keys=True))


if __name__ == "__main__":
    main()
