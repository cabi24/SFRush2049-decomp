#!/usr/bin/env python3
"""Replay the stock-IDO match and bounded negative controls, without installation."""
import argparse
import contextlib
import hashlib
import io
import json
import re
import subprocess
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


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def layout(obj):
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    functions = [symbol for index, section in enumerate(sections) if section["type"] == 2
                 for symbol in score._symbol_table(data, sections, index)
                 if symbol["type"] == 2 and symbol["section"] == text_index]
    roots = [symbol for symbol in functions if symbol["name"] == NAME]
    require(len(roots) == 1, "missing or duplicate root function")
    root = roots[0]
    words = score.text_words(obj)
    frames = [65536 - (word & 65535)
              for word in words[root["value"] // 4:(root["value"] + root["size"]) // 4]
              if word >> 16 == 0x27BD and word & 0x8000]
    return {"functions": {symbol["name"]: symbol["size"] for symbol in functions},
            "root_frame_bytes": frames[0] if frames else None}


def result(obj):
    with contextlib.redirect_stdout(io.StringIO()):
        r = score.compare(obj, NAME, show=0)
    return dict(differing_words=r.differing, target_words=r.total,
                extra_nonzero_words=r.extra_words, unresolved=r.unresolved,
                unverified=r.unverified, errors=r.errors,
                strict_match=r.accepted(), frame_bytes=layout(obj)["root_frame_bytes"])


def inputs():
    source = (HERE / "group.c").read_text()
    spec = json.loads((HERE / "group.json").read_text())
    symbols = score.image_symbols()
    tokens = set(re.findall(r"\b[A-Za-z_]\w*\b", re.sub(r"/\*.*?\*/|//[^\n]*", "", source, flags=re.S)))
    references = {name: symbols.get(name, score.address_named(name))
                  for name in sorted(tokens)
                  if name in symbols or score.address_named(name) is not None}
    words = score.targets()[NAME]
    return dict(protocol_version=3, compiler="ido5.3", population="game_blob",
                recipe=spec, files={name: hashlib.sha256((HERE / name).read_bytes()).hexdigest()
                for name in ("group.c", "group.json", "replay.py", "semantics.c", "README.md")},
                targets={NAME: dict(address=hex(symbols[NAME]), size=len(words) * 4,
                words_sha256=hashlib.sha256(b"".join(struct.pack(">I", w) for w in words)).hexdigest())},
                references=references,
                compiler_executables={name: hashlib.sha256(Path(score.ido(name)).read_bytes()).hexdigest()
                                      for name in ("cc", "cfe", "uld", "usplit", "umerge", "uopt", "ugen", "as1")},
                verification_tools={name: hashlib.sha256((ROOT/name).read_bytes()).hexdigest()
                                    for name in ("tools/cloud/score.py", "tools/cloud/owndata.py", "blob_matched.lock.json")})


def verify_scope(spec):
    require(spec["claims"] == spec["members"] == spec["keep"] == [NAME]
            and spec["context"] == [] and spec["files"] == ["group.c"],
            "complete group scope changed")
    require(spec["flags"] == FLAGS and not spec.get("allow_unverified"), "compiler contract changed")


def verify_report(report):
    verify_scope(report["inputs"]["recipe"])
    require(report["claims"] == [NAME] and report["name"] == NAME, "claim identity changed")
    require(report["target_bytes"] == report["individually_strict_bytes"] == 700
            and report["accepted_image_bytes"] == 0, "credit changed")
    require(report["image_gate"] == report["rom_gate"] == "not run", "integration scope changed")
    require(report["flags"] == FLAGS and report["as1_extra_flag"] == score.R4300_AS1, "compiler flags changed")
    require(set(report["positive"]) == {"group_first", "group_repeat", "single"}, "positive inventory changed")
    expected_negatives = {"wrong_velocity_threshold": (1, 72), "missing_live_config_pointer": (10, 64),
                          "swapped_clear_order": (8, 72), "joined_active_state_line": (2, 72),
                          "joined_mode_arm_lines": (2, 72)}
    require(set(report["negative_controls"]) == set(expected_negatives), "negative inventory changed")
    require(set(report["layouts"]) == set(report["positive"]) | set(expected_negatives), "layout inventory changed")
    for category in ("positive", "negative_controls"):
        for label, row in report[category].items():
            positive = category == "positive"
            difference, frame = (0, 72) if positive else expected_negatives[label]
            require(row["strict_match"] is positive and row["differing_words"] == difference
                    and row["target_words"] == 175 and row["frame_bytes"] == frame,
                    "comparison result changed: " + label)
            require(not any(row[key] for key in ("extra_nonzero_words", "unresolved", "unverified", "errors")),
                    "comparison uncertainty changed: " + label)
            require(report["layouts"][label] == {"functions": {NAME: 700}, "root_frame_bytes": frame},
                    "function extent/inventory/frame changed: " + label)
    require(report["positive"]["group_first"] == report["positive"]["group_repeat"] == report["positive"]["single"],
            "positive comparisons disagree")
    require(report["host_semantics"] == "288 bounded host cases passed", "host semantics changed")


def check_saved(saved):
    require(saved["inputs"] == inputs(), "saved source, compiler, verifier or target inputs changed")
    verify_report(saved)


def semantic_check(directory):
    executable = directory / "semantics"
    subprocess.run(["cc", "-std=c11", "-O2", "-Wall", "-Wextra", "-Werror",
                    "-fsanitize=undefined", "-fno-sanitize-recover=undefined",
                    str(HERE/"semantics.c"), "-o", str(executable)], check=True, capture_output=True, text=True)
    checked = subprocess.run([str(executable)], capture_output=True, text=True)
    require(checked.returncode == 0 and checked.stdout == "288 bounded host cases passed\n" and not checked.stderr,
            "host semantic check failed: " + repr((checked.returncode, checked.stdout, checked.stderr)))
    return checked.stdout.strip()


def verify():
    source = (HERE / "group.c").read_text()
    spec = json.loads((HERE / "group.json").read_text())
    verify_scope(spec)
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
        "positive": {}, "negative_controls": {}, "layouts": {}, "claims": [NAME],
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
            report["layouts"][label] = layout(obj)
        obj = d / "single.o"
        score.compile_single(d / "group.c", FLAGS, obj)
        report["positive"]["single"] = result(obj)
        report["layouts"]["single"] = layout(obj)
        for label, text in controls.items():
            (d / "group.c").write_text(text)
            obj = d / (label + ".o")
            score.compile_group(d, obj)
            report["negative_controls"][label] = result(obj)
            report["layouts"][label] = layout(obj)
        report["host_semantics"] = semantic_check(d)
    verify_report(report)
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="validate stored inputs and replay the receipt")
    args = parser.parse_args()
    if args.check:
        saved = json.loads((HERE / "proof.json").read_text())
        check_saved(saved)
    report = verify()
    if args.check and report != saved:
        raise RuntimeError("fresh proof differs from the stored receipt")
    print(json.dumps(report, indent=2, sort_keys=True))


if __name__ == "__main__":
    main()
