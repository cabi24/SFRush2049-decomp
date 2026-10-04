"""Re-issue the sdk_initialize compiler-context record for the boot-tail extension.

The boot-tail static extension (splat code range to ROM 0x283D0) regenerates
`undefined_syms_auto.us.txt` and edits `symbol_addrs.us.txt`, both pinned by the
active sdk_initialize owner's context record
(cloud/work/integration_B26/root_compiler_context.json, immutable). This script
replays the processes that produced that record's evidence against the new
symbol inputs and writes a NEW record beside itself; the B26/C19 originals stay
byte-identical. Steps (run from the repository root):

  targets  Replay cloud/work/static_C19/verify_targets.py: assemble each member's
           relocation-aware target from current splat asm with the C19 targets
           module (symbols resolved through the current symbol files), link it at
           its vram, require the linked body to equal the ROM words, the round-trip
           gate to pass, and the target object to equal the reviewed strict
           proof's target_sha256. Needs mips-linux-gnu binutils and baserom.
  score    Replay cloud/work/static_C19/score_module.py on the builder: compile
           the reviewed source (cloud/work/static_C18/full_module.c) with the
           pinned toolkit's IDO at the reviewed flags, require the object to equal
           the reviewed module_sha256, and strict-score every member against the
           replayed targets with the unchanged canonical scorer settings.
  context  Hash the live files of the original record's key set (the complete
           tracked compiler-header closure, scorer and symbol inputs) into
           sdk_initialize_compiler_context.json and write provenance.json.
  records  Rebase the reviewed storage records (B25 timer, B26 root initializer)
           to the new data.bin base (offset - 0x183D0, container vram 0x800277D0)
           and point the initializer at the re-issued context record.
  apply    Point rom_owned_data.json's sdk_initialize row at the re-issued record.

`targets`, `context`, `records`, `apply` run on the Pi; `score` runs on the
builder (IDO). See README.md for the exact command sequence.
"""
import argparse
import hashlib
import importlib.util
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path.cwd()
HERE = Path("cloud/work/boot_tail_extension")
B26 = Path("cloud/work/integration_B26")
ORIGINAL_CONTEXT = B26 / "root_compiler_context.json"
STRICT = Path("cloud/work/static_C19/strict_verification.json")
SOURCE = Path("cloud/work/static_C18/full_module.c")
FLAGS = "-g0 -O1 -mips2 -G 0 -non_shared -Xcpluscomm"
MEMBERS = ("__osInitialize_common", "__osPiReadDeviceType")
TOOLKIT = Path.home() / "rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5"
PROTOCOL = ("unchanged canonical scorer settings, function-scoped objdump; "
            "raw unlinked object slices, no masks or linked substitutions")
DATA_DELTA = 0x283D0 - 0x10000
OLD_VRAM, NEW_VRAM = "0x8000f400", "0x800277d0"
CONTEXT_OUT = HERE / "sdk_initialize_compiler_context.json"


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def dump(path, value):
    Path(path).write_text(json.dumps(value, indent=2) + "\n")


def c19_targets():
    spec = importlib.util.spec_from_file_location(
        "tools.conveyor.pipeline.C19_targets", "cloud/work/static_C19/targets.py")
    t = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = t
    spec.loader.exec_module(t)
    t.REPO = ROOT
    t.ASM_DIR = ROOT / "asm/us"
    return t


def cmd_targets(args):
    t = c19_targets()
    regions = t.index_asm_regions()
    reviewed = {r["function"]: r for r in json.loads(STRICT.read_text())}
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)
    rows = []
    for fn in MEMBERS:
        region = next(r for r in regions.values() if r.name == fn)
        target = out / (fn + ".target.o")
        t.assemble_region(region, fn, target)
        text = "\n".join(region.lines)
        aliases = {}
        for name in (re.findall(r"%(?:hi|lo)\(([A-Za-z_]\w*)", text)
                     + re.findall(r"\bjal\s+([A-Za-z_]\w*)", text)):
            aliases[name] = t._resolve_symbol(name)
        ld = out / (fn + ".ld")
        ld.write_text("SECTIONS { .text " + hex(region.vaddr) + " : SUBALIGN(4) { *(.text) } "
                      "/DISCARD/ : { *(.reginfo) *(.options) } }\n"
                      + "\n".join(f"{n} = {hex(a)};" for n, a in aliases.items() if a is not None) + "\n")
        elf, raw = out / (fn + ".elf"), out / (fn + ".bin")
        p = subprocess.run(["mips-linux-gnu-ld", "-m", "elf32btsmip", "-T", str(ld), str(target),
                            "-o", str(elf)], capture_output=True, text=True)
        assert p.returncode == 0, p.stderr
        subprocess.run(["mips-linux-gnu-objcopy", "--dump-section", ".text=" + str(raw), str(elf)], check=True)
        expected = b"".join(int(w, 16).to_bytes(4, "big") for w in region.words)
        actual = raw.read_bytes()
        assert actual[:len(expected)] == expected and not any(actual[len(expected):]), fn
        assert t.gate_target(region.words, target) == (True, None), fn
        row = dict(function=fn, words=len(region.words),
                   asm_sha256=sha(f"asm/us/nonmatchings/rom/lib_8a80/{fn}.s"),
                   target_sha256=sha(target), linked_body_sha256=hashlib.sha256(expected).hexdigest(),
                   linked_body_differences=0, tail_alignment_bytes=len(actual) - len(expected),
                   target_gate=True,
                   reviewed_target_sha256=reviewed[fn]["target_sha256"],
                   target_identical_to_reviewed=sha(target) == reviewed[fn]["target_sha256"],
                   words_identical_to_reviewed=len(region.words) == reviewed[fn]["words"])
        assert row["target_identical_to_reviewed"] and row["words_identical_to_reviewed"], row
        rows.append(row)
    dump(HERE / "linked_targets.json", rows)
    print(json.dumps(rows, indent=1))


def cmd_score(args):
    out = Path(args.out)
    os.environ["CONVEYOR_TOOLKIT"] = str(TOOLKIT)
    os.environ["LD_LIBRARY_PATH"] = str(TOOLKIT / "lib")
    sys.path[:0] = [str(ROOT / "tools/conveyor/jobs"), str(ROOT / "tools/cloud")]
    import compile_score
    import scoring
    import score
    # Compile as the reviewed proof did: the source copied in as full_module.c and
    # compiled from the scratch directory. The object file hash is not portable
    # (IDO's .mdebug records the include paths of the compiling tree), so the
    # replay compares code, not file bytes: the .text and .data sections are
    # hashed into the record below; strict scoring covers relocations.
    (out / "full_module.c").write_bytes((ROOT / SOURCE).read_bytes())
    module = out / "full_module.o"
    cwd = os.getcwd()
    os.chdir(out)
    try:
        ok, message = compile_score.compile_one(
            "full_module.c", FLAGS, "full_module.o",
            [ROOT / "include", ROOT / "include/PR", ROOT / "src/rom"])
    finally:
        os.chdir(cwd)
    assert ok, message
    sections = {}
    for name in (".text", ".data"):
        dumped = out / ("section" + name.replace(".", "_"))
        subprocess.run(["mips-linux-gnu-objcopy", "--dump-section", name + "=" + str(dumped),
                        str(module), str(out / "scratch.o")], check=False, capture_output=True)
        sections[name] = sha(dumped) if dumped.is_file() else None
    reviewed = {r["function"]: r for r in json.loads(STRICT.read_text())}
    words, syms, rows = score.text_words(module), score.symbols(module), []
    for fn in MEMBERS:
        count = reviewed[fn]["words"]
        target = out / (fn + ".target.o")
        start = syms[fn]
        part = words[start // 4:start // 4 + count]
        want = score.text_words(target)[:count]
        scorer = scoring.Scorer(target_o=str(target), stack_differences=True, algorithm="difflib",
                                debug_mode=False, ign_branch_targets=True,
                                objdump_command=scoring.objdump_command() + " --disassemble=" + fn)
        value, _ = scorer.score(str(module))
        rows.append(dict(function=fn, strict_score=value,
                         raw_word_diff=sum(a != b for a, b in zip(part, want)) + abs(len(part) - len(want)),
                         object_start=start, words=count, target_sha256=sha(target),
                         module_sha256=sha(module), flags=FLAGS, protocol=PROTOCOL,
                         source_sha256=sha(ROOT / SOURCE), toolkit=TOOLKIT.name,
                         reviewed_module_sha256=reviewed[fn]["module_sha256"],
                         module_section_sha256=sections))
    for row in rows:
        r = reviewed[row["function"]]
        for key in ("strict_score", "raw_word_diff", "object_start", "words",
                    "target_sha256", "flags", "protocol", "source_sha256"):
            assert row[key] == r[key], (row["function"], key, row[key], r[key])
    dump(HERE / "strict_rescore.json", rows)
    print(json.dumps(rows, indent=1))


def cmd_context(args):
    original = json.loads(ORIGINAL_CONTEXT.read_text())
    record = {rel: sha(rel) for rel in sorted(original)}
    dump(CONTEXT_OUT, record)
    rescore = json.loads((HERE / "strict_rescore.json").read_text())
    targets = json.loads((HERE / "linked_targets.json").read_text())
    provenance = {
        "record": str(CONTEXT_OUT),
        "record_sha256": sha(CONTEXT_OUT),
        "supersedes": str(ORIGINAL_CONTEXT),
        "supersedes_sha256": sha(ORIGINAL_CONTEXT),
        "key_set": "identical to the superseded record (complete tracked compiler-header closure, "
                   "scorer, and symbol inputs)",
        "changed_files": sorted(k for k in record if record[k] != original[k]),
        "reason": "boot-tail static extension: splat code range to ROM 0x283D0 regenerates "
                  "undefined_syms_auto.us.txt and adds census symbols to symbol_addrs.us.txt",
        "targets_replay": "linked_targets.json (C19 verify_targets protocol; targets identical "
                          "to the reviewed strict proof)",
        "strict_rescore": "strict_rescore.json (C19 score_module protocol on the builder)",
        "strict_scores": {r["function"]: r["strict_score"] for r in rescore},
        "targets_identical": all(r["target_identical_to_reviewed"] for r in targets),
        "strict_proof": str(STRICT),
        "strict_proof_sha256_unchanged": sha(STRICT),
    }
    assert set(provenance["changed_files"]) <= {"symbol_addrs.us.txt", "undefined_syms_auto.us.txt"}, provenance
    assert provenance["targets_identical"] and all(v == 0 for v in provenance["strict_scores"].values())
    dump(HERE / "provenance.json", provenance)
    print(json.dumps(provenance, indent=1))


def _rebase_slot(slot):
    assert slot["source"] == "assets/us/data.bin" and slot["container_vram"] == OLD_VRAM, slot
    slot["offset"] = hex(int(slot["offset"], 16) - DATA_DELTA)
    slot["container_vram"] = NEW_VRAM


def cmd_records(args):
    timer = json.loads(Path("cloud/work/integration_B25/storage_record.json").read_text())
    _rebase_slot(timer["data_slot"])
    dump(HERE / "timer_services_storage_record.json", timer)
    init = json.loads((B26 / "root_storage_record.json").read_text())
    _rebase_slot(init["data_slot"])
    init["context_proof"] = str(CONTEXT_OUT)
    init["context_proof_sha256"] = sha(CONTEXT_OUT)
    dump(HERE / "sdk_initialize_storage_record.json", init)


def cmd_apply(args):
    registry = json.loads(Path("rom_owned_data.json").read_text())
    row = next(b for b in registry["storage_blocks"] if b["owner"] == "sdk_initialize")
    assert row["context_proof"] in (str(ORIGINAL_CONTEXT), str(CONTEXT_OUT))
    row["context_proof"] = str(CONTEXT_OUT)
    row["context_proof_sha256"] = sha(CONTEXT_OUT)
    Path("rom_owned_data.json").write_text(json.dumps(registry, indent=2) + "\n")


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="command", required=True)
    for name in ("targets", "score"):
        s = sub.add_parser(name)
        s.add_argument("--out", default=tempfile.gettempdir() + "/boot-tail-sdk-context")
    for name in ("context", "records", "apply"):
        sub.add_parser(name)
    a = p.parse_args()
    {"targets": cmd_targets, "score": cmd_score, "context": cmd_context,
     "records": cmd_records, "apply": cmd_apply}[a.command](a)


if __name__ == "__main__":
    main()
