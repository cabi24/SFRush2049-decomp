#!/usr/bin/env python3
"""Opt-in priority experiment with stock evidence and separate traced diagnostics.

Supply a locally built diagnostic uopt. This script never downloads tools and
never uses its oracle as matching evidence. Raw intermediate files are temporary.
"""
import argparse
import dataclasses
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import tempfile
import verify as v

COMMON = ["-mips2", "-EB", "-g0", "-O3"]
READ = "                if (player) {} /* Code-free allocation-priority read; see header. */\n"

def run(args, directory, env=None):
    r = subprocess.run(list(map(str, args)), cwd=directory, env=env, capture_output=True, text=True)
    v.require(r.returncode == 0, (args[0], r.stderr))
    return r

def prepare(directory, source):
    (directory / "group.c").write_text(source)
    (directory / "keep.txt").write_text("func_803A1EAC\n")
    for args in [
        ["cc", "-j", *shlex.split(v.FLAGS), "group.c"],
        ["uld", "-L/usr/lib/mips2/nonshared", "-_SYSTYPE_SVR4", "-mips2", "-non_shared", "-g0", "-no_AutoGnum", "-kp", "keep.txt", "group.u", "-ko", "linked"],
        ["usplit", "-mips2", "-o", "split", "-t", "st", "linked"],
        ["umerge", "-Olimit", "5000", *COMMON, "split", "-o", "merged", "-t", "st"],
    ]:
        run([v.score.IDO / args[0], *args[1:]], directory)
    shutil.copy(directory / "st", directory / "st.initial")

def finish(directory, uopt, environment):
    shutil.copy(directory / "st.initial", directory / "st")
    result = run([uopt, "-G", "0", "-Olimit", "5000", *COMMON, "merged", "opt", "-t", "st", "optlog"], directory, environment)
    for args in [
        ["ugen", "-G", "0", *COMMON, "opt", "-o", "gen", "-t", "st", "-temp", "ugtmp"],
        ["as1", "-elf", "-G", "0", "-p0", *COMMON, v.score.R4300_AS1, "-Olimit", "5000", "gen", "-o", "candidate.o", "-t", "st"],
    ]:
        run([v.score.IDO / args[0], *args[1:]], directory)
    return dataclasses.asdict(v.score.compare(directory / "candidate.o", v.NAME, show=0)), result.stderr

def replay(tracer):
    v.inputs()
    v.require(v.compilers.identify_dir(v.score.IDO) == "ido5.3", "stock IDO identity failed")
    source = (v.GROUP / "group.c").read_text()
    v.require(source.count(READ) == 1, "source lever changed")
    baseline = source.replace(READ, "")
    variants = {
        "baseline": baseline,
        "panel_holder": source,
        "slot_holder_control": baseline.replace("for(i=0;i!=44;i++) {", "for(i=0;i!=44;i++) {\n            if (player) {}"),
    }
    output = {}
    with tempfile.TemporaryDirectory(prefix="cleanup-priority-research-") as td:
        for name, text in variants.items():
            directory = Path(td) / name
            directory.mkdir()
            prepare(directory, text)
            stock, _ = finish(directory, v.score.IDO / "uopt", dict(os.environ))
            expected = {"baseline": 18, "panel_holder": 0, "slot_holder_control": 7}[name]
            v.require(stock["differing"] == expected and stock["total"] == 169, (name, stock))
            stock_opt = (directory / "opt").read_bytes()
            stock_obj = (directory / "candidate.o").read_bytes()
            environment = dict(os.environ, CDX_LOG="1", CDX_OUT=str(directory / "cdx"),
                               W5D_PROC=v.NAME, W5D_OUT=str(directory / "w5d"), SAVELOG="1")
            traced, save = finish(directory, tracer, environment)
            v.require(stock_opt == (directory / "opt").read_bytes() and stock_obj == (directory / "candidate.o").read_bytes(), "tracer fidelity failed")
            # Compiler decisions only. Never retain raw native/disassembly/ucode data.
            decisions = [line for line in (directory / "cdx").read_text().splitlines() if " p1dec " in line]
            saves = [line for line in save.splitlines() if line.startswith("[SAVE] lr=13 ")]
            output[name] = {"source_sha256": v.sha(text.encode()), "canonical_stock_score": stock,
                            "traced_opt_and_complete_object_equal_stock": True,
                            "allocation_decisions": decisions, "player_save_inputs": saves}
            if name == "baseline":
                environment["PRIO"] = "13:1.75"
                oracle, _ = finish(directory, tracer, environment)
                v.require(oracle["differing"] == 0, oracle)
                output[name]["diagnostic_priority_oracle"] = {"PRIO": "13:1.75", "score": oracle,
                                                            "match_evidence": False}
    return output

if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("--uopt", type=Path, required=True)
    p.add_argument("--output", type=Path)
    a = p.parse_args()
    report = json.dumps(replay(a.uopt.resolve()), indent=2) + "\n"
    if a.output:
        a.output.write_text(report)
    print(report)
