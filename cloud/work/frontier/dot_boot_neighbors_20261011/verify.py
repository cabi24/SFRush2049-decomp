#!/usr/bin/env python3
"""Replay the 172-byte static match without a ROM or coordinator database."""
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers

FN = "func_80022324"
TU = "lib_22300"
SOURCE = ROOT / "cloud/matches/boot_tail/func_80022324.c"
FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def sha(data):
    return hashlib.sha256(data).hexdigest()


def inputs():
    score.ASM_DIR = ROOT / "asm/us/boot_tail"
    words = score.targets()[FN]
    extent = next(row for row in json.loads((score.ASM_DIR / "extents.json").read_text())["functions"]
                  if row["name"] == FN)
    names = ("D_8004BEB8", "D_8004FA18", "func_80021700")
    symbols = score.image_symbols()
    return {"source_sha256": sha(SOURCE.read_bytes()),
            "verify_sha256": sha(Path(__file__).read_bytes()),
            "target_words_sha256": sha(struct.pack(">%dI" % len(words), *words)),
            "target_extent": extent, "target_symbols": {n: symbols.get(n, score.address_named(n)) for n in names}}


def command(args):
    proc = subprocess.run([str(x) for x in args], cwd=ROOT, text=True,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    assert proc.returncode == 0, "command failed: " + proc.stdout + proc.stderr
    return proc.stdout


def signature(obj):
    """Allocated code/data and symbolic REL rows, excluding debug and ABI metadata."""
    data, sections = score._elf(obj)
    included = {".text", ".data", ".rodata", ".rdata", ".bss", ".sdata", ".sbss", ".lit4", ".lit8"}
    result = {}
    for index, sec in enumerate(sections):
        if sec["name"] not in included or not sec["size"]:
            continue
        payload = bytes(sec["size"]) if sec["type"] == 8 else data[sec["off"]:sec["off"] + sec["size"]]
        relocs = []
        for reloc in sections:
            if reloc["type"] != 9 or reloc["info"] != index:
                continue
            table = score._symbol_table(data, sections, reloc["link"])
            for offset in range(reloc["off"], reloc["off"] + reloc["size"], 8):
                location, info = struct.unpack_from(">II", data, offset)
                sym = table[info >> 8]
                relocs.append([location, info & 255, sym["name"], sym["value"]])
        result[sec["name"]] = {"size": sec["size"], "sha256": sha(payload), "relocations": sorted(relocs)}
    return result


def run():
    pinned = inputs()
    assert pinned["target_extent"]["size"] == 172
    cc = compilers.tu_cc(TU, score.IDO / "cc")
    assert compilers.identify_dir(cc.parent) == "ido7.1"
    original_ido = score.IDO
    score.IDO = cc.parent
    context = load("boot_neighbor_context", ROOT / "cloud/work/boot_tail_promotion/context_check.py")
    context.CC = cc
    context.COMPILER = "ido7.1"
    with tempfile.TemporaryDirectory(prefix="boot-neighbor-") as raw:
        work = Path(raw)
        obj = work / "single.o"
        score.compile_single(SOURCE, FLAGS, obj)
        proof = score.compare(obj, FN, show=0)
        assert proof.accepted() and proof.total == 43 and proof.extra_words == 0
        words = score.text_words(obj)
        resolved, masks, unresolved, unverified, errors = score.relocate(
            obj, words, 0, len(words) * 4, score.image_symbols())
        assert not any((masks, unresolved, unverified, errors))
        assert resolved[:43] == score.targets()[FN] and not any(resolved[43:])
        assert not context.data_sections(obj)
        data, sections = score._elf(obj)
        symbols = [symbol for i, section in enumerate(sections) if section["type"] == 2
                   for symbol in score._symbol_table(data, sections, i) if symbol["name"] == FN]
        assert len(symbols) == 1 and symbols[0]["size"] == 172

        # Independently assemble the symbolic native target with GNU as.
        native = ROOT / ("asm/us/nonmatchings/rom/" + TU + "/" + FN + ".s")
        assembly = work / "target.s"
        assembly.write_text((ROOT / "tools/asm-processor/prelude.inc").read_text()
                            + "\n.text\n.type " + FN + ", @function\n" + native.read_text())
        target = work / "target.o"
        command(["mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-Iinclude", "-o", target, assembly])
        assert context.disasm(target, FN) == context.disasm(obj, FN)
        assert signature(target) == signature(obj)

        # The code-neutral narrowing is necessary, rather than an unverified claim.
        control = work / "control.c"
        text = SOURCE.read_text()
        needle = "(u32)(u16)(command->word[0] >> 16) << 16"
        assert text.count(needle) == 1
        control.write_text(text.replace(needle, "(command->word[0] >> 16) << 16"))
        control_obj = work / "control.o"
        score.compile_single(control, FLAGS, control_obj)
        rejected = score.compare(control_obj, FN, show=0)
        assert not rejected.accepted() and rejected.differing == 6 and rejected.extra_words == 0

        row = context.check(FN, work, str(SOURCE.relative_to(ROOT)))
        assert row["status"] == "ok", row
        row.update(function=FN, compiler="ido7.1", flagset=FLAGS)

        # Replay promotion only into a temporary TU, using the current master inputs.
        baseline = ROOT / ("src/rom/" + TU + ".c")
        pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/' + TU + '/' + FN + '.s")'
        temporary = work / (TU + ".c")
        if pragma in baseline.read_text():
            promote = load("boot_neighbor_promote", ROOT / "cloud/work/boot_tail_promotion/promote_batch.py")
            promote.PREFIX_DEDUP = {"rom/" + TU}
            promote.splice([{"fn": FN, "seg": {"rom_tu": "rom/" + TU, "flagset": FLAGS},
                             "src": str(SOURCE.relative_to(ROOT)), "row": row, "slot": None}], out_dir=work)
        else:
            # A coordinator promotion is compatible; do not require an old master.
            assert re.search(r"\b" + FN + r"\s*\([^;]*?\)\s*\{", baseline.read_text(), re.S)
            temporary.write_text(baseline.read_text())
        tuples = []
        for label, source in (("before", baseline), ("after", temporary)):
            output = work / (label + ".o")
            command([sys.executable, "tools/asm-processor/build.py", cc, "--",
                     "mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-Iinclude", "--",
                     "-c", *context.cflags_for(FLAGS), "-o", output, source])
            tuples.append(signature(output))
        assert tuples[0] == tuples[1], "current whole TU differs after scratch promotion"
        result = {"result": "MATCH", "population": "static", "function": FN,
                  "newly_matched_bytes": 172, "landed_rom_bytes_claimed": 0,
                  "compiler": "ido7.1", "flags": FLAGS, "inputs": pinned,
                  "canonical": proof.summary(), "native_extent_bytes": 172,
                  "object_symbol_bytes": 172, "zero_alignment_bytes": len(words) * 4 - 172,
                  "gnu_symbolic_relocations": len(signature(obj)[".text"]["relocations"]),
                  "gnu_object_equal": True, "own_data_sections": [],
                  "control_differing_words": rejected.differing,
                  "rom_tu_context": row, "current_whole_tu_equal": True,
                  "whole_tu_text_bytes": tuples[0][".text"]["size"]}
    score.IDO = original_ido
    return result


if __name__ == "__main__":
    result = run()
    if len(sys.argv) == 2:
        Path(sys.argv[1]).write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))
