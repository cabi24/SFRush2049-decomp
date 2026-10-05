#!/usr/bin/env python3
"""Replay complete row-rotation matches without changing acceptance state.

Uses the unchanged authenticated retail scorer, then separately requires the
ELF function size to equal the complete target extent and compares all relocated
bytes without masks. Object files and private diagnostics stay in build/.
"""
import argparse
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

NAMES = ("func_800ACFF8", "func_800AD090")
FLAGS = "-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul"


def digest(data):
    return hashlib.sha256(data).hexdigest()


def function_extent(obj, name):
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    matches = [symbol for i, section in enumerate(sections)
               if section["type"] == 2
               for symbol in score._symbol_table(data, sections, i)
               if symbol["type"] == 2 and symbol["section"] == text_index
               and symbol["name"] == name]
    assert len(matches) == 1, (name, "ambiguous ELF function")
    symbol = matches[0]
    assert symbol["size"] > 0, (name, "missing ELF function size")
    return symbol["value"], symbol["size"]


def verify_object(obj, name):
    target = score.targets()[name]
    addresses = score.image_symbols()
    start, size = function_extent(obj, name)
    assert size == len(target) * 4 == 152, (name, "wrong complete extent", size)
    comparison = score.compare(obj, name, show=0)
    assert comparison.accepted(), (name, comparison.summary())
    words = score.text_words(obj)
    relocated, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + size, addresses)
    assert not (masks or unresolved or unverified or errors), name
    actual = relocated[start // 4:(start + size) // 4]
    assert actual == target, (name, "full relocated bytes differ")
    target_bytes = struct.pack(">%dI" % len(target), *target)
    actual_bytes = struct.pack(">%dI" % len(actual), *actual)
    return {
        "function": name,
        "native_start": "0x%08X" % addresses[name],
        "native_end_exclusive": "0x%08X" % (addresses[name] + size),
        "target_extent_bytes": len(target_bytes),
        "elf_function_size_bytes": size,
        "source_sha256": digest((ROOT / "cloud/matches" / (name + ".c")).read_bytes()),
        "target_bytes_sha256": digest(target_bytes),
        "relocated_function_bytes_sha256": digest(actual_bytes),
        "full_relocated_bytes_equal": actual_bytes == target_bytes,
        "comparison": asdict(comparison),
    }


def complete_object_bytes(obj, name):
    """Return an entire ELF-defined function after strict relocation resolution."""
    start, size = function_extent(obj, name)
    words = score.text_words(obj)
    relocated, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + size, score.image_symbols())
    assert not (masks or unresolved or unverified or errors), name
    return struct.pack(">%dI" % (size // 4),
                       *relocated[start // 4:(start + size) // 4])


def caller_probe(output):
    """Extend a real existing caller group without editing any context source."""
    source_group = ROOT / "src/blob/groups/frontier_traction_control"
    context = ("traction_control", "vector_diff_process", "steering_sensitivity",
               "func_800A61B0", "math_utility")
    objects = []
    for extended in (False, True):
        group = output / ("caller_extended" if extended else "caller_baseline")
        group.mkdir(exist_ok=True)
        spec = json.loads((source_group / "group.json").read_text())
        for filename in spec["files"]:
            (group / filename).write_bytes((source_group / filename).read_bytes())
        if extended:
            for name in NAMES:
                filename = name + ".c"
                (group / filename).write_bytes((ROOT / "cloud/matches" / filename).read_bytes())
                spec["files"].append(filename)
                spec["keep"].append(name)
        (group / "group.json").write_text(json.dumps(spec, indent=2) + "\n")
        obj = group / "candidate.o"
        score.compile_group(group, obj)
        objects.append(obj)

    preservation = []
    for name in context:
        baseline, extended = [complete_object_bytes(obj, name) for obj in objects]
        assert baseline == extended, (name, "caller context changed")
        before, after = [score.compare(obj, name, show=0) for obj in objects]
        assert asdict(before) == asdict(after)
        if name != "steering_sensitivity":
            assert before.accepted(), (name, before.summary())
        else:
            assert not before.accepted(), "the retained caller is an unclaimed near-miss"
        preservation.append({
            "function": name, "elf_function_size_bytes": len(baseline),
            "baseline_relocated_bytes_sha256": digest(baseline),
            "extended_relocated_bytes_sha256": digest(extended),
            "complete_relocated_bytes_unchanged": True,
            "baseline_comparison": asdict(before), "extended_comparison": asdict(after),
        })
    matches = []
    for name in NAMES:
        result = verify_object(objects[1], name)
        result.update(mode="real_caller_group", flags=FLAGS)
        matches.append(result)
    return matches, {
        "source_group": str(source_group.relative_to(ROOT)),
        "context_source_sha256": digest((source_group / "group.c").read_bytes()),
        "context_spec_sha256": digest((source_group / "group.json").read_bytes()),
        "context_source_changed": False,
        "caller_matches_retail": False,
        "context_results": preservation,
    }


def replay(output):
    output = output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    results = []
    for optimization in (3, 2):
        flags = FLAGS.replace("-O3", "-O%d" % optimization)
        for name in NAMES:
            source = ROOT / "cloud/matches" / (name + ".c")
            assert source.read_text().splitlines()[0] == "/* flags: " + FLAGS + " */"
            obj = output / (name + "_O%d.o" % optimization)
            score.compile_single(source, flags, obj)
            result = verify_object(obj, name)
            result.update(mode="single", flags=flags)
            results.append(result)

    # Real adjacent functions only. No stand-ins or invented keepers.
    group = output / "pair_group"
    group.mkdir(exist_ok=True)
    for name in NAMES:
        (group / (name + ".c")).write_bytes(
            (ROOT / "cloud/matches" / (name + ".c")).read_bytes())
    (group / "group.json").write_text(json.dumps({
        "files": [name + ".c" for name in NAMES],
        "keep": list(NAMES), "flags": FLAGS,
    }, indent=2) + "\n")
    group_obj = output / "pair_group.o"
    score.compile_group(group, group_obj)
    for name in NAMES:
        result = verify_object(group_obj, name)
        result.update(mode="real_pair_group", flags=FLAGS)
        results.append(result)

    caller_matches, caller_context = caller_probe(output)
    results.extend(caller_matches)
    receipt = {
        "schema": 1,
        "base_commit": "cf10b3392d7f00ae42d75c008b79fdc2541aab6b",
        "claim": "strict complete-function object matches only; not ROM coverage",
        "scorer_sha256": digest((ROOT / "tools/cloud/score.py").read_bytes()),
        "target_manifest_sha256": digest((ROOT / "asm/us/blob/SHA256SUMS").read_bytes()),
        "compiler_sha256": {name: digest((score.IDO / name).read_bytes())
                            for name in ("cc", "acpp", "cfe", "uld", "usplit",
                                         "umerge", "uopt", "ugen", "as1")},
        "results": results,
        "real_caller_probe": caller_context,
    }
    (output / "verification.json").write_text(json.dumps(receipt, indent=2) + "\n")
    return receipt


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=ROOT / "build/dot_matrix_rows_verify")
    args = parser.parse_args()
    receipt = replay(args.output)
    for result in receipt["results"]:
        print(result["function"], result["mode"], result["flags"],
              "MATCH; ELF extent 152 bytes; relocated bytes equal")
