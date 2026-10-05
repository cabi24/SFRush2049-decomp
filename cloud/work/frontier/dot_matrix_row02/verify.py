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

NAMES = ("func_800C40E8",)
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



def replay(output):
    output = output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    name = NAMES[0]
    source = ROOT / "cloud/matches" / (name + ".c")
    assert source.read_text().splitlines()[0] == "/* flags: " + FLAGS + " */"
    results = []
    for optimization in (3, 2):
        flags = FLAGS.replace("-O3", "-O%d" % optimization)
        obj = output / ("candidate_O%d.o" % optimization)
        score.compile_single(source, flags, obj)
        result = verify_object(obj, name)
        result.update(mode="single", flags=flags)
        results.append(result)
    altered = source.read_text().replace(
        "m[2][0] * cosine - m[0][0] * sine",
        "m[2][0] * cosine + m[0][0] * sine")
    assert altered != source.read_text()
    mutant = output / "wrong_rotation.c"
    mutant.write_text(altered)
    obj = output / "wrong_rotation.o"
    score.compile_single(mutant, FLAGS, obj)
    comparison = score.compare(obj, name, show=0)
    assert not comparison.accepted(), "wrong rotation unexpectedly passed"
    try:
        verify_object(obj, name)
    except AssertionError:
        pass
    else:
        raise AssertionError("complete-body verifier accepted wrong rotation")
    receipt = {
        "schema": 1,
        "base_commit": "cf10b3392d7f00ae42d75c008b79fdc2541aab6b",
        "claim": "strict complete-function object match only; not ROM coverage",
        "scorer_sha256": digest((ROOT / "tools/cloud/score.py").read_bytes()),
        "target_manifest_sha256": digest((ROOT / "asm/us/blob/SHA256SUMS").read_bytes()),
        "compiler_sha256": {name: digest((score.IDO / name).read_bytes())
                            for name in ("cc", "acpp", "cfe", "uld", "usplit",
                                         "umerge", "uopt", "ugen", "as1")},
        "results": results,
        "negative_control": {
            "change": "row-2 subtraction replaced with addition",
            "comparison": asdict(comparison),
            "strict_verifier_rejects": True,
        },
    }
    (output / "verification.json").write_text(json.dumps(receipt, indent=2) + "\n")
    return receipt


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=ROOT / "build/dot_matrix_row02_verify")
    args = parser.parse_args()
    receipt = replay(args.output)
    for result in receipt["results"]:
        print(result["function"], result["flags"],
              "MATCH; ELF extent 152 bytes; relocated bytes equal")
    print("Wrong-rotation control rejected")
