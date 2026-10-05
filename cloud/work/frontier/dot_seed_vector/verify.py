#!/usr/bin/env python3
"""Compile and verify this candidate without publishing target words or objects."""
from contextlib import redirect_stdout
import hashlib
import io
import json
from pathlib import Path
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

NAME = "func_8010C02C"
SOURCE = Path(__file__).with_name(NAME + ".c")
CALLEES = ("func_8009E820", "func_800A61B0", "random_float")


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def verify_object(path):
    data, sections = score._elf(path)
    definitions = [symbol for i, section in enumerate(sections)
                   if section["type"] == 2
                   for symbol in score._symbol_table(data, sections, i)
                   if symbol["name"] == NAME and symbol["type"] == 2]
    assert len(definitions) == 1, definitions
    definition = definitions[0]
    start, size = definition["value"], definition["size"]
    expected = score.targets()[NAME]
    assert size == len(expected) * 4 == 696, (size, len(expected))
    words = score.text_words(path)
    resolved, masks, unresolved, unverified, errors = score.relocate(
        path, words, start, start + size, score.image_symbols())
    assert not masks and not unresolved and not unverified and not errors
    actual = resolved[start // 4:(start + size) // 4]
    assert actual == expected
    with redirect_stdout(io.StringIO()):
        comparison = score.compare(path, NAME)
    assert comparison.accepted()
    # The stock scorer's extent ends at the next function or section boundary.
    # Check actual symbol size above, and characterize alignment separately.
    next_start = min((v for v in score.symbols(path).values() if v > start), default=len(words) * 4)
    tail = words[(start + size) // 4:next_start // 4]
    assert not any(tail), "Nonzero instructions after the ELF function extent"
    return {"strict_match": True, "actual_elf_function_bytes": size,
            "compared_full_words": len(actual), "different_words": 0,
            "unresolved_relocations": 0, "unverified_relocations": 0,
            "relocation_masks": 0, "nonzero_extra_words": 0,
            "zero_alignment_words_after_symbol": len(tail)}


def main():
    lock = json.loads((ROOT / "blob_matched.lock.json").read_text())
    result = {"function": NAME, "source_sha256": digest(SOURCE),
              "base_commit": "cf10b3392d7f00ae42d75c008b79fdc2541aab6b",
              "target_manifest_sha256": digest(score.ASM_DIR / "SHA256SUMS"),
              "target_words_sha256": hashlib.sha256(struct.pack(
                  ">174I", *score.targets()[NAME])).hexdigest(),
              "accepted_lock_present": NAME in lock, "builds": {}}
    with tempfile.TemporaryDirectory(prefix="seed-vector-") as directory:
        tmp = Path(directory)
        for opt in ("O2", "O3"):
            flags = f"-g0 -{opt} -mips2 -G 0 -non_shared"
            obj = tmp / f"{opt}.o"
            score.compile_single(SOURCE, flags, obj)
            result["builds"][opt] = {"flags": flags + " " + score.R4300_CC,
                                     **verify_object(obj)}
        files = ["candidate.c"]
        (tmp / "candidate.c").write_bytes(SOURCE.read_bytes())
        result["direct_callees"] = {}
        for name in CALLEES:
            path = ROOT / lock[name]["source"]
            assert digest(path) == lock[name]["source_sha256"], name
            filename = name + ".c"
            files.append(filename)
            (tmp / filename).write_bytes(path.read_bytes())
            result["direct_callees"][name] = {"source": lock[name]["source"],
                                                   "source_sha256": digest(path)}
        flags = "-g0 -O3 -mips2 -G 0 -non_shared"
        spec = {"files": files, "members": [NAME], "keep": [NAME, *CALLEES], "flags": flags}
        (tmp / "group.json").write_text(json.dumps(spec))
        obj = tmp / "direct-callees.o"
        score.compile_group(tmp, obj)
        result["builds"]["O3_direct_callee_group"] = {
            "flags": flags + " " + score.R4300_CC, **verify_object(obj)}
        abi = tmp / "abi.c"
        abi.write_text('''#include "candidate.c"
#define OFFSET(T, M) ((unsigned int)&((T *)0)->M)
typedef char pointer_is_32[(sizeof(void *) == 4) ? 1 : -1];
typedef char vector_size[(sizeof(Vector3) == 12) ? 1 : -1];
typedef char model_size[(sizeof(CollisionModel) == 0x808) ? 1 : -1];
typedef char bounds_size[(sizeof(CollisionBounds) == 0x20) ? 1 : -1];
typedef char record_size[(sizeof(CollisionReckon) == 0x40) ? 1 : -1];
typedef char record_basis[(OFFSET(CollisionReckon, basis) == 4) ? 1 : -1];
typedef char record_position[(OFFSET(CollisionReckon, position) == 0x28) ? 1 : -1];
typedef char record_velocity[(OFFSET(CollisionReckon, velocity) == 0x34) ? 1 : -1];
typedef char object_index[(OFFSET(CollisionObject, index) == 0x65) ? 1 : -1];
''')
        score.compile_single(abi, flags, tmp / "abi.o")
        result["native_layout_compile_assertions"] = "passed"
    result["promotion"] = {"spliced": False, "full_blob_unit_checked": False,
                           "image_gate_run": False, "rom_hash_gate_run": False}
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
