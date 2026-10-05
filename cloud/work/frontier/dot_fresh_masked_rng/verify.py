#!/usr/bin/env python3
"""Source-bound, unmasked word/ELF-extent verification. Never promotes a match."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / "tools/cloud"))
import score

NAME = "func_800B23E0"
FLAGS = "-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul"


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def inspect_object(obj, name=NAME):
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    functions = [symbol for i, section in enumerate(sections)
                 if section["type"] == 2
                 for symbol in score._symbol_table(data, sections, i)
                 if symbol["type"] == 2 and symbol["section"] == text_index
                 and symbol["name"] == name]
    if len(functions) != 1:
        raise ValueError("expected exactly one defined target function")
    symbol = functions[0]
    start, extent = symbol["value"], symbol["size"]
    if extent == 0 or extent % 4 or start + extent > sections[text_index]["size"]:
        raise ValueError("invalid or absent ELF function extent")
    target = score.targets()[name]
    words = score.text_words(obj)
    relocated, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + extent, score.image_symbols())
    actual = relocated[start // 4:(start + extent) // 4]
    differences = [i for i in range(max(len(actual), len(target)))
                   if i >= len(actual) or i >= len(target) or actual[i] != target[i]]
    extent_exact = extent == len(target) * 4
    return {
        "target_address": hex(score.image_symbols()[name]),
        "target_bytes": len(target) * 4,
        "target_words": len(target),
        "elf_symbol_bytes": extent,
        "elf_extent_exact": extent_exact,
        "full_word_differences": len(differences),
        "differing_word_offsets": [i * 4 for i in differences],
        "masked_relocations": len(masks),
        "unresolved": unresolved,
        "unverified": unverified,
        "relocation_errors": errors,
        "target_words_sha256": sha256(struct.pack(">%dI" % len(target), *target)),
        "relocated_body_sha256": sha256(struct.pack(">%dI" % len(actual), *actual)),
        "strict_match": not (differences or masks or unresolved or unverified or errors)
                        and extent_exact,
    }


def verify(source):
    with tempfile.TemporaryDirectory(prefix="masked-rng-verify-") as temp:
        obj = Path(temp) / "candidate.o"
        score.compile_single(source, FLAGS, obj)
        result = inspect_object(obj)
    return {
        "function": NAME,
        "base_commit": "cf10b3392d7f00ae42d75c008b79fdc2541aab6b",
        "source_sha256": sha256(source.read_bytes()),
        "flags": FLAGS,
        "compiler_sha256": {name: sha256((score.IDO / name).read_bytes())
                            for name in ("cc", "cfe", "uopt", "ugen", "as1")},
        "target_manifest_sha256": sha256((score.ASM_DIR / "SHA256SUMS").read_bytes()),
        "scorer_sha256": sha256((ROOT / "tools/cloud/score.py").read_bytes()),
        "status": "strict-match" if result["strict_match"] else "nonmatch",
        "verification": result,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = verify(Path(__file__).with_name("best.c"))
    text = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(text)
    print(text, end="")
    # Successful verification of a research nonmatch is not a matching claim.
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
