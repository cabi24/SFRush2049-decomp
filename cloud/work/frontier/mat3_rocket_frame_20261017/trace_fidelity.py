"""Prove stock/traced-uopt complete-object fidelity, retaining raw output only in build/."""
import argparse
import hashlib
import json
import os
import shutil
import subprocess
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]


def require(condition, message):
    if not condition:
        raise ValueError(message)


def run(out, traced_uopt):
    out = Path(out).resolve()
    out.mkdir(parents=True, exist_ok=True)
    group = HERE / "group"
    spec = json.loads((group / "group.json").read_text())
    ido = Path(os.environ.get("IDO_DIR", ROOT / "tools/cloud/ido")).resolve()
    common = ["-mips2", "-EB", "-g0", "-O3"]
    for name in spec["files"]:
        shutil.copyfile(group / name, out / name)
    (out / "keep.txt").write_text("".join(name + "\n" for name in spec["keep"]))
    steps = [["cc", "-j", *spec["flags"].split(), *spec["files"]],
             ["uld", "-L/usr/lib/mips2/nonshared", "-_SYSTYPE_SVR4", "-mips2", "-non_shared",
              "-g0", "-no_AutoGnum", "-kp", "keep.txt", *[name[:-2]+".u" for name in spec["files"]], "-ko", "linked"],
             ["usplit", "-mips2", "-o", "split", "-t", "st", "linked"],
             ["umerge", "-Olimit", "5000", *common, "split", "-o", "merged", "-t", "st"]]
    for command in steps:
        subprocess.run([str(ido / command[0]), *command[1:]], cwd=out, check=True, capture_output=True)
    for kind in ("stock", "trace"):
        shutil.copyfile(out / "st", out / ("st."+kind))
        command = [str(traced_uopt if kind == "trace" else ido / "uopt"), "-G", "0", "-Olimit", "5000",
                   *common, "merged", "opt."+kind, "-t", "st."+kind, "optlog."+kind]
        env = dict(os.environ)
        if kind == "trace":
            env.update(CDX_LOG="1", CDX_DETAIL_WEB="all", CDX_OUT="trace.cdx",
                       W5D_LEVEL="3", W5D_OUT="trace.w5d", SPLOG="1")
            command += ["-l", "trace.list"]
        proc = subprocess.run(command, cwd=out, env=env, capture_output=True)
        (out / (kind+".log")).write_bytes(proc.stdout+proc.stderr)
        proc.check_returncode()
        for command in [["ugen", "-G", "0", *common, "opt."+kind, "-o", "gen."+kind,
                         "-t", "st."+kind, "-temp", "ugtmp"],
                        ["as1", "-elf", "-G", "0", "-p0", *common, "-r4300_mul", "-Olimit", "5000",
                         "gen."+kind, "-o", kind+".o", "-t", "st."+kind]]:
            subprocess.run([str(ido / command[0]), *command[1:]], cwd=out, check=True, capture_output=True)
    stock = (out / "stock.o").read_bytes()
    require(stock == (out / "trace.o").read_bytes(), "instrumented compiler changed the object")
    return {"complete_objects_identical": True, "stock_object_sha256": hashlib.sha256(stock).hexdigest(),
            "trace_object_sha256": hashlib.sha256(stock).hexdigest(),
            "tracing_enabled": ["CDX_LOG", "CDX_DETAIL_WEB", "W5D_LEVEL", "SPLOG"]}


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--traced-uopt", required=True, type=Path)
    parser.add_argument("--out", type=Path, default=ROOT / "build/mat3_rocket_frame_20261017/trace")
    args = parser.parse_args()
    print(json.dumps(run(args.out, args.traced_uopt.resolve()), indent=2))
