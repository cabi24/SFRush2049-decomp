#!/usr/bin/env python3
"""Reproduce bounded source-boundary experiments; emit metadata, never native words.

The stock scorer/compiler and every production input remain untouched. Eight
fresh group compiles are optional. Generated sources and objects live only in a
temporary directory. There is deliberately no flag, keep, or spelling search.
"""
import argparse
import contextlib
import dataclasses
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
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

BASE = "f88dc3cb3807b8be3246b719a9b1e008210b477c"
FLAGS = "-g0 -O3 -mips2 -G 0 -non_shared"
WALKERS = ["model_transform_setup", "model_data_load"]
ACCESSORS = ["func_8008AE64", "func_8008AE48", "func_8008AE2C", "func_8008AE10"]
WALKER_INPUTS = {
    "model_data_load": "cloud/work/frontier/agentB/model_data_load/best.c",
    "model_transform_setup": "cloud/work/r5_b/model_transform_setup.c",
}
CLEANUP_INPUTS = {
    "shutdown": "cloud/work/ipa-groups/codex_shutdown_inline_a137",
    "wheel": "cloud/work/ipa-groups/codex_cleanup_wrappers_a148",
}
ALLOCATORS = ["func_8008E26C", "func_800A78BC", "func_80091B00",
              "func_80090284", "func_800B3704"]
CLEANUPS = ["func_800C885C", "wheel_params_set", "func_800C8918"]
TOOLS = ["cc", "cfe", "uld", "usplit", "umerge", "uopt", "ugen", "as1"]
PROTOTYPES = """
s16 func_8008AE10(s32);
s16 func_8008AE2C(s32);
void func_8008AE48(s32, s32);
s32 func_8008AE64(s16);
"""
CANONICAL_PREAMBLE = """typedef signed char s8; typedef unsigned char u8;
typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32;
typedef struct { u32 flags; u8 unknown4[18]; s16 child; s16 sibling; u8 unknown26[42]; } Ent;
extern Ent D_8012E700[];
"""
CANONICAL_BODIES = {
    "func_8008AE64": "s32 func_8008AE64(s16 index) { return D_8012E700[index].flags; }\n",
    "func_8008AE48": "void func_8008AE48(s32 index, s32 flags) { D_8012E700[index].flags = flags; }\n",
    "func_8008AE2C": "s16 func_8008AE2C(s32 index) { return D_8012E700[index].child; }\n",
    "func_8008AE10": "s16 func_8008AE10(s32 index) { return D_8012E700[index].sibling; }\n",
}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def source_inputs():
    paths = list(WALKER_INPUTS.values())
    paths += ["src/blob/" + name + ".c" for name in ACCESSORS]
    paths += [base + "/" + file for base in CLEANUP_INPUTS.values()
              for file in ("group.c", "group.json")]
    return {path: sha((ROOT / path).read_bytes()) for path in sorted(paths)}


def tool_sources():
    return {path: sha((ROOT / path).read_bytes()) for path in
            ("tools/cloud/score.py", "tools/cloud/owndata.py")}


def stable_native(record):
    """Bind native content, not harmless acceptance comments in region files.

    targets() independently authenticates the CURRENT complete manifest. The
    historical region hashes remain provenance but need not stay identical when
    upstream splices a different function and changes a source annotation.
    """
    return {key: value for key, value in record.items() if key != "target_manifest"}


def frame(words):
    return next((65536 - (word & 65535) for word in words[:30]
                 if word >> 16 == 0x27BD and word & 32768), 0)


def calls(words):
    return [0x80000000 | ((word & 0x3FFFFFF) << 2)
            for word in words if word >> 26 == 3]


def direct_transfers(words):
    """J/JAL only; indirect calls and address-taken references are not ruled out."""
    return [0x80000000 | ((word & 0x3FFFFFF) << 2)
            for word in words if word >> 26 in (2, 3)]


def stack_stores(words):
    return sorted({word & 65535 for word in words
                   if word >> 26 == 43 and (word >> 21) & 31 == 29})


def release_homes(words, destination):
    """Immediate pre-call lw a1,N(sp) witnesses; no broad dataflow inference."""
    out = []
    for i, word in enumerate(words):
        if word >> 26 != 3 or 0x80000000 | ((word & 0x3FFFFFF) << 2) != destination:
            continue
        previous = words[i - 1] if i else 0
        home = (previous & 65535) if (previous >> 26 == 35
                and (previous >> 21) & 31 == 29
                and (previous >> 16) & 31 == 5) else None
        out.append({"call_offset": i * 4, "immediate_address_home": home})
    return out


def native_summary():
    targets = score.targets()  # fail-closed protected manifest verification
    symbols = score.image_symbols()
    transfers = {name: set(direct_transfers(words)) for name, words in targets.items()}
    names = sorted(set(ALLOCATORS + CLEANUPS + WALKERS + ACCESSORS
                       + ["car_angular_velocity_clamp"]))
    records = {}
    for name in names:
        words = targets[name]
        records[name] = {
            "address": "0x%08X" % symbols[name],
            "native_bytes": len(words) * 4,
            "native_sha256": sha(struct.pack(">" + "I" * len(words), *words)),
            "frame_bytes": frame(words),
            "stack_store_offsets": stack_stores(words),
        }
        if name in ACCESSORS:
            records[name]["direct_j_or_jal_callers"] = sorted(
                caller for caller, destinations in transfers.items()
                if symbols[name] in destinations)
        if name in CLEANUPS or name == "car_angular_velocity_clamp":
            records[name]["release_homes"] = release_homes(
                words, symbols["audio_reverb_update"])
    # Conservative function-level candidate filter, not a CFG or alias proof.
    # The high half and matching address-form/load/store low half must both occur.
    addr = 0x801569A8
    high = ((addr + 0x8000) >> 16) & 65535
    low = addr & 65535
    direct_candidates = sorted(name for name, words in targets.items()
        if any(word >> 26 == 15 and word & 65535 == high for word in words)
        and any(word >> 26 in (9, 13, 32, 33, 35, 36, 37, 40, 41, 43, 49, 57)
                and word & 65535 == low for word in words))
    return {
        "function_count": len(targets),
        "word_count": sum(len(words) for words in targets.values()),
        "target_manifest": score.target_manifest(),
        "allocator_highwater_direct_candidates": direct_candidates,
        "functions": records,
    }


def accessor_calls(text, all_accessors=False):
    """Fixed operation-preserving substitution; caller locals/control flow stay fixed."""
    text = text.replace("extern Ent D_8012E700[];",
                        "extern Ent D_8012E700[];" + PROTOTYPES)
    text = re.sub(r"D_8012E700\[\(s16\)\s*a\]\.flags", "func_8008AE64(a)", text)
    text = re.sub(r"(?:D_8012E700\[a\]|E\(a\))\.flags = ([^;]+);",
                  r"func_8008AE48(a, \1);", text)
    if all_accessors:
        text = re.sub(r"(?:D_8012E700\[a\]|E\(a\))\.child", "func_8008AE2C(a)", text)
        text = re.sub(r"(?:D_8012E700\[a\]|E\(a\))\.sibling", "func_8008AE10(a)", text)
    return text


def annotate_free(text):
    declaration = "void audio_effect_process(u32 address) {"
    if text.count(declaration) != 1:
        raise ValueError("expected exactly one genuine free-wrapper definition")
    return text.replace(declaration, "__inline " + declaration)


def object_summary(obj, names, inspect=()):
    data, sections = score._elf(obj)
    functions = {symbol["name"]: symbol for i, section in enumerate(sections)
                 if section["type"] == 2
                 for symbol in score._symbol_table(data, sections, i)
                 if symbol["type"] == 2}
    words = score.text_words(obj)
    result = {}
    for name in names:
        if name not in functions:
            result[name] = {"absent": True}
            continue
        with contextlib.redirect_stdout(io.StringIO()):
            comparison = score.compare(obj, name, show=0)
        function = functions[name]
        start, size = function["value"], function["size"]
        body = words[start // 4:(start + size) // 4]
        result[name] = dict(dataclasses.asdict(comparison),
            strict_match=comparison.accepted(), elf_function_bytes=size,
            frame_bytes=frame(body))
        if name in inspect:
            resolved, _, _, _, _ = score.relocate(
                obj, words, start, start + size, score.image_symbols())
            resolved_body = resolved[start // 4:(start + size) // 4]
            if name in WALKERS:
                result[name]["out_of_line_accessor_calls"] = {
                    accessor: calls(resolved_body).count(score.image_symbols()[accessor])
                    for accessor in ACCESSORS}
            else:
                result[name]["stack_store_offsets"] = stack_stores(body)
                result[name]["out_of_line_release_wrapper_calls"] = calls(
                    resolved_body).count(score.image_symbols()["audio_effect_process"])
                result[name]["release_homes"] = release_homes(
                    resolved_body, score.image_symbols()["audio_reverb_update"])
    return result


def compile_record(directory, spec, inspect=()):
    (directory / "group.json").write_text(json.dumps(spec, indent=2) + "\n")
    obj = directory / "probe.o"
    score.compile_group(directory, obj)
    names = spec["members"] + spec.get("context", [])
    return {
        "functions": object_summary(obj, names, inspect),
        "source_sha256": {name: sha((directory / name).read_bytes()) for name in spec["files"]},
        "flags": spec["flags"], "keep": spec["keep"], "claims": spec.get("claims", []),
        "object_sha256": sha(obj.read_bytes()),
    }


def compiler_summary():
    out = {"compiler_sha256": {tool: sha((score.IDO / tool).read_bytes()) for tool in TOOLS},
           "scene_accessors": {}, "cleanup_inline": {}}
    with tempfile.TemporaryDirectory(prefix="rush-boundary-") as tmp:
        base = Path(tmp)
        names = WALKERS + ACCESSORS
        for variant in ("baseline", "flag_accessors", "all_accessors"):
            directory = base / variant
            directory.mkdir()
            for name in ACCESSORS:
                shutil.copyfile(ROOT / "src/blob" / (name + ".c"), directory / (name + ".c"))
            for name, path in WALKER_INPUTS.items():
                text = (ROOT / path).read_text()
                if variant != "baseline":
                    text = accessor_calls(text, variant == "all_accessors")
                (directory / (name + ".c")).write_text(text)
            spec = dict(members=names, files=[name + ".c" for name in names],
                        flags=FLAGS, keep=names, claims=[])
            out["scene_accessors"][variant] = compile_record(directory, spec, inspect=WALKERS)

        directory = base / "canonical_helpers"
        directory.mkdir()
        for name in ACCESSORS:
            (directory / (name + ".c")).write_text(CANONICAL_PREAMBLE + CANONICAL_BODIES[name])
        spec = dict(members=ACCESSORS, files=[name + ".c" for name in ACCESSORS],
                    flags=FLAGS, keep=ACCESSORS, claims=[])
        canonical = compile_record(directory, spec)
        canonical["accessor_gate_passed"] = all(
            result["strict_match"] for result in canonical["functions"].values())
        canonical["caller_rerun_performed"] = False
        # Stop unconditionally: this packet contains exactly the one gated probe,
        # not an automatic search or permission to expand the experiment later.
        out["scene_accessors"]["canonical_helpers"] = canonical

        for label, path in CLEANUP_INPUTS.items():
            original = (ROOT / path / "group.c").read_text()
            spec = json.loads((ROOT / path / "group.json").read_text())
            for variant in ("baseline", "inline_keyword"):
                directory = base / (label + "_" + variant)
                directory.mkdir()
                text = original if variant == "baseline" else annotate_free(original)
                (directory / "group.c").write_text(text)
                out["cleanup_inline"][label + "_" + variant] = compile_record(
                    directory, spec, inspect=spec["members"])
    return out


def check_inputs(expected):
    actual = source_inputs()
    if actual != expected["source_inputs"]:
        changed = sorted(path for path in set(actual) | set(expected["source_inputs"])
                         if actual.get(path) != expected["source_inputs"].get(path))
        raise ValueError("source snapshot changed; review/replay required: " + ", ".join(changed))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", action="store_true", help="run all eight fixed IDO group compiles")
    parser.add_argument("--output", type=Path, help="write metadata JSON (never objects or native words)")
    parser.add_argument("--record", action="store_true", help="record a reviewed new snapshot instead of comparing")
    args = parser.parse_args()
    expected = None if args.record else json.loads((HERE / "verification.json").read_text())
    if expected is not None:
        check_inputs(expected)
    result = {"schema": 1, "base": BASE, "claims": [],
              "source_inputs": source_inputs(), "tool_sources": tool_sources(),
              "native": native_summary()}
    if expected is not None and stable_native(result["native"]) != stable_native(expected["native"]):
        raise SystemExit("native metadata changed; review/replay required")
    if args.compiler:
        result["compiler"] = compiler_summary()
        if expected is not None and result["compiler"] != expected["compiler"]:
            raise SystemExit("compiler receipt differs from the reviewed bounded experiment")
    if args.output:
        args.output.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n")
    print("Source/native evidence verified; " + (
        "eight compiler experiments reproduced; no match claims." if args.compiler
        else "compiler experiments not run; no match claims."))


if __name__ == "__main__":
    main()
