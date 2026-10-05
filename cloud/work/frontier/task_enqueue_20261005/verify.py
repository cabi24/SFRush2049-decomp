#!/usr/bin/env python3
"""Rebuild this nonmatch without changing the target, source, or acceptance gates."""
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import re
import struct
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

FUNCTION = "func_8010FBE0"
EXPECTED_BYTES = 128


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def function_extent(obj, name):
    data, sections = score._elf(obj)
    matches = [
        symbol
        for index, section in enumerate(sections)
        if section["type"] == 2
        for symbol in score._symbol_table(data, sections, index)
        if symbol["name"] == name and symbol["type"] == 2
    ]
    if len(matches) != 1 or matches[0]["size"] <= 0:
        raise ValueError("A unique nonzero ELF function extent is required")
    return matches[0]["value"], matches[0]["size"]


def accepts_exact(comparison, actual_bytes, expected_bytes):
    # score.compare reports extra *nonzero* words; it is not a st_size gate.
    return comparison.accepted() and actual_bytes == expected_bytes


def measure(source, flags, obj):
    score.compile_single(source, flags, obj)
    result = score.compare(obj, FUNCTION, show=0)
    start, size = function_extent(obj, FUNCTION)
    words = score.text_words(obj)
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + size, score.image_symbols())
    fully_resolved = not (masks or unresolved or unverified or errors)
    body = struct.pack(">%dI" % (size // 4), *resolved[start // 4:(start + size) // 4])
    return {
        "source_sha256": sha256(source.read_bytes()),
        "flags": flags,
        "comparison": asdict(result),
        "verdict": result.summary(),
        "elf_function_bytes": size,
        "text_section_bytes": len(words) * 4,
        "trailing_section_padding_bytes": len(words) * 4 - start - size,
        "all_function_relocations_verified": fully_resolved,
        "relocated_function_sha256": sha256(body) if fully_resolved else None,
        "strict_match_with_extent": accepts_exact(result, size, EXPECTED_BYTES),
    }


def verify():
    source = HERE / "candidate.c"
    header = source.read_text().splitlines()[0]
    match = re.fullmatch(r"/\* flags: (.+) \*/", header)
    if not match:
        raise ValueError("Missing exact flag header")
    flags = match[1]
    target = score.targets()[FUNCTION]  # Validates protected manifest.
    if len(target) * 4 != EXPECTED_BYTES:
        raise ValueError("Canonical extent changed")
    with tempfile.TemporaryDirectory(prefix="task-enqueue-proof-") as tmp:
        obj = Path(tmp) / "candidate.o"
        proof = measure(source, flags, obj)
        score.compile_single(HERE / "abi_probe.c", flags, Path(tmp) / "abi.o")
        old = {}
        for relative in (
            "cloud/work/tiny_A116/func_8010FBE0.c",
            "cloud/work/tiny_A116/control.scalar_globals.c",
            "cloud/work/tiny_A116/control.packet_view.c",
            "cloud/work/tiny_A50/func_8010FBE0.c",
        ):
            old[relative] = measure(
                ROOT / relative, "-g0 -O2 -mips2 -G 0 -non_shared", obj)
    return {
        "function": FUNCTION,
        "start": "0x8010FBE0",
        "end_exclusive": "0x8010FC60",
        "expected_bytes": EXPECTED_BYTES,
        "target_manifest_sha256": sha256((score.ASM_DIR / "SHA256SUMS").read_bytes()),
        "compiler_cc_sha256": sha256(Path(score.ido("cc")).read_bytes()),
        "candidate": proof,
        "ido_abi_assertions_passed": True,
        "prior_extent_corrections": old,
        "claims": [],
        "coverage_delta_bytes": 0,
    }


if __name__ == "__main__":
    receipt = verify()
    print(json.dumps(receipt, indent=2, sort_keys=True))
