#!/usr/bin/env python3
"""Replay the bounded natural-source/real-donor investigation (41 compiles)."""
from dataclasses import asdict
import json
from pathlib import Path
import tempfile

from verify import FLAGS, ROOT, inspect_object, score, sha256

HERE = Path(__file__).resolve().parent


def concise_proof(obj):
    proof = inspect_object(obj)
    return {key: proof[key] for key in (
        "elf_symbol_bytes", "elf_extent_exact", "full_word_differences",
        "masked_relocations", "unresolved", "unverified", "relocation_errors",
        "relocated_body_sha256", "strict_match")}


def candidates():
    old = (ROOT / "cloud/work/game_C26/func_800B23E0.c").read_text()
    base = (ROOT / "cloud/work/game_C26/float_local.c").read_text()
    for label, source in (("old_direct", old), ("old_float", base)):
        for optimization in ("O2", "O3"):
            yield label + "_" + optimization, source, FLAGS.replace("O3", optimization)
    variants = {
        "table_condition": base.replace("    u32 mask = D_80123418[index];\n", "").replace(
            "mask & (1 << choice)", "D_80123418[index] & (1 << choice)"),
        "mask_after_decls": base.replace("    u32 mask = D_80123418[index];\n", "").replace(
            "    f32 value;", "    f32 value;\n    u32 mask = D_80123418[index];"),
        "mask_assignment": base.replace("    u32 mask = D_80123418[index];", "    u32 mask;").replace(
            "    do {", "    mask = D_80123418[index];\n    do {"),
        "reuse_index": base.replace("    u8 choice;\n", "").replace("choice", "index"),
        "bit_local": base.replace("    f32 value;", "    f32 value;\n    u32 bit;").replace(
            "    } while (!(mask & (1 << choice)));", "        bit = 1U << choice;\n    } while (!(mask & bit));"),
        "signed_lcg": base.replace("1103515245U", "1103515245"),
        "long_mask": base.replace("u32 mask", "unsigned long mask"),
    }
    for typ in ("s32", "u32", "long", "unsigned long"):
        for stage, expression, use in (
            ("seed", "D_8011735C", "(f32)(((s32)result >> 16) & 0x7fff)"),
            ("shift", "D_8011735C >> 16", "(f32)(result & 0x7fff)"),
            ("rand", "(D_8011735C >> 16) & 0x7fff", "(f32)result"),
        ):
            source = base.replace("    f32 value;", "    f32 value;\n    " + typ + " result;")
            source = source.replace("        value =", "        result = " + expression + ";\n        value =")
            variants[typ.replace(" ", "_") + "_" + stage] = source.replace(
                "(f32)((D_8011735C >> 16) & 0x7fff)", use)
    for label, source, predicate in (
        ("mask_break", base, "mask"),
        ("table_break", variants["table_condition"], "D_80123418[index]"),
    ):
        variants[label] = source.replace("    do {", "    for (;;) {").replace(
            "    } while (!(" + predicate + " & (1 << choice)));",
            "        if (" + predicate + " & (1 << choice)) break;\n    }")
    for typ in ("u32", "s32", "unsigned long"):
        variants["return_" + typ.replace(" ", "_")] = base.replace("u8 func_800B23E0", typ + " func_800B23E0")
    variants.update({
        "unsigned_bit": base.replace("1 << choice", "1U << choice"),
        "commuted_bit": base.replace("mask & (1 << choice)", "(1U << choice) & mask"),
        "reverse_mult": base.replace("(f32)((D_8011735C >> 16) & 0x7fff) * 32.0f",
                                     "32.0f * (f32)((D_8011735C >> 16) & 0x7fff)"),
        "divide_then_scale": base.replace("* 32.0f / 32768.0f", "/ 32768.0f * 32.0f"),
        "seed_assignment_value": base.replace(
            "        D_8011735C = D_8011735C * 1103515245U + 12345;\n", "").replace(
            "((D_8011735C >> 16)", "(((D_8011735C = D_8011735C * 1103515245U + 12345) >> 16)"),
    })
    for label, source in variants.items():
        yield label, source.replace("-O2", "-O3"), FLAGS


def main():
    rows = []
    with tempfile.TemporaryDirectory(prefix="masked-rng-experiments-") as temporary:
        work = Path(temporary)
        for label, text, flags in candidates():
            source, obj = work / "candidate.c", work / "candidate.o"
            source.write_text(text)
            score.compile_single(source, flags, obj)
            rows.append(dict(label=label, kind="single", flags=flags,
                             source_sha256=sha256(source.read_bytes()),
                             verification=concise_proof(obj)))
        caller = """typedef unsigned char u8; typedef unsigned int u32;
extern u32 D_80123418[];
float func_8008B2E4(float);
u8 func_800B23E0(u8 index) {
 u32 mask = D_80123418[index];
 u8 choice;
 do {
  choice = (u32)func_8008B2E4(32.0f);
 } while (!(mask & (1 << choice)));
 return choice;
}
"""
        donors = {
            "tiny": (ROOT / "cloud/work/tiny_A23/func_8008B2E4.c").read_text(),
            "frand": (ROOT / "cloud/work/bigfish/libhunt/nearmiss/func_8008B2E4_frand.c").read_text(),
            "natural": """extern int D_8011735C;
float func_8008B2E4(float range) {
 D_8011735C=D_8011735C*1103515245U+12345;
 return (float)((D_8011735C >> 16) & 0x7fff)*range/32768.0f;
}
""",
            "rand": (ROOT / "src/blob/func_8008B2B4.c").read_text() + """
float func_8008B2E4(float range) { return (float)func_8008B2B4()*range/32768.0f;}
""",
        }
        for donor, modes in (("tiny", ("keep", "internal")), ("frand", ("keep", "internal")),
                             ("natural", ("keep", "single")), ("rand", ("keep", "single"))):
            for mode in modes:
                directory = work / (donor + "_" + mode)
                directory.mkdir()
                (directory / "donor.c").write_text(donors[donor])
                (directory / "caller.c").write_text(caller)
                obj = directory / "out.o"
                keep = ["func_800B23E0"] + ([] if mode == "internal" else ["func_8008B2E4"])
                if donor == "rand":
                    keep.append("func_8008B2B4")
                if mode == "single":
                    combined = donors[donor] + (caller.replace(" typedef unsigned int u32;", "")
                                                if donor == "rand" else caller)
                    (directory / "combined.c").write_text(combined)
                    score.compile_single(directory / "combined.c", FLAGS, obj)
                else:
                    (directory / "group.json").write_text(json.dumps(dict(
                        files=["donor.c", "caller.c"], keep=keep, flags=FLAGS, claims=[])))
                    score.compile_group(directory, obj)
                rows.append(dict(label=donor + "_" + mode, kind="donor-context", flags=FLAGS,
                                 source_sha256=sha256(donors[donor].encode()),
                                 caller_sha256=sha256(caller.encode()), keep=keep,
                                 verification=concise_proof(obj),
                                 donor_comparison=asdict(score.compare(obj, "func_8008B2E4", show=0))))
    result = dict(compiles=len(rows), strict_matches=sum(row["verification"]["strict_match"] for row in rows),
                  best_word_differences=min(row["verification"]["full_word_differences"] for row in rows),
                  rows=rows)
    (HERE / "experiments.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({key: value for key, value in result.items() if key != "rows"}))


if __name__ == "__main__":
    main()
