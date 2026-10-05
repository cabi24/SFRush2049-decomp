#!/usr/bin/env python3
"""Read-only, local actual-TU regression for the sequence-context declaration repair.

No coordinator, remote builder, lock edits, promotion, or ROM gate is invoked.
Only temporary source overlays and object/link products are written under --out.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import sys

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))
from tools.cloud import score
from tools.conveyor.pipeline.lock import body_sha

BASE = "cf10b3392d7f00ae42d75c008b79fdc2541aab6b"
TU = "src/rom/lib_17dc0.c"
HERE = Path(__file__).resolve().parent
CANDIDATES = tuple("func_" + n for n in (
    "80017540", "80018A30", "80018B3C", "80018C2C", "80018D40", "80019194", "800198C8"))
HEADER = HERE / "sequence_context.h"
LEGACY_TYPES = ("AudioNode", "AudioState", "Entry", "ContextPrefix", "VoiceRecord")
TYPE_MAP = {"AudioNode": "SequenceNode", "Entry": "SequenceNode",
            "AudioState": "SequenceContext", "ContextPrefix": "SequenceContext",
            "VoiceRecord": "SequenceContext", "headF78": "active", "headF7C": "pending"}
CFLAGS = ["-G", "0", "-mips2", "-O2", "-non_shared", "-Iinclude",
          "-Iinclude/PR", "-D_LANGUAGE_C", "-Wab,-r4300_mul", "-Xcpluscomm",
          "-Isrc/rom", "-I" + str(HERE)]
DATA_SECTIONS = {".data", ".rodata", ".rdata", ".bss", ".sdata", ".sbss", ".lit4", ".lit8"}


class VerificationError(RuntimeError):
    pass


def require(condition, message):
    if not condition:
        raise VerificationError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def run(args):
    return subprocess.run([str(a) for a in args], cwd=REPO,
                          capture_output=True, text=True)


def checked(args):
    result = run(args)
    require(result.returncode == 0, "command failed: " + " ".join(map(str, args))
            + "\n" + result.stdout + result.stderr)
    return result.stdout


def bodies(text):
    from tools.conveyor.seeds.extract_candidates import extract_functions
    return {n: text[s:e] for n, s, e in extract_functions(text)}


def adapt_context(text):
    """Temporary canonical type refactor; never write the production TU.

    The five legacy one-line declarations are known views in the reviewed TU.
    Refuse ambiguous syntax rather than erase a declaration heuristically.
    """
    if '#include "sequence_context.h"' in text:
        return text
    for name in LEGACY_TYPES:
        pattern = r"^typedef struct " + name + r" \{[^\n]+\} " + name + r";\n"
        text, count = re.subn(pattern, "", text, flags=re.M)
        require(count == 1, "expected one reviewed legacy declaration: " + name)
    for old, new in TYPE_MAP.items():
        text = re.sub(r"\b" + old + r"\b", new, text)
    return text.replace('#include "rom_tu.h"',
                        '#include "rom_tu.h"\n#include "sequence_context.h"', 1)


def overlay(tu, name, source):
    pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/' + name + '.s")'
    existing = bodies(tu)
    if name in existing:
        require(pragma not in tu, name + ": both C and passthrough present")
        return tu
    require(tu.count(pragma) == 1, name + ": candidate must have exactly one passthrough slot")
    candidate = bodies(source)
    require(set(candidate) == {name}, "expected exactly one adapted candidate body")
    return tu.replace(pragma, candidate[name], 1)


def compile_tu(text, out, name, expect_error=False):
    source, obj = out / (name + ".c"), out / (name + ".o")
    source.write_text(text)
    command = [sys.executable, REPO / "tools/asm-processor/build.py", score.ido("cc"),
               "--", "mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-Iinclude",
               "--", "-c", *CFLAGS, "-o", obj, source]
    result = run(command)
    log = result.stdout + result.stderr
    if expect_error:
        require(result.returncode != 0 and "redeclaration of" in log,
                "the original declaration conflict was not reproduced")
        symbols = re.findall(r"redeclaration of '([^']+)'", log)
        require(any(n in {"D_8004BE80", "D_80043EB8", "func_8001729C", "func_8001734C", "func_80017470"}
                    for n in symbols), "negative control failed for an unrelated declaration")
        return {"rejected": True, "reason": "conflicting native record declarations",
                "conflicting_symbols": symbols}
    require(result.returncode == 0 and obj.is_file(), "actual-TU compilation failed:\n" + log)
    return obj


def function_symbols(obj):
    data, sections = score._elf(obj)
    text = score._text_index(sections)
    all_symbols = [sym for i, section in enumerate(sections) if section["type"] == 2
                   for sym in score._symbol_table(data, sections, i)]
    functions = {s["name"]: s for s in all_symbols if s["section"] == text and s["type"] == 2}
    require(not any(s["size"] and s["name"] in DATA_SECTIONS for s in sections),
            "unexpected allocated data section in the actual TU")
    return functions, all_symbols


def exact_extent(symbols, name, expected):
    require(name in symbols, name + ": missing function symbol")
    require(symbols[name]["size"] == expected, name + ": STT_FUNC extent differs from target")
    start = symbols[name]["value"]
    following = min((s["value"] for s in symbols.values() if s["value"] > start), default=None)
    require(following is None or start + expected <= following,
            name + ": function extends into its neighbor")


def relocated_bytes(obj, name, expected, addresses):
    symbols, _ = function_symbols(obj)
    exact_extent(symbols, name, expected)
    start = symbols[name]["value"]
    words = score.text_words(obj)
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + expected, addresses)
    require(not (masks or unresolved or unverified or errors),
            name + ": incomplete relocation verification")
    return struct.pack(">" + "I" * (expected // 4), *resolved[start // 4:(start + expected) // 4])


def link_tu(obj, out, addresses):
    """Independently resolve the complete real-TU object using GNU ld."""
    functions, all_symbols = function_symbols(obj)
    names = sorted(n for n in functions if re.fullmatch(r"func_[0-9A-F]{8}", n))
    base = min(addresses[n] for n in names)
    for name in names:
        require(functions[name]["value"] + base == addresses[name],
                name + ": actual TU layout no longer has the native offset")
    assigns = []
    for sym in all_symbols:
        if sym["section"] != 0 or not sym["name"]:
            continue
        name = sym["name"]
        address = addresses.get(name, score.address_named(name))
        require(address is not None and re.fullmatch(r"[A-Za-z_]\w*", name),
                "unresolved linker symbol: " + name)
        assigns.append(f"{name} = 0x{address:08x};")
    script = out / (obj.stem + ".ld")
    script.write_text("\n".join(assigns) + "\nSECTIONS { . = 0x%08x; .text : { *(.text) } }\n" % base)
    elf, binary = out / (obj.stem + ".elf"), out / (obj.stem + ".bin")
    checked(["mips-linux-gnu-ld", "-EB", "-T", script, "-o", elf, obj])
    checked(["mips-linux-gnu-objcopy", "-O", "binary", "--only-section=.text", elf, binary])
    return base, binary.read_bytes(), names


def check_object(obj, required, out, targets, addresses):
    functions, _ = function_symbols(obj)
    base, linked, all_names = link_tu(obj, out, addresses)
    rows = []
    for name in required:
        want = struct.pack(">" + "I" * len(targets[name]), *targets[name])
        got = relocated_bytes(obj, name, len(want), addresses)
        start = functions[name]["value"]
        independent = linked[start:start + len(want)]
        require(got == want, name + ": full relocated bytes differ from target")
        require(independent == want, name + ": GNU-linked bytes differ from target")
        rows.append({"function": name, "target_bytes": len(want),
                     "function_bytes": functions[name]["size"], "full_word_differences": 0,
                     "unresolved": 0, "unverified": 0, "relocation_errors": 0,
                     "target_sha256": sha(want), "relocated_sha256": sha(got),
                     "gnu_linked_sha256": sha(independent)})
    # Check every slot, including assembly passthroughs, with GNU relocation.
    for name in all_names:
        want = struct.pack(">" + "I" * len(targets[name]), *targets[name])
        start = addresses[name] - base
        require(linked[start:start + len(want)] == want, name + ": complete TU slot differs")
    end = max(addresses[n] - base + len(targets[n]) * 4 for n in all_names)
    require(not any(linked[end:]), "nonzero bytes beyond complete TU target extent")
    return {"functions": rows, "all_tu_slots_verified": len(all_names),
            "linked_text_sha256": sha(linked), "linked_text_bytes": len(linked),
            "trailing_zero_padding_bytes": len(linked) - end}


def compile_plain(source, obj):
    checked([score.ido("cc"), "-c", *CFLAGS, "-o", obj, source])
    return obj


def _verify(out):
    out = Path(out).resolve()
    out.mkdir(parents=True, exist_ok=True)
    original = checked(["git", "show", BASE + ":" + TU])
    current = (REPO / TU).read_text()
    base_locks = json.loads(checked(["git", "show", BASE + ":matched.lock.json"]))
    baseline = sorted(k.split(":")[1] for k in base_locks if k.startswith(TU + ":"))
    require(len(baseline) == 16, "expected the 16 accepted bodies from the reviewed base")
    locks = json.loads((REPO / "matched.lock.json").read_text())
    accepted = sorted(k.split(":")[1] for k in locks if k.startswith(TU + ":"))
    require(set(baseline) <= set(accepted), "an existing accepted body lost its lock")
    for name in accepted:
        require(body_sha(REPO / TU, name) == locks[TU + ":" + name]["body_sha256"],
                name + ": production locked body changed without relocking")
    adapted = adapt_context(current)
    combined = adapted
    for name in CANDIDATES:
        combined = overlay(combined, name, (HERE / "sources" / (name + ".c")).read_text())
    # Preserve the old refusal as a real compiler negative control for each body.
    context = {r["function"]: r for r in map(json.loads, checked([
        "git", "show", BASE + ":cloud/work/boot_tail_promotion/context.jsonl"]).splitlines())}
    negative = {}
    for name in CANDIDATES:
        row = context[name]
        source = checked(["git", "show", BASE + ":" + row["source"]])
        legacy = "\n".join(row["preamble"]) + "\n" + bodies(source)[name]
        pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/' + name + '.s")'
        require(original.count(pragma) == 1, "missing original candidate slot")
        negative[name] = compile_tu(original.replace(pragma, legacy, 1), out,
                                   "legacy_" + name, expect_error=True)
    compile_plain(HERE / "layout_checks.c", out / "layout_checks.o")
    score.ASM_DIR = REPO / "asm/us/boot_tail"
    targets, addresses = score.targets(), score.image_symbols()
    stages = {}
    for name, text, required in (
        ("baseline", original, baseline),
        ("production", current, accepted),
        ("canonical_adapter", adapted, accepted),
        ("candidate_overlay", combined, sorted(set(accepted) | set(CANDIDATES))),
    ):
        obj = compile_tu(text, out, name)
        stages[name] = check_object(obj, required, out, targets, addresses)
    require(len({s["linked_text_sha256"] for s in stages.values()}) == 1,
            "complete linked TU text changed")
    standalone = {}
    for name in CANDIDATES:
        path = HERE / "sources" / (name + ".c")
        obj = compile_plain(path, out / (name + ".o"))
        want = struct.pack(">" + "I" * len(targets[name]), *targets[name])
        got = relocated_bytes(obj, name, len(want), addresses)
        require(got == want, name + ": adapted standalone body differs")
        standalone[name] = {"function_bytes": len(want), "match": True,
                            "relocated_sha256": sha(got), "target_sha256": sha(want)}
    # A plausible field-layout error must fail the actual-TU byte comparison,
    # not merely a textual source assertion or the standalone-source checker.
    header = HEADER.read_text()
    bad_header = header.replace("unknownFF4[4]", "unknownFF4[8]")
    require(bad_header != header, "stride negative control did not mutate source")
    wrong_stride = combined.replace('#include "sequence_context.h"', bad_header, 1)
    obj = compile_tu(wrong_stride, out, "wrong_record_stride")
    try:
        check_object(obj, sorted(set(accepted) | set(CANDIDATES)), out, targets, addresses)
    except VerificationError as exc:
        negative["wrong_record_stride"] = {"rejected": True, "reason": str(exc)}
    else:
        raise VerificationError("wrong record stride was not rejected")
    bad_behavior = combined.replace("flagsFEE |= 8;", "flagsFEE |= 4;", 1)
    require(bad_behavior != combined, "behavior negative control did not mutate source")
    obj = compile_tu(bad_behavior, out, "wrong_pending_flag")
    try:
        check_object(obj, sorted(set(accepted) | set(CANDIDATES)), out, targets, addresses)
    except VerificationError as exc:
        negative["wrong_pending_flag"] = {"rejected": True, "reason": str(exc)}
    else:
        raise VerificationError("wrong pending flag was not rejected")
    current_bodies, adapted_bodies = bodies(current), bodies(adapted)
    relock = [name for name in accepted if current_bodies[name] != adapted_bodies[name]]
    source_paths = [Path(TU), HEADER.relative_to(REPO),
                    (HERE / "layout_checks.c").relative_to(REPO)]
    source_paths += [(HERE / "host_test.c").relative_to(REPO),
                     Path("tests/cloud/test_sequence_context_contract.py")]
    source_paths += [(HERE / "sources" / (n + ".c")).relative_to(REPO) for n in CANDIDATES]
    evidence = ["func_800171C0", "func_800178B0", *CANDIDATES,
                "func_80017D38", "func_80018184", "func_80018634"]
    native_paths = ["asm/us/nonmatchings/rom/lib_17dc0/" + n + ".s" for n in evidence]
    native_paths += ["asm/us/nonmatchings/rom/lib_1a660/" + n + ".s" for n in
                     ("func_80019A60", "func_8001B9F8", "func_8001BDB8")]
    native_paths += ["asm/us/nonmatchings/rom/lib_207b0/func_800201D0.s"]
    result = {"base_commit": BASE, "translation_unit": TU,
              "candidates": list(CANDIDATES), "candidate_bytes": sum(
                  len(targets[n]) * 4 for n in CANDIDATES), "flags": CFLAGS[:-1] + ["-I<contract-dir>"],
              "negative_controls": negative, "standalone_candidates": standalone,
              "stages": stages, "native_layout_assertions": "passed",
              "accepted_bodies_requiring_maintainer_relock": relock,
              "production_source_written": False, "matching_credit_added": 0,
              "rom_gate": "not run; maintainer canonical refactor, relock and promotion transaction required",
              "verification_script_sha256": sha(Path(__file__).read_bytes()),
              "baseline_source_sha256": sha(original.encode()),
              "source_sha256": {str(p): sha((REPO / p).read_bytes()) for p in source_paths},
              "native_evidence_sha256": {p: sha((REPO / p).read_bytes()) for p in native_paths},
              "protected_inputs_sha256": {p: sha((REPO / p).read_bytes()) for p in (
                  "asm/us/boot_tail/SHA256SUMS", "asm/us/boot_tail/symbols.json",
                  "matched.lock.json", "src/rom/rom_tu.h", "tools/cloud/score.py",
                  "tools/asm-processor/build.py", "tools/asm-processor/asm_processor.py")},
              "ido_sha256": {n: sha(Path(score.ido(n)).read_bytes())
                             for n in ("cc", "cfe", "uopt", "ugen", "as1")},
              "gnu_ld_version": checked(["mips-linux-gnu-ld", "--version"]).splitlines()[0]}
    (out / "verification.json").write_text(json.dumps(result, indent=2) + "\n")
    return result


def verify(out):
    old = score.ASM_DIR, score._targets, score._target_fingerprint
    try:
        return _verify(out)
    finally:
        score.ASM_DIR, score._targets, score._target_fingerprint = old


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path, default=REPO / "build/sequence_context_contract")
    args = parser.parse_args()
    try:
        result = verify(args.out)
    except (OSError, VerificationError) as exc:
        print(str(exc), file=sys.stderr)
        return 1
    for name, stage in result["stages"].items():
        print(f"{name}: {len(stage['functions'])} exact C extents and relocated bodies; "
              f"{stage['all_tu_slots_verified']} complete TU slots MATCH")
    print("seven adapted standalone candidates: 1596 bytes MATCH")
    print("nine negative controls rejected; native layout assertions passed")
    print("production and locks unchanged; ROM gate requires maintainer transaction")
    return 0


if __name__ == "__main__":
    sys.exit(main())
