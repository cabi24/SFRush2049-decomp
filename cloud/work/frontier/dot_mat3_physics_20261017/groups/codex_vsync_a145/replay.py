#!/usr/bin/env python3
"""Replay the complete seven-member stock-IDO group and bounded controls."""
import argparse
import contextlib
import hashlib
import io
import json
from pathlib import Path
import re
import shutil
import struct
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[5]
sys.path.insert(0, str(ROOT))
from tools.cloud import owndata, score

NAME = "func_800DE45C"
OLD = ["speed_set", "vsync_wait", "speed_mode0_wrapper", "speed_mode1_wrapper",
       "continue_prompt", "func_800D6530"]
FLAGS = "-g0 -O3 -mips2 -G 0 -non_shared"
SOURCE = "func_800DE45C.c"


def replace_once(source, old, new):
    if source.count(old) != 1:
        raise RuntimeError("control anchor is missing or ambiguous")
    return source.replace(old, new, 1)


def result(obj, name):
    with contextlib.redirect_stdout(io.StringIO()):
        r = score.compare(obj, name, show=0)
    return dict(differing_words=r.differing, target_words=r.total,
                extra_nonzero_words=r.extra_words, unresolved=r.unresolved,
                unverified=r.unverified, errors=r.errors,
                strict_match=r.accepted())


def group_result(obj):
    return {name: result(obj, name) for name in OLD + [NAME]}


def object_layout(obj):
    elf = owndata._Object(obj)
    names = score.image_symbols()
    objects = {}
    for i, section in enumerate(elf.sections):
        if section["type"] != owndata.SHT_SYMTAB:
            continue
        for sym in elf.symbols(i):
            if sym["name"] not in ("input_rec0", "D_80146180"):
                continue
            sec = elf.sections[sym["section"]]
            objects[sym["name"]] = dict(object_section=sec["name"],
                object_offset=sym["value"], size=sym["size"],
                section_type=sec["type"], image_address=hex(names[sym["name"]]))
    funcs = sorted(name for name, _, _ in elf.functions())
    words = score.text_words(obj)
    start = score.symbols(obj)[NAME] // 4
    frames = [65536-(w&65535) for w in words[start:start+100] if w>>16==0x27BD and w&0x8000]
    if set(objects) != {"input_rec0", "D_80146180"}:
        raise RuntimeError("expected named data definitions absent")
    if any(v["section_type"] != owndata.SHT_NOBITS for v in objects.values()):
        raise RuntimeError("a named BSS definition unexpectedly has image bytes")
    if funcs != sorted(OLD+[NAME]):
        raise RuntimeError("unexpected emitted helper or missing group member")
    return dict(named_objects=objects, emitted_functions=funcs,
                root_frame_bytes=frames[0] if frames else None)


def inputs():
    spec = json.loads((HERE / "group.json").read_text())
    source = "\n".join((HERE/name).read_text() for name in spec["files"])
    symbols = score.image_symbols()
    tokens = set(re.findall(r"\b[A-Za-z_]\w*\b", re.sub(r"/\*.*?\*/|//[^\n]*", "", source, flags=re.S)))
    refs = {name: symbols.get(name, score.address_named(name)) for name in sorted(tokens)
            if name in symbols or score.address_named(name) is not None}
    targets = score.targets()
    return dict(protocol_version=2, compiler="ido5.3", population="game_blob",
                recipe=spec, files={name: hashlib.sha256((HERE/name).read_bytes()).hexdigest()
                for name in spec["files"] + ["group.json", "replay.py", "semantics.c"]},
                targets={name: dict(address=hex(symbols[name]), size=len(targets[name])*4,
                    words_sha256=hashlib.sha256(b"".join(struct.pack(">I",w)
                    for w in targets[name])).hexdigest()) for name in OLD+[NAME]},
                references=refs)


def verify():
    source = (HERE / SOURCE).read_text()
    spec = json.loads((HERE / "group.json").read_text())
    if spec["members"] != OLD+[NAME] or spec["claims"] != OLD+[NAME] or spec["context"]:
        raise RuntimeError("complete group members/claims changed")
    controls = {
        "wrong_option_count": replace_once(source, "i < 41", "i < 40"),
        "extern_player_records": replace_once(source, "Player input_rec0[4];", "extern Player input_rec0[4];"),
        "extern_tail_flags": replace_once(source, "s8 D_80146180[4];", "extern s8 D_80146180[4];"),
        "inline_count_stores": replace_once(replace_once(source,
            "static void reset_player_counts(void) {\n    D_80151AD0 = 1;\n    D_8014A108 = 1;\n}\n\n", ""),
            "    reset_player_counts();", "    D_80151AD0 = 1;\n    D_8014A108 = 1;"),
        "shared_initial_counter": replace_once(source,
            "        int j;\n        for (j = 0; j < 4; j++) {\n            input_rec0[j].unk1 = 5;\n            input_rec0[j].unk42 = 0;\n            input_rec0[j].unk44 = -1;\n            input_rec0[j].node = j;",
            "        for (i = 0; i < 4; i++) {\n            input_rec0[i].unk1 = 5;\n            input_rec0[i].unk42 = 0;\n            input_rec0[i].unk44 = -1;\n            input_rec0[i].node = i;"),
        "joined_tail_loop_line": replace_once(source,
            "    for (i = 0; i < 4; i++) {\n        D_80146180[i] = 0;\n    }",
            "    for (i = 0; i < 4; i++) { D_80146180[i] = 0; }"),
    }
    report = dict(inputs=inputs(), flags=FLAGS, as1_extra_flag=score.R4300_AS1,
        positive={}, negative_controls={}, claims=OLD+[NAME],
        preserved_members=OLD, new_individually_strict_bytes=1020,
        accepted_image_bytes=0, whole_unit_gate="not run", image_gate="not run", rom_gate="not run")
    with tempfile.TemporaryDirectory(prefix="rush-settings-proof-") as tmp:
        d=Path(tmp)
        for name in spec["files"]+["group.json"]:
            shutil.copyfile(HERE/name,d/name)
        for label in ("group_first","group_repeat"):
            obj=d/(label+".o")
            score.compile_group(d,obj)
            report["positive"][label]=group_result(obj)
            if label=="group_first": report["object_layout"]=object_layout(obj)
        for label,text in controls.items():
            (d/SOURCE).write_text(text)
            obj=d/(label+".o")
            score.compile_group(d,obj)
            report["negative_controls"][label]=group_result(obj)
    if any(not r["strict_match"] for group in report["positive"].values() for r in group.values()):
        raise RuntimeError("positive group replay failed")
    if report["object_layout"]["root_frame_bytes"] != 56:
        raise RuntimeError("native frame changed")
    if any(group[NAME]["strict_match"] for group in report["negative_controls"].values()):
        raise RuntimeError("negative control was incorrectly accepted")
    if any(not group[name]["strict_match"] for group in report["negative_controls"].values() for name in OLD):
        raise RuntimeError("a bounded root control disturbed a preserved member")
    return report


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check",action="store_true",help="check receipt inputs and fresh output")
    args=parser.parse_args()
    if args.check:
        saved=json.loads((HERE/"proof.json").read_text())
        if inputs()!=saved["inputs"]: raise RuntimeError("saved inputs changed")
    report=verify()
    if args.check and report!=saved: raise RuntimeError("fresh receipt changed")
    print(json.dumps(report,indent=2,sort_keys=True))


if __name__=="__main__": main()
