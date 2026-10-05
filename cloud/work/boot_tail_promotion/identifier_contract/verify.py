#!/usr/bin/env python3
"""Read-only, local actual-TU regression for the identifier declaration repair.

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

BASE = "abc0f256e8c6b5e2004662af09aca7c5bf915204"
TU = "src/rom/lib_207b0.c"
CANDIDATE = "func_800201D0"
SOURCE = "cloud/matches/boot_tail/func_800201D0.c"
OLD_DECL = "extern u32 func_8001EDF4(u32);"
NEW_DECL = "extern int func_8001EDF4(u32);"
PRAGMA = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_207b0/func_800201D0.s")'
CFLAGS = ["-G", "0", "-mips2", "-O2", "-non_shared", "-Iinclude",
          "-Iinclude/PR", "-D_LANGUAGE_C", "-Wab,-r4300_mul", "-Xcpluscomm",
          "-Isrc/rom"]
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


def overlay(tu, source):
    require(tu.count(PRAGMA) == 1, "candidate must have exactly one passthrough slot")
    # Keep the original verified standalone preamble. The duplicate signed
    # declaration is compatible; the old unsigned one deliberately is not.
    return tu.replace(PRAGMA, source.rstrip(), 1)


def compile_tu(text, out, name, expect_error=False):
    source, obj = out / (name + ".c"), out / (name + ".o")
    source.write_text(text)
    command = [sys.executable, REPO / "tools/asm-processor/build.py", score.ido("cc"),
               "--", "mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-Iinclude",
               "--", "-c", *CFLAGS, "-o", obj, source]
    result = run(command)
    log = result.stdout + result.stderr
    if expect_error:
        require(result.returncode != 0 and "redeclaration of 'func_8001EDF4'" in log,
                "the original declaration conflict was not reproduced")
        return {"rejected": True, "reason": "redeclaration of 'func_8001EDF4'"}
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


def _verify(out):
    out = Path(out).resolve()
    out.mkdir(parents=True, exist_ok=True)
    original = checked(["git", "show", BASE + ":" + TU])
    current = (REPO / TU).read_text()
    require(original.count(OLD_DECL) == 1, "unexpected original declaration")
    require(OLD_DECL not in current and NEW_DECL in current,
            "the current TU must retain the compatible signed declaration")
    base_locks = json.loads(checked(["git", "show", BASE + ":matched.lock.json"]))
    baseline = sorted(k.split(":")[1] for k in base_locks if k.startswith(TU + ":"))
    require(len(baseline) == 17, "expected the 17 accepted bodies from the reviewed base")
    locks = json.loads((REPO / "matched.lock.json").read_text())
    accepted = sorted(k.split(":")[1] for k in locks if k.startswith(TU + ":"))
    require(set(baseline) <= set(accepted), "an existing accepted body lost its lock")
    for name in accepted:
        require(body_sha(REPO / TU, name) == locks[TU + ":" + name]["body_sha256"],
                name + ": locked body changed")
    require(body_sha(REPO / SOURCE, CANDIDATE) == base_locks[SOURCE + ":" + CANDIDATE]["body_sha256"],
            "standalone candidate body changed from the reviewed input")
    score.ASM_DIR = REPO / "asm/us/boot_tail"
    targets, addresses = score.targets(), score.image_symbols()
    source = (REPO / SOURCE).read_text()
    rejected = compile_tu(overlay(original, source), out, "original_overlay", expect_error=True)
    # Future maintainer promotions may add C bodies or migrate this candidate's
    # lock. Continue testing the whole actual TU rather than freezing its text.
    combined = current if CANDIDATE in accepted else overlay(current, source)
    stages = {}
    for name, text, required in (
        ("baseline", original, baseline),
        ("repaired", current, accepted),
        ("candidate_overlay", combined, sorted(set(accepted) | {CANDIDATE})),
    ):
        obj = compile_tu(text, out, name)
        stages[name] = check_object(obj, required, out, targets, addresses)
    require(stages["baseline"]["linked_text_sha256"] == stages["repaired"]["linked_text_sha256"]
            == stages["candidate_overlay"]["linked_text_sha256"],
            "complete linked TU text changed")
    standalone = out / "standalone.o"
    score.compile_single(REPO / SOURCE, score.DEFAULT_FLAGS, standalone)
    want = struct.pack(">12I", *targets[CANDIDATE])
    got = relocated_bytes(standalone, CANDIDATE, 48, addresses)
    require(got == want, "standalone candidate no longer matches")
    result = {"base_commit": BASE, "translation_unit": TU, "candidate": CANDIDATE,
              "flags": CFLAGS, "negative_control": rejected,
              "standalone_candidate": {"function_bytes": 48, "match": True},
              "stages": stages, "rom_gate": "not run; maintainer transaction required",
              "verification_script_sha256": sha(Path(__file__).read_bytes()),
              "baseline_source_sha256": sha(original.encode()),
              "source_sha256": {p: sha((REPO / p).read_bytes()) for p in [TU, SOURCE]},
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
    # Importing this local regression must not retarget other scorer tests.
    old = score.ASM_DIR, score._targets, score._target_fingerprint
    try:
        return _verify(out)
    finally:
        score.ASM_DIR, score._targets, score._target_fingerprint = old


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path, default=REPO / "build/identifier_contract")
    args = parser.parse_args()
    try:
        result = verify(args.out)
    except (OSError, VerificationError) as exc:
        print(str(exc), file=sys.stderr)
        return 1
    for name, stage in result["stages"].items():
        print(f"{name}: {len(stage['functions'])} exact C extents and relocated bodies; "
              f"{stage['all_tu_slots_verified']} complete TU slots MATCH")
    print("original overlay: expected declaration conflict reproduced")
    print("standalone candidate: 48 bytes MATCH")
    print("ROM gate: not run; maintainer transaction required")
    return 0


if __name__ == "__main__":
    sys.exit(main())
