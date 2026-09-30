"""group_search executor: permute one function of an IDO -O3 call group.

About 40% of the game was compiled as whole-program IPA groups, so a function
cannot be searched alone: its registers depend on its callers and callees.
This job runs the permuter on the group's source, mutating only the target
function, but compiles every candidate as the whole group (cc -j, then uld -kp
keep.txt, usplit, umerge, uopt, ugen, as1 -r4300_mul — the stages of
blob_group.builder_script) and scores just the target's slice, linked at its
image address (see groupdump).

Manifest:
    {
      "job_type": "group_search",
      "toolkit_sha": "...",
      "target_id": "func_800C9590",     # the function being permuted
      "group": "controller_poll",
      "seed_file": "base.c",            # under inputs/: the file holding the target
      "seed_name": "group.c",           # its name inside the group
      "extra_files": ["other.c"],       # under inputs/, compiled with it
      "keep": ["fn", ...],              # uld -kp list
      "compile_flags": "-g0 -O3 -mips2 -G 0 -non_shared",
      "spec_file": "spec.json",         # groupdump spec (see groupdump.py)
      "budget": {"wall_seconds": 14400, "iterations": null}
    }
"""
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

_HERE = Path(__file__).resolve().parent
if str(_HERE) not in sys.path:
    sys.path.insert(0, str(_HERE))

import groupdump  # noqa: E402
import permuter_search  # noqa: E402
import _permuter  # noqa: E402


def pipeline(toolkit, flags, files, workdir="."):
    """The build stages, as shell. Kept identical to blob_group.builder_script
    (a test compares the two); only the toolkit path differs."""
    units = " ".join(f[:-2] + ".u" if f.endswith(".c") else f for f in files)
    opt = "-O3"
    T = f"{toolkit}/ido"
    return (
        f'T="{T}"; cd {workdir}; '
        f"$T/cc -j {flags} {' '.join(files)} >cc.log 2>&1 || {{ cat cc.log >&2; exit 1; }}; "
        f"$T/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 "
        f"-no_AutoGnum -kp keep.txt {units} -ko linked; "
        f"$T/usplit -mips2 -o split -t st linked; "
        f"$T/umerge -Olimit 5000 -mips2 -EB -g0 {opt} split -o merged -t st >/dev/null; "
        f"$T/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 {opt} merged opt -t st optlog >/dev/null; "
        f"$T/ugen -G 0 -mips2 -EB -g0 {opt} opt -o gen -t st -temp ugtmp >/dev/null; "
        f"$T/as1 -elf -G 0 -p0 -mips2 -EB -g0 {opt} -r4300_mul -Olimit 5000 gen -o group.o -t st >/dev/null"
    )


def write_compile_sh(perm_dir, toolkit, manifest, static_dir):
    """compile.sh IN.c -o OUT.o: build the group with IN.c as the target file."""
    files = [manifest["seed_name"]] + list(manifest.get("extra_files", []))
    script = perm_dir / "compile.sh"
    script.write_text(
        "#!/bin/sh\n"
        "set -e\n"
        'SRC="$1"; OUT="$3"\n'
        'W=$(mktemp -d "${TMPDIR:-/tmp}/grp.XXXXXX") || exit 1\n'
        'trap \'rm -rf "$W"\' EXIT\n'
        f'cp -R "{static_dir}"/. "$W"/ || exit 1\n'
        f'cp "$SRC" "$W/{manifest["seed_name"]}" || exit 1\n'
        + pipeline(toolkit, manifest["compile_flags"], files, workdir='"$W"') + "\n"
        'cp "$W/group.o" "$OUT"\n'
    )
    script.chmod(0o755)


def run(job_dir, manifest, progress):
    job_dir = Path(job_dir)
    inputs = job_dir / "inputs"
    toolkit = permuter_search._toolkit()
    budget = manifest["budget"] or {}
    wall_budget = budget.get("wall_seconds") or 4 * 3600
    cores = int(os.environ.get("CONVEYOR_CORES", "1"))

    perm_dir = Path(tempfile.mkdtemp(prefix="grpjob-"))
    static_dir = perm_dir / "static"
    try:
        static_dir.mkdir()
        shutil.copy(inputs / manifest["seed_file"], perm_dir / "base.c")
        for name in manifest.get("extra_files", []):
            shutil.copy(inputs / name, static_dir / name)
        (static_dir / "keep.txt").write_text("".join(f"{k}\n" for k in manifest["keep"]))

        spec = json.loads((inputs / manifest["spec_file"]).read_text())
        spec_path = perm_dir / "spec.json"
        shutil.copy(inputs / manifest["spec_file"], spec_path)
        target_o = perm_dir / "target.o"
        target_o.write_bytes(groupdump.stub_bytes(bytes.fromhex(spec["retail"])))

        (perm_dir / "function.txt").write_text(manifest["target_id"] + "\n")
        write_compile_sh(perm_dir, toolkit, manifest, static_dir)
        (perm_dir / "settings.toml").write_text(
            f'objdump_command = "{sys.executable} {_HERE / "groupdump.py"} {spec_path}"\n')

        base_score = None
        with tempfile.NamedTemporaryFile(suffix=".o") as base_o:
            proc = subprocess.run(
                [str(perm_dir / "compile.sh"), str(perm_dir / "base.c"), "-o", base_o.name],
                capture_output=True, text=True, timeout=600)
            if proc.returncode == 0:
                scorer = _permuter.Scorer(
                    target_o=str(target_o), stack_differences=True, algorithm="difflib",
                    debug_mode=False, ign_branch_targets=False,
                    objdump_command=f"{sys.executable} {_HERE / 'groupdump.py'} {spec_path}")
                base_score = scorer.score(base_o.name)[0]
            error = (proc.stderr or proc.stdout)[-600:]
        if base_score is None:
            return {"target_id": manifest["target_id"], "group": manifest["group"],
                    "final_best_score": None, "base_score": None,
                    "error": "group does not build: " + " ".join(error.split())}, {}
        progress.update(best_score=base_score,
                        best_source=permuter_search._gz_b64(perm_dir / "base.c"))
        if base_score == 0:
            payload = {"target_id": manifest["target_id"], "group": manifest["group"],
                       "final_best_score": 0, "base_score": 0, "wall_seconds_used": 0}
            return payload, {"best.c": str(perm_dir / "base.c")}

        payload, artifacts = permuter_search.drive(
            perm_dir, manifest, progress, base_score, wall_budget, cores,
            extra_args=("--stack-diffs",))
        payload["group"] = manifest["group"]
        return payload, artifacts
    finally:
        shutil.rmtree(perm_dir, ignore_errors=True)
