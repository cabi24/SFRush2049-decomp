#!/usr/bin/env python3
"""Locally verify the queue-wrapper repair, without promoting or syncing anything.

Compile the actual lib_25bb0 TU with asm-processor, both as committed and with
only the three repaired candidates spliced into temporary copies. Use the
unchanged strict cloud scorer for every function, including assembly slots.
The ROM SHA-1 transaction remains a separate maintainer gate.
"""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score  # noqa: E402
from tools.conveyor.pipeline.lock import body_sha, load_lock, normalize_body  # noqa: E402
from tools.conveyor.seeds.extract_candidates import extract_functions, extract_named_function  # noqa: E402

BASE = "abc0f256e8c6b5e2004662af09aca7c5bf915204"
TU = Path("src/rom/lib_25bb0.c")
FUNCTIONS = ("func_800250F0", "func_80025120", "func_80025150")
LOCKED = ("func_80024FB0", "func_8002506C", "func_800250AC",
          "func_80025264", "func_80025594")
STANDALONE_FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"
# Exact Makefile ROM-TU default recipe; -Isrc/rom locates the real header for
# temporary copies. No synthetic context, structure, SDK stub, or flag sweep.
TU_FLAGS = ["-G", "0", "-mips2", "-O2", "-non_shared", "-Iinclude",
            "-Iinclude/PR", "-D_LANGUAGE_C", "-Wab,-r4300_mul", "-Xcpluscomm",
            "-Isrc/rom"]
DATA_SECTIONS = (".data", ".rodata", ".bss", ".sdata", ".sbss", ".rdata", ".lit4", ".lit8")


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def baseline_text(path):
    return subprocess.check_output(
        ["git", "show", BASE + ":" + str(path)], cwd=ROOT, text=True)


def baseline_inputs():
    return baseline_text(TU), json.loads(baseline_text("matched.lock.json"))


def text_bodies(text):
    return {fn: text[start:end] for fn, start, end in extract_functions(text)}


def text_body_sha(body):
    return hashlib.sha256(normalize_body(body).encode()).hexdigest()


def current_state(text, entries, baseline_text, baseline_locks):
    """Validate actual accepted bodies; only residual passthroughs are overlaid."""
    bodies = text_bodies(text)
    for fn in LOCKED:
        key = str(TU) + ":" + fn
        if fn not in bodies or key not in entries:
            raise AssertionError(f"{fn}: original TU lock/body missing")
        if text_body_sha(bodies[fn]) != baseline_locks[key]["body_sha256"]:
            raise AssertionError(f"{fn}: original locked body changed")
    for fn, body in bodies.items():
        key = str(TU) + ":" + fn
        if key not in entries or text_body_sha(body) != entries[key]["body_sha256"]:
            raise AssertionError(f"{fn}: current TU lock/body mismatch")
    pending = []
    for fn in FUNCTIONS:
        pragma = f'#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/{fn}.s")'
        count = text.count(pragma)
        if count == 1 and fn not in bodies:
            pending.append(fn)
        elif count != 0 or fn not in bodies:
            raise AssertionError(f"{fn}: missing/duplicate candidate slot")
    def names(source):
        return set(text_bodies(source)) | set(re.findall(
            r'GLOBAL_ASM\("[^\"]+/(func_[0-9A-F]+)\.s"\)', source))
    if names(text) != names(baseline_text):
        raise AssertionError("current TU population differs from fixed baseline")
    return pending, sorted(bodies), sorted(names(text))


def context_module():
    spec = importlib.util.spec_from_file_location(
        "queue_wrapper_context", ROOT / "cloud/work/boot_tail_promotion/context_check.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.CC = score.IDO / "cc"  # Only compiler location changes, never flags.
    return module


def candidate_paths():
    return {fn: ROOT / "cloud/matches/boot_tail" / (fn + ".c") for fn in FUNCTIONS}


def fit(paths, work):
    context = context_module()
    rows = {}
    for fn, path in paths.items():
        row = context.check(fn, work, str(path))
        if row["status"] != "ok":
            raise AssertionError(f"{fn}: {row}")
        rows[fn] = row
    return rows


def splice(text, paths, rows):
    """Same exact-statement deduplication as promote_batch.splice; temp only."""
    for fn, path in paths.items():
        pragma = f'#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_25bb0/{fn}.s")'
        if text.count(pragma) != 1:
            raise AssertionError(f"expected one passthrough for {fn}")
        have = {" ".join(line.split()) for line in text.splitlines() if line.strip()}
        decls = [s for s in rows[fn]["preamble"]
                 if s.startswith("#") or " ".join(s.split()) not in have]
        block = "".join(s + "\n" for s in decls) + extract_named_function(path, fn)
        text = text.replace(pragma, block, 1)
    return text


def build(text, work, label, expect_failure=False):
    source, obj = work / (label + ".c"), work / (label + ".o")
    source.write_text(text)
    command = [sys.executable, "tools/asm-processor/build.py", str(score.IDO / "cc"),
               "--", "mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-Iinclude",
               "--", "-c", *TU_FLAGS, "-o", str(obj), str(source)]
    proc = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if expect_failure:
        if proc.returncode == 0 or "redeclaration of 'D_800586A8'" not in proc.stderr:
            raise AssertionError("old queue declaration no longer reproduces the expected refusal")
        return next(line.strip() for line in proc.stderr.splitlines()
                    if "redeclaration of 'D_800586A8'" in line)
    if proc.returncode or not obj.is_file():
        raise AssertionError(f"{label} compile failed:\n{proc.stdout}\n{proc.stderr}")
    return obj


def compare_all(obj, names, extents, exact_names=()):
    symbols = score.symbols(obj)
    if set(symbols) != set(names):
        raise AssertionError("compiled TU function set differs from actual source slots")
    words = score.text_words(obj)
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    sizes = {symbol["name"]: symbol["size"]
             for index, section in enumerate(sections) if section["type"] == 2
             for symbol in score._symbol_table(data, sections, index)
             if symbol["section"] == text_index and symbol["type"] == 2}
    out = {}
    for fn in sorted(names):
        result = score.compare(obj, fn, show=0)
        if not result.accepted(False):
            raise AssertionError(f"{fn}: {result.summary()}")
        if result.total * 4 != extents[fn]["size"]:
            raise AssertionError(f"{fn}: target length differs from protected extent")
        if fn in exact_names and sizes[fn] != extents[fn]["size"]:
            raise AssertionError(f"{fn}: C function symbol size differs from protected extent")
        end = min((offset for offset in symbols.values() if offset > symbols[fn]),
                  default=len(words) * 4)
        out[fn] = dict(asdict(result), target_bytes=result.total * 4,
                       object_span_bytes=end - symbols[fn],
                       function_symbol_bytes=sizes[fn], strict_match=True)
    return out


def _verify(work, source_text=None, entries=None):
    if Path.cwd().resolve() != ROOT:
        raise RuntimeError("run from the repository root")
    score.ASM_DIR = ROOT / "asm/us/boot_tail"
    manifest = score.target_manifest()
    score.targets()  # Hash-protected exact retail targets; no masks/overrides.
    extents = {row["name"]: row for row in json.loads(score.verified_bytes(
        score.ASM_DIR / "extents.json", manifest))["functions"]}
    paths = candidate_paths()
    baseline_source, baseline_locks = baseline_inputs()
    if source_text is None:
        source_text = (ROOT / TU).read_text()
    if entries is None:
        entries = load_lock()
    pending, current_c, names = current_state(
        source_text, entries, baseline_source, baseline_locks)
    for fn, source in paths.items():
        key = str(source.relative_to(ROOT)) + ":" + fn
        if body_sha(source, fn) != baseline_locks[key]["body_sha256"]:
            raise AssertionError(f"{fn}: original candidate source body changed")

    standalone = {}
    for fn, path in paths.items():
        obj = work / (fn + ".standalone.o")
        score.compile_single(path, STANDALONE_FLAGS, obj)
        standalone[fn] = compare_all(obj, [fn], extents, [fn])[fn]
    rows = fit(paths, work)
    baseline = build(baseline_source, work, "baseline")
    current = build(source_text, work, "current")
    residual_paths = {fn: paths[fn] for fn in pending}
    combined_text = splice(source_text, residual_paths, rows)
    combined = build(combined_text, work, "combined")
    baseline_results = compare_all(baseline, names, extents, LOCKED)
    current_results = compare_all(current, names, extents, current_c)
    combined_results = compare_all(combined, names, extents, set(current_c) | set(FUNCTIONS))
    if not len(score.text_words(baseline)) == len(score.text_words(current)) == len(score.text_words(combined)):
        raise AssertionError("TU text/padding extent changed")
    if not score.symbols(baseline) == score.symbols(current) == score.symbols(combined):
        raise AssertionError("TU function offsets changed")
    for label, obj in (("baseline", baseline), ("current", current), ("combined", combined)):
        _, sections = score._elf(obj)
        if any(s["size"] for s in sections if s["name"] in DATA_SECTIONS):
            raise AssertionError(f"{label}: unexpected allocated data")

    # Reintroduce the historical incompatible declaration. Header-only fitting
    # still succeeds, but the actual combined TU must refuse D_800586A8.
    old_paths = {}
    for fn, path in paths.items():
        old = work / (fn + ".old.c")
        old.write_text(baseline_text(path.relative_to(ROOT)))
        old_paths[fn] = old
    old_rows = fit(old_paths, work)
    build(splice(baseline_source, old_paths, old_rows), work, "old_conflict", True)

    # A wrong blocking flag must fail the full-word comparison. This guards
    # against accidentally reducing verification to body hashes or reloc masks.
    wrong_text = combined_text.replace("osRecvMesg(&D_800586A8, 0, 1);",
                                       "osRecvMesg(&D_800586A8, 0, 0);", 1)
    if wrong_text == combined_text:
        raise AssertionError("negative control did not mutate the intended body")
    wrong = build(wrong_text, work, "wrong_blocking_flag")
    wrong_result = score.compare(wrong, FUNCTIONS[0], show=0)
    if wrong_result.accepted(False) or wrong_result.differing == 0:
        raise AssertionError("wrong blocking flag escaped strict comparison")

    files = [TU, Path("matched.lock.json"), Path("Makefile"),
             Path("src/rom/rom_tu.h"), Path("include/rom_auto.h"),
             Path("include/types.h"), Path("include/PR/os_message.h"),
             Path("include/PR/os_thread.h"), Path("include/m2c_types.h"),
             Path("tools/cloud/score.py"), Path("tools/asm-processor/build.py"),
             Path("tools/asm-processor/asm_processor.py"),
             Path("cloud/work/boot_tail_promotion/context_check.py"),
             Path(__file__).relative_to(ROOT)]
    files += [path.relative_to(ROOT) for path in paths.values()]
    files += [Path("asm/us/boot_tail") / name for name in manifest]
    files += [Path("asm/us/boot_tail/SHA256SUMS")]
    files += sorted(Path("asm/us/nonmatchings/rom/lib_25bb0").glob("*.s"))
    return {
        "result": "PASS", "baseline_commit": BASE,
        "scope": list(FUNCTIONS), "candidate_bytes": sum(extents[f]["size"] for f in FUNCTIONS),
        "standalone_flags": STANDALONE_FLAGS + " " + score.R4300_CC,
        "full_tu_flags": TU_FLAGS,
        "assembler_flags": ["-march=vr4300", "-mabi=32", "-Iinclude"],
        "standalone": standalone, "header_context": rows,
        "baseline_tu": baseline_results, "current_tu": current_results,
        "combined_tu": combined_results,
        "lifecycle": {"pending_candidates": pending,
                      "promoted_candidates": [fn for fn in FUNCTIONS if fn not in pending],
                      "current_locked_c_functions": current_c,
                      "current_tu_sha256": hashlib.sha256(source_text.encode()).hexdigest(),
                      "current_locks_sha256": hashlib.sha256(
                          json.dumps(entries, sort_keys=True).encode()).hexdigest(),
                      "all_tu_function_offsets_unchanged": True},
        "baseline_tu_sha256": hashlib.sha256(baseline_source.encode()).hexdigest(),
        "baseline_locks_sha256": hashlib.sha256(
            json.dumps(baseline_locks, sort_keys=True).encode()).hexdigest(),
        "all_tu_text_bytes": len(score.text_words(combined)) * 4,
        "locked_c_regressions": list(LOCKED),
        "all_tu_bytes": sum(extents[f]["size"] for f in names),
        "all_tu_functions": len(names), "allocated_data_bytes": 0,
        "failure_controls": {
            "old_declaration_header_only": "passes; insufficient promotion evidence",
            "old_declaration_full_tu": "redeclaration of 'D_800586A8' (expected refusal)",
            "wrong_blocking_flag": asdict(wrong_result)},
        "source_sha256": {str(path): digest(ROOT / path) for path in files},
        "compiler_sha256": {path.name: digest(path) for path in sorted(score.IDO.iterdir())
                            if path.is_file()},
        "assembler_sha256": digest(shutil.which("mips-linux-gnu-as")),
        "full_rom_gate": "NOT RUN; maintainer ROM/build inputs required",
        "promotion": "NOT PERFORMED; production TU and locks unchanged",
    }


def verify(work, source_text=None, entries=None):
    previous_targets = score.ASM_DIR
    try:
        return _verify(work, source_text, entries)
    finally:
        score.ASM_DIR = previous_targets


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix="queue-wrapper-check-") as tmp:
        report = verify(Path(tmp))
    output = json.dumps(report, indent=2, sort_keys=True) + "\n"
    if args.output:
        args.output.write_text(output)
    else:
        print(output, end="")


if __name__ == "__main__":
    main()
