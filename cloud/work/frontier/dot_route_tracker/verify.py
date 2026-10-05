#!/usr/bin/env python3
"""Rebuild the clean real caller/helper group; retain metadata, never native bytes.

Run from any directory with IDO_DIR pointing to the pinned IDO 5.3 toolchain.
--write refreshes verification.json; the default checks the recorded source and
result binding. The unchanged canonical scorer remains the admission authority.
"""
import argparse
import contextlib
import dataclasses
import hashlib
import io
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata

BASE = "52904b374dc5eb8e88d5e003d6ea4666cdde71d2"
CONTEXT = ["func_800BA2B8", "func_800BA61C", "audio_channel_alloc"]
TARGETS = ["audio_mixer_main", "audio_priority_find"]
FLAGS = "-g0 -O3 -mips2 -G 0 -non_shared"


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def run():
    lock = json.loads((ROOT / "blob_matched.lock.json").read_text())
    inputs = {str((HERE / "group.c").relative_to(ROOT)): sha(HERE / "group.c")}
    for name in CONTEXT:
        path = ROOT / lock[name]["source"]
        assert sha(path) == lock[name]["source_sha256"], name + ": accepted source drift"
        inputs[str(path.relative_to(ROOT))] = sha(path)
    recipe = dict(files=["group.c"] + [n + ".c" for n in CONTEXT],
                  members=TARGETS, context=CONTEXT,
                  keep=["audio_mixer_main"] + CONTEXT, flags=FLAGS, claims=[])
    image = owndata.ImageData.from_artifact(ROOT / "asm/us/blob_data")
    assert image is not None, "owned-data artifact unavailable"
    proof = dict(schema=1, base=BASE, claims=[], scope="research-only real five-body group",
                 sources=inputs, recipe=recipe,
                 scorer_sha256=sha(ROOT / "tools/cloud/score.py"),
                 owndata_sha256=sha(ROOT / "tools/cloud/owndata.py"),
                 target_manifest_sha256=sha(ROOT / "asm/us/blob/SHA256SUMS"),
                 data_manifest_sha256=sha(ROOT / "asm/us/blob_data/SHA256SUMS"),
                 toolkit_id_from_context_lock=lock[CONTEXT[0]]["toolkit_sha"],
                 tool_executable_sha256={}, results={})
    for name in ["cc", "cfe", "uld", "usplit", "umerge", "uopt", "ugen", "as1"]:
        proof["tool_executable_sha256"][name] = sha(score.ido(name))
    with tempfile.TemporaryDirectory(prefix="route-tracker-proof-") as tmp:
        work = Path(tmp)
        shutil.copyfile(HERE / "group.c", work / "group.c")
        for name in CONTEXT:
            shutil.copyfile(ROOT / lock[name]["source"], work / (name + ".c"))
        (work / "group.json").write_text(json.dumps(recipe))
        obj = work / "group.o"
        score.compile_group(work, obj)
        proof["object_sha256"] = sha(obj)
        words = score.text_words(obj)
        starts = score.symbols(obj)
        for name in TARGETS + CONTEXT:
            with contextlib.redirect_stdout(io.StringIO()):
                result = score.compare(obj, name, show=0)
            own = owndata.verify(obj, name, score.targets()[name],
                                 address=score.image_symbols()[name], image=image,
                                 addresses=score.image_symbols())
            end = min((v for v in starts.values() if v > starts[name]), default=len(words) * 4)
            first = words[starts[name] // 4]
            frame = ((first & 0xffff) ^ 0x8000) - 0x8000 if first >> 16 == 0x27bd else None
            proof["results"][name] = dict(comparison=dataclasses.asdict(result),
                                          canonical_summary=result.summary(),
                                          canonical_strict_match=result.accepted(),
                                          symbol_slice_bytes=end - starts[name],
                                          entry_stack_adjust=frame,
                                          own_data=dict(ok=own.ok, references=own.references,
                                                        sites=len(own.sites), notes=own.notes,
                                                        failures=own.failures, unverified=own.unverified))
        assert proof["results"]["audio_priority_find"]["canonical_strict_match"]
        for name in CONTEXT:
            row = proof["results"][name]
            c = row["comparison"]
            assert c["differing"] == 0 and c["extra_words"] == 0
            assert not c["unresolved"] and not c["errors"] and row["own_data"]["ok"]
            assert row["canonical_strict_match"], name + ": canonical context proof failed"

        # A temporary native object is only a diagnostic input, never an output.
        name = "audio_mixer_main"
        native = work / "native.s"
        native.write_text(".set noreorder\n.text\n.globl " + name + "\n.type " + name
                          + ",@function\n" + name + ":\n"
                          + "".join(".word 0x%08x\n" % w for w in score.targets()[name])
                          + ".size " + name + ",.-" + name + "\n")
        subprocess.run(["mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-EB",
                        "-o", str(work / "native.o"), str(native)], check=True, capture_output=True)
        diag = subprocess.run([sys.executable, str(ROOT / "tools/workbench.py"), "diagnose",
                               str(work / "native.o"), str(obj), "--function", name,
                               "--objdump", os.environ.get("MIPS_OBJDUMP", "mips-linux-gnu-objdump"),
                               "--pager", "never", "--color", "never"],
                              check=True, capture_output=True, text=True).stdout
        # Store only high-level evidence; full diagnostic includes native instructions.
        match = re.search(r"verdict=([\w-]+).*?true_insn_delta=([+-]?\d+)", diag)
        assert match, "diagnostic format changed"
        proof["diagnosis"] = dict(verdict=match[1], true_instruction_delta=int(match[2]),
                                  target_frame=80, candidate_frame=72,
                                  save_bytes_target=40, save_bytes_candidate=40,
                                  causal_next_variant=None,
                                  reason="Frame provenance and second-loop CSE remain unexplained. No further variants run.",
                                  limitation="Workbench comparison uses a relocation-free native object; canonical scorer owns exact word results.")
        assert "frame mismatch: target=-80 candidate=-72" in diag
        assert "save-slots=40->40 bytes; non-save=40->32 bytes" in diag
    return proof


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--write", action="store_true")
    args = parser.parse_args()
    proof = run()
    saved = HERE / "verification.json"
    if args.write:
        saved.write_text(json.dumps(proof, indent=2, sort_keys=True) + "\n")
    else:
        expected = json.loads(saved.read_text())
        assert proof == expected, "fresh proof differs from source-bound receipt"
    for name, result in proof["results"].items():
        print(name + ": " + result["canonical_summary"])
    print("Research replay PASS; no match or ROM claims.")


if __name__ == "__main__":
    main()
