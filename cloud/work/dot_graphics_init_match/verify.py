#!/usr/bin/env python3
"""Replay genuine source context; keep objects and native words under build/."""
import argparse
import contextlib
import dataclasses
import hashlib
import io
import json
from pathlib import Path
import shutil
import struct
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
sys.path.insert(0, str(ROOT / "tools/cloud"))
import score

STRICT = ("sound_init", "func_8008705C", "func_800878E0", "func_8008A148",
          "func_8008A46C", "object_render")
MODE = "func_80086A50"
EXPECTED_MODE_REFS = [".rodata+0x0 at +0xc", ".rodata+0x0 at +0x14"]
PROTECTED = ("blob_matched.lock.json", "matched.lock.json", "symbol_addrs.us.txt",
             "include/game_types.h", "tools/conveyor/pipeline/disasm.py",
             "tools/cloud/score.py", "asm/us/blob/SHA256SUMS",
             "asm/us/blob/symbols.json")


def sha(data):
    return hashlib.sha256(data).hexdigest()


def word_hash(words):
    return sha(b"".join(struct.pack(">I", word) for word in words))


def materialize(directory):
    recipe = json.loads((HERE / "recipe.json").read_text())
    directory.mkdir(parents=True, exist_ok=True)
    for entry in recipe["files"]:
        shutil.copyfile(ROOT / entry["source"], directory / entry["name"])
    spec = {key: recipe[key] for key in
            ("members", "keep", "flags", "context", "claims", "unprototyped")}
    spec["files"] = [entry["name"] for entry in recipe["files"]]
    (directory / "group.json").write_text(json.dumps(spec, indent=2) + "\n")
    return recipe, spec


def function_record(obj, name):
    data, sections = score._elf(obj)
    symbols = [symbol for i, section in enumerate(sections) if section["type"] == 2
               for symbol in score._symbol_table(data, sections, i)]
    text_index = score._text_index(sections)
    symbol = next(s for s in symbols if s["name"] == name
                  and s["type"] == 2 and s["section"] == text_index)
    start, size = symbol["value"], symbol["size"]
    following = min((s["value"] for s in symbols if s["section"] == text_index
                     and s["type"] == 2 and s["value"] > start),
                    default=len(score.text_words(obj)) * 4)
    words = score.text_words(obj)
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + size, score.image_symbols())
    got = resolved[start // 4:(start + size) // 4]
    native = score.targets()[name]
    with contextlib.redirect_stdout(io.StringIO()):
        comparison = dataclasses.asdict(score.compare(obj, name, show=0))
    relocations = []
    for section in sections:
        if section["type"] != 9 or section["info"] != text_index:
            continue
        reloc_symbols = score._symbol_table(data, sections, section["link"])
        for k in range(section["size"] // 8):
            offset, info = struct.unpack_from(">II", data, section["off"] + 8 * k)
            if start <= offset < start + size:
                relocations.append({"offset": offset - start, "type": info & 255,
                                    "symbol": reloc_symbols[info >> 8]["name"]})
    normalized = [(got[i] & masks.get(start + i * 4, 0xFFFFFFFF))
                  for i in range(len(got))]
    return {
        "native_bytes": len(native) * 4,
        "elf_st_size": size,
        "following_symbol_distance": following - start,
        "native_sha256": word_hash(native),
        "fully_resolved_sha256": word_hash(got) if not masks else None,
        "masked_context_sha256": word_hash(normalized),
        "full_word_equal": got == native and not masks and not unresolved
                           and not unverified and not errors,
        "relocation_count": len(relocations),
        "relocations": relocations,
        "masked_offsets": [offset - start for offset in masks],
        "comparison": comparison,
    }


def section_record(obj, names):
    data, sections = score._elf(obj)
    return {section["name"]: {"bytes": section["size"],
            "sha256": sha(data[section["off"]:section["off"] + section["size"]])}
            for section in sections if section["name"] in names}


def assert_strict(record):
    assert record["elf_st_size"] == record["native_bytes"], "wrong ELF extent"
    assert record["following_symbol_distance"] >= record["elf_st_size"]
    assert record["full_word_equal"], "full relocated words differ or are unverified"
    assert record["fully_resolved_sha256"] == record["native_sha256"]
    assert record["masked_offsets"] == []
    assert record["comparison"] == {
        "differing": 0, "total": record["native_bytes"] // 4,
        "unresolved": [], "unverified": [], "errors": [], "extra_words": 0,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    build = ROOT / "build/dot_graphics_init_match"
    recipe, spec = materialize(build / "final")
    protected_before = {name: sha((ROOT / name).read_bytes()) for name in PROTECTED}
    obj = build / "final.o"
    score.compile_group(build / "final", obj)
    records = {fn: function_record(obj, fn) for fn in (*STRICT, MODE)}
    for fn in STRICT:
        assert_strict(records[fn])
    mode = records[MODE]
    assert mode["elf_st_size"] == mode["native_bytes"] == 1548
    assert mode["comparison"]["differing"] == 0
    # Since 2026-10-05 the scorer proves own .rodata against retail bytes, so the
    # mode helper's jump-table references are verified (empty list) rather than
    # reported; either form is the same evidence.
    assert mode["comparison"]["unverified"] in (EXPECTED_MODE_REFS, [])
    assert mode["comparison"]["unresolved"] == []
    assert mode["comparison"]["errors"] == []
    assert mode["comparison"]["extra_words"] == 0

    # Replay accepted context as-is; no file or keep-list edits in src/blob/.
    baseline_obj = build / "accepted_gfx.o"
    score.compile_group(ROOT / "src/blob/groups/gfx_modes", baseline_obj)
    baseline = {fn: function_record(baseline_obj, fn)
                for fn in (MODE, *STRICT[1:-1])}
    for fn, record in baseline.items():
        assert record["elf_st_size"] == records[fn]["elf_st_size"]
        assert record["masked_context_sha256"] == records[fn]["masked_context_sha256"]
        if fn != MODE:
            assert_strict(record)
    # Same table image and table relocation records as the accepted context.
    # This proves non-regression, not the table's placement in the native image.
    table_sections = section_record(obj, (".rodata", ".rel.rodata"))
    assert table_sections == section_record(baseline_obj, (".rodata", ".rel.rodata"))
    owner_obj = build / "accepted_object_render.o"
    score.compile_single(ROOT / "src/blob/object_render.c", recipe["flags"], owner_obj)
    baseline["object_render"] = function_record(owner_obj, "object_render")
    assert_strict(baseline["object_render"])
    assert baseline["object_render"]["fully_resolved_sha256"] == records["object_render"]["fully_resolved_sha256"]

    # Bounded causal controls: authentic old source, then its real symbol owner.
    controls = {}
    old_init = ROOT / "cloud/work/dot_graphics_init_closure/init.c"
    for tag, with_owner in (("legacy_in_accepted_gfx", False),
                            ("legacy_with_real_owner", True)):
        directory = build / tag
        _, control_spec = materialize(directory)
        shutil.copyfile(old_init, directory / "init.c")
        if not with_owner:
            for key in ("files", "members", "keep"):
                control_spec[key] = [x for x in control_spec[key]
                                     if x not in ("object_render", "object_render.c")]
        (directory / "group.json").write_text(json.dumps(control_spec, indent=2) + "\n")
        control_obj = build / (tag + ".o")
        score.compile_group(directory, control_obj)
        controls[tag] = function_record(control_obj, "sound_init")
    assert controls["legacy_in_accepted_gfx"]["comparison"]["differing"] == 75
    assert controls["legacy_in_accepted_gfx"]["elf_st_size"] == 404
    assert controls["legacy_with_real_owner"]["comparison"]["differing"] == 27
    assert controls["legacy_with_real_owner"]["elf_st_size"] == 400
    assert protected_before == {name: sha((ROOT / name).read_bytes()) for name in PROTECTED}
    report = {
        "status": "STRICT MATCH for sound_init; five accepted strict members unchanged",
        "coverage": "No splice, image, or cartridge claim; mode helper retains two pre-existing unverified local-table references.",
        "base": recipe["base"], "flags": recipe["flags"],
        "assembler_workaround": score.R4300_AS1,
        "recipe_sha256": sha((HERE / "recipe.json").read_bytes()),
        "sources_sha256": {entry["source"]: sha((ROOT / entry["source"]).read_bytes())
                           for entry in recipe["files"]},
        "protected_sha256": protected_before,
        "compiler_sha256": {name: sha(Path(score.ido(name)).read_bytes())
                            for name in ("cc", "cfe", "uld", "usplit", "umerge", "uopt", "ugen", "as1")},
        "functions": records,
        "accepted_baseline": baseline,
        "mode_table_sections_unchanged": table_sections,
        "controls": controls,
    }
    output = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(output)
    else:
        print(output, end="")


if __name__ == "__main__":
    main()
